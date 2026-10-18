#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void save_data(int *data[N])
{
	FILE *f1 = fopen("input.bin", "wb");
	for (int i = 0; i < N; ++i)
		fwrite(data[i], sizeof(int), matrix_size * matrix_size, f1);
	fclose(f1);
}

void read_data(int *data[N])
{
	FILE *f2 = fopen("input.bin", "rb");
	for (int i = 0; i < N; ++i) {
		fread(data[i], sizeof(data[i][0]), matrix_size * matrix_size, f2);
	};
	fclose(f2);
}

void print_input_data(int *data[N], int height, int width)
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

void fill_random(int *data[N])
{
	srand(1);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < matrix_size * matrix_size; ++j)
			data[i][j] = rand() % 10;
	}
}

void generate_input()
{
	int *input_data[N];

	/* prepare matrix */
	for (int i = 0; i < N; ++i) {
		input_data[i] = malloc(matrix_size * matrix_size * sizeof(int));
	}

	/* fill random integers only for */
	fill_random(input_data);

	save_data(input_data);
	// print_input_data(input_data, matrix_size, matrix_size);

	for (int i = 0; i < N; ++i) {
		free(input_data[i]);
	}
}
