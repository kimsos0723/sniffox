#include <tins/tins.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Tins {  
namespace Memory {
    class OutputMemoryStream;
} // Memory
typedef struct {    
    byte_array bytes;
    std::string server_names();
}Extentions_t;

typedef struct {   
    unsigned char b[3];
    uint toInt() {
        return uint((b[0]<<16) | (b[1]<<8) | (b[2]));
    }
} uint24_t;


#define stringizing(x) #x

 struct ContentType_t{
    int8_t value;
    enum class ContentType
        {
            CHANGE_CIPHER_SPEC = 20,
            ALTER = 21,
            HANDSHAKE = 22,
            APPLICATION = 23,
            HEARTBEAT = 24
        };
    std::string str() {       
        switch (static_cast<ContentType>(this->value)) {
        case ContentType::ALTER:
            return stringizing(ALTER);
        case ContentType::APPLICATION:
            return stringizing(APPLICATIOM);
        case ContentType::CHANGE_CIPHER_SPEC:
            return stringizing(CHANGE_CIPHER_SPEC);
        case ContentType::HANDSHAKE:
            return stringizing(HANDSHAKE);
        case ContentType::HEARTBEAT:
            return stringizing(HEARTBEAT);
        default:
            return "not-declared";
        }
    }
};
struct ver_t{
    int16_t num;
    enum class Versions {
        SSL3_0 = 0x0300,
        TLS1_0 = 0x0301,
        TLS1_1 = 0x0302,
        TLS1_2 = 0x0303,
        TLS1_3 = 0x0304
    };
    std::string str() {
        switch (static_cast<Versions>(this->num)) {
        case Versions::SSL3_0:
            return stringizing(SSL3_0);
        case Versions::TLS1_0:
            return stringizing(TLS1_0);
        case Versions::TLS1_1:
            return stringizing(TLS1_1);
        case Versions::TLS1_2:
            return stringizing(TLS1_2);
        case Versions::TLS1_3:
            return stringizing(TLS1_3);
        default:
            return "not-declared";
        }
    }
};

struct Htype_t {
    int8_t num;
    enum HandshakeType {
        HELLOREQ = 0,
        CLIENT_HELLO = 1,
        SERV_HELLO = 2,
        NST = 4,
        NEW_SESSION_TICKET = NST,
        ENCRYPTED_EXTENSINS = 8,
        ENC_EXTENT = ENCRYPTED_EXTENSINS,        
        CERTIFICATE = 11,
        SERVER_KEY_EXCHANGE = 12,
        SERV_KEY_EX = SERVER_KEY_EXCHANGE,
        CERTIFICATE_REQ = 13,
        CRT_REQ = CERTIFICATE_REQ,
        SERVER_HELLO_DONE = 14,
        SERV_HELLO_DONE = SERVER_HELLO_DONE,
        CERTIFICATE_VERFIY = 15,
        CRT_VERIFY = CERTIFICATE_VERFIY,
        CLIENT_KEY_EXCHANGE = 16,
        CLI_KEY_EX = CLIENT_KEY_EXCHANGE,
        FINSHED = 20  
    };
    std::string str() {
         switch (static_cast<HandshakeType>(this->num)) {
             case HandshakeType::HELLOREQ:
                return stringizing(HELLOREQ);
            case HandshakeType::CLIENT_HELLO:
                return stringizing(CLIENT_HELLO);        
            case HandshakeType::SERV_HELLO:
                return stringizing(SERV_HELLO);        
            case HandshakeType::NST:
                return stringizing(NST); 
            case HandshakeType::ENC_EXTENT:
                return stringizing(ENC_EXTENT);
            case HandshakeType::CERTIFICATE:
                return stringizing(CERTIFICATE);
            case HandshakeType::SERV_KEY_EX:
                return stringizing(SERV_KEY_EX);                    
            case HandshakeType::CRT_REQ:
                return stringizing(CRT_REQ);
            case HandshakeType::SERV_HELLO_DONE:
                return stringizing(SERV_HELLO_DONE);                 
            case HandshakeType::CRT_VERIFY:
                return stringizing(CRT_VERIFY);                
            case HandshakeType::CLI_KEY_EX:
                return stringizing(CLI_KEY_EX);
            case HandshakeType::FINSHED:
                return stringizing(FINSHED);
         }
    }
};

class TLS {
public:  
    ContentType_t messageType;
    ver_t ver;
    uint16_t len;    
   
    typedef struct Handshake {
        byte_array bytes;
        Htype_t type;
        uint24_t len;
        ver_t ver;
        byte_array random;
        uint8_t sIdlen;
        byte_array sId;
        uint16_t CSlen;
        std::vector<uint16_t> CS;
        uint8_t ComprssMethodLen;
        byte_array ComprssMethods;
        uint16_t ExtentionsLen;
        Extentions_t Extensions;
        Handshake() = default;
        explicit Handshake(byte_array& b);
    }Handshake_t;
    //todo    
    typedef struct ChangeCipherSpec{} ccs;
    typedef struct Alert{} alt;
    typedef struct Application{} app;
    typedef struct Heartbeat{} Heartbeat;     
    Handshake_t handshake;
    byte_array hmac;
    byte_array padding;
    byte_array bytes;

    static const PDU::PDUType pdu_type;
    TLS() = default;
    explicit TLS(const byte_array dump);
    ~TLS() {        
    }    
    
};

} // Tins
