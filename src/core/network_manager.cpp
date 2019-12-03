#include "network_manager.hpp"
namespace ctrl {

NetworkManager::NetworkManager(const std::string& device, const std::string& snif_filter, bool promise_mod, pcap_direction_t direction)
    : __iface(device), __sniffer(device, __config) {
    __config.set_promisc_mode(promise_mod);
    __config.set_filter(snif_filter);
    __config.set_direction(direction);
}

optional<EthernetII> NetworkManager::pop_recved_qeue() {
    if (not __recv_buffer.empty()) {
        r_queue_mutex.lock();
        optional<EthernetII> top = __recv_buffer.front();
        __recv_buffer.pop();
        r_queue_mutex.unlock();
        return top;
    } else {
        return {};
    }
}

void NetworkManager::push_send_queue(EthernetII eth) {
    s_queue_mutex.lock();
    __send_buffer.push(eth);
    s_queue_mutex.unlock();
}

void NetworkManager::push_recved_qeue(EthernetII eth) {
    r_queue_mutex.lock();
    __recv_buffer.push(eth);
    r_queue_mutex.unlock();
}

optional<EthernetII> NetworkManager::pop_send_queue() {
    optional<EthernetII> oeth_result;
    s_queue_mutex.lock();
    if (!__send_buffer.empty()) {
        oeth_result = __send_buffer.front();
        __send_buffer.pop();
    } else {
        oeth_result = {};
    }
    s_queue_mutex.unlock();
    return oeth_result;
}

void NetworkManager::sniff_packet_loop() {
    while (true) {
        __recv_buffer.push(__sniffer.next_packet().pdu()->rfind_pdu<EthernetII>());
    }
}

void NetworkManager::send_packet_loop() {
    while (true) {        
        auto pkt = pop_send_queue();
        if (pkt.has_value()) {
            pkt.value().send(__sender, __iface);                
    }
}

};  // namespace ctrl
