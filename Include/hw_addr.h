#pragma once

#include "assets.h"

constexpr short MACSIZE = 6;
using mac_t = uint8_t[MACSIZE];

namespace core {
#pragma pack(push, 1)
class HWAddr {
public:
    explicit HWAddr(){};
    explicit HWAddr(const string mac_str);
    HWAddr(const HWAddr& hwaddr_) { memcpy(this->_mac, hwaddr_._mac,sizeof(mac_t));}    
    explicit HWAddr(const mac_t& mac) { copy(mac, mac + MACSIZE, this->_mac); }

    mac_t& mac() { return _mac; }
    void setip(mac_t mac) { copy(mac, mac + MACSIZE, this->_mac); }
    
    ~HWAddr()
    {
        for (int i = 0; i < MACSIZE; i++)
            this->_mac[i] = 0;
    }

    explicit operator string() const;
    explicit operator uint64_t() const;    
    HWAddr& operator=(HWAddr hd) { memcpy( this->_mac, hd._mac, sizeof(HWAddr)); return *this; }
    friend ostream& operator<<(ostream& o, const HWAddr& i) { return o << string(i); }

private:
    mac_t _mac;
};
#pragma pack(pop)
}
