#pragma once

#include "./assets.h"

using ip_t = uint32_t;

class Ipv4Addr {
public:
    ip_t ip;

public:
    Ipv4Addr() = delete;
    explicit Ipv4Addr(const Ipv4Addr& ip_bin) : ip(ip_bin.ip) {}
    explicit Ipv4Addr(const uint32_t& ip_int) : ip(ip_int) {}
    explicit Ipv4Addr(const string& ip_str);

    operator string() const;
    operator uint32_t() const { return ip; }
    void Clear() { ip = 0; }
};
