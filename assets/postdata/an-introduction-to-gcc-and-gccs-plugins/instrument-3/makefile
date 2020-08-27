# The gcc used to compile our plugin (consider that is written in C++!)
HOST_GCC = g++

# The gcc that will support our plugin 
TARGET_GCC = gcc

# The name of the file we want to compile 
PLUGIN_SOURCE_FILES = instrument.cpp

# Plugins directory of GCC
GCC_PLUGINS_DIR = $(shell $(TARGET_GCC) -print-file-name=plugin)

# GCC CPP flags (we need position independent code and run time type info support disabled)
CXXFLAGS+= -I$(GCC_PLUGINS_DIR)/include -fPIC -fno-rtti

# our recipes
.PHONY: all clean test

all: inst_plugin.so

inst_plugin.so: $(PLUGIN_SOURCE_FILES)
	$(HOST_GCC) -shared $(CXXFLAGS) $^ -o $@

test: inst_plugin.so
	gcc target.c -fplugin=$(shell pwd)/inst_plugin.so -o bin

clean: 
	rm -f inst_plugin.so bin