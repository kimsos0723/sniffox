#include "include.h"
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
    std::cout << "asdf"<<std::endl;
    auto tm = s.ip_to_mac(*target_ip);
    
    sender_mac = &sm;
    target_mac = &tm;
}

class ArpSpoofer {
public:
    template <typename... Ss>
    ArpSpoofer(std::string dev_name, Ss... sess)
        : arp_sender(iface), iface(dev_name)
    {        
        (sessions.push_back(sess), ...);
    }

    void run();
    Ethernet make_infection(session sess);
private:
    Interface iface;
    Sender arp_sender;
    capturer cap;
    vector<session> sessions;
};

