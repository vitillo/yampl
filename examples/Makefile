CXX = g++
CXXFLAGS = -std=c++11 -O2 -g -Wall -I../include -DNDEBUG -Wall
LDFLAGS = -L../ -lipc -Wl,-rpath,'$$ORIGIN'/..

EXE = producer consumer client server benchmark multithread

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE)
