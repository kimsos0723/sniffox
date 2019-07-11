#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <signal.h>
#include "./arpspoof/arpspoof.h"

using namespace boost;
using namespace std;
using namespace boost::program_options;

void intro() 
{
    cout<<R"(
███████╗███╗   ██╗██╗███████╗███████╗ ██████╗ ██╗  ██╗
██╔════╝████╗  ██║██║██╔════╝██╔════╝██╔═══██╗╚██╗██╔╝
███████╗██╔██╗ ██║██║█████╗  █████╗  ██║   ██║ ╚███╔╝ 
╚════██║██║╚██╗██║██║██╔══╝  ██╔══╝  ██║   ██║ ██╔██╗ 
███████║██║ ╚████║██║██║     ██║     ╚██████╔╝██╔╝ ██╗
╚══════╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝      ╚═════╝ ╚═╝  ╚═╝
                                                      
)"<<endl;
}

void setsetPacketBufferOpt() {
    if(!system("sudo sysctl -w net.ipv4.ip_forward=1")){
        cout<<"Successful access to the net.ipv4.ip_forward"<<endl;
    }else {
        cout<<"- can not access 'net.ipv4.ip_forward'"<<endl;
        exit(-1);
    }
    
    if(!system("sudo sysctl -w net.ipv4.conf.all.send_redirects=1")){
        cout<<"Successful access to the net.ipv4.ip_forward"<<endl;
    }else {
        cout<<"- can not access 'net.ipv4.ip_forward'"<<endl;
        exit(-1);
    }
}

void exitHandler(int s) {
    system("sudo iptables -t nat -D POSTROUTING -s 0/0 -j MASQUERADE");
    system("sudo sysctl -w net.ipv4.ip_forward=0");
    system("sudo sysctl -w net.ipv4.conf.all.send_redirects=0");

    cout<<"\n\nGood Bye"<<endl;
    exit(0);
}

int main(int argc, char const *argv[]) {        
    intro();
    if (getuid()) {
        printf("%s", "You must be root!\n");
        return 0;
    }
    else printf("%s", "OK, you are root.\n");

    setsetPacketBufferOpt();
    signal (SIGINT,exitHandler);
    options_description desc("Allowed options");

    desc.add_options()
    ("help,h", "produce a help screen")
    ("version,v", "print the version number")
    ("target,t", program_options::value<std::string>(),"target ip address")
    ("ip,i", program_options::value<std::string>(), "your ip address")
    ("gateway,g", program_options::value<std::string>(), "your gateway ip address")
    ("device,d", program_options::value<std::string>(), "network device to use");    

    variables_map vm;

    store(parse_command_line(argc, argv, desc),vm);

    if(vm.count("help")) {
        std::cout << "Usage: regex [options]\n";
        std::cout << desc;
        return 0;
    }
    if(vm.count("version")) {
        std::cout << "Version 2.4\n";
        return 0;
    }    
    try {       
    } catch(...) {
        std::cout << "You should write '_ ./arpfox -h'"<<std::endl;
        return 1;
    }

    // auto spoof = new ArpSpoofer(Tins::IPv4Address(vm["target"].as<std::string>()), Tins::IPv4Address(vm["ip"].as<std::string>()), Tins::IPv4Address(vm["gateway"].as<std::string>()), vm["device"].as<std::string>());
    AttackInfo attackInfo = AttackInfo(
        Tins::IPv4Address(vm["target"].as<std::string>()), Tins::IPv4Address(vm["ip"].as<std::string>()), Tins::IPv4Address(vm["gateway"].as<std::string>()), vm["device"].as<std::string>()
    ); 
    auto spoof = new ArpSpoof(attackInfo);
    spoof->run();     
    delete spoof;
    return 0;
}
