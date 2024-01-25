#
# GCC instrumentation for AFL
# -----------------------------------------
# 
# Designed and implemented by KB <million4862@gmail.com>
#
# 

PREFIX     ?= /usr/local
BIN_PATH    = $(PREFIX)/bin
HELPER_PATH = $(PREFIX)/lib/afl
DOC_PATH    = $(PREFIX)/share/doc/afl
MISC_PATH   = $(PREFIX)/share/afl

VERSION      = $(shell grep '^\#define VERSION ' ../config.h | cut -d '"' -f2)

CFLAGS     ?= -O3 -funroll-loops
CFLAGS     += -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign \
	      -DAFL_PATH=\"$(HELPER_PATH)\" -DDOC_PATH=\"$(DOC_PATH)\" \
	      -DBIN_PATH=\"$(BIN_PATH)\"
ifdef AFL_TRACE_PC
  CFLAGS    += -DUSE_TRACE_PC=1
endif

CXXFLAGS    ?= -O3 -funroll-loops
CXXFLAGS    += -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign \
               -DVERSION=\"$(VERSION)\" -Wno-variadic-macros

PLUGINDIR    = $(shell $(CXX) -print-file-name=plugin)
CXXFLAGS    += -I$(PLUGINDIR)/include

LDFLAGS     += -std=c++11

PROG         =  ../gcc-fast ../gcc_pass.so ../gcc-rt.o ../gcc-rt-32.o ../gcc-rt.64.o

COMM_HDR     = ../config.h ../debug.h

#CC           = gcc
#CXX          = g++


all: $(PROG)

../gcc-fast: gcc-fast.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
	ln -sf gcc-fast ../gcc-fast++
	ln -sf gcc-fast ../g++-fast

../gcc_pass.so: gcc_pass.so.o
	$(CXX) $(LDFLAGS) -shared -o $@ $<

gcc_pass.so.o: gcc_pass.cc 
	$(CXX) $(CXXFLAGS) -fPIC -fno-rtti -c -o $@ $<

../gcc-rt.o: gcc-rt.o.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

../gcc-rt-32.o: gcc-rt.o.c
	@printf "[*] Building 32-bit variant of the runtime (-m32)... "
	@$(CC) $(CFLAGS) -m32 -fPIC -c $< -o $@ 2>/dev/null; if [ "$$?" = "0" ]; then echo "success!"; else echo "failed (that's fine)"; fi

../gcc-rt.64.o: gcc-rt.o.c
	@printf "[*] Building 64-bit variant of the runtime (-m64)... "
	@$(CC) $(CFLAGS) -m64 -fPIC -c $< -o $@ 2>/dev/null; if [ "$$?" = "0" ]; then echo "success!"; else echo "failed (that's fine)"; fi

clean:
	rm -f ../gcc-fast gcc_pass.so.o ../gcc_pass.so ../gcc-rt.o ../gcc-rt-32.o ../gcc-rt.64.o ../gcc-fast++ ../g++-fast

