#pragma once

#include "assets.h"
#include "hw_addr.h"
#include "ip_addr.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace core;
struct Interface {
    Interface() {}
    Interface(std::string device)
    {
        iface.dev = device;
        int fd;
        ifreq ifr;
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        ifr.ifr_addr.sa_family = AF_INET;
        memcpy(ifr.ifr_name, device.c_str(), IFNAMSIZ - 1);
        ioctl(fd, SIOCGIFHWADDR, &ifr);
        iface.mac = *(HWAddr*)&ifr.ifr_hwaddr.sa_data;

        ioctl(fd, SIOCGIFADDR, &ifr);
        close(fd);
        iface.ip = *(Ipv4Addr*)&ifr.ifr_addr;
        iface.ip = *(Ipv4Addr*)&((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr;        
    }
    Interface(Interface& ifa){ this->iface = ifa.iface; }
    HWAddr& mac() { return iface.mac; }
    Ipv4Addr& ip() { return iface.ip; }
    std::string dev() { return iface.dev; }
    struct interface_t {
        std::string dev;
        HWAddr mac;
        Ipv4Addr ip;
    };
    interface_t iface;
};