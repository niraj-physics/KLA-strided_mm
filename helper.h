#ifndef HELPER_H
#define HELPER_H

#include "config.h"

// helper.c and main.c are compiled as C, main.cu as C++ (nvcc),
// so the declarations need C linkage to link together.
#ifdef __cplusplus
extern "C" {
#endif

void save_data(int *data);

void print_input_data(int *data);

void fill_random(int *data);

void read_data(int *data);

void cpu_split_matrix(int *data, int (*output)[N][E]);

void cpu_compute(int (*data)[N][E], double (*output)[N][N]);

#ifdef __cplusplus
}
#endif

#endif
