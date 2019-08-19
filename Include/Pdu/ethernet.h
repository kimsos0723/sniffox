#pragma once

#include "assets.h"
#include "hw_addr.h"
#include "pdu.h"
#include "ip.h"
using core::HWAddr;
using bytes = vector<uint8_t>;

namespace pdu {
class Ethernet : public PDU {
public:    
    static const PDU::PDUType pdu_flag = PDU::PDUType::ETHERNET_II;
    
    Ethernet() {}
    Ethernet(HWAddr& dst_addr,HWAddr& src_addr, EtherType t);                        
    // Ethernet(const uint8_t* buffer, size_t size);    
    ~Ethernet() {}
    void write_serialization(bytes& bs);    
    
    size_t header_size() const{return sizeof(header_); }
    size_t trailer_size() const { return 0;}
    // Ethernet* clone() const override { return new Ethernet (*this); }
    PDUType pdu_type() const { return PDUType::ETHERNET_II; }    
    
    void deserialize(bytes& b ) {                 
        memcpy(&header_, &b[0], sizeof(ethernet_header));        
        if(inner_pdu() != nullptr) {
            bytes tmp(b.begin()+sizeof(ethernet_header),b.end());
            inner_pdu()->deserialize(tmp);
        }
    }
    
    HWAddr src_mac() {return header_.src_mac; }
    HWAddr dst_mac() {return header_.dst_mac; }    
    EtherType type() {return header_.ether_type;}
private:
#pragma pack(push, 1)
    struct ethernet_header {
        HWAddr dst_mac;
        HWAddr src_mac;
        EtherType ether_type;
    };
#pragma pack(pop)
    ethernet_header header_;
};
}