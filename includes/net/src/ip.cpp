#include "../includes/ip.h"
#include "../includes/err.h"
#include <regex>



Ipv4Addr::Ipv4Addr(const string& ip_str)
{    
    if(not inet_pton(AF_INET, ip_str.c_str(), &(this->ip))) {
        throw network::NotIpAddrString();
    }
}

Ipv4Addr::operator string() const
{
    char s[INET_ADDRSTRLEN] = {0,};    
    inet_ntop(AF_INET, &(this->ip), s, INET_ADDRSTRLEN);    
    return string(s);
}