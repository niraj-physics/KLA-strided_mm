// Original Problem
// Height of Matrix : 1024
// Weidth of Matrix : 1024
// No of Channels   : 16
// block_size	    : 128

#define matrix_size 8
#define N 2
#define block_size 2
#define E (block_size * block_size)

void save_data(int *data[N]);

void print_input_data(int *data[N], int height, int width);

void generate_input();

void fill_random(int *data[N]);

void read_data(int *data[N]);
