CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -Wall
LDFLAGS = -L../ -lyampl -Wl,-rpath,'$$ORIGIN'/../

EXE = size calls 

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE)
