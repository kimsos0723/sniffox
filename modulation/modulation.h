#ifndef MODULATION_H
#define MODULATION_H

#include <tins/tins.h>
#include <iostream>

using namespace Tins;
using namespace std;

EthernetII& change_tcp(EthernetII& new_eth){
    cout<<"=========[SEND 2 Forwarding]========="<<endl; 
    cout<<"[ETHERNET II]" << endl;
    cout<<"hw dst : "<<new_eth.dst_addr().to_string()<<endl;                    
    cout<<"hw src : "<<new_eth.src_addr().to_string()<<endl;
    cout<<"ip dst : "<<new_eth.find_pdu<IP>()->dst_addr().to_string()<< endl;
    cout<<"ip src : "<<new_eth.find_pdu<IP>()->src_addr().to_string()<< endl;
    cout<<"src port : "<<new_eth.find_pdu<TCP>()->sport()<<endl;
    cout<<"dst port : "<<new_eth.find_pdu<TCP>()->dport()<<endl;
    byte_array dump = new_eth.rfind_pdu<RawPDU>().serialize();
    cout<<"[DUMP]  "<< endl<< string(dump.begin(), dump.end())<<endl;                                           
    return new_eth;
}

#endif //MODULATION_H
