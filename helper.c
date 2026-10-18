#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void save_data(int *data[N])
{
	FILE *f1 = fopen("input.bin", "wb");
	for (int i = 0; i < N; ++i)
		fwrite(data[i], sizeof(int), MATRIX_SIZE * MATRIX_SIZE, f1);
	fclose(f1);
}

void read_data(int *data[N])
{
	FILE *f2 = fopen("input.bin", "rb");
	for (int i = 0; i < N; ++i) {
		fread(data[i], sizeof(data[i][0]), MATRIX_SIZE * MATRIX_SIZE, f2);
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
		for (int j = 0; j < MATRIX_SIZE * MATRIX_SIZE; ++j)
			data[i][j] = rand() % 10;
	}
}
