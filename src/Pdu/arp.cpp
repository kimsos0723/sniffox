#include "Pdu/arp.h"
#include "Pdu/ethernet.h"
#include "Pdu/rawpdu.h"
namespace pdu {
ARP::ARP(Operation opcode, 
    Ipv4Addr sender_ip,
    Ipv4Addr target_ip,    
    HWAddr sender_hw,
    HWAddr target_hw)
    : header_()
{
    header_.htype = htons(1);
    header_.ptype = 0x0008;
    header_.hlen = sizeof(HWAddr);
    header_.plen = sizeof(Ipv4Addr);
    header_.oper = static_cast<Operation>(htons(static_cast<uint16_t>(opcode)));
    header_.sip = Ipv4Addr(sender_ip);
    header_.tip = Ipv4Addr(target_ip);
    header_.smac = HWAddr(sender_hw);
    header_.tmac = HWAddr(target_hw);
}

ARP::ARP(uint8_t* buffer, uint32_t total_sz)
{
    memcpy(buffer, &header_, sizeof(arp_header));
    if (total_sz > sizeof(arp_header)) {
        auto b = bytes(buffer + sizeof(arp_header), buffer + total_sz);
        RawPDU h = RawPDU(b);
        h.size = total_sz - sizeof(arp_header);
    }
}

void ARP::write_serialization(bytes& bs)
{        
    uint8_t* b_arr = reinterpret_cast<uint8_t*>(&header_);
    for (int i = 0; i < 28; i++) {
        bs.push_back(b_arr[i]);        
    }
    
}

} // namespace pdu
 