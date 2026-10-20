#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void gpu_compute(int *data, double (*output)[N][N])
{
	int row = blockIdx.x;
	int col = blockIdx.y;

	__shared__ int temp1[N][E];

	int origin = MATRIX_SIZE * BLOCK_SIZE * row + col * BLOCK_SIZE;
	int blk = row * NBC + col; // block id

	int i = threadIdx.x;
	int j = threadIdx.y;

	for (int channel = 0; channel < N; ++channel) {
		// temp1[channel][i * BLOCK_SIZE + j] = data[channel][origin + i * MATRIX_SIZE + j];
		temp1[channel][i * BLOCK_SIZE + j] =
		    data[channel * MATRIX_SIZE * MATRIX_SIZE + origin + i * MATRIX_SIZE + j];
	}
	__syncthreads();

	double sum = 0;
	for (int k = 0; k < E; ++k)
		sum += (double)temp1[i][k] * temp1[j][k];

	output[blk][i][j] = sum;
	output[blk][j][i] = sum;
}

int main()
{
	// prepare input >>>>
	int *h_data, *g_data;

	h_data = (int *)malloc(sizeof(int) * N * MATRIX_SIZE * MATRIX_SIZE);
	cudaMalloc(&g_data, sizeof(int) * N * MATRIX_SIZE * MATRIX_SIZE);

	read_data(h_data);
	cudaMemcpy(g_data, h_data, sizeof(int) * N * MATRIX_SIZE * MATRIX_SIZE,
		   cudaMemcpyHostToDevice);

	print_input_data(h_data);

	double (*h_output)[N][N], (*g_output)[N][N];
	h_output = (double (*)[N][N])malloc(sizeof(double) * NBLK * N * N);
	cudaMalloc(&g_output, sizeof(double) * NBLK * N * N);

	// configure kernnels
	dim3 block(BLOCK_SIZE, BLOCK_SIZE, 1);
	dim3 grid(NBC, NBC, 1);

	gpu_compute<<<grid, block>>>(g_data, g_output);

	// kernel calls

	cudaMemcpy(h_output, g_output, sizeof(double) * NBLK * N * N, cudaMemcpyDeviceToHost);

	// print outputs
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", h_output[b][i][j]);
		}
		printf("\n");
	}

	// deallocate memmory
	free(h_data);
	cudaFree(g_data);
	cudaFree(g_output);
}
