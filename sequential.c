#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void read_data(int *data[N])
{
	FILE *f2 = fopen("input.bin", "rb");
	for (int i = 0; i < N; ++i) {
		fread(data[i], sizeof(data[i][0]), matrix_size * matrix_size, f2);
	};
	fclose(f2);
}

// Generate inputs (temprorarily for testing )
// for 1 channel Input matrix is of size (1024 * 1024) , it is flattened
// to a row There are 16 such channels ,thus input matrix have 16 rows ,
// thus each row corresponds to specific channels
//

int main()
{
	int *data[N];

	for (int i = 0; i < N; ++i) {
		data[i] = malloc(matrix_size * matrix_size * sizeof(int));
	}

	read_data(data); // data stored in input.bin acessed and stored in

	// Split the matrix into chunk of blocks
	int nbc = matrix_size / block_size;
	int nblk = nbc * nbc;
	int (*temp)[N][E] = malloc(sizeof(int) * nblk * N * E);

	printf("--- Split the Matrix into Chunk of Blocks at each Channels ---\n");

	for (int row = 0; row < nbc; ++row) {
		for (int col = 0; col < nbc; ++col) {
			int origin = matrix_size * block_size * row + col * block_size;
			int blk = row * nbc + col; // block id

			for (int channel = 0; channel < N; ++channel) {
				for (int i = 0; i < block_size; ++i) {
					for (int j = 0; j < block_size; ++j) {
						temp[blk][channel][i * block_size + j] =
						    data[channel][origin + i * matrix_size + j];
					}
				}
			}
		}
	}

	for (int b = 0; b < nblk; ++b)
		for (int c = 0; c < N; ++c) {
			printf("block %2d ch %d:", b, c);
			for (int k = 0; k < E; ++k)
				printf(" %2d", temp[b][c][k]);
			printf("\n");
		}

	// compute transpose
	printf("--- Computing the Transpose at  Blocks locations ---\n");

	double (*output)[N][N] = malloc(sizeof(double) * nblk * N * N);
	for (int b = 0; b < nblk; ++b) {

		for (int i = 0; i < N; ++i) {
			for (int j = i; j < N; ++j) {
				double sum = 0;
				for (int k = 0; k < E; ++k)
					sum += (double)temp[b][i][k] * temp[b][j][k];
				output[b][i][j] = sum;
				output[b][j][i] = sum;
			}
		}
	}

	// print output for debugging purpose only

	for (int b = 0; b < nblk; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", output[b][i][j]);
		}
		printf("\n");
	}

	free(temp);
	free(output);

	for (int i = 0; i < N; ++i) {
		free(data[i]);
	}
}
