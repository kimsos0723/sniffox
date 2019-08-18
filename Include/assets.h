#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::regex;

using std::exception;
using std::ostream;
using std::string;

using std::uint16_t;
using std::uint64_t;
using std::uint8_t;

using std::copy;

using std::stringstream;

using std::vector;

static thread_local stringstream ss;

inline void clear_ss() { 
    ss.str("");
    ss.clear();
}