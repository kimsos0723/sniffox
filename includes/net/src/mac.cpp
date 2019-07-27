#include "../includes/mac.h"
#include "err.h"
#include <algorithm>

using namespace std;

HWAddr::HWAddr(const HWAddr& mac_)
{
    std::copy(mac_.mac, mac_.mac + SIZE, this->mac);
}

HWAddr::HWAddr(const mac_t& num)
{
    std::copy(num, num + SIZE, this->mac);
}
HWAddr::HWAddr(string& mac_str)
{
    smatch m;
    regex octet("([0-9a-fA-f]{2})([0-9a-fA-f]{2})([0-9a-fA-f]{2})([0-9a-fA-f]{2})([0-9a-fA-f]{2})([0-9a-fA-f]{2})");        
    if (std::regex_match(mac_str, m, octet)) {
        for(int i=1;i<7;i++){            
            mac[i-1] = stoi(m[i].str(), nullptr, 16);
        }
        
    } else {
        throw std::invalid_argument("Worng MAC Address Syntax");
    }
}
HWAddr::operator string() const
{
}
HWAddr::operator uint64_t() const
{
}

ostream& operator<<(ostream& o, const mac_t& m)
{
    return o;
}
