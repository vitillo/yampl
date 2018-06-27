CXX = g++
CXXFLAGS = -O2 -g -Wall -I../include -Wall
LDFLAGS = -L../ -lyampl -lpthread -lrt -luuid -Wl,-rpath,'$$ORIGIN'/../

EXE = size calls dest

all: $(EXE)

$(EXE): %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf $(EXE)
