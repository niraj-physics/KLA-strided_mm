#ifndef HELPER_H
#define HELPER_H

#include "config.h"
#include <stdint.h>

// helper.c and main.c are compiled as C, main.cu as C++ (nvcc),
// so the declarations need C linkage to link together.
#ifdef __cplusplus
extern "C" {
#endif

void save_data(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void print_input_data(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void fill_random(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void read_data(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE]);
// void cpu_compute_v0(int (*data)[MATRIX_SIZE * MATRIX_SIZE], double (*output)[N][N]);

// void cpu_split_matrix(int (*data)[MATRIX_SIZE  * MATRIX_SIZE], int (*output)[N][E]);
// void cpu_compute(int (*data)[N][E], double (*output)[N][N]);

#ifdef __cplusplus
}
#endif

#endif
