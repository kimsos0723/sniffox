#include "forward_proxy.hpp"

namespace ctrl {

ForwardProxy::ForwardProxy(Session origin_src, Session origin_dst)
    : __origin_dst(origin_dst), __origin_src(origin_src) {}

/**
 * @brief Push to received-packet-buffer from other funcions
*/

void ForwardProxy::push_recved_packet(const EthernetII& ether) {
    __recv_buffer.push(ether);
}

EthernetII ForwardProxy::pop_recved_packet() {
    EthernetII result = EthernetII();
    if (!__recv_buffer.empty())
        result(__recv_buffer.front());
    return result;
}

void ForwardProxy::push_sending_packet(const EthernetII& ether){
    __send_buffer.push(ether)}

EthernetII ForwardProxy::pop_sending_packet() noexcept {
    EthernetII result = EthernetII();
    if (!__send_buffer.empty())
        result(__send_buffer.front());
    return result;
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
void ForwardProxy::modify_packet(const MACAddress& tohw, EthernetII& ether) {
    ether.dst_addr(tohw);
    ether.src_addr(ether.dst_addr);
}

void ForwardProxy::run() _GLIBCXX_NORETURN {
    // auto to_origin_dst = curry(
    //     modify_packet, __origin_dst.__hw);

    // auto to_origin_src = curry(
    //     modify_packet, __origin_src.__hw);

    while (true) {
        
        // MACAddress current_src_addr = current_packet.src_addr();

        // if (current_src_addr == __origin_src.__hw()) {
        //     to_origin_dst(current_packet);
        // } else if(current_src_addr == __origin_dst.__hw()) {
        //     to_origin_src(current_packet);
        // } else {

        // }

        
    }
}

};  // namespace ctrl