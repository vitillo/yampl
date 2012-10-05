CXX = g++
CXXFLAGS = -O2 -g -Wall -Iinclude -fPIC
LDFLAGS = -lzmq -shared

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIB = libipc.so
EXAMPLES = examples

all: $(LIB) $(EXAMPLES)

$(LIB): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(LIB)

$(EXAMPLES): $(LIB)
	$(MAKE) -C $@

.depend: $(SOURCES)
	rm -rf ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend

include .depend

.PHONY: clean
clean:
	$(MAKE) -C $(EXAMPLES) clean
	rm -rf $(LIB) $(OBJECTS)
