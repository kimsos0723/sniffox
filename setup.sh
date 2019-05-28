#!/bin/bash 
sudo apt-get update
sudo apt install libpcap-dev libssl-dev cmake
git clone https://github.com/mfontanini/libtins.git
cd libtins/
mkdir build
cd build
cmake ../
make 
make install
sudo apt install libboost-program-options-dev 
cd ../
mkdir sniffox
git clone 
g++ -std=c++17 ./src/sslstrip/ssl.cpp \
./src/arpspoof/arpspoof.cpp ./src/main.cpp -lpthread -ltins -lboost_program_options -o arpfox.run