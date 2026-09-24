#ifndef BALLOC_H
#define BALLOC_H

typedef void *Balloc;

//Creates a balloc struct that will be passed in with the other methods
extern Balloc bcreate(unsigned int size, int l, int u);

//Deletes the balloc struct after freeing all allocated memory
extern void   bdelete(Balloc ba);

//Grabs a block of memory and returns it as a void*
extern void *balloc(Balloc ba, unsigned int size);

//frees the memory at the void* passed in
extern void  bfree(Balloc ba, void *mem);

//returns the size of the allocated block
extern unsigned int bsize(Balloc ba, void *mem);

//a debug tool so you can see what is going on inside the allocater and freelist
extern void bprint(Balloc ba);

#endif
