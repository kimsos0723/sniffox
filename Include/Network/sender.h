#pragma once

#include "Pdu/arp.h"
#include "Pdu/ethernet.h"
#include "Pdu/ip.h"
#include "interface.h"
class Sender {
public:
    Sender(std::string dev)
        : iface(dev)
    {
        std::cout<<"sended"<<std::endl;
    }
    Sender(Interface& ifa)
        : iface(ifa)
    {
    }
    HWAddr ip_to_mac(Ipv4Addr qip);
    bool send_packet(bytes packet);

private:
    Interface iface;
};