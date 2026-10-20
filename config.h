// Original Problem
// Height of Matrix : 1024
// Weidth of Matrix : 1024
// No of Channels   : 16
// block_size	    : 128

#define MATRIX_SIZE 8
#define N 2
#define BLOCK_SIZE 2
#define E (BLOCK_SIZE * BLOCK_SIZE)

// Split the matrix into chunk of blocks
#define NBC MATRIX_SIZE / BLOCK_SIZE
#define NBLK (NBC * NBC)

void save_data(int *data);

void print_input_data(int *data);

void fill_random(int *data);

void read_data(int *data);

void cpu_split_matrix(int *data, int (*output)[N][E]);

void cpu_compute(int (*data)[N][E], double (*output)[N][N]);
