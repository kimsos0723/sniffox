#include "include.h"
#include <queue>
#include <thread>
#include <mutex>

using namespace pdu;
class session {
public:
    session(string,string, string);
    Ipv4Addr* sender_ip;
    Ipv4Addr* target_ip;
    HWAddr* sender_mac;
    HWAddr* target_mac;
};

session::session(string sip,string tip, string dev_name)
{
    Sender s(dev_name);
    auto si = Ipv4Addr(sip);
    auto ti = Ipv4Addr(tip);
    sender_ip = &si;
    target_ip = &ti;
    auto sm = s.ip_to_mac(*sender_ip);   
    auto tm = s.ip_to_mac(*target_ip);    
    sender_mac = &sm;
    target_mac = &tm;
}

class ArpSpoofer {
public:
    // template <typename... Ss>
    // ArpSpoofer(std::string dev_name, Ss... sess)
    //     : arp_sender(iface), iface(dev_name)
    // {        
    //     (sessions.push_back(sess), ...);
    // }
    ArpSpoofer(vector<session> sesss) 
        : sessions(sesss)
    {        
    }
    void push_relay_packet();
    void run();
    Ethernet make_infection(session sess);
private:
    std::mutex mtx;
    capturer capturer_;    
    Interface iface;
    Sender arp_sender;    
    vector<session> sessions;
    std::queue<bytes> relay_qeue;
};

