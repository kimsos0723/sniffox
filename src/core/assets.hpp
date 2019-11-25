#include <tins/tins.h>

#include <functional>
#include <queue>
#include <string>

using Tins::HWAddress;
using Tins::IPv4Address;

using Tins::EthernetII;
using Tins::IP;
using Tins::PDU;

using Tins::PacketSender;
using Tins::SnifferConfiguration;
using Tins::Sniffer;

using MACAddress = HWAddress<6>;
using PacketBuffer = queue<PDU>;

using std::function;
using std::queue;
using std::string;