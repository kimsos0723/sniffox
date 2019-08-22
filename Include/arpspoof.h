#include "include.h"
#include <queue>
#include <thread>
#include <mutex>

using namespace pdu;
class session {
public:
    session(string,string, string);
    Ipv4Addr sender_ip;
    Ipv4Addr target_ip;
    HWAddr sender_mac;
    HWAddr target_mac;
};

session::session(string sip,string tip, string dev_name)
{    
    Sender s(dev_name);
    auto si = Ipv4Addr(sip);
    auto ti = Ipv4Addr(tip);
    sender_ip = si;
    target_ip = ti;
    auto sm = s.ip_to_mac(sender_ip);   
    auto tm = s.ip_to_mac(target_ip);    
    sender_mac = sm;
    target_mac = tm;
}

class ArpSpoofer {
public:    
    ArpSpoofer(std::string device_name, vector<session> sesss)     
    :arp_sender(device_name), iface(device_name)
    {
        sessions = sesss;    
    }

    void push_relay_packet(pcap_t*);
    std::thread relay_thread();
    void run();
    Ethernet make_infection(session sess);
private:
    std::mutex mtx;       
    Interface iface;
    Sender arp_sender;    
    vector<session> sessions;
    std::queue<bytes> relay_qeue;
};

