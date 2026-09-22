#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

static const int bitsperbyte=8;

//calls mmap somehow?
extern void *mmalloc(size_t size);

//is it memclear? or something for mmap freeing
extern void mmfree(void *p, size_t size);

//Divide and round up
extern size_t divup(size_t n, size_t d);

//convert num bits to num bytes by dividing or something
extern size_t bits2bytes(size_t bits);

//Exponent to size 2^e
extern size_t e2size(int e);

//Size to exponent (base 2)
extern int size2e(size_t size);


//is bit the int that tells us how far to loop in to find the actual bit?
//Use a char array to go through bytes then you can do a OR/AND to that char after a bit shift

//Mark a specfic bit to be what you want?? to a 1?
extern void bitset(void *p, int bit);

//Clear the bit? to 0???
extern void bitclr(void *p, int bit);

//Invert the bit?
extern void bitinv(void *p, int bit);

//returns 0 or 1 depending on what it says
extern int  bittst(void *p, int bit);

#endif
