#include "./assets.hpp"

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
    void runProxy() _GLIBCXX_NORETURN;

    void push_recved_packet(const PDU&);  
    std::optional<PDU> pop_sending_packet();

   private:
    const Session __origin_src;
    const Session __origin_dst;

    mutable PacketBuffer __recv_buffer;  // @brief packet-buffer what before processed
    mutable PacketBuffer __send_buffer;  // @brief packet-buffer what after processed

    std::optional<PDU> pop_recved_packet();
    void push_sending_packet(const PDU&);
    void modify_packet(const MACAddress&, PDU&);  /// @todo Modify packet to forward
};

};  // namespace ctrl
