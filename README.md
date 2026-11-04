# Strided Batch Matrix Multiplications
TODO

## Build and run

```sh
make              # build both: main_cpu (CPU reference) and main_gpu (GPU)
make cpu          # build main_cpu only
make gpu          # build main_gpu only
make clean        # remove object files and binaries
make help         # list targets
```

> **IMPORTANT** Run the CPU version first, then the GPU version:
> - The CPU version creates the input.bin file , which is the input test cases ,
>  and then it computes and prints the output for refrence.
> - The same input.bin is used by different GPU version , which computes and print the results

## Step by Step procedure 

```sh
./main_cpu          > cpu.txt
./main_gpu --cublas > cublas.txt
./main_gpu --custom > custom.txt
./main_gpu --mma    > mma.txt

cmp cpu.txt cublas.txt && echo "cublas OK"
cmp cpu.txt custom.txt && echo "custom OK"
cmp cpu.txt mma.txt    && echo "mma OK"
```

## Profiling Reports at ncu_reports/
1. ncu_reports/cublas_most_recent.ncu-rep
2. ncu_reports/mma_most_recent.ncu-rep
3. ncu_reports/custom_most_recent.ncu-rep

## Details 
### Architecture
```text
name       compute_cap   driver_version   memory.total [MiB]   clocks.max.sm [MHz]   clocks.max.memory [MHz]
Tesla T4   7.5           580.82.07        15360 MiB            1590 MHz              5001 MHz
```
### NVCC Versions
```text

nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2025 NVIDIA Corporation
Built on Wed_Aug_20_01:58:59_PM_PDT_2025
Cuda compilation tools, release 13.0, V13.0.88
Build cuda_13.0.r13.0/compiler.36424714_0
```
## TODO

- [x] Naive CPU version to understand the problem statement
- [x] Tiled GPU kernel (`main.cu`)
- [X] Verify GPU output against the CPU output
- [X] Change input data type from `int` to `fp16`
- [ ] Improve Performance and Optimize
- [X] CuBLAS implementations 
- [X] Tensor Core implmentations 
- [ ] Calculate Theoretical Peak and roofline analysis
- [ ] Improve README
