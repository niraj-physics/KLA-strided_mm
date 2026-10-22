CC      = gcc
NVCC    = nvcc
CFLAGS  = -Wall -Wextra -O2

# GPU architecture. double atomicAdd needs sm_60 or newer.
# Override on the command line, e.g.  make gpu ARCH=sm_80
ARCH    = sm_75
NVFLAGS = -O2 -arch=$(ARCH)

.PHONY: all cpu gpu clean help

all: cpu gpu

cpu: main

gpu: main_gpu

help:
	@echo "make              build CPU (main) and GPU (main_gpu) versions"
	@echo "make cpu          build CPU version only"
	@echo "make gpu          build GPU version only (needs nvcc)"
	@echo "make gpu ARCH=sm_75   build GPU version for a different architecture"
	@echo "make clean        remove object files and binaries"

# CPU version (main.c + helper.c)
main: main.o helper.o
	$(CC) $(CFLAGS) -o $@ main.o helper.o

# GPU version (main.cu + helper.c)
main_gpu: main_gpu.o helper.o
	$(NVCC) $(NVFLAGS) -o $@ main_gpu.o helper.o

main.o: main.c config.h helper.h
	$(CC) $(CFLAGS) -c $< -o $@

helper.o: helper.c config.h helper.h
	$(CC) $(CFLAGS) -c $< -o $@

main_gpu.o: main.cu config.h helper.h
	$(NVCC) $(NVFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files"
	rm -f *.o main main_gpu
