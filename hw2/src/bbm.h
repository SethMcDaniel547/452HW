// A baddr bitmap, for the Buddy System.

#ifndef BBM_H
#define BBM_H

#include <stdio.h>

typedef void *BBM;

//makes a new buddy bit map
extern BBM  bbmcreate(size_t size, int e);

//deletes the buddy bit map
extern void bbmdelete(BBM b);

//sets the bitmap bit (makes it 1)
extern void bbmset(BBM b, void *base, void *mem, int e);

//clears the bitmap bit (makes it 0)
extern void bbmclr(BBM b, void *base, void *mem, int e);

//tests the bit at the specfic address in the buddy bit map
extern  int bbmtst(BBM b, void *base, void *mem, int e);

//print buddy bit map for debug
extern void bbmprt(BBM b);

// sets the e-th bit relative to the base and returns the address
extern void *baddrset(void *base, void *mem, int e);

// clears the e-th bit relative to the base and returns the address
extern void *baddrclr(void *base, void *mem, int e);

// inverts the e-th bit relative to the base and returns the address
extern void *baddrinv(void *base, void *mem, int e);

// tests the e-th bit relative to the base and returns either 0 or 1
extern int   baddrtst(void *base, void *mem, int e);

#endif
