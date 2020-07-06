CXX ?= clang

.PHONY: all clean

all: slice

slice: main.cpp
	$(CXX) main.cpp -std=c++11 -o slice -Wall -Wextra -O3

clean:
	rm -f slice
