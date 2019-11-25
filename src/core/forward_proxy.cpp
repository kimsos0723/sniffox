#include "forward_proxy.hpp"

namespace ctrl {

ForwardProxy::ForwardProxy(Session origin_src, Session origin_dst)
    : __origin_dst(origin_dst), __origin_src(origin_src) {}

/**
 * @brief Push to received-packet-buffer from other funcions
*/
void ForwardProxy::push_recved_packet(const EthernetII& eth) {
    __recv_buffer.push(eth);
}

std::optional<EthernetII> ForwardProxy::pop_recved_packet() noexcept {
    if (!__recv_buffer.empty())
        return __recv_buffer.front();
    return {};
}

void ForwardProxy::push_sending_packet(const EthernetII& eth) {
    __send_buffer.push(eth);
}

std::optional<EthernetII> ForwardProxy::pop_sending_packet() noexcept {
    if (!__send_buffer.empty())
        return __send_buffer.front();
    return {};
}

/**
 * @todo Use Curry (functional programming skill)
*/
template <typename F, typename... Args>
auto curry(F func, Args... args) {
    return [=](auto... lastParam) {
        return func(args..., lastParam);
    };
}

/**
 * @details src ip of packet to 
*/
void ForwardProxy::modify_packet(const MACAddress& tohw, EthernetII& eth) {
    eth.dst_addr(tohw);
    eth.src_addr(eth.dst_addr);
}

void ForwardProxy::runProxy() {
    auto to_origin_dst = curry(
        modify_packet, __origin_dst.__hw);

    auto to_origin_src = curry(
        modify_packet, __origin_src.__hw);

    while (true) {
        // auto current_packet = pop_recved_packet();
        if (auto ocurrent_packet = pop_recved_packet(); ocurrent_packet) {
            auto current_packet = ocurrent_packet.value();
            MACAddress current_src_addr = current_packet.src_addr();

            if (current_src_addr == __origin_src.__hw) {
                to_origin_dst(current_packet);
            } else if (current_src_addr == __origin_dst.__hw) {
                to_origin_src(current_packet);
            } else {
                continue;
            }
            push_sending_packet(current_packet);
        }
    }
}

};  // namespace ctrl