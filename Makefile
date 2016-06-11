BUILD ?= debug
VPATH = src include lib
CPPVERSION = -std=c++14

# Debug
ifeq ($(BUILD),debug)
__DEBUGINFO = -g
__OPTIMIZEFLAG_2 =
endif

# Release
ifeq ($(BUILD),release)
__DEBUGINFO = 
__OPTIMIZEFLAG_2 = -O3 -march=native -m64
endif

OBJ      = main.o node.o ptable.o transaction.o acs.o
BIN      = HUIMACS
CXXFLAGS = $(CPPVERSION) $(__DEBUGINFO) $(__OPTIMIZEFLAG_2) -pipe -Wall -I include 

.PHONY: all clean 

all: $(BIN)

clean:
	$(RM) lib/*.o
	$(RM) HUIMACS

$(BIN): $(OBJ)
	$(CXX) -o $@ $(__DEBUGINFO) $^

lib/main.o: main.cpp node.h ptable.h transaction.h cmdline.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

lib/node.o: node.cpp ptable.h transaction.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

lib/ptable.o: ptable.cpp ptable.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

lib/transaction.o: transaction.cpp transaction.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

lib/acs.o: acs.cpp acs.h node.h ptable.h transaction.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

