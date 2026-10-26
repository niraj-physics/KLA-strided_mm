#include "config.h"
#include "helper.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cuda_fp16.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 512
#define CHUNKS (BLOCK_SIZE * BLOCK_SIZE / CHUNK_SIZE)

__global__ void gpu_compute_tiled_v3(int16_t (*data)[MATRIX_SIZE * MATRIX_SIZE],
				     double (*output)[N][N])
{

	// bank confilict fix
	__shared__ int16_t temp1[16][CHUNK_SIZE + 2];

	int i = threadIdx.x;
	int j = threadIdx.y;

	int base = blockIdx.x * BLOCK_SIZE * BLOCK_SIZE + blockIdx.y * CHUNK_SIZE;

	// uncoalesced
	// for (int t = j; t < CHUNK_SIZE; t += 16) {
	// 	temp1[i][t] = data[i][base + t];
	// }

	// coalesced load of the [16][512] tile
	int tid = j * blockDim.x + i;		// 0..255
	int nthreads = blockDim.x * blockDim.y; // 256
	for (int idx = tid; idx < N * CHUNK_SIZE; idx += nthreads) {
		int r = idx / CHUNK_SIZE;
		int c = idx % CHUNK_SIZE;
		temp1[r][c] = data[r][base + c];
	}
	__syncthreads();

	double sum = 0;
	for (int k = 0; k < CHUNK_SIZE; ++k)
		sum += (double)((int)temp1[i][k] * (int)temp1[j][k]);

	atomicAdd(&output[blockIdx.x][i][j], sum);
}

int main()
{
	size_t inbytes = sizeof(int16_t) * N * MATRIX_SIZE * MATRIX_SIZE;
	size_t outbytes = sizeof(double) * NBLK * N * N;

	int16_t (*h_data)[MATRIX_SIZE * MATRIX_SIZE];
	int16_t (*g_data)[MATRIX_SIZE * MATRIX_SIZE];
	h_data = (int16_t (*)[MATRIX_SIZE * MATRIX_SIZE]) malloc(inbytes);
	cudaMalloc(&g_data, inbytes);

	double (*h_output)[N][N];
	double (*g_output)[N][N];
	h_output = (double (*)[N][N])malloc(outbytes);
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
