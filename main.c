#include "config.h"
#include "helper.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
int main()
{
	int *input_data = malloc(sizeof(int) * N * MATRIX_SIZE * MATRIX_SIZE);

	fill_random(input_data);

	save_data(input_data);
	// verifications purpose
	print_input_data(input_data);

	int (*split_output)[N][E] = malloc(sizeof(int) * NBLK * N * E);
	// //
	// printf("--- Split the Matrix into Chunk of Blocks at each Channels ---\n");
	// //
	cpu_split_matrix(input_data, split_output);
	//
	// for (int b = 0; b < NBLK; ++b)
	// 	for (int c = 0; c < N; ++c) {
	// 		printf("block %2d ch %d:", b, c);
	// 		for (int k = 0; k < E; ++k)
	// 			printf(" %2d", split_output[b][c][k]);
	// 		printf("\n");
	// 	}
	// //
	// // compute transpose
	// printf("--- Computing the Transpose at  Blocks locations ---\n");
	double (*output)[N][N] = malloc(sizeof(double) * NBLK * N * N);
	cpu_compute(split_output, output);

	// print output
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", output[b][i][j]);
		}
		printf("\n");
	}

	free(input_data);
	free(output);
	free(split_output);
}
