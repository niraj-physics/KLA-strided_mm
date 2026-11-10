# custom Kernel 

## Duration [us]
226.82

## Roofline Analysis
![Roofline Plot](./custom_roofline.png)
| Memory Level | Arithmetic Intensity (FLOP/byte) | Performance (FLOP/s) |
|---|---:|---:|
| L1  | 8.00 | 2,366,988,713,318.28 |
| L2 | 15.96 | 2,366,988,713,318.28 |
| DRAM | 9.32 | 2,366,988,713,318.28 |

## Overall Analysis
Compute (SM) Throughput [%]	62.79

Memory Throughput [%]	111.31

> Memmory Bound 

**41% of FP32 peak performance**

# CUBLAS Kernel 
> cublasGemmAlgo_t : CUBLAS_GEMM_DEFAULT_TENSOR_OP

## Duration [us]
757.18

##  Roofline Analysis
![Roofline Plot](./cublas_roofline.png)
| Memory Level | Arithmetic Intensity (OP/byte) | Performance (OP/s) |
|---|---:|---:|
| L1 | 63.68 | 45,378,320,682,951.57 |
| L2 | 1,016.72 | 45,378,320,682,951.57 |
| DRAM | 660.67 | 45,378,320,682,951.57 |

## Overall Analysis
Compute (SM) Throughput [%]	69.78

Memory Throughput [%]	36.45

> Compute Bound

**69.72% of  FP32 peak performance**

# Tensor Core (MMA  Kernel)

## Duration [us]
171.65

## Roofline Analysis 
![Roofline](./tensor_roofline.png)
| Memory Level | Arithmetic Intensity (OP/byte) | Performance (OP/s) |
|---|---:|---:|
| L1 | 8.00 | 3,127,743,475,018.64 |
| L2 | 15.96 | 3,127,743,475,018.64 |
| DRAM | 10.61 | 3,127,743,475,018.64 |


## Overall Analysis
Compute (SM) Throughput [%]	27.74

Memory Throughput [%]	98.73

Memmory Bound 

**achieved 5.19% FP32 peak performance**

# Summary
TODO include durations, % of FP32 Peak performance , and compute/memmory bound
# Summary

| Kernel | Duration [us] | % of FP32 Peak | Bound |
|---|---:|---:|---|
| Custom | 226.82 | 41% | Memory bound |
| cuBLAS (`CUBLAS_GEMM_DEFAULT_TENSOR_OP`) | 757.18 | 69.72% | Compute bound |
| Tensor Core (MMA) | 171.65 | 5.19% | Memory bound |

The tensor core kernel is the fastest at 171.65 us, about 1.3x faster than the custom kernel. Both are limited by memory throughput (111.31% and 98.73% respectively), so neither reaches its compute ceiling. The cuBLAS kernel is compute bound, with the highest SM throughput (69.78%) and only 36.45% memory throughput.
