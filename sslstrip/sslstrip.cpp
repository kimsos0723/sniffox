#include "./sslstrip.h"
#include <sstream>

using std::copy;

TLS::TLS(const byte_array d) : dump(d) {
    size_t tmpIdx = 0;

    this->cType = dump[0];
    copy(&dump[1],&dump[3],this->ver1.v);
    copy(&dump[3],&dump[5], this->len);
    this->hType = dump[5];
    copy(&dump[6],&dump[9],this->hLen.b);
    copy(&dump[9],&dump[11],this->ver2.v);
    
    if(this->ver2.to_string() == "undefined") return;
     
    copy(&dump[11],&dump[44],this->random);
    this->sIdLen = dump[43];
    this->sId.assign(dump.begin()+44,dump.begin()+44+this->sIdLen);
    tmpIdx = this->sIdLen+44;
    if(this->cType == this->HANDSHAKE) {
        copy(&dump[tmpIdx+1],&dump[tmpIdx+3],this->CSLen);
        this->cipherSuit.assign(dump.begin()+static_cast<int>(tmpIdx+2),dump.begin()+static_cast<int>(tmpIdx+*this->CSLen+2));
        tmpIdx += *this->CSLen+2;
        this->CMLen = dump[tmpIdx];
        this->compressionMethods.assign(dump.begin()+static_cast<int>(tmpIdx+1),dump.begin()+static_cast<int>(tmpIdx+this->CMLen+1));
        tmpIdx += this->CMLen+1;
        copy(&dump[tmpIdx],&dump[tmpIdx+2],this->exLen);
        this->extensions.assign(dump.begin()+static_cast<int>(tmpIdx+2), dump.begin()+static_cast<int>(tmpIdx+2+ static_cast<uint>((this->exLen[0]<<8)|(this->exLen[1])) ));               
    }    
}

std::string TLS::servername() {
    for(int i=0;i<((this->exLen[0]<<8)|(this->exLen[1]));i+=4){                       
            if(static_cast<int>(this->extensions[static_cast<size_t>(i)]) == 0 && static_cast<int>(this->extensions[static_cast<size_t>(i+1)]) == 0) {
                copy(&this->extensions[static_cast<size_t>(i)], &this->extensions[static_cast<size_t>(i+2)],this->sn.type);
                copy(&this->extensions[static_cast<size_t>(i)+2], &this->extensions[static_cast<size_t>(i+4)],this->sn.len);                
                copy(&this->extensions[static_cast<size_t>(i+4)], &this->extensions[static_cast<size_t>(i+6)],this->sn.sni.listLen);
                this->sn.sni.type = this->extensions[static_cast<size_t>(i+6)];
                copy(&this->extensions[static_cast<size_t>(i+7)], &this->extensions[static_cast<size_t>(i+9)],this->sn.sni.nameLen);                                
                this->sn.sni.name.assign(this->extensions.begin()+i+9,this->extensions.begin()+i+9+((this->sn.sni.nameLen[0]<<8)|(this->sn.sni.nameLen[1])));
                return std::string(this->sn.sni.name.begin(), this->sn.sni.name.end());
            
            } else {
                i+=((this->extensions[static_cast<size_t>(i)+2]<<8)|(this->extensions[static_cast<size_t>(i)+3]));
            }
    }
    return "NOTFOUND";
}

