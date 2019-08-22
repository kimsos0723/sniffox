#include "Network/sender.h"
#include <ctime>
#include <iomanip>
using namespace pdu;
HWAddr Sender::ip_to_mac(Ipv4Addr qip)
{
SEND:
    char errbuf[PCAP_ERRBUF_SIZE] = { 0, };
    pcap_t* desc = pcap_open_live(iface.dev().c_str(), 8192, 0, 512, errbuf);
    int res = 0;
    pcap_pkthdr* hdr;
    const u_char* pkt;
    ARP a = ARP();
    Ethernet eth = Ethernet();
    eth /= a;
    {                           
        ARP my_arp = ARP(ARP::Operation::REQ, iface.ip(), qip, iface.mac(), HWAddr("00:00:00:00:00:00"));
        auto broad = HWAddr("ff:ff:ff:ff:ff:ff");
        auto e = Ethernet(broad, iface.mac(), Ethernet::EtherType::ARP);
        auto s = e.serialize(my_arp);
        send_packet(s);
        e /= my_arp;
        e.deserialize(s);                
    }
    /*
        recv
    */    
    {            
        time_t ct = time(NULL);
        while((res = pcap_next_ex(desc, &hdr, &pkt)) >= 0) {                            
            time_t dt = time(NULL);            
            if(ct-dt >= 1) goto SEND;

            if(res == 0) continue;            
            bytes b = bytes(pkt, pkt+sizeof(Ethernet)+sizeof(ARP));
            eth.deserialize(b);
            if( ((uint16_t)eth.type()) != htons((uint16_t)Ethernet::EtherType::ARP)) continue;
            if(memcmp(eth.dst_mac().mac(), iface.mac().mac(), sizeof(HWAddr)) != 0) continue;            
            if(a.sender_ip_addr().ip() != qip.ip()) continue;
            std::cout << a.sender_ip_addr() << std::endl;            
            std::cout << eth.src_mac() << std::endl;
            pcap_close(desc);            
            return eth.src_mac();
        }                
    }
    
    return HWAddr("000000000000");
}

bool Sender::send_packet(bytes b)
{
    char errbuf[PCAP_ERRBUF_SIZE] = {
        0,
    };
    pcap_t* desc = pcap_open_live(iface.dev().c_str(), 8192, 0, 512, errbuf);
    if (!desc)
        return false;    
    pcap_sendpacket(desc, reinterpret_cast<uint8_t*>(&b[0]), b.size());    
    pcap_close(desc);
    return true;    
}