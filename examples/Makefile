CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -DNDEBUG
LDFLAGS = -L../ -lipc -lzmq -Wl,-rpath,$(PWD)

EXE = producer consumer client server benchmark
SOCKETS = pipe service

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJECTS) $(SOCKETS)
