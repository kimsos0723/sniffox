#ifndef _SNIFF_H_
#define _SNIFF_H_

#include <tins/ip_address.h>
#include <tins/hw_address.h>
#include <tins/sniffer.h>
#include <queue>

using std::queue;

using std::make_pair;
using std::make_shared;
using std::make_unique;
using std::pair;
using std::shared_ptr;
using std::unique_ptr;

using namespace Tins;

using MAC = HWAddress<6>;
using session_object = std::pair<shared_ptr<IPv4Address>, shared_ptr<MAC>>;

namespace sniffox
{

    class SessionInfo
    {
    public:
        // SessionInfo() = delete;
        SessionInfo(const IPv4Address &src_ip, const IPv4Address &dst_ip, const MAC &src_mac, const MAC &dst_mac);
        // virtual ~SessionInfo();
        session_object get_src_info();
        session_object get_dst_info();

    private:
        const IPv4Address _src_ip, _dst_ip;
        const MAC _src_mac, _dst_mac;
    };

    class PacketReceiveUnit
    {
    public:
        PacketReceiveUnit(SessionInfo);
        virtual ~PacketReceiveUnit();

    private:
        unique_ptr<Sniffer> sniffer;
        queue<Packet> _recv_queue;
    };

} // namespace sniffox

#endif // #ifndef _SNIFF_H_
