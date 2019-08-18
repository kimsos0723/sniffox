#include "hw_addr.h"

using namespace core;

HWAddr::HWAddr(string mac_str)
{
    std::smatch m;
    std::regex octet("([0-9a-fA-f]{2})[:,-]?([0-9a-fA-f]{2})[:,-]?([0-9a-fA-f]{2})[:,-]?([0-9a-fA-f]{2})[:,-]?([0-9a-fA-f]{2})[:,-]?([0-9a-fA-f]{2})");
    // std::cout << mac_str <<std::endl;
    
    if (std::regex_match(mac_str, m, octet)) {
        for (int i = 1; i < MACSIZE + 1; i++) {
            _mac[i - 1] = stoi(m[i].str(), nullptr, 16);
        }
    } else {
        throw std::invalid_argument("Worng MAC Address Syntax");
    }
}

HWAddr::operator string() const
{
    clear_ss();
    for(auto& i : this->_mac) {
        ss << std::hex  << int(i);        
    }
    return ss.str();
}
HWAddr::operator uint64_t() const
{
    return uint64_t(_mac[0]) << 40 | uint64_t(_mac[1]) << 32 | (_mac[2]) << 24 | (_mac[3]) << 16 | (_mac[4]) << 8 | (_mac[5]);
}
