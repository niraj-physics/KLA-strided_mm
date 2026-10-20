#include "config.h"
#include <stddef.h>

void cpu_split_matrix(int *data, int (*output)[N][E])
{
	for (int row = 0; row < NBC; ++row) {
		for (int col = 0; col < NBC; ++col) {
			int origin = MATRIX_SIZE * BLOCK_SIZE * row + col * BLOCK_SIZE;
			int blk = row * NBC + col; // block id

			for (int channel = 0; channel < N; ++channel) {
				for (int i = 0; i < BLOCK_SIZE; ++i) {
					for (int j = 0; j < BLOCK_SIZE; ++j) {
						output[blk][channel][i * BLOCK_SIZE + j] =
						    data[channel * MATRIX_SIZE * MATRIX_SIZE +
							 origin + i * MATRIX_SIZE + j];
					}
				}
			}
		}
	}
}

void cpu_compute(int (*data)[N][E], double (*output)[N][N])
{
	// double (*output)[N][N] = malloc(sizeof(double) * NBLK * N * N);
	for (int b = 0; b < NBLK; ++b) {

		for (int i = 0; i < N; ++i) {
			for (int j = i; j < N; ++j) {
				double sum = 0;
				for (int k = 0; k < E; ++k)
					sum += (double)data[b][i][k] * data[b][j][k];
				output[b][i][j] = sum;
				output[b][j][i] = sum;
			}
		}
	}
}
