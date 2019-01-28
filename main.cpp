#include "./arp/arp.h"

using namespace std;
using namespace Tins;

void start(){
    cout<<"\033[H\033[J";
    cout << "██╗  ██╗██╗   ██╗██╗      ██████╗ ███████╗███╗   ██╗██╗███████╗███████╗"<<endl;
    cout <<"╚██╗██╔╝╚██╗ ██╔╝██║     ██╔═══██╗██╔════╝████╗  ██║██║██╔════╝██╔════╝"<<endl;
    cout << " ╚███╔╝  ╚████╔╝ ██║     ██║   ██║███████╗██╔██╗ ██║██║█████╗  █████╗  "<<endl;
    cout << " ██╔██╗   ╚██╔╝  ██║     ██║   ██║╚════██║██║╚██╗██║██║██╔══╝  ██╔══╝  "<<endl;
    cout <<"██╔╝ ██╗   ██║   ███████╗╚██████╔╝███████║██║ ╚████║██║██║     ██║     "<<endl;
    cout <<"╚═╝  ╚═╝   ╚═╝   ╚══════╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝     "<<endl;
    cout <<"================================================================================="<<endl<<endl<<endl;
}


int main(int argc, char const *argv[]) {
    if(argc != 4){
        cout << "Usage :" << argv[0] <<" <Gateway IP> <Target IP> <Network Device> <My local IP>" << endl;
        return 1;
    }
    start();

    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info info = iface.addresses();
    IPv4Address gw, target, my_ip_addr;
    try{
        gw= argv[1];
        target = argv[2];
        my_ip_addr = argv[3];
    } catch(...){
        cout <<"Invalid ip found...\n";
        return 2;
    }
    do_arp_spoofing(iface,gw,target,info,my_ip_addr);
    return 0;
}

