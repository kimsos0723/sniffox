#pragma once

#include "./assets.h"
#include <stdexcept>

using ip_t = uint32_t;
using std::exception;
class Ipv4Addr {
public:
    ip_t ip;
public:
    Ipv4Addr() = delete;    
    explicit Ipv4Addr(const Ipv4Addr& ip_bin) : ip(ip_bin.ip) {}
    explicit Ipv4Addr(const uint32_t& ip_int) : ip(ip_int) {}
    explicit Ipv4Addr(const string& ip_str);    
    ~Ipv4Addr() {this->ip = 0xFFFFFFFF;}    
    explicit operator string() const;
    explicit operator uint32_t() const { return ip; }    

    friend ostream& operator<<(ostream& o, const Ipv4Addr& i) { return o << string(i); }  
};