| Parameter     | Original problem | Current test value |
|---------------|------------------|--------------------|
| `matrix_size` | 1024             | 8                  |
| `N` (channels)| 16               | 2                  |
| `block_size`  | 128              | 2                  |

Constraint: `matrix_size` must be divisible by `block_size`.

## Files

| File         | Purpose                                                                          |
|--------------|----------------------------------------------------------------------------------|
| `config.h`   | Problem sizes (`matrix_size`, `N`, `block_size`, `E`) and helper prototypes      |
| `helper.c`   | Input helpers: `generate_input`, `fill_random`, `save_data`, `read_data`, `print_input_data` |
| `main.c`     | Generates the input, reads it back, splits into blocks, computes `X * X^T`       |
| `Makefile`   | Build and clean targets                                                          |
| `output.txt` | Sample run of the CPU version with the test values in the table above            |

### `input.bin` format

Raw binary `int` values: channel 0 flattened row by row
(`matrix_size * matrix_size` ints), then channel 1, and so on up to
channel `N - 1`. `generate_input()` (in `helper.c`) writes it using
`srand(1)`, so the data is the same on every run. The file is written
again at the start of every run of `main`.

## Build and run

Requires `gcc` and `make`.

```sh
make              # build main (main.o + helper.o)
./main            # generate input.bin and run the CPU version
make clean        # remove object files and the main binary
make help         # list targets
```

| Target  | What it does                                              |
|---------|-----------------------------------------------------------|
| `all`   | Builds `main` (default)                                   |
| `main`  | Links `main.o` and `helper.o`                             |
| `clean` | Removes `*.o` and `main` (`input.bin` is left in place)   |
| `help`  | Prints the list of targets                                |

Each `.o` file is rebuilt when its `.c` file or `config.h` changes.

## Sample output

`output.txt` is the output of one run with the test values above
(`matrix_size = 8`, `N = 2`, `block_size = 2`):

```sh
./main > output.txt
```

It has three parts:

1. **Input**: the two 8x8 channel matrices (`Channel 0`, `Channel 1`).
2. **Blocks**: the matrix is split into `(matrix_size / block_size)^2 = 16`
   blocks. Each line `block b ch c: ...` is one 2x2 block of channel `c`,
   flattened row by row.
3. **Result**: one line per block with the `N x N` matrix `X * X^T`, printed
   row by row. Here `X` has one row per channel and `E = block_size^2`
   columns. For example, `block 0   127  71  71  90` is the matrix
   `[[127, 71], [71, 90]]`: the first two numbers are the squared norm of
   channel 0 and the dot product of channels 0 and 1 in block 0, and the last
   two are the same dot product and the squared norm of channel 1.

The values depend on `rand()` from the C library. This file was produced with
glibc on Linux, and other C libraries may give different numbers. If you
change `config.h`, regenerate it with the command above.

## Changing the problem size

1. Edit `config.h`.
2. Run `make` and then `./main`.

## TODO

- [x] Naive CPU version to understand the problem statement
- [ ] Remove prints for debugging once Algorithm is verified
- [ ] Write GPU Kernels
