#include <cuda_fp16.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 8
#define BLOCK_SIZE 2
#define N 2

#define NBC (MATRIX_SIZE / BLOCK_SIZE)
#define NBLK (NBC * NBC)
#define NBLK (NBC * NBC)

void print_input_data(__half (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	for (int i = 0; i < N; ++i) {
		printf("channel : %d \n", i);
		for (int j = 0; j < MATRIX_SIZE * MATRIX_SIZE; ++j) {
			printf("%3.0f", __half2float(data[i][j]));
		}
		printf("\n");
	}
}

void fill_random(__half (*data)[MATRIX_SIZE * MATRIX_SIZE])
{
	srand(1);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < MATRIX_SIZE * MATRIX_SIZE; ++j) {
			data[i][j] = __float2half((float)(rand() % 10));
		}
	}
}

// taken from main_cpu.cu
// compute_cpu_v0 function
// parameter : __half (*data)[MATRIX_SIZE * MATRIX_SIZE], float (*output)[N][N])
void cpu_compute_v0(__half (*data)[MATRIX_SIZE * MATRIX_SIZE], float (*output)[N][N])
{
	for (int b = 0; b < NBLK; ++b) {
		// particular block
		for (int bi = 0; bi < N; ++bi) {
			for (int bj = 0; bj < N; ++bj) {
				// elements
				double sum = 0.0f;
				for (int e = 0; e < BLOCK_SIZE * BLOCK_SIZE; ++e) {
					sum +=
					    __half2float(
						data[bi][b * BLOCK_SIZE * BLOCK_SIZE + e]) *
					    __half2float(data[bj][b * BLOCK_SIZE * BLOCK_SIZE + e]);
				}
				output[b][bi][bj] = sum;
			}
		}
	}
}

// The main function
// allocates space for input data
// calls fill_random (helper.cu) : which fills the data with random integer
// calls save_data (helper.cu)  : which creates input.bin file
// prints the input data -- for correctness check
// calls cpu_compute_v0 : to compute
// prints the output data -- for correctness check
int main()
{
	size_t inbytes = sizeof(__half) * N * MATRIX_SIZE * MATRIX_SIZE;
	__half(*data)[MATRIX_SIZE * MATRIX_SIZE];

	data = (__half(*)[MATRIX_SIZE * MATRIX_SIZE]) malloc(inbytes);

	fill_random(data);

	print_input_data(data);
	float (*output)[N][N] = (float (*)[N][N])malloc(sizeof(float) * NBLK * N * N);

	cpu_compute_v0(data, output);

	// print output
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", output[b][i][j]);
		}
		printf("\n");
	}

	free(data);
	free(output);
}
