// Original Problem
// Height of Matrix : 1024
// Weidth of Matrix : 1024
// No of Channels   : 16
// block_size	    : 128
#ifndef CONFIG_H
#define CONFIG_H
// ... existing defines ...
#define MATRIX_SIZE 1024
#define N 16
#define BLOCK_SIZE 128
#define E (BLOCK_SIZE * BLOCK_SIZE)

#define TILE_SIZE 16
#define TILES (E / TILE_SIZE)

// Split the matrix into chunk of blocks
#define NBC (MATRIX_SIZE / BLOCK_SIZE)
#define NBLK (NBC * NBC)

// for kernel - v2
#define SPLIT 16		      // CHG: CTAs per output block along the reduction
#define TILES_PER_CTA (TILES / SPLIT) // CHG: 64 tiles per CTA
#endif
