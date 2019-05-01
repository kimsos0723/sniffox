#include "ssl.h"
#include <arpa/inet.h>
using std::copy;

namespace Tins
{     
uint16_t catUint8(int8_t f, int8_t l);
uint32_t catUint16(int16_t f, int16_t l);

TLS::Handshake::Handshake(byte_array& dump) : bytes(dump) {
    size_t cur=0;
    this->type.num = dump.at(cur++);
    if(this->type.num != Htype_t::CLIENT_HELLO) return;
    this->len.b[0] = dump.at(cur++);
    this->len.b[1] = dump.at(cur++);
    this->len.b[2] = dump.at(cur++);    
    this->ver.num = catUint8(cur++, cur++);
    if(this->ver.num == 0) return;    
    this->random.assign(dump.begin()+cur, dump.begin()+cur+32);    
    cur+=32;
    this->sIdlen = dump.at(cur++);
    this->sId.assign(dump.begin()+cur, dump.begin()+cur+32);
    cur+=32;
    this->CSlen = htons(catUint8(dump.at(cur++), dump.at(cur++)));     
    this->CS.assign( dump.begin()+cur, dump.begin()+cur+CSlen);
    cur+=CSlen;
    this->ComprssMethodLen = dump.at(cur++);
    this->ComprssMethods.assign( dump.begin()+cur, dump.begin()+cur+ (ComprssMethodLen) );    
    cur += ComprssMethodLen;
    this->ExtentionsLen = htons(catUint8(dump.at(cur++),dump.at(cur++)));
    this->Extensions.bytes.assign(dump.begin()+cur, dump.begin()+cur+ExtentionsLen);
}

TLS::TLS(const byte_array dump) : bytes(dump) {
    std::size_t cur = 0;
    this->messageType.value = dump.at(cur++);
    this->ver.num = catUint8(dump.at(cur++),dump.at(cur++));
    this->len = htons(catUint8(dump.at(cur++), dump.at(cur++)));
    byte_array a;
    a.assign(dump.begin()+cur,dump.end());
    if(this->messageType.value == static_cast<int8_t>(ContentType_t::ContentType::HANDSHAKE)) {
        this->handshake = Tins::TLS::Handshake_t(a);
    }
}

typedef struct serverNameIndication {
    uint8_t type;
    uint16_t len;
    uint16_t snlistLen;
    uint8_t snType;
    uint16_t snlen;
    std::string servName;
}sni_t;

std::string Extentions_t::server_names() {    
    sni_t sni;        
    for (int i = 0; i < bytes.size(); i++) {
        if (catUint8(bytes[i], bytes[i + 1]) == 0x0000) {
            sni.len = catUint8(bytes[i], bytes[i + 1]);
            sni.snType = bytes[i + 2];
            sni.snlistLen = catUint8(bytes[i + 3], bytes[i + 4]);
            sni.snType = bytes[i + 5];
            sni.snlen = catUint8(bytes[i + 6], bytes[i + 7]);
            sni.servName = std::string(bytes.begin() + i + 9, bytes.begin() + i + 9 + sni.snlen);
            return sni.servName;
        } else {
            i += catUint8(bytes[i + 3], bytes[i + 4]);
        }
    }    
    return "";
}

uint16_t catUint8(int8_t f, int8_t l) {
    uint16_t r = 0x0000;
    r = f;
    r = r << 8;
    r |= l;
    return r;
}
uint32_t catUint16(int16_t f, int16_t l) {
    uint32_t r = 0;
    r=f;
    r = r<<16;
    r|=l;
    return  r;
}

} // namespace Tins
