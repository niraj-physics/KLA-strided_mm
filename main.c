#include "config.h"
#include "helper.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void cpu_compute_v0(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE], double (*output)[N][N])
{
	// CPU implementations with out using split funcitons
	for (int b = 0; b < NBLK; ++b) {
		// particular block
		for (int bi = 0; bi < N; ++bi) {
			for (int bj = 0; bj < N; ++bj) {
				// elements
				double sum = 0.0f;
				for (int e = 0; e < BLOCK_SIZE * BLOCK_SIZE; ++e) {
					sum += data[bi][b * BLOCK_SIZE * BLOCK_SIZE + e] *
					       data[bj][b * BLOCK_SIZE * BLOCK_SIZE + e];
				}
				output[b][bi][bj] = sum;
			}
		}
	}
}

int main()
{
	// int *h_data, *g_data;
	size_t inbytes = sizeof(int16_t) * N * MATRIX_SIZE * MATRIX_SIZE;
	int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE];

	data = (int16_t (*)[MATRIX_SIZE * MATRIX_SIZE]) malloc(inbytes);

	// run this code to generate new input.bin
	fill_random(data);
	save_data(data);

	// assumes input.bin is available
	read_data(data);

	print_input_data(data);
	double (*output)[N][N] = malloc(sizeof(double) * NBLK * N * N);

	cpu_compute_v0(data, output);

	// print output
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", output[b][i][j]);
		}
		printf("\n");
	}

	free(data);
	free(output);
}
