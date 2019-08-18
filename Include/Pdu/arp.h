#pragma once

#include "hw_addr.h"
#include "ip_addr.h"
#include "pdu.h"

using core::HWAddr;
using core::Ipv4Addr;

namespace pdu {
class ARP : public PDU {
public:   
    PDUType pdu_flag = PDUType::ARP;
    
    enum class Operation : int16_t {
        REQ = 1,
        REPLY = 2,
        REVREQ = 3,
        REVREPLY = 4,
        INVREQ = 8,
        INVREPLY = 9
    };     
    ARP(){};

    ARP(Operation opcode, Ipv4Addr sender_ip, Ipv4Addr target_ip,  HWAddr sender_hw,HWAddr target_hw);
    
    ARP(uint8_t* buffer, uint32_t total_sz);
    HWAddr& sender_hw_addr() {
        return header_.smac;
    }
    
    HWAddr& target_hw_addr() {
        return header_.tmac;
    }

    Ipv4Addr& sender_ip_addr() {
        return header_.sip;
    }

    uint16_t ptye() const {
        return htons(header_.ptype);
    }

    uint16_t hlen() const {
        return header_.hlen;
    }

    uint16_t plen() const {
        return htons(header_.plen);
    }
    
    Operation opcode() const {
        return header_.oper;
    }

    size_t header_size() const { return sizeof(header_); };
    void write_serialization(bytes& bs);
    PDUType pdu_type() const { return pdu_flag; }
    size_t trailer_size() const { return 0;}
    void deserialize(bytes& b) {
        memcpy(&header_, &b[0], sizeof(arp_header));        
        if(inner_pdu() != nullptr) { 
            bytes tmp (b.begin()+sizeof(arp_header),b.end());           
            inner_pdu()->deserialize(tmp);
        }
    }
private:
#pragma pack(push, 1)
    struct arp_header {
        uint16_t htype;
        uint16_t ptype;
        uint8_t hlen;
        uint8_t plen;
        Operation oper;
        HWAddr smac;
        Ipv4Addr sip;
        HWAddr tmac;
        Ipv4Addr tip;
    };
#pragma pack(pop)
    arp_header header_;
};

} // namespace packet