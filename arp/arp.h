#ifndef ARP_H
#define ARP_H

#include <tins/tins.h>
#include <iostream>

using namespace Tins;
using namespace std;

typedef EthernetII::address_type addr_type;

EthernetII::address_type itoh(NetworkInterface, IPv4Address);
void do_arp_spoofing(NetworkInterface iface, IPv4Address gw,
                    IPv4Address target, const NetworkInterface::Info& info,IPv4Address my_ip_addr);
void ip_forwarding(IPv4Address to, IPv4Address from,addr_type to_hw,
                     addr_type from_hw, string device, IPv4Address my_ip_addr);
#endif //ARP_H