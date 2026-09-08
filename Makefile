CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -I. -Isrc

.PHONY: all run test clean

all: programa test_prog

programa: main.cpp src/core_numeric.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o programa

test_prog: test.cpp src/core_numeric.cpp
	$(CXX) $(CXXFLAGS) test.cpp -o test_prog

run: programa
	./programa

test: test_prog
	./test_prog

clean:
	rm -f programa test_prog