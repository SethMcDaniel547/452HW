#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

static const int bitsperbyte=8;

//calls mmap somehow?
void *mmalloc(size_t size) {
    mmap(0,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)
}

//is it memclear? or something for mmap freeing
void mmfree(void *p, size_t size);

//Divide and round up
size_t divup(size_t n, size_t d);

//convert num bits to num bytes by dividing or something
size_t bits2bytes(size_t bits);

//Exponent to size 2^e
size_t e2size(int e);

//Size to exponent (base 2)
int size2e(size_t size);


//is bit the int that tells us how far to loop in to find the actual bit?
//Use a char array to go through bytes then you can do a OR/AND to that char after a bit shift

//Mark a specfic bit to a 1? OR
void bitset(void *p, int bit);

//Clear the bit? to 0??? AND a byte of all 1's except for the bit to make 0
void bitclr(void *p, int bit);

//Invert the bit? XOR
void bitinv(void *p, int bit);

//returns 0 or 1 depending on what it says
int  bittst(void *p, int bit);

#endif