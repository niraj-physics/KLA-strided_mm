#include "config.h"
#include "helper.cuh"
#include <cstddef>
#include <cstdlib>
#include <cuda_fp16.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 512
#define CHUNKS (BLOCK_SIZE * BLOCK_SIZE / CHUNK_SIZE)

__global__ void gpu_compute_tiled_v3(__half (*data)[MATRIX_SIZE * MATRIX_SIZE],
				     float (*output)[N][N])
{

	// bank confilict fix
	__shared__ __half temp1[16][CHUNK_SIZE + 2];

	int i = threadIdx.x;
	int j = threadIdx.y;

	int base = blockIdx.x * BLOCK_SIZE * BLOCK_SIZE + blockIdx.y * CHUNK_SIZE;

	// coalesced load of the [16][512] tile
	int tid = j * blockDim.x + i;		// 0..255
	int nthreads = blockDim.x * blockDim.y; // 256

	int total_counter = CHUNK_SIZE / nthreads;
	for (int counter = 0; counter < total_counter; ++counter) {
		int col = tid + nthreads * counter;
		for (int ii = 0; ii < N; ++ii) {
			temp1[ii][col] = data[ii][base + col];
		}
	}

	__syncthreads();

	float sum = 0;
	for (int k = 0; k < CHUNK_SIZE; ++k)
		sum += __half2float(temp1[i][k]) * __half2float(temp1[j][k]);

	// memmory colalescing fix  interchange i <-> j
	atomicAdd(&output[blockIdx.x][j][i], (float)sum);
}

int main()
{
	size_t inbytes = sizeof(__half) * N * MATRIX_SIZE * MATRIX_SIZE;
	size_t outbytes = sizeof(float) * NBLK * N * N;

	__half(*h_data)[MATRIX_SIZE * MATRIX_SIZE];
	__half(*g_data)[MATRIX_SIZE * MATRIX_SIZE];
	h_data = (__half(*)[MATRIX_SIZE * MATRIX_SIZE]) malloc(inbytes);
	cudaMalloc(&g_data, inbytes);

	float (*h_output)[N][N];
	float (*g_output)[N][N];
	h_output = (float (*)[N][N])malloc(outbytes);
	cudaMalloc(&g_output, outbytes);
	cudaMemset(g_output, 0, outbytes); // TODO check this memset

	// assumes input.bin is available
	read_data(h_data);
	cudaMemcpy(g_data, h_data, inbytes, cudaMemcpyHostToDevice);
	print_input_data(h_data); // for testing correctness

	// Kernel config
	dim3 grid(NBLK, CHUNKS);
	dim3 block(N, N); // 256 threads per block
	gpu_compute_tiled_v3<<<grid, block>>>(g_data, g_output);

	cudaMemcpy(h_output, g_output, outbytes, cudaMemcpyDeviceToHost);

	// print output
	for (int b = 0; b < NBLK; ++b) {
		printf("block %3d", b);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				printf("  %5.0f", h_output[b][i][j]);
		}
		printf("\n");
	}

	free(h_data);
	free(h_output);
	cudaFree(g_data);
	cudaFree(g_output);
}
