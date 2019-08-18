#pragma once
#include "interface.h"
#include <pcap/pcap.h>
struct capturer {    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    struct bpf_program fp;
    const u_char* packet;
    bpf_u_int32 netp;
    pcap_t* setfilter(std::string filter_str = "")
    {
        bpf_program* filter = new bpf_program;
        char err_buf[PCAP_ERRBUF_SIZE] = {
            0,
        };
        pcap_t* desc = pcap_open_live(iface.dev().c_str(), 8192, 0, 512, err_buf);
        if (pcap_compile(desc, filter, filter_str.c_str(), 1, 0) == -1 and pcap_setfilter(desc, filter) == -1) {
            throw;
        }
        return desc;
    }
    Interface iface;
};