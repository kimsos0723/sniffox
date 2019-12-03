CXX := g++
CXXFLAGS := -c -std=c++17 -Wall -Wextra -O2

TARGET := sniffox
DIR_ROOT := $(abspath .)
DIR_SRC  := $(DIR_ROOT)/src
DIR_OBJ  := $(DIR_ROOT)/obj

all: build

build: $(TARGET)
	mkdir -p -v

