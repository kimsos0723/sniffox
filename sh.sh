g++ src/hw_addr.cpp src/ip_addr.cpp\
 src/Pdu/pdu.cpp src/Pdu/rawpdu.cpp src/Pdu/arp.cpp src/Pdu/ethernet.cpp src/Pdu/ip.cpp \
 src/Network/sender.cpp src/arpspoof.cpp -lpcap -lpthread -I./Include -std=c++17  -o test

