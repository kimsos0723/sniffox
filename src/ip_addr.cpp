#include "ip_addr.h"
namespace core {

Ipv4Addr::Ipv4Addr(string ip_str)
{
    std::cout << ip_str << std::endl;
    if ( not inet_pton(AF_INET, ip_str.c_str(),&(this->_ip)) ) {
        throw std::invalid_argument("Worng IP Syntax");
    }
}

Ipv4Addr::operator string() const
{
    char s[INET_ADDRSTRLEN] = { 0, };
    inet_ntop(AF_INET, &(this->_ip), s, INET_ADDRSTRLEN);
    return string(s);
}
}