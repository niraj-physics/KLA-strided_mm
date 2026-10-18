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

	for (int i = 0; i < N; ++i) {
		free(input_data[i]);
	}
}
