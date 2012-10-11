CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -DNDEBUG -Wall
LDFLAGS = -L../ -lipc -Wl,-rpath,'$$ORIGIN'/..

EXE = producer consumer client server benchmark

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE)
