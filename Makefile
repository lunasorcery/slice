CC ?= clang

.PHONY: all install uninstall clean

all: slice

slice: main.c
	$(CC) main.c -std=c89 -o slice -Wall -Wextra -O3

install: slice
	cp slice /usr/local/bin/slice

uninstall:
	rm -f /usr/local/bin/slice

clean:
	rm -f slice
