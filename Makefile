CC = gcc
CFLAGS = -Wall -Wextra -O2

.PHONY: clean help all test_run

all : generate_input sequential

help :
	@echo "make 				build both programs"
	@echo "make clean			remove build Files"
	@echo "make test_run			runs sequentially on CPU"

input.bin : generate_input
	./generate_input > /dev/null

generate_input: config.h generate_input.c
	$(CC) $(CFLAGS) -o generate_input generate_input.c


sequential : sequential.c config.h 
	$(CC) $(CFLAGS) -o sequential sequential.c

clean :
	@echo "Cleaning build files"
	command rm -f *.o sequential generate_input input.bin

#run on cpu  
test_run : sequential input.bin
	./sequential
