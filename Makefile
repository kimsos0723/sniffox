DIR_ROOT	:= $(abspath .)
DIR_OBJ		:= $(DIR_ROOT)/obj
DIR_SRC		:= $(DIR_ROOT)/src

CXX := g++
CXX_FLAGS := -W -Wall -I$(DIR_ROOT)/include

TARGET = test
SHARED_OBJ = sniff.so

LDLIBS  = -ltins -ldl



.PHONY: all clean build edit init shared

all: build shared

$(DIR_OBJ):
	$(shell mkdir  -p $(DIR_OBJ))

build: $(DIR_OBJ) $(DIR_OBJ)/main.o
	$(CXX) -I$(DIR_ROOT)/include -o $(TARGET) $(DIR_OBJ)/main.o  -ldl 

$(DIR_OBJ)/main.o: $(DIR_SRC)/main.cc
	$(CXX) $(CXX_FLAGS) -c -o $(DIR_OBJ)/main.o $(DIR_SRC)/main.cc -ltins 

shared: $(DIR_OBJ) $(DIR_OBJ)/sniff.o
	$(CXX) $(CXX_FLAGS) -shared -o $(SHARED_OBJ)  $(DIR_OBJ)/sniff.o

$(DIR_OBJ)/sniff.o: $(DIR_SRC)/sniff.cc
	$(CXX) -Wl,-dynamic-list,symbol-list$(CXX_FLAGS) -rdynamic $(DIR_SRC)/sniff.cc -c -o $(DIR_OBJ)/sniff.o -fPIC 

clean:
	rm -rf $(DIR_OBJ)
	rm -f $(TARGET) $(SHARED_OBJ)
