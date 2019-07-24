#pragma once

#include "assets.h"

namespace network {
    
enum class IpErr {
    NOTIP,
};

IpErr NotIpAddrString()
{
    return IpErr::NOTIP;
}
} // namespace network

