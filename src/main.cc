
#include <iostream>
#include <dlfcn.h>

#include <cstdlib>
#include <tins/tins.h>
#include <boost/program_options.hpp>

#include "sniff.hh"

using namespace std;
using namespace Tins;

int main(int argc, char const *argv[])
{
    void* phandle = nullptr;        
    // phandle = dlopen("./sniff.so", RTLD_LAZY);
    phandle = dlopen("/home/xylitol/Workspace/sniffox/sniff.so", RTLD_LAZY);
    if(not phandle) {
        fprintf(stderr, "ERROR:unknown library\n %s", dlerror());
        return 1;
    }
    sniffox::SessionInfo(*make_session_info)(const IPv4Address&, const IPv4Address&, const MAC&, const MAC&);
    
    make_session_info = \
        (sniffox::SessionInfo(*)(const IPv4Address&, const IPv4Address&, const MAC&, const MAC&))dlsym(phandle, "_ZN7sniffox11SessionInfoC1ERKN4Tins11IPv4AddressES4_RKNS1_9HWAddressILm6EEES8_");
    
    if(!make_session_info) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        dlclose(phandle);
        return 1;
    }
    
    return 0;
}
