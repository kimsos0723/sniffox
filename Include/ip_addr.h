#pragma once

#include "./assets.h"

using ip_t = uint32_t;
constexpr size_t IPSIZE = 4;
namespace core {
#pragma pack(push, 1)
class Ipv4Addr {

public:
    Ipv4Addr() {};
    Ipv4Addr(const Ipv4Addr& ip_bin)
    {
        this->_ip = ip_bin._ip;
    }
        
    Ipv4Addr( string ip_str);

    ~Ipv4Addr() { this->_ip = 0xFFFFFFFF;}

    ip_t ip() { return _ip; }
    void setip(ip_t ip) { this->_ip = ip; }

    explicit operator string() const;
    explicit operator uint32_t() const { return _ip; }
    Ipv4Addr& operator=(Ipv4Addr ip) { this->_ip=ip._ip; return *this; }
    friend ostream& operator<<(ostream& o, const Ipv4Addr& i) { return o << string(i);  }
private:
    ip_t _ip;
};
#pragma pack(pop)
}
