CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -DNDEBUG -Wall
LDFLAGS = -L../ -lipc -Wl,-rpath,$(PWD)

EXE = producer consumer client server benchmark
SOCKETS = pipe service

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJECTS) $(SOCKETS)
