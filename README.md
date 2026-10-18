| Parameter     | Original problem | Current test value |
|---------------|------------------|--------------------|
| `matrix_size` | 1024             | 8                  |
| `N` (channels)| 16               | 2                  |
| `block_size`  | 128              | 2                  |

Constraint: `matrix_size` must be divisible by `block_size`.

## Files

| File                | Purpose                                                       |
|---------------------|---------------------------------------------------------------|
| `config.h`          | Problem sizes (`matrix_size`, `N`, `block_size`, `E`)         |
| `generate_input.c`  | Writes random input (values 0-9, fixed seed) to `input.bin`   |
| `sequential.c`      | Reads `input.bin`, splits into blocks, computes `X * X^T`     |
| `Makefile`          | Build, run and clean targets                                  |

### `input.bin` format

Raw binary `int` values: channel 0 flattened row by row
(`matrix_size * matrix_size` ints), then channel 1, and so on up to
channel `N - 1`. The generator uses `srand(1)`, so the data is the same on
every run.

## Build and run

Requires `gcc` and `make`.

```sh
make              # build generate_input and sequential
make test_run     # build if needed, generate input.bin, run the CPU version
make clean        # remove binaries and input.bin
make help         # list targets
```

| Target       | What it does                                              |
|--------------|-----------------------------------------------------------|
| `all`        | Builds `generate_input` and `sequential` (default)        |
| `input.bin`  | Runs `generate_input` to create the input file            |
| `test_run`   | Runs `sequential` on `input.bin` (rebuilding as needed)   |
| `clean`      | Removes binaries, object files and `input.bin`           |

## Changing the problem size

1. Edit `config.h`.
2. Run `make test_run`.



## TODO

- [x] Naive CPU version to understand the problem statement
- [ ] Remove prints for debugging once Algorithm is verified 
- [ ] Write GPU Kernels 
