#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	// create input and save it in input_data.bin
	int *input_data[N];
	for (int i = 0; i < N; ++i) {
		input_data[i] = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
	}

	fill_random(input_data);

	// verifications purpose
	print_input_data(input_data, MATRIX_SIZE, MATRIX_SIZE);

	int (*split_output)[N][E] = malloc(sizeof(int) * NBLK * N * E);

	printf("--- Split the Matrix into Chunk of Blocks at each Channels ---\n");

	cpu_split_matrix(input_data, split_output);

	for (int b = 0; b < NBLK; ++b)
		for (int c = 0; c < N; ++c) {
			printf("block %2d ch %d:", b, c);
			for (int k = 0; k < E; ++k)
				printf(" %2d", split_output[b][c][k]);
			printf("\n");
		}

	// compute transpose
	printf("--- Computing the Transpose at  Blocks locations ---\n");
	double (*output)[N][N] = malloc(sizeof(double) * NBLK * N * N);
	cpu_compute(split_output, output);

	// print
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", output[b][i][j]);
		}
		printf("\n");
	}

	free(split_output);
	free(output);

	for (int i = 0; i < N; ++i) {
		free(input_data[i]);
	}
}
