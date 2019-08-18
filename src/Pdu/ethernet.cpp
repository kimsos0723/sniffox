#include "Pdu/ethernet.h"

namespace pdu
{ 
    Ethernet::Ethernet(HWAddr& dst_addr,HWAddr& src_addr,Ethernet::EtherType type )
        :header_() 
    {
        header_.dst_mac = HWAddr(dst_addr);
        header_.src_mac = HWAddr(src_addr);          
        header_.ether_type = static_cast<EtherType>(htons(static_cast<uint16_t>(type)));        
    }        

    void Ethernet::write_serialization(bytes& bs) {
        uint8_t* pt = reinterpret_cast<uint8_t*>(&header_);
        for(int i = 0; i<sizeof(ethernet_header); i++) {
            bs.push_back(pt[i]);
        }
    }
} // namespace pdu
