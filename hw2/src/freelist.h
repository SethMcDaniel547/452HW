#ifndef FREELIST_H
#define FREELIST_H

#include <stdio.h>

typedef void *FreeList;

//This makes the freelist that you will pass in for the other functions
extern FreeList freelistcreate(size_t size, void *base, int l, int u);

//deletes the freelist and frees up what was stored
extern void     freelistdelete(FreeList f, int l, int u);

//grabs a block of memory for the user and updates the freelist and bitmap
extern void *freelistalloc(FreeList f, void *base, int e, int l);

//frees up the block at the passed in mem
extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l);

//figures out the size of the block of memory from the mem pointer
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);

//debug tool to see whats going on in freelist
extern void freelistprint(FreeList f, int l, int u);

#endif
