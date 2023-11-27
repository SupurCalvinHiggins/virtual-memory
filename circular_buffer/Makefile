CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

SOURCE = main.cpp
EXECUTABLE = main
LDLIBS = -lbenchmark -lpthread

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(EXECUTABLE)
