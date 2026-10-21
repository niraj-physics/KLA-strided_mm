// Original Problem
// Height of Matrix : 1024
// Weidth of Matrix : 1024
// No of Channels   : 16
// block_size	    : 128

#define MATRIX_SIZE 1024
#define N 16
#define BLOCK_SIZE 128
#define E (BLOCK_SIZE * BLOCK_SIZE)

#define TILE_SIZE 32
#define TILES (E / TILE_SIZE)

// Split the matrix into chunk of blocks
#define NBC (MATRIX_SIZE / BLOCK_SIZE)
#define NBLK (NBC * NBC)
