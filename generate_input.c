#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// function save_data
//
// saves the data to input.bin file in the following format
// C0 [ single matrix corresponding to C0 flattened to row ]
// C1 [ ...................................................]
// .				  .
// .				  .
// .				  .
// C15 [...................................................]
void save_data(int *data[N])
{
	FILE *f1 = fopen("input.bin", "wb");
	for (int i = 0; i < N; ++i)
		fwrite(data[i], sizeof(int), matrix_size * matrix_size, f1);
	fclose(f1);
}

// TODO Mention Comments
void print_data(int *data[N], int height, int width)
{
	// print for debugging
	for (int i = 0; i < N; ++i) {
		printf("Channel %2d \n ", i);
		for (int j = 0; j < height * width; ++j) {
			printf("%2d", data[i][j]);
			if ((j + 1) % width == 0)
				printf("\n");
		}
		printf("\n");
	}
}

// TODO Mention comments
void fill_random(int *data[N])
{
	srand(1);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < matrix_size * matrix_size; ++j)
			data[i][j] = rand() % 10;
	}
}

int main()
{
	int *input_data[N];

	/* prepare matrix */
	for (int i = 0; i < N; ++i) {
		input_data[i] = malloc(matrix_size * matrix_size * sizeof(int));
	}

	/* fill random integers only for */
	fill_random(input_data);

	save_data(input_data);
	print_data(input_data, matrix_size, matrix_size);

	// /* Split the matrix into chunk of blocks*/
	// int nbc = matrix_size / block_size;
	// int nblk = nbc * nbc;
	// int (*temp)[N][E] = malloc(sizeof(int) * nblk * N * E);
	//
	// // for (int channel = 0; channel < N; ++channel)
	// //	temp[channel] = malloc(sizeof(int) * matrix_size * matrix_size);
	// //	block channel elements
	//
	// for (int row = 0; row < nbc; ++row) {
	// 	for (int col = 0; col < nbc; ++col) {
	// 		int origin = matrix_size * block_size * row + col * block_size;
	// 		int blk = row * nbc + col; // block id
	//
	// 		for (int channel = 0; channel < N; ++channel) {
	// 			for (int i = 0; i < block_size; ++i) {
	// 				for (int j = 0; j < block_size; ++j) {
	// 					temp[blk][channel][i * block_size + j] =
	// 					    data[channel][origin + i * matrix_size + j];
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	// // print output debugging purpose only
	// for (int b = 0; b < nblk; ++b)
	// 	for (int c = 0; c < N; ++c) {
	// 		printf("block %2d ch %d:", b, c);
	// 		for (int k = 0; k < E; ++k)
	// 			printf(" %2d", temp[b][c][k]);
	// 		printf("\n");
	// 	}
	//
	// // compute transpose
	// double (*output)[N][N] = malloc(sizeof(double) * nblk * N * N);
	// for (int b = 0; b < nblk; ++b) {
	//
	// 	for (int i = 0; i < N; ++i) {
	// 		for (int j = i; j < N; ++j) {
	// 			double sum = 0;
	// 			for (int k = 0; k < E; ++k)
	// 				sum += (double)temp[b][i][k] * temp[b][j][k];
	// 			output[b][i][j] = sum;
	// 			output[b][j][i] = sum;
	// 		}
	// 	}
	// }
	//
	// // print output for debugging purpose only
	//
	// for (int b = 0; b < nblk; ++b) {
	// 	printf("block %3d", b);
	// 	for (int i = 0; i < N; ++i) {
	// 		for (int j = 0; j < N; ++j)
	// 			printf("  %5.0f", output[b][i][j]);
	// 	}
	// 	printf("\n");
	// }
	//
	// free(temp);
	// free(output);

	for (int i = 0; i < N; ++i) {
		free(input_data[i]);
	}
}
