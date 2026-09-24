#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include "bm.h"
#include "bbm.h"

typedef void *FreeList;

struct freelist_internal {
    size_t size;
    int l;
    int u;
    unsigned char *allocated;
    BBM bbms[64];
    void *heads[64];
};

typedef struct freelist_internal *fl_t;

extern FreeList freelistcreate(size_t size, void *base, int l, int u) {
    fl_t f = (fl_t)mmalloc(sizeof(struct freelist_internal));
    if (!f) return NULL;

    f->size = size;
    f->l = l;
    f->u = u;
    f->allocated = mmalloc(divup(size, e2size(l)));
    if (!f->allocated) {
        mmfree(f, sizeof(struct freelist_internal));
        return NULL;
    }
    memset(f->allocated, 0, divup(size, e2size(l)));

    for (int k = l; k <= u; k++) {
        f->bbms[k] = bbmcreate(size, k);
        f->heads[k] = NULL;
    }

    f->heads[u] = base;
    *(void **)base = NULL;

    return (FreeList)f;
}

extern void     freelistdelete(FreeList f, int l, int u) {
    if (!f) return;
    fl_t list = (fl_t)f;

    for (int k = l; k <= u; k++) {
        if (list->bbms[k]) {
            bbmdelete(list->bbms[k]);
        }
    }
    mmfree(list->allocated, divup(list->size, e2size(list->l)));
    mmfree(list, sizeof(struct freelist_internal));
}

extern void *freelistalloc(FreeList f, void *base, int e, int l) {
    (void)l; //I know I shouldnt but this felt better than changing the header file...
    fl_t list = (fl_t)f;
    int k = e;
    while (k <= list->u && list->heads[k] == NULL) {
        k++;
    }

    if (k > list->u) {
        return NULL;
    }
    void *block = list->heads[k];
    list->heads[k] = *(void **)block;
    while (k > e) {
        bbmset(list->bbms[k], base, block, k);
        k--;
        void *buddy = baddrinv(base, block, k);
        *(void **)buddy = list->heads[k];
        list->heads[k] = buddy;
        bbmclr(list->bbms[k], base, buddy, k);
    }
    bbmclr(list->bbms[e], base, block, e);
    list->allocated[((char *)block - (char *)base) / e2size(list->l)] =
        (unsigned char)(e + 1);

    return block;
}

extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l) {
    (void)l; //I know I shouldnt but this felt better than changing the header file...
    fl_t list = (fl_t)f;
    size_t index = (size_t)((char *)mem - (char *)base) / e2size(list->l);
    int k = e;
    while (k < list->u) {
        void *buddy = baddrinv(base, mem, k);
        void **curr = &(list->heads[k]);
        int found = 0;
        while (*curr != NULL) {
            if (*curr == buddy) {
                *curr = *(void **)*curr;
                found = 1;
                break;
            }
            curr = (void **)*curr;
        }
        if (!found) {
            break;
        }
        bbmclr(list->bbms[k], base, mem, k);
        if (mem > buddy) {
            mem = buddy;
        }
        k++;
    }
    *(void **)mem = list->heads[k];
    list->heads[k] = mem;
    list->allocated[index] = 0;
}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u) {
    (void)l; //I know I shouldnt but this felt better than changing the header file...
    fl_t list = (fl_t)f;
    uintptr_t start = (uintptr_t)base;
    uintptr_t address = (uintptr_t)mem;
    size_t min_size = e2size(l);

    if (address < start || address >= start + list->size ||
        (address - start) % min_size != 0) {
        return -1;
    }

    size_t index = (address - start) / min_size;
    if (index >= divup(list->size, min_size) || list->allocated[index] == 0) {
        return -1;
    }
    return list->allocated[index] - 1;
}

extern void freelistprint(FreeList f, int l, int u) {
    fl_t list = (fl_t)f;
    printf("FreeList print levels %d to %d\n", l, u);
    for (int k = u; k >= l; k--) {
        printf("Level %2d (size 2^%d): ", k, k);
        void *curr = list->heads[k];
        int count = 0;
        while (curr != NULL) {
            count++;
            curr = *(void **)curr;
        }
        printf("%d free blocks\n", count);
        if (list->bbms[k]) {
            printf("  BBM: ");
            bbmprt(list->bbms[k]);
        }
    }
}

extern int freelistsize_debug(FreeList f, void *base, void *mem, int l, int u) {
    fl_t list = (fl_t)f;
    size_t byte_offset = (size_t)((char *)mem - (char *)base);
    
    printf("\n=== FREELISTSIZE DEBUG START ===\n");
    printf("  Base Address: %p\n", base);
    printf("  Target Mem:   %p\n", mem);
    printf("  Byte Offset:  %zu (0x%zx)\n", byte_offset, byte_offset);
    printf("  Scanning from u = %d down to l = %d\n", u, l);

    for (int candidate_level = u; candidate_level >= l; candidate_level--) {
        size_t block_size = e2size(candidate_level);
        
        int aligned = (block_size > 0 && (byte_offset % block_size) == 0);
        int bbm_val = -1;

        if (aligned && list->bbms[candidate_level] != NULL) {
            bbm_val = bbmtst(list->bbms[candidate_level], base, mem, candidate_level);
        }

        printf("  [Level %2d] size = %-6zu | aligned = %d | bbm state = %d\n", 
               candidate_level, block_size, aligned, bbm_val);

        if (aligned && list->bbms[candidate_level] != NULL && bbm_val == 0) {
            printf("  >>> MATCH FOUND at level %d (block size %zu)!\n", candidate_level, block_size);
            printf("=== FREELISTSIZE DEBUG END ===\n\n");
            return candidate_level;
        }
    }

    printf("  >>> NO MATCH FOUND. Returning -1.\n");
    printf("=== FREELISTSIZE DEBUG END ===\n\n");
    return -1;
}
