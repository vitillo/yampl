CXX = g++
CXXFLAGS = -O2 -g -Wall -Iinclude -fPIC $(shell pkg-config libzmq --cflags --silence-errors)
LDFLAGS = -shared $(shell pkg-config libzmq --libs --silence-errors)

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIB = libipc.so
EXAMPLES = examples

all: $(LIB) $(EXAMPLES)

$(LIB): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(LIB)

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
	rm -rf $(LIB) $(OBJECTS)
