#include "config.h"
#include "helper.cuh"
#include <cstddef>
#include <cstdlib>
#include <cublas_v2.h>
#include <cuda_fp16.h>
#include <mma.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 256 // slightly fast but mio throttle
#define CHUNKS (BLOCK_SIZE * BLOCK_SIZE / CHUNK_SIZE)

/*
 * Kernel function gpu_compute_tiled_v3
 *
 */
__global__ void gpu_compute_tiled_v3(__half (*data)[MATRIX_SIZE * MATRIX_SIZE],
				     float (*output)[N][N])
{
	__shared__ __half temp1[N][CHUNK_SIZE + 4]; // incrased from 2 to 4

	int base = blockIdx.x * BLOCK_SIZE * BLOCK_SIZE + blockIdx.y * CHUNK_SIZE;

	// fill
	int tid = threadIdx.y * blockDim.x + threadIdx.x;
	int nthreads = blockDim.x * blockDim.y;
	for (int counter = 0; counter < CHUNK_SIZE / nthreads; ++counter) {
		int col = tid + nthreads * counter;
		for (int ii = 0; ii < N; ++ii)
			temp1[ii][col] = data[ii][base + col];
	}
	__syncthreads();

	//  each thread owns a 2x2 patch of the output
	int i = threadIdx.x * 2;
	int j = threadIdx.y * 2;

	float s00 = 0.f, s01 = 0.f, s10 = 0.f, s11 = 0.f;
	for (int k = 0; k < CHUNK_SIZE; ++k) {
		float a0 = __half2float(temp1[i][k]); // 4 loads ...
		float a1 = __half2float(temp1[i + 1][k]);
		float b0 = __half2float(temp1[j][k]);
		float b1 = __half2float(temp1[j + 1][k]);
		s00 += a0 * b0; // ... feed 4 FMAs
		s01 += a0 * b1;
		s10 += a1 * b0;
		s11 += a1 * b1;
	}

	atomicAdd(&output[blockIdx.x][i][j], s00);
	atomicAdd(&output[blockIdx.x][i][j + 1], s01);
	atomicAdd(&output[blockIdx.x][i + 1][j], s10);
	atomicAdd(&output[blockIdx.x][i + 1][j + 1], s11);
}

/*
 * TODO Write comments
 */
cublasStatus_t compute_cublas(cublasHandle_t cublasH, __half (*data)[MATRIX_SIZE * MATRIX_SIZE],
			      float (*output)[N][N])
{
	const float alpha = 1.0f, beta = 0.0f;
	int ld = MATRIX_SIZE * MATRIX_SIZE;
	int K = BLOCK_SIZE * BLOCK_SIZE;

	cublasStatus_t status = cublasGemmStridedBatchedEx(
	    cublasH, CUBLAS_OP_T, CUBLAS_OP_N, N, N, K, &alpha, data, CUDA_R_16F, ld, K, data,
	    CUDA_R_16F, ld, K, &beta, output, CUDA_R_32F, N, N * N, NBLK, CUBLAS_COMPUTE_32F,
	    CUBLAS_GEMM_DEFAULT_TENSOR_OP);

	return status;
}

/*
 * MMA Kernel
 */

using namespace nvcuda;

__global__ void wmma_kernel(__half (*data)[MATRIX_SIZE * MATRIX_SIZE], float (*output)[N][N])
{
	/* must be multiple of 8 thus 256+8 */
	__shared__ __align__(32) __half temp1[N][CHUNK_SIZE + 8];
	__shared__ __align__(32) float tile[N][N];

	int base = blockIdx.x * BLOCK_SIZE * BLOCK_SIZE + blockIdx.y * CHUNK_SIZE;

	// fill the shared memmory , coalesced
	int tid = threadIdx.x;
	int nthreads = 32;
	for (int col = tid; col < CHUNK_SIZE; col += nthreads)
		for (int ii = 0; ii < N; ++ii)
			temp1[ii][col] = data[ii][base + col];
	__syncthreads();

	wmma::fragment<wmma::matrix_a, N, N, N, half, wmma::row_major> a_frag;
	wmma::fragment<wmma::matrix_b, N, N, N, half, wmma::col_major> b_frag;
	wmma::fragment<wmma::accumulator, N, N, N, float> c_frag;
	wmma::fill_fragment(c_frag, 0.0f);

	for (int k = 0; k < CHUNK_SIZE; k += 16) {

		wmma::load_matrix_sync(a_frag, &temp1[0][k], CHUNK_SIZE + 8);
		wmma::load_matrix_sync(b_frag, &temp1[0][k], CHUNK_SIZE + 8);
		wmma::mma_sync(c_frag, a_frag, b_frag, c_frag);
	}
	wmma::store_matrix_sync(&tile[0][0], c_frag, N, wmma::mem_row_major);
	__syncwarp();

	for (int e = threadIdx.x; e < N * N; e += 32)
		atomicAdd(&output[blockIdx.x][e / N][e % N], tile[e / N][e % N]);
}

int main(int argc, char *argv[])
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
	cudaMemset(g_output, 0, outbytes);

	// assumes input.bin is available which is created by main_cpu
	read_data(h_data);
	cudaMemcpy(g_data, h_data, inbytes, cudaMemcpyHostToDevice);

	/*
	 * Depending on the parameter passed with ./main_gpu , it uses the
	 * corresponding functions to compute , three options are valid
	 * 1. custom : calls gpu_compute_tiled_v3
	 * 2. cublas : calls compute_cublas
	 * 3. mma    : calls wmma_kernel
	 */
	if (argc < 2) {
		printf("usage: %s -- custom|cublas|mma\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "--custom") == 0) {
		dim3 grid(NBLK, CHUNKS);
		dim3 block(N / 2, N / 2);
		gpu_compute_tiled_v3<<<grid, block>>>(g_data, g_output);

	} else if (strcmp(argv[1], "--cublas") == 0) {
		cublasHandle_t cublasH = NULL;
		cublasCreate_v2(&cublasH);
		cublasStatus_t status = compute_cublas(cublasH, g_data, g_output);
		if (status != CUBLAS_STATUS_SUCCESS)
			printf("Error");
		cublasDestroy(cublasH);

	} else if (strcmp(argv[1], "--mma") == 0) {
		dim3 grid(NBLK, CHUNKS);
		dim3 block(32);
		wmma_kernel<<<grid, block>>>(g_data, g_output);

	} else {
		printf("Unknown");
		return 1;
	}

	/* Error checking*/
	cudaError_t err = cudaGetLastError();
	if (err != cudaSuccess)
		printf("launch error: %s\n", cudaGetErrorString(err));
	err = cudaDeviceSynchronize();
	if (err != cudaSuccess)
		printf("kernel error: %s\n", cudaGetErrorString(err));

	/*Copy back result to cpu*/
	cudaMemcpy(h_output, g_output, outbytes, cudaMemcpyDeviceToHost);

	/* print both input and output for testing correctness*/
	print_input_data(h_data);
	// print output data
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
