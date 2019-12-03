#include "./assets.hpp"
#include "network_manager.hpp"
namespace ctrl {

    /**
     * @brief The struct used to identify the Session     
    */
struct Session {
    const MACAddress __hw;
    const IPv4Address __ip;

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
    ForwardProxy() = delete;                                               /// @warning deleted
    ForwardProxy(Session origin_src, Session origin_dst);  /// @brief default Constructor
    ForwardProxy(const ForwardProxy&) = delete;                            /// @warning deleted
    ForwardProxy& operator=(const ForwardProxy&) = delete;                 /// @warning deleted
    void runProxy(NetworkManager) _GLIBCXX_NORETURN;

   private:    
    const Session __origin_src;
    const Session __origin_dst;
};

};  // namespace ctrl
