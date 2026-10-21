| Parameter     | Original problem |
|---------------|------------------|
| `matrix_size` | 1024             |
| `N` (channels)| 16               |
| `block_size`  | 128              |

Constraint: `matrix_size` must be divisible by `block_size`.

## Files

| File         | Purpose                                                                          |
|--------------|----------------------------------------------------------------------------------|
| `config.h`   | Problem sizes (`matrix_size`, `N`, `block_size`, `E`, tile and block counts)     |
| `helper.h`   | Prototypes for the helpers and CPU functions (C linkage, usable from `.c` and `.cu`) |
| `helper.c`   | Helpers: `fill_random`, `save_data`, `read_data`, `print_input_data`             |
| `main.c`     | CPU version: generates the input, splits into blocks, computes `X * X^T`         |
| `main.cu`    | GPU version: tiled CUDA kernel computing `X * X^T` per block                     |
| `Makefile`   | Build and clean targets for both versions                                        |
| `output.txt` | Sample run of the CPU version with the test values in the table above            |

## Build and run

Requires `gcc` and `make`. The GPU version also requires `nvcc` (CUDA toolkit).

```sh
make              # build both: main (CPU) and main_gpu (GPU)
make cpu          # build the CPU version only
make gpu          # build the GPU version only
make clean        # remove object files and binaries
make help         # list targets
```

Run the CPU version first, then the GPU version:

```sh
./main            # CPU reference
./main_gpu        # GPU version; its output should match the CPU output
```

| Target  |                                                          |
|---------|----------------------------------------------------------------------|
| `all`   | Builds `main` and `main_gpu` (default)                               |
| `cpu`   | Builds `main` from `main.o` and `helper.o` (gcc)                     |
| `gpu`   | Builds `main_gpu` from `main_gpu.o` and `helper.o` (nvcc)            |
| `clean` | Removes `*.o`, `main` and `main_gpu`                                 |
| `help`  | Prints the list of targets                                           |

Each `.o` file is rebuilt when its source file, `config.h` or `helper.h` changes.

### gpu architecture


```sh
make gpu ARCH=sm_75
```

## Changing the problem size

1. Edit `config.h`.
2. Run `make` and then `./main` (and `./main_gpu`).

## TODO

- [x] Naive CPU version to understand the problem statement
- [x] Tiled GPU kernel (`main.cu`)
- [X] Verify GPU output against the CPU output
- [ ] Change input data type from `int` to `int16`
- [ ] Improve Performance and Optimize
- [ ] CuBLAS implementations 
- [ ] TPU MMA implmentations 
