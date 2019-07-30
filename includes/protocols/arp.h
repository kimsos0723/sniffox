#include "../core/ip.h"
#include "../core/mac.h"

using IP = core::Ipv4Addr;
using MAC = core::HWAddr;
namespace packet {
struct ARP {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t oper;
    uint16_t senderHA;
    MAC smac;
    IP sip;
    MAC tmac;
    IP tip;
    enum class Htype {
        NETROM   = 0,
        ETHER = 1,
        EETHER = 2,
        AX25 = 3,
        PRONET = 4,
        IEEE801 = 6,
        ARCNET = 7,
        HYPER = 8, 
        LANSTAR = 9,
        FRA_RELAY = 15, //frame reflay
        ASYNC_TRANSFER1 = 16, // asynchronous transfer mode (ATM)
        HDLC = 17,
        FIBER = 18,
        ASYNC_TRANSFER2 = 19,
        SERIAL = 20
    };

    enum class Operation { 
        Qeq =1,
        Reply =2,
        RevReq =3 ,
        RevReply =4 ,
        INV_Req = 8,
        INV_Reply = 9        
    };
};
} // namespace packet
