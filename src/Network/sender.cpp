#include "Network/sender.h"
#include <ctime>

using namespace pdu;
HWAddr Sender::ip_to_mac(Ipv4Addr qip)
{        
SEND:
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
    time_t curr_time = time(NULL);
    int i = 0;
    {
        char errbuf[PCAP_ERRBUF_SIZE] = { 0, };
        pcap_t* desc = pcap_open_live(iface.dev().c_str(), 8192, 0, 512, errbuf);
        int res = 0;
        pcap_pkthdr* hdr;
        const u_char* pkt;
        while((res = pcap_next_ex(desc, &hdr, &pkt)) >= 0) {                
            time_t rcv_time = time(NULL);
            i++;
            if((rcv_time-curr_time) >= 2){
                if(i>3) {
                    throw;
                }                
            }            
            if(res == 0) continue;
            ARP a = ARP();
            Ethernet e = Ethernet();
            e/=a;
            bytes b = bytes(pkt, pkt+sizeof(Ethernet)+sizeof(ARP));
            e.deserialize(b);
            if (e.dst_mac().mac() != iface.mac()) continue;
            if( ((int)e.type()) != htons((uint16_t)Ethernet::EtherType::ARP)) continue;
            std::cout << e.src_mac() << std::endl;
            return e.src_mac();
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
    return true;
}