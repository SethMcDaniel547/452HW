#include "balloc.h"

Balloc bcreate(unsigned int size, int l, int u) {
    //Need to create that whole structure that holds
    //size, min, max, mem pointer, and free list pointer
}

void   bdelete(Balloc pool) {
    //deallocates an allocator
}

void *balloc(Balloc pool, unsigned int size) {
    //requests a block of size bytes from an allocator
}

void  bfree(Balloc pool, void *mem) {
    //Grab that block of memory and overwrite the first part of it to store
    //the pointer to the next block and put it at the front
}

unsigned int bsize(Balloc pool, void *mem) {
    //return the size of an allocation (not the request size)
}

void bprint(Balloc pool) {
    //writes a textual representation of an allocator to stdout: a valuable debugging tool
}

