#pragma once

#include "assets.h"

enum class IpErr {
    NOTIP,
};

IpErr NotIpAddrString()
{    
    return  IpErr::NOTIP;
}