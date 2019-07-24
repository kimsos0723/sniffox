#include "../includes/ip.h"
#include "../includes/err.h"
#include <sstream>

#include <iostream>
#include <regex>
using namespace std;
Ipv4Addr::Ipv4Addr(const string ip_str)
{
    smatch m;
    regex e("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
    if (not regex_match(ip_str, e)) {
         throw NotIpAddrString();
    }        
    string res;
    uint8_t tmp[4] = {0,};

    int j = 0;    

    for (auto i : ip_str + '.') {
        if (i != '.') {
            res += i;
        } else {            
            tmp[j++] = atoi(res.c_str());
            res.clear();
        }
    }
 
    this->ip = tmp[0] << 24 | tmp[1] << 16 | tmp[2] << 8 | tmp[3];
 
}

Ipv4Addr::operator string() const
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&ip);

#if __BYTE_ORDER == __LITTLE_ENDIAN
    stringstream strm;
    strm << int(p[3]) << "." << int(p[2]) << "." << int(p[1]) << "." << int(p[0]);
#elif __BYTE_ORDER == __BIG_ENDIAN
    strm << int(p[0]) << "." << int(p[1]) << "." << int(p[2]) << "." << int(p[3]) << endl;
#endif
    return strm.str();
}