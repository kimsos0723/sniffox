#include "Pdu/rawpdu.h"
using namespace pdu;

namespace pdu {    
    RawPDU::RawPDU(const uint8_t* payload, size_t size)
        : payload_(payload, payload+size)
    {     
    }    

    void RawPDU::write_serialization(bytes& bs) 
    {
        uint8_t* b_arr = reinterpret_cast<uint8_t*>(payload_[0]);
        for (int i = 0; i < sizeof(size); i++) {
            bs.push_back(b_arr[i]);
        }
    }
}