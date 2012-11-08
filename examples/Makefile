CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -DNDEBUG -Wall
LDFLAGS = -L../ -lyampl -Wl,-rpath,'$$ORIGIN'/../../

EXE = producer_consumer/producer producer_consumer/consumer \
      client_server/client client_server/server \
      multithreaded/multithread \
      benchmarks/benchmark

all: $(EXE)

.PHONY: clean
clean:
	rm -rf $(EXE)
