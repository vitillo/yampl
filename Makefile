CXX = g++
CXXFLAGS = -O2 -g -Wall -Iinclude -fPIC -Izeromq/include -Wall
LDFLAGS = -shared -lpthread -lrt -luuid

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIBIPC = libipc.so
LIBZMQ = ./zeromq/src/.libs/libzmq.a
EXAMPLES = examples

all: $(LIBZMQ) $(LIBIPC) $(EXAMPLES)
	ctags -R ./

$(LIBZMQ):
	cd zeromq && ./configure --with-pic && make

$(LIBIPC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LIBZMQ) -o $(LIBIPC)

.PHONY: $(EXAMPLES)
$(EXAMPLES): 
	$(MAKE) -C $@

.depend: $(SOURCES)
	rm -rf ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend

include .depend

.PHONY: clean
clean:
	$(MAKE) -C $(EXAMPLES) clean
	$(MAKE) -C zeromq clean
	rm -rf $(LIBIPC) $(OBJECTS) tags
