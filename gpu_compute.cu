#include "config.h"
#include <__clang_cuda_builtin_vars.h>
#include <__clang_cuda_runtime_wrapper.h>
#include <stddef.h>

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
