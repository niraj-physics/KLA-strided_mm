#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void save_data(int *data)
{
	FILE *f1 = fopen("input.bin", "wb");
	fwrite(data, sizeof(int), MATRIX_SIZE * MATRIX_SIZE * N, f1);
	fclose(f1);
}

void read_data(int *data)
{
	FILE *f2 = fopen("input.bin", "rb");
	fread(data, sizeof(int), MATRIX_SIZE * MATRIX_SIZE * N, f2);
	fclose(f2);
};

void print_input_data(int *data)
{
	for (int i = 0; i < N; ++i) {
		printf("channel : %d \n", i);
		for (int j = i * MATRIX_SIZE * MATRIX_SIZE; j < MATRIX_SIZE * MATRIX_SIZE * (i + 1);
		     ++j) {
			printf("%2d", data[j]);
			if ((j + 1) % MATRIX_SIZE == 0)
				printf("\n");
		}
	}
}

void fill_random(int *data)
{
	srand(1);
	for (int i = 0; i < N * MATRIX_SIZE * MATRIX_SIZE; ++i) {
		data[i] = rand() % 10;
	}
}
