#include "../core/mac.h"

namespace packet {
using MAC = core::HWAddr;
#pragma pack(push, 1)
struct Ethernet final {
    MAC dst_mac;
    MAC src_mac;
    uint16_t ether_type;
    uint16_t type() { return ntohs(this->ether_type); }
    enum class Type : uint16_t {
        IPV4 = 0x800,
        ARP = 0x0806,
        IPV6 = 0x86DD,
        APPLE_TALK = 0x809B
    };
};
#pragma pack(pop)
}