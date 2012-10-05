CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -DNDEBUG
LDFLAGS = -L../ -lipc -Wl,-rpath,$(PWD) $(shell pkg-config libzmq --libs --silence-errors)


EXE = producer consumer client server benchmark
SOCKETS = pipe service

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJECTS) $(SOCKETS)
