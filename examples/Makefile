CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include
LDFLAGS = -L../ -lipc -lzmq -Wl,-rpath,$(PWD)

EXE = producer consumer client server
SOCKETS = pipe service

all: $(EXE)

.depend: $(SOURCES)
	rm -rf ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend

include .depend

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJECTS) $(SOCKETS)
