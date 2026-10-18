CC     = gcc
CFLAGS = -Wall -Wextra -O2

.PHONY: all clean help

all: main

help:
	@echo "make              build sequential"
	@echo "make clean        remove build files and input.bin"

main: main.o helper.o
	$(CC) $(CFLAGS) -o $@ main.o helper.o

%.o: %.c config.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@echo "Cleaning build files"
	rm -f *.o main 
