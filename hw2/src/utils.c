#include "utils.h"
#include <stdio.h>
#include <sys/mman.h>

//calls mmap to get a pool of memory
extern void *mmalloc(size_t size) {
    return mmap(0,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
}

//frees up the used memory
extern void mmfree(void *p, size_t size) {
    if (p && size > 0) {
        munmap(p, size);
    }
}

//Divide and round up
extern size_t divup(size_t n, size_t d) {
    return (n + d - 1) / d;
}

//convert num bits to num bytes by bit shift
extern size_t bits2bytes(size_t bits) {
    return divup(bits, bitsperbyte);
}

//Exponent to size 2^e
extern size_t e2size(int e) {
    return (size_t)1 << e;
}

//Size to exponent (base 2)
extern int size2e(size_t size) {
    int e = 0;
    size_t s = 1;
    while (s < size) {
        s <<= 1;
        e++;
    }
    return e;
}


//Use a char array to go through bytes then you can do a OR/AND to that char after a bit shift

//Mark a specfic bit to a 1, OR
extern void bitset(void *p, int bit) {
    unsigned char *bytes = (unsigned char *)p;
    bytes[bit / bitsperbyte] |= (1 << (bit % bitsperbyte));
}

//Clear the bit to 0, AND a byte of all 1's except for the bit to make 0
extern void bitclr(void *p, int bit) {
    unsigned char *bytes = (unsigned char *)p;
    bytes[bit / bitsperbyte] &= ~(1 << (bit % bitsperbyte));
}

//Invert the bit, XOR
extern void bitinv(void *p, int bit) {
    unsigned char *bytes = (unsigned char *)p;
    bytes[bit / bitsperbyte] ^= (1 << (bit % bitsperbyte));
}

//returns 0 or 1 depending on what it says
extern int  bittst(void *p, int bit) {
    unsigned char *bytes = (unsigned char *)p;
    return (bytes[bit / bitsperbyte] >> (bit % bitsperbyte)) & 1;
}

