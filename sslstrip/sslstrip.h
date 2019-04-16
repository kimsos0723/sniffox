#ifndef SSL_STRIP_H
#define SSL_STRIP_H

#include <iostream>
#include <tins/tcp.h>
#include <tins/sniffer.h>
#include <tins/network_interface.h>
#include <tins/pdu.h>
#include <tins/tcp.h>
#include <tins/rawpdu.h>
#include <iomanip>
#include <arpa/inet.h>

using Tins::byte_array;
using Tins::RawPDU;
using Tins::PDU;
using std::vector;
using std::string;
typedef struct tlsVersion {
    
    uint8_t v[2];  
    enum TLSVER {
        VERSSL3_0 = 0x0300,
        VER1_0 = 0x0301,
        VER1_1 = 0x0302,
        VER1_2 = 0x0303
    };
    std::string to_string() {
        
        switch (htons(*reinterpret_cast<uint16_t*>(this->v))) {
            case VER1_0:
                return "v1.0";
            case VER1_1:
                return "v1.1";
            case VER1_2:
                return "v1.2";
            case VERSSL3_0:
                return "SSL v3.0";            
            default:    
                return "undefined";
        };
    }
}ver;
typedef struct {   
public: 
    unsigned char b[3];
    uint toInt() {
        return uint((b[0]<<16) | (b[1]<<8) | (b[2]));
    }
}uint24_t;

typedef struct {
        uint8_t listLen[2];
        uint8_t type;
        uint8_t nameLen[2];
        vector<char> name;
}sni_t;

typedef struct TlsServerName{
    uint8_t type[2];
    uint8_t len[2];
    sni_t sni;
}servName_t;    

class TLS { 
public:
    byte_array dump;
    uint8_t cType;
    ver ver1;
    uint8_t len[2];
    uint8_t hType;
    uint24_t hLen;
    ver ver2;
    uint8_t random[32];
    uint8_t sIdLen;
    vector<uint8_t> sId;        
    uint8_t CSLen[2];
    vector<uint8_t> cipherSuit;
    uint8_t CMLen;
    vector<uint8_t> compressionMethods;
    uint8_t exLen[2];
    vector<uint8_t> extensions;    

    servName_t sn;     
    explicit TLS(const byte_array);
    enum ContentType {
        ChangeCipherSpec = 20,
        ALERT,
        HANDSHAKE = 22,
        APPLICATION,
        HEARTBEAT
    };
    enum HANDSHAKEType {        
        HELLOREQ = 0x00,
        CLNTHELLO = 0x01,
        SERVHELLO =0x02,
        NEWTICKET,
        ENCRYEXTENS,
        CERTIF,
        SERVKEYEXCHANGE,
        CERTIFREQ,
        SERVHELLODONE,
        CERTIFVERIFY,
        CLNTKEYEXCHANGE,
        FIN
    };    
    std::string servername();    
};

class SslStrip {
public:
    vector<TLS> tlsList;
    vector<std::string> httpList;        
};


#endif //SSL_STRIP_H
