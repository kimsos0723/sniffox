#ifndef ARPSPOOF_H
#define ARPSPOOF_H

#include <iostream>
#include <tins/tins.h>
#include <thread>
using namespace Tins;
using  std::cout;
using std::endl;
class ArpSpoofer {
private:
    NetworkInterface iface;
    NetworkInterface::Info info;    
    std::string device;
    PacketSender arpSender;    
    HWAddress<6> gwHwAddr, targetHwAddr;
    IPv4Address targetIp, myIp, gwIp;
    EthernetII toGw, toTarget;

public:    
    ArpSpoofer(IPv4Address target, IPv4Address my, IPv4Address gw, std::string device);    
    void doArpspoof();    
    void ipForward(IPv4Address to, IPv4Address from, HWAddress<6> toHw,
                   HWAddress<6> from_hw, std::string device, IPv4Address myIp);
                       
    IPv4Address getGwIp() {return this->gwIp;}
    IPv4Address getTargetIP() {return this->targetIp;}
    HWAddress<6> getGwHw() {return this->gwHwAddr;}
    HWAddress<6> getTargetHw() {return this->targetHwAddr;}
    std::string getDevice() {return this->device;}
    IPv4Address getMyIp() {return this->myIp;}
    EthernetII MakeArpReply(IPv4Address to, IPv4Address from, ARP::hwaddress_type toHw);    
    std::thread ipForwardToGwThread() {
        return std::thread(
            [=]{ipForward(this->getGwIp(), this->getTargetIP(), this->getGwHw(), this->getTargetHw(), this->getDevice(), this->getMyIp());}
        );
    }
    std::thread ipForwardToTargetThread() {
        return std::thread(
            [=]{ipForward(this->getTargetIP(), this->getGwIp(), this->getTargetHw(), this->getGwHw(), this->getDevice(), this->getMyIp());}
        );
    }
};

#endif 
