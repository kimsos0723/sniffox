#pragma once

#include "assets.h"

using bytes = vector<uint8_t>;
namespace pdu {

class PDU {
public:
    enum class PDUType : uint16_t {
        RAW,
        ETHERNET_II,
        IEEE802_3,
        DOT3 = IEEE802_3,
        RADIOTAP,
        DOT11,
        DOT11_ACK,
        DOT11_ASSOC_REQ,
        DOT11_ASSOC_RESP,
        DOT11_AUTH,
        DOT11_BEACON,
        DOT11_BLOCK_ACK,
        DOT11_BLOCK_ACK_REQ,
        DOT11_CF_END,
        DOT11_DATA,
        DOT11_CONTROL,
        DOT11_DEAUTH,
        DOT11_DIASSOC,
        DOT11_END_CF_ACK,
        DOT11_MANAGEMENT,
        DOT11_PROBE_REQ,
        DOT11_PROBE_RESP,
        DOT11_PS_POLL,
        DOT11_REASSOC_REQ,
        DOT11_REASSOC_RESP,
        DOT11_RTS,
        DOT11_QOS_DATA,
        LLC,
        SNAP,
        IP,
        ARP,
        TCP,
        UDP,
        ICMP,
        BOOTP,
        DHCP,
        EAPOL,
        RC4EAPOL,
        RSNEAPOL,
        DNS,
        LOOPBACK,
        IPv6,
        ICMPv6,
        SLL,
        DHCPv6,
        DOT1AD,
        DOT1Q,
        PPPOE,
        STP,
        PPI,
        IPSEC_AH,
        IPSEC_ESP,
        PKTAP,
        MPLS,
        UNKNOWN = 999,
        USER_DEFINED_PDU = 1000
    };

    enum class EtherType : uint16_t {
        UNKNOWN = 0,
        SPRITE = 0x0500, // Sprite
        IP = 0x0800, // IP
        ARP = 0x0806, // Address resolution
        MPLS = 0x8847, // MPLS
        REVARP = 0x8035, // Reverse ARP
        AT = 0x809B, // AppleTalk protocol
        AARP = 0x80F3, // AppleTalk ARP
        VLAN = 0x8100, // IEEE 802.1Q VLAN tagging
        QINQ = 0x88a8, // IEEE 802.1ad VLAN tagging
        OLD_QINQ = 0x9100, // IEEE 802.1ad VLAN tagging (old, deprecated, value)
        IPX = 0x8137, // IPX
        IPV6 = 0x86dd, // IP protocol version 6
        PPPOED = 0x8863, // PPPoE Discovery
        PPPOES = 0x8864, // PPPoE Session
        EAPOL = 0x888e, // EAPOL
        LOOPBACK = 0x9000 // used to test interfaces
    };

    PDU()
    {
        inner_pdu_ = nullptr;
        parent_pdu_ = nullptr;
    }
    virtual ~PDU();
    size_t size() const;

    PDU* inner_pdu() const { return inner_pdu_; }
    PDU* parent_pdu() const { return parent_pdu_; }
    void inner_pdu(PDU* next_pdu) { this->inner_pdu_ = next_pdu; }
    void parent_pdu(PDU* befor_pdu) { this->inner_pdu_ = befor_pdu; }

    virtual PDUType pdu_type() const = 0;
    bytes serialize();

    virtual void deserialize(bytes& b) = 0;
    virtual void write_serialization(bytes& bs) = 0;
    virtual size_t header_size() const = 0;
    virtual size_t trailer_size() const = 0;
    bytes serialize(PDU& r)
    {
        bytes res(this->serialize());
        bytes rbytes = r.serialize();
        res.insert(res.end(), rbytes.begin(), rbytes.end());
        return res;
    }

private:
    PDU* inner_pdu_;
    PDU* parent_pdu_;
};
template <typename T>
T& operator/=(T& l, const PDU& r)
{
    PDU* last = &l;
    while (last->inner_pdu()) {
        last = last->inner_pdu();
    }
    last->inner_pdu(const_cast<PDU*>(&r));
    return l;
}

}
