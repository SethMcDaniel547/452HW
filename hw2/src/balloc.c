#include "balloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "freelist.h"

struct balloc_internal {
    void *base;
    size_t size;
    int l;
    int u;
    FreeList freelist; 
};

typedef struct balloc_internal *bpool_t;

Balloc bcreate(unsigned int size, int l, int u) {
    int size_e = size2e(size);
    int pool_e = (size_e > u) ? size_e : u;
    size_t actual_size = e2size(pool_e);

    //setup the struct
    bpool_t pool = (bpool_t)mmalloc(sizeof(struct balloc_internal));
    if (!pool) return NULL;

    pool->l = l;
    pool->u = u;
    pool->size = actual_size;

    //grab the block of mem
    pool->base = mmalloc(pool->size);
    if (!pool->base) {
        mmfree(pool, sizeof(struct balloc_internal));
        return NULL;
    }

    //freelist holding blocks
    pool->freelist = freelistcreate(pool->size, pool->base, l, pool_e);
    if (!pool->freelist) {
        mmfree(pool->base, pool->size);
        mmfree(pool, sizeof(struct balloc_internal));
        return NULL;
    }

    return (Balloc)pool;
}

void   bdelete(Balloc ba) {
    //deallocates an allocator
    if (!ba) return;
    bpool_t pool = (bpool_t)ba;

    freelistdelete(pool->freelist, pool->l, size2e(pool->size));
    mmfree(pool->base, pool->size);
    mmfree(pool, sizeof(struct balloc_internal));
}

void *balloc(Balloc ba, unsigned int size) {
    if (!ba) return NULL;
    bpool_t pool = (bpool_t)ba;
    int e = size2e(size);
    if (e > pool->u) {
        return NULL;
    }
    if (e < pool->l) {
        e = pool->l;
    }
    return freelistalloc(pool->freelist, pool->base, e, pool->l);
}

void  bfree(Balloc ba, void *mem) {
    //Grab that block of memory and overwrite the first part of it to store
    //the pointer to the next block and put it at the front
    if (!ba || !mem) return;
    bpool_t pool = (bpool_t)ba;
    int e = freelistsize(pool->freelist, pool->base, mem, pool->l, ((size2e(pool->size) > pool->u) ? size2e(pool->size) : pool->u));
    if (e < 0) {
        return;
    }
    freelistfree(pool->freelist, pool->base, mem, e, pool->l);
}

unsigned int bsize(Balloc ba, void *mem) {
    if (!ba || !mem) return 0;
    bpool_t pool = (bpool_t)ba;
    int e = freelistsize(pool->freelist, pool->base, mem, pool->l, pool->u);
    if (e < 0) {
        return 0;
    }
    return (unsigned int)e2size(e);
}

void bprint(Balloc ba) {
    //writes a textual representation of an allocator to stdout: a valuable debugging tool
    if (!ba) return;
    bpool_t pool = (bpool_t)ba;

    printf("=== Buddy Allocator Debug Info ===\n");
    printf("Base Address: %p\n", pool->base);
    printf("Total Pool Size: %zu bytes (2^%d)\n", pool->size, ((size2e(pool->size) > pool->u) ? size2e(pool->size) : pool->u));
    printf("Constraints : l = %d (min block: 2^%d = %zu bytes)\n", pool->l, pool->l, e2size(pool->l));
    printf("              u = %d (max block: 2^%d = %zu bytes)\n", pool->u, pool->u, e2size(pool->u));
    
    freelistprint(pool->freelist, pool->l, ((size2e(pool->size) > pool->u) ? size2e(pool->size) : pool->u));
    printf("\n");
}

