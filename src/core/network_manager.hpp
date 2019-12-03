#include <mutex>
#include <optional>
#include "assets.hpp"
using std::mutex;
using std::optional;

namespace ctrl {
/**
 * @todo Send and Receive Packets
*/
class NetworkManager {
   public:
    NetworkManager() = delete;
    NetworkManager(const std::string&, const std::string&, bool promise_mod, pcap_direction_t direction);

    optional<EthernetII> pop_recved_qeue();
    void push_send_queue(EthernetII);

    void sniff_packet_loop() _GLIBCXX_NORETURN;
    void send_packet_loop() _GLIBCXX_NORETURN;

   private:
    mutex s_queue_mutex;
    mutex r_queue_mutex;

    void push_recved_qeue(EthernetII);
    optional<EthernetII> pop_send_queue();

    PacketBuffer __recv_buffer;  // @brief packet-buffer what before processed
    PacketBuffer __send_buffer;  // @brief packet-buffer what after processed

    NetworkInterface __iface;
    PacketSender __sender;
    SnifferConfiguration __config;
    Sniffer __sniffer;
};

};  // namespace ctrl
