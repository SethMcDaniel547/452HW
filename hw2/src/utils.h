#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

static const int bitsperbyte=8;

//calls mmap to get a pool of memory
extern void *mmalloc(size_t size);

//frees up the used memory
extern void mmfree(void *p, size_t size);

//Divide and round up
extern size_t divup(size_t n, size_t d);

//convert num bits to num bytes by bit shift
extern size_t bits2bytes(size_t bits);

//Exponent to size 2^e
extern size_t e2size(int e);

//Size to exponent (base 2)
extern int size2e(size_t size);

//Mark a specfic bit to a 1, OR
extern void bitset(void *p, int bit);

//Clear the bit to 0, AND a byte of all 1's except for the bit to make 0
extern void bitclr(void *p, int bit);

//Invert the bit, XOR
extern void bitinv(void *p, int bit);

//returns 0 or 1 depending on what it says
extern int  bittst(void *p, int bit);

#endif
