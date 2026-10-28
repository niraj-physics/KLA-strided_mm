
#ifndef HELPER_CUH
#define HELPER_CUH

#include "config.h"
#include <cuda_fp16.h>

void save_data(__half (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void print_input_data(__half (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void fill_random(__half (*data)[MATRIX_SIZE * MATRIX_SIZE]);
void read_data(__half (*data)[MATRIX_SIZE * MATRIX_SIZE]);

#endif
