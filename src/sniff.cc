#include "sniff.hh"

namespace sniffox
{
    SessionInfo::SessionInfo(const IPv4Address &src_ip, const IPv4Address &dst_ip, const MAC &src_mac, const MAC &dst_mac)
        : _src_ip(src_ip), _dst_ip(dst_ip), _src_mac(src_mac), _dst_mac(dst_mac){};
    
    session_object SessionInfo::get_dst_info() {
        auto a = make_pair(
            make_shared<IPv4Address>(_dst_ip), 
            make_shared<MAC>(_dst_mac)
        );
        return a;
    }

    session_object SessionInfo::get_src_info() {
        auto a = make_pair(
            make_shared<IPv4Address>(_dst_ip), 
            make_shared<MAC>(_dst_mac)
        );
        return a;
    }

} // namespace sniffox
