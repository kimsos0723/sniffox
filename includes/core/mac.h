#include "assets.h"

namespace core {

constexpr short MACSIZE = 6;
using mac_t = uint8_t[MACSIZE];
class HWAddr {
    mac_t mac;

public:
    HWAddr() = delete;
    explicit HWAddr(const string& mac_str);

    explicit HWAddr(const HWAddr& hwaddr_) { copy(hwaddr_.mac, hwaddr_.mac + MACSIZE, this->mac); }
    explicit HWAddr(const mac_t& mac_) { copy(mac_, mac_ + MACSIZE, this->mac); }
    

    ~HWAddr()
    {
        for (int i = 0; i < MACSIZE; i++)
            this->mac[i] = 0;
    }

    explicit operator string() const;
    explicit operator uint64_t() const;

    friend ostream& operator<<(ostream& o, const HWAddr& i);
};
}