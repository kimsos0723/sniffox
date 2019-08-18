#include "Pdu/pdu.h"

namespace pdu {
size_t PDU::size() const
{
    size_t s = header_size();        
    return s;
}

PDU::~PDU() {
    
}

bytes PDU::serialize() {
    bytes b(0);          
    write_serialization(b);    
    return b;
}

} // namespace pdu
