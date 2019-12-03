#include "../src/core/network_manager.hpp"
int main() {
    ctrl::NetworkManager netman = ctrl::NetworkManager("wlp2s0", "proc ether", false,PCAP_D_INOUT);
    
    for(int i = 0; i <500000; i++)
        netman.push_send_queue(EthernetII() / RawPDU(std::to_string(i)));

    netman.send_packet_loop();
}