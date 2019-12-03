#include <tins/tins.h>

#include <functional>
#include <queue>
#include <string>

using Tins::HWAddress;
using Tins::IPv4Address;

using Tins::EthernetII;
using Tins::IP;
using Tins::RawPDU;
using Tins::PDU;

using Tins::PacketSender;
using Tins::SnifferConfiguration;
using Tins::Sniffer;
using Tins::NetworkInterface;

using MACAddress = HWAddress<6>;
using PacketBuffer = std::queue<EthernetII>;

using std::function;
using std::queue;
using std::string;