#pragma once

#include "assets.h"
#include "ip_addr.h"
#include "pdu.h"

using core::Ipv4Addr;
using bytes = vector<uint8_t>;

namespace pdu {
class IP : public PDU {
public:    
    static const PDU::PDUType pdu_flag = PDU::PDUType::IP;
    
    IP() {}
    IP(IP& src_addr,IP& dst_addr);                        
    ~IP() {}
    void write_serialization(bytes& bs);        
    size_t header_size() const{return sizeof(header_); }
    size_t trailer_size() const { return 0;}    
    PDUType pdu_type() const { return PDUType::IP; }        
    
    void deserialize(bytes& b ) {                 
        memcpy(&header_, &b[0], sizeof(ip_header));        
        if(inner_pdu() != nullptr) {
            bytes tmp(b.begin()+sizeof(ip_header),b.end());
            inner_pdu()->deserialize(tmp);
        }
    }    
    Ipv4Addr src_ip() {return header_.sip;}
    Ipv4Addr dest_ip() {return header_.dip;}
private:
#pragma pack(push, 1)
    struct ip_header {
        struct {        
            uint8_t ver : 4;
            uint8_t UHL : 4;
        }vhl;
        uint8_t tos;
        uint8_t tolen;
        uint16_t id; 
        uint16_t fragment;
        uint8_t ttl;
        uint8_t proto;
        uint16_t checksum;
        Ipv4Addr sip;
        Ipv4Addr dip;        
    };
#pragma pack(pop)
    ip_header header_;
};
}
