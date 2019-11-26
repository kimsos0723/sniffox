#include "assets.hpp"
#include "forward_proxy.hpp"

#include <mutex>
using std::mutex;
namespace ctrl {
/**
 * @todo Send and Receive Packets
*/
class NetworkManager {
   public:
    NetworkManager() = delete;
    NetworkManager(std::string, std::string, bool, int, std::function<PDU>);
    
   private:
    mutex s_queue_mutex;
    mutex r_queue_mutex;

    PacketSender __sender;
    SnifferConfiguration __config;
    Sniffer __sniffer;
    void push_recved_packet(const ForwardProxy&);
    std::optional<EthernetII> pop_sending_packet(const ForwardProxy&);
};

};  // namespace ctrl
