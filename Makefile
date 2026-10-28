# Build for the fp16 (__half) -> float channel-correlation project.
# Every source file contains __half / cuda_fp16.h, so everything is built with nvcc.
#
#   main.cu      -> GPU version  (binary: main_gpu)
#   main_cpu.cu  -> CPU reference (binary: main_cpu)
#   helper.cu    -> shared helpers (input file I/O, printing, random fill)

NVCC    = nvcc

# GPU architecture. Override on the command line, e.g.  make ARCH=sm_80
ARCH    = sm_75
NVFLAGS = -O2 -arch=$(ARCH) -Xcompiler -Wall,-Wextra

.PHONY: all cpu gpu clean help

all: cpu gpu
cpu: main_cpu
gpu: main_gpu

help:
	@echo "make                  build CPU reference (main_cpu) and GPU (main_gpu)"
	@echo "make cpu              build main_cpu only"
	@echo "make gpu              build main_gpu only"
	@echo "make ARCH=sm_80       build for a different GPU architecture"
	@echo "make clean            remove object files and binaries"

main_cpu: main_cpu.o helper.o
	$(NVCC) $(NVFLAGS) -o $@ $^

main_gpu: main.o helper.o
	$(NVCC) $(NVFLAGS) -o $@ $^

%.o: %.cu config.h helper.cuh
	$(NVCC) $(NVFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files"
	rm -f *.o main_cpu main_gpu input.bin
