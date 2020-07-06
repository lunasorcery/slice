CXX ?= clang

.PHONY: all install uninstall clean

all: slice

slice: main.cpp
	$(CXX) main.cpp -std=c++11 -o slice -Wall -Wextra -O3

install: slice
	cp slice /usr/local/bin/slice

uninstall:
	rm -f /usr/local/bin/slice

clean:
	rm -f slice
