#if !defined(ARPSPOOF_H)
#define ARPSPOOF_H

#include <iostream>
#include <tins/tins.h>

#include <thread>
#include <mutex>

#include <queue>
constexpr int BufSize = 10;

using std::cout;
using std::endl;
using std::queue;
using std::thread;

using namespace Tins;
using Buffer = std::queue<EthernetII>;

struct AttackInfo {
    IPv4Address targetIp;
    IPv4Address myIp;
    IPv4Address gwIp;    
        
    NetworkInterface iface;
    std::string deviceName;
    HWAddress<6> gwHwAddr;
    HWAddress<6> targetHwAddr;
    AttackInfo() : targetIp(nullptr), myIp(nullptr), gwIp(nullptr), iface(nullptr), deviceName(nullptr), gwHwAddr(nullptr), targetHwAddr(nullptr) {};
    AttackInfo(IPv4Address targetIp, IPv4Address myIp, IPv4Address gwIp, std::string deviceName) 
               : iface(NetworkInterface(deviceName)), myIp(myIp), gwIp(gwIp), targetIp(targetIp), deviceName(deviceName)
    {
        auto ps = PacketSender(this->iface);        
        this->gwHwAddr = Utils::resolve_hwaddr(this->iface, gwIp , ps);
        this->targetHwAddr = Utils::resolve_hwaddr(this->iface, targetIp, ps);          
    }
};

struct SendingInfo {
    NetworkInterface iface;
    NetworkInterface::Info info;
    std::string deviceName;
    SendingInfo(): iface(nullptr), deviceName(nullptr) {};
    SendingInfo(NetworkInterface iface, std::string deviceName) 
                : iface(iface), deviceName(deviceName){
                    info = iface.info();
                }
};

struct ArpStream {    
    thread toGwSetPacktRoutine, toTargetSetPacktRoutine;
    thread sendArpRoutine, arpPoisoningRoutine;
    std::mutex mutexLock;
    Buffer arpBuf;
    Buffer crackedPacketBuf;
    ARP toGwARP, toTargetARP; 
};

class ArpSpoof {
private:
    SendingInfo sendInfo;
    PacketSender arpSender;            
    AttackInfo attInfo;     
public:
    ArpStream arpStrm;   
    ArpSpoof() = delete;    
    ArpSpoof(const AttackInfo&);

    void MakeArpReply();
    void arpPoisoning();
        
    EthernetII crackPacketFromGw(const PDU& pdu);
    EthernetII crackPacketFromTarget(const PDU& pdu);

    void setCrackedPackToGwbuf(const PDU&);
    void setCrackedPackToTargetbuf(const PDU&);

    void ForwardPacketFromGw();
    void ForwardPacketFromTarget();

    void sendCrackedPacket();
    void run();
};

#endif

