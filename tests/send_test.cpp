#include "../src/core/network_manager.hpp"
int main(const int argc, const char* argv[]) {
    ctrl::NetworkManager netman = ctrl::NetworkManager(argv[1], "proc ether", false,PCAP_D_INOUT);
    
    for(int i = 0; i <500000; i++)
        netman.push_send_queue(EthernetII() / RawPDU(std::to_string(i)));

    auto t1 = netman.send_packet_loop_thread();
    t1.join();
}