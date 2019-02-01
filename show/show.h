#ifndef SHOW_H
#define SHOW_H

#include <tins/tins.h>
#include <iostream>

using namespace Tins;
using namespace std;

EthernetII& show_tcp(EthernetII& new_eth){
    cout<<"=========[ S E N D ]========="<<endl; 
    cout<<"[ETHERNET II]" << endl;
    cout<<"hw dst : "<<new_eth.dst_addr().to_string()<<endl;                    
    cout<<"hw src : "<<new_eth.src_addr().to_string()<<endl;
    cout<<"[IP]"<<endl;
    cout<<"ip dst : "<<new_eth.find_pdu<IP>()->dst_addr().to_string()<< endl;
    cout<<"ip src : "<<new_eth.find_pdu<IP>()->src_addr().to_string()<< endl;
    cout<<"[TCP]"<<endl;
    cout<<"src port : "<<new_eth.find_pdu<TCP>()->sport()<<endl;
    cout<<"dst port : "<<new_eth.find_pdu<TCP>()->dport()<<endl;    
    byte_array dump = new_eth.rfind_pdu<RawPDU>().serialize();
    cout<<"[DUMP]  "<< endl<< string(dump.begin(), dump.end())<<endl;                                           
    cout<<endl<<endl;
    return new_eth;
    }

    void show_dns(DNS my_dns) {
        cout << "┎─[DNS]───────────────────┒" << endl;
        for (const auto &query : my_dns.queries()) {
            cout << "┃ [TYPE] " << DNS::QRType(query.type()) << endl;
            cout << "┃ [QTYPE] " << query.query_type() << endl;
            cout << "┃ [DNAME] " << query.dname() << endl;
            cout << "┃ [CLASS] " << query.query_class() << endl;        
        }
        auto dns_dump(my_dns.serialize());    
        cout << "┖─────────────────────────┚" << endl;
        return;

    }

    EthernetII& show_udp(EthernetII& new_eth) {
        cout<<"=========[ S E N D ]========="<<endl; 
        cout<<"[ETHERNET II]" << endl;
        cout<<"hw dst : "<<new_eth.dst_addr().to_string()<<endl;                    
        cout<<"hw src : "<<new_eth.src_addr().to_string()<<endl;                        
        cout<<"[IP]"<<endl;
        cout<<"ip dst : "<<new_eth.find_pdu<IP>()->dst_addr().to_string()<< endl;
        cout<<"ip src : "<<new_eth.find_pdu<IP>()->src_addr().to_string()<< endl;
        cout<<"[UDP]"<<endl;
        auto my_udp = new_eth.rfind_pdu<UDP>();
        cout<<"dport : "<<my_udp.dport()<<endl;    
        cout<<"sport : "<<my_udp.sport()<<endl;    
        if(my_udp.dport() == 53 || my_udp.sport() == 53){
            show_dns(my_udp.rfind_pdu<RawPDU>().to<DNS>());
        }else{
            auto inner_dump =my_udp.rfind_pdu<RawPDU>().serialize();
            cout<<"[DUMP]"<<endl<<string(inner_dump.begin(),inner_dump.end())<<endl;
        }
        cout<<endl<<endl;
        return new_eth;
    }

#endif //SHOW_H
