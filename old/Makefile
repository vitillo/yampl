CXX = g++
CC=gcc
CXXFLAGS = -O2 -g -Wall -Iinclude -fPIC -Izeromq/include
LDFLAGS = -shared -lpthread -lrt -luuid

SOURCES = $(wildcard src/pipe/*.cpp) \
	  $(wildcard src/shm/*.cpp)  \
	  $(wildcard src/zeromq/*.cpp)  \
	  $(wildcard src/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(SOURCES:.cpp=.d)
LIBYAMPL = libyampl.so
LIBZMQ = ./zeromq/src/.libs/libzmq.a
EXAMPLES = examples
TESTS = tests
PREFIX = /usr/lib/yampl
TAGS = tags

all: $(LIBZMQ) $(LIBYAMPL) $(EXAMPLES) $(TESTS) $(TAGS)

$(TAGS):
	ctags -R ./

$(LIBZMQ):
	+cd zeromq && CXX=${CXX} CC=${CC} ./configure --with-pic && make

-include $(DEPS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MT $@ -MMD -c $< -o $@

$(LIBYAMPL): $(OBJECTS) $(LIBZMQ)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LIBZMQ) -o $(LIBYAMPL)

.PHONY: $(EXAMPLES)
$(EXAMPLES): $(LIBYAMPL)
	+$(MAKE) -C $@

.PHONY: $(TESTS)
$(TESTS): $(LIBYAMPL)
	+$(MAKE) -C $@

install: all
	mkdir -p $(PREFIX)/{lib/pkgconfig,include}
	cp $(LIBYAMPL) $(PREFIX)/lib/
	cp yampl.pc $(PREFIX)/lib/pkgconfig/
	cp -R ./include/yampl $(PREFIX)/include

uninstall: 
	rm -rf $(PREFIX)/lib/$(LIBYAMPL)

check:
	./examples/benchmarks/benchmark
	@echo
	./examples/benchmarks/benchmark -y
	@echo
	./examples/benchmarks/benchmark -y -m 32 -n 128 -s 250000
	@echo
	./examples/benchmarks/benchmark -i pipe
	@echo
	./examples/benchmarks/benchmark -i pipe -y
	@echo
	./examples/benchmarks/benchmark -i pipe -m 32 -n 128 -s 250000
	@echo
	./examples/benchmarks/benchmark -i shm
	@echo
	./examples/benchmarks/benchmark -i shm -y
	@echo
	./examples/benchmarks/benchmark -i shm -m 32 -n 128 -s 250000
	@echo
	./examples/benchmarks/benchmark -c THREAD
	@echo
	./examples/benchmarks/benchmark -c DISTRIBUTED
	@echo
	./tests/size
	@echo
	./tests/calls
	@echo
	./tests/dest

.PHONY: clean
clean:
	$(MAKE) -C $(EXAMPLES) clean
	$(MAKE) -C $(TESTS) clean
	$(MAKE) -C zeromq clean
	rm -rf $(LIBYAMPL) $(OBJECTS) $(DEPS) tags
