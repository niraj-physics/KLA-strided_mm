#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void save_data(int (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	FILE *f1 = fopen("input.bin", "wb");
	for (int i = 0; i < N; ++i)
		fwrite(data[i], sizeof(int), MATRIX_SIZE * MATRIX_SIZE, f1);
	fclose(f1);
}

void read_data(int (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	FILE *f2 = fopen("input.bin", "rb");
	for (int i = 0; i < N; ++i)
		fread(data[i], sizeof(int), MATRIX_SIZE * MATRIX_SIZE, f2);
	fclose(f2);
};

void print_input_data(int (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	for (int i = 0; i < N; ++i) {
		printf("channel : %d \n", i);
		for (int j = 0; j < MATRIX_SIZE * MATRIX_SIZE; ++j) {
			printf("%3d", data[i][j]);
		}
		printf("\n");
	}
}

void fill_random(int (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	srand(1);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < MATRIX_SIZE * MATRIX_SIZE; ++j) {
			data[i][j] = rand() % 10;
		}
	}
}
