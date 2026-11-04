# Build and run

```sh
make              # build both: main (CPU) and main_gpu (GPU)
make clean        # remove object files and binaries
make help         # list targets
```

[IMPORTANT] Run the CPU version first, then the GPU version:
- The CPU version creates the input.bin file , which is the input test cases ,
  and then it computes and prints the output for refrence.
- The same input.bin is used by different GPU version , which computes and print the results

# Step by Step procedure 

```sh
make 
./main_cpu  >> cpu.txt # generates input.bin file -> compute -> saves the result in cpu.txt

./main_gpu --cublas >> cublas.txt   	# compute using cublas function -> saves the result in cublas.txt
./main_gpu --custom >> custom.txt   	# uses custom written kernel -> saves the result in custom.txt
./main_gpu --mma    >> mma.txt      	# uses wmma kernel

cmp cpu.txt cublas.txt 			          # cpu refrence and cublas implementations comparison 
cmp cpu.txt custom.txt 			          # cpu refrence and custom kernel comparison
cmp cpu.txt mma.txt			              # cpu refrence and mma kernel comparison
```

# Profiling Reports at ncu_reports/
1. ncu_reports/cublas_most_recent.ncu-rep
2. ncu_reports/mma_most_recent.ncu-rep
3. ncu_reports/custom_most_recent.ncu-rep

# Architecture

+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 580.82.07              Driver Version: 580.82.07      CUDA Version: 13.0     |
+-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  Tesla T4                       Off |   00000000:00:04.0 Off |                    0 |
| N/A   42C    P8             16W /   70W |       0MiB /  15360MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+

+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI              PID   Type   Process name                        GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+

# TODO

- [x] Naive CPU version to understand the problem statement
- [x] Tiled GPU kernel (`main.cu`)
- [X] Verify GPU output against the CPU output
- [X] Change input data type from `int` to `fp16`
- [ ] Improve Performance and Optimize
- [X] CuBLAS implementations 
- [X] TPU MMA implmentations 
- [ ] Calculate Theoretical Peak and roofline analysis
