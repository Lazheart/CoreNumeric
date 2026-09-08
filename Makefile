CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

.PHONY: all run clean

all: programa

programa: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o programa

run: programa
	./programa

clean:
	rm -f programa