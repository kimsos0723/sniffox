#ifndef ARPSPOOF_H
#define ARPSPOOF_H

#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <array>
using namespace Tins;

constexpr int BufSize = 6;

using  std::cout;
using std::endl;
using Array = std::array<EthernetII, BufSize>;



struct ForwardParam { 
        IPv4Address to;
        IPv4Address from;
        HWAddress<6> toHw;
        HWAddress<6> from_hw; 
        std::string device; 
        IPv4Address myIp;        
        ForwardParam() = default;

        ForwardParam(IPv4Address to, IPv4Address from, HWAddress<6> toHw,
            HWAddress<6> from_hw, std::string& device, IPv4Address myIp) : device(device){
                this->to = to;
                this->from = from;
                this->toHw = toHw;
                this->from_hw = from_hw;                
                this->myIp = myIp;
        }
    };
class ArpSpoofer {
    
private:    
    NetworkInterface iface;
    NetworkInterface::Info info;    
    std::string device;
    PacketSender arpSender;    
    HWAddress<6> gwHwAddr, targetHwAddr;
    IPv4Address targetIp, myIp, gwIp;
    EthernetII toGw, toTarget;
    ForwardParam targetInfo;
    ForwardParam gwInfo;

    Array Gbuffer;
    Array Vbuffer;

public:                    
    ArpSpoofer() = default;
    ArpSpoofer(IPv4Address target, IPv4Address my, IPv4Address gw, std::string device);    

    IPv4Address getGwIp() {return this->gwIp;}
    IPv4Address getTargetIP() {return this->targetIp;}
    HWAddress<6> getGwHw() {return this->gwHwAddr;}
    HWAddress<6> getTargetHw() {return this->targetHwAddr;}
    std::string getDevice() {return this->device;}
    IPv4Address getMyIp() {return this->myIp;}
        
    void doArpspoof();
    EthernetII modifyPacket(PDU* pdu, ForwardParam& p);

    void setPacketBuffer(ForwardParam p, Array a);
    void sendPacketBuffer(Array a) {
        for(auto i : a)
            this->arpSender.send(i);
    };

    EthernetII MakeArpReply(IPv4Address to, IPv4Address from, ARP::hwaddress_type toHw);

    std::thread bufferSendToGwThread() {
        return std::thread(
            [&]{sendPacketBuffer(this->Gbuffer);}
        );
    }

    std::thread bufferSendToTargetThread() {
        return std::thread(
            [&]{sendPacketBuffer(this->Vbuffer);}
        );
    }

    std::thread setPacketBufferToGwThread() { 
        return std::thread(
            [&]{setPacketBuffer(this->gwInfo,this->Gbuffer);}
        );
    }
    
    std::thread setPacketBufferToTargetThread() { 
        return std::thread(
            [&]{setPacketBuffer(this->targetInfo,this->Vbuffer);}
        );
    }

};

#endif 
