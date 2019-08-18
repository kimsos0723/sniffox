#pragma once

#include "pdu.h"

namespace pdu {
class RawPDU : public PDU {
public:
    size_t size;
    RawPDU(){}
    RawPDU(const uint8_t* payload, size_t size);
    RawPDU(bytes data)
        : payload_(data) { }
    RawPDU(const std::string& data)
        : payload_(data.begin(), data.end())
    {
    }

    RawPDU* clone() const {return new RawPDU(*this);};
    PDUType pdu_type() const {return PDUType::RAW;};
    size_t header_size() const  {return 0;};
    size_t trailer_size()const{return 0;};
    void write_serialization(bytes& bs);
    void deserialize(bytes& b) { memcpy(&payload_[0], &b[0],b.size());}
    bytes payload_;
};
} // namespace pdu
