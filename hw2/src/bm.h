// A general-purpose bitmap.

#ifndef BM_H
#define BM_H

#include <stdio.h>

typedef void *BM;

//makes a new bitmap with the provided number of bits
extern BM   bmcreate(size_t bits);

//deletes the bitmap
extern void bmdelete(BM b);

//sets the bit at i to 1
extern void bmset(BM b, size_t i);

//clears the bit at i to 0
extern void bmclr(BM b, size_t i);

//returns the bit at i
extern int  bmtst(BM b, size_t i);

//debug tool to print the bitmap in hex
extern void bmprt(BM b);

#endif
