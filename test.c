#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Height of Matrix : 1024
 * Weidth of Matrix : 1024
 * No of Channels   : 16
 * block_size       : 128
 */

#define matrix_size 8
#define N 2
#define block_size 2

/*
 * function save_data
 * saves the data to input.bin file in the following format
 * C0 [ single matrix corresponding to C0 flattened to row ]
 * C1 [ ...................................................]
 * .                              .
 * .                              .
 * .                              .
 * C15 [...................................................]
 */
void save_data(int *data[N])
{
	FILE *f1 = fopen("input.bin", "wb");
	for (int i = 0; i < N; ++i)
		fwrite(data[i], sizeof(int), matrix_size * matrix_size, f1);
	fclose(f1);
}

/*
 * function :  read_data
 * input paramater : int *data[N]
 * reads the data stored in input.bin file
 * and stores them in 'data'
 */
void read_data(int *data[N])
{
	FILE *f2 = fopen("input.bin", "rb");
	for (int i = 0; i < N; ++i) {
		fread(data[i], sizeof(data[i][0]), matrix_size * matrix_size,
		      f2);
	};
	fclose(f2);
}

/*
 * TODO Mention Comments
 */
void print_data(int *data[N], int height, int width)
{
	// print for debugging
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < height * width; ++j) {
			printf("%2d", data[i][j]);
			if ((j + 1) % width == 0)
				printf("\n");
		}
		printf("\n");
	}
}

/*
 * Generate inputs (temprorarily for testing )
 * for 1 channel Input matrix is of size (1024 * 1024) , it is flattened
 * to a row There are 16 such channels ,thus input matrix have 16 rows ,
 * thus each row corresponds to specific channels
 */

int main()
{
	int *input_data[N];
	int *data[N];

	for (int i = 0; i < N; ++i) {
		input_data[i] = malloc(matrix_size * matrix_size * sizeof(int));
		data[i] = malloc(matrix_size * matrix_size * sizeof(int));
	}

	srand(1);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < matrix_size * matrix_size; ++j)
			input_data[i][j] = rand() % 10;
	}

	save_data(input_data); // input_data saved in input.bin
	read_data(data);       // data stored in input.bin acessed and stored in
	print_data(data, matrix_size, matrix_size);

	// Split the matrix into chunk of blocks
	int origin;
	int *temp[N];

	for (int channel = 0; channel < N; ++channel)
		temp[channel] = malloc(sizeof(int) * matrix_size * matrix_size);

	int nbc = matrix_size / block_size;
	for (int row = 0; row < nbc; ++row) {
		for (int col = 0; col < nbc; ++col) {
			origin =
			    matrix_size * block_size * row + col * block_size;
			int blk = row * nbc + col;
			int base = blk * block_size * block_size;

			for (int channel = 0; channel < N; ++channel) {

				for (int i = 0; i < block_size; ++i) {
					for (int j = 0; j < block_size; ++j) {
						temp[channel]
						    [base + i * block_size +
						     j] = data[channel]
							      [i * matrix_size +
							       j + origin];
					}
				}
			}
		}
	}
	print_data(temp, matrix_size, matrix_size);

	for (int channel = 0; channel < N; ++channel)
		free(temp[channel]);

	for (int i = 0; i < N; ++i) {
		free(input_data[i]);
		free(data[i]);
	}
}
