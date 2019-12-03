#include "assets.hpp"
#include <optional>
#include <mutex>
using std::mutex;
using std::optional;

namespace ctrl {
/**
 * @todo Send and Receive Packets
*/
class NetworkManager {
   public:
    NetworkManager() = delete;
    NetworkManager(std::string, std::string, bool, int);

    optional<EthernetII> pop_recved_qeue();
    void push_send_queue(EthernetII) noexcept;
    
   private:
    mutex s_queue_mutex;
    mutex r_queue_mutex;
    
    queue<EthernetII> send_queue;
    queue<EthernetII> recv_queue;

    void push_recved_qeue(EthernetII);
    optional<EthernetII> pop_send_queue();
    
    mutable PacketBuffer __recv_buffer;  // @brief packet-buffer what before processed
    mutable PacketBuffer __send_buffer;  // @brief packet-buffer what after processed       

    PacketSender __sender;
    SnifferConfiguration __config;
    Sniffer __sniffer;
};

};  // namespace ctrl
