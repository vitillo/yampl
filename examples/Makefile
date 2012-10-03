CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include
LDFLAGS = -L../ -lipc -lzmq -Wl,-rpath,$(PWD)

EXE = producer consumer client server
SOCKETS = pipe service

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJECTS) $(SOCKETS)
