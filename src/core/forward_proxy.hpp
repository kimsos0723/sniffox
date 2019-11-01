#include <tins/ethernetII.h>
#include <tins/hw_address.h>
#include <tins/ip.h>
#include <tins/ip_address.h>
#include <tins/tcp.h>
#include <tins/utils.h>

#include <queue>

using std::queue, std::pair;

using Tins::HWAddress;
using Tins::IPv4Address;

using Tins::EthernetII;
using Tins::IP;

using MACAddress = HWAddress<6>;

namespace ctrl {

/**
     * @brief The struct used to identify the Session     
    */
struct Session {
    MACAddress __hw;
    IPv4Address __ip;

    Session() = delete;  /// @warning deleted

    Session(MACAddress mac, IPv4Address ip) : __hw(mac), __ip(ip) {}  /// @brief default Constructor

    Session& operator=(const Session& other) { return *this; }  /// @brief assign-constructor
};

class ForwardProxy {
    /**     
     * @author xylitol
     * @date 2019-10-31
     * @brief IP Proxy To IP forwarding
     * @details Modify Ip packet dst to Origin dst
    */

   public:
    ForwardProxy() = delete;  /// @warning deleted

    ForwardProxy(Session origin_src, Session origin_dst);  /// @brief default Constructor

    ForwardProxy(const ForwardProxy&) = delete;             /// @warning deleted
    ForwardProxy& operator=(const ForwardProxy&) = delete;  /// @warning deleted
    void runProxy() _GLIBCXX_NORETURN ;
   protected:
    void push_recved_packet(const EthernetII&);  /// @todo It would be call by other functions to push received packet buffer
    EthernetII pop_sending_packet();             /// @todo It would be call by other functions to pop modified packet

   private:
    const Session __origin_src;
    const Session __origin_dst;

    mutable queue<EthernetII> __recv_buffer;  // @brief packet-buffer what before processed
    mutable queue<EthernetII> __send_buffer;  // @brief packet-buffer what after processed

    EthernetII pop_recved_packet();
    void push_sending_packet(const EthernetII&);

    void modify_packet(const MACAddress&, EthernetII&);  /// @todo Modify packet to forward
};

};  // namespace ctrl