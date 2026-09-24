#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "deq.h"

Str int_to_str(Data d) {
  char *s;
  asprintf(&s, "%d", (int)(intptr_t)d);
  return s;
}

int main() {
  Deq q = deq_new();

    //fill q
    deq_head_put(q, (void *)(intptr_t)2);
    deq_head_put(q, (void *)(intptr_t)1);
    deq_tail_put(q, (void *)(intptr_t)3);
    deq_tail_put(q, (void *)(intptr_t)4);

    //initial state
    char *s = deq_str(q, int_to_str);
    printf("Initial (Len: %d): %s\n", deq_len(q), s);
    free(s);

    //ith showcase
    printf("Head index 1: %d\n", (int)(intptr_t)deq_head_ith(q, 1));
    printf("Tail index 1: %d\n", (int)(intptr_t)deq_tail_ith(q, 1));

    //get showcase
    printf("Head Get: %d\n", (int)(intptr_t)deq_head_get(q));
    printf("Tail Get: %d\n", (int)(intptr_t)deq_tail_get(q));

    s = deq_str(q, int_to_str);
    printf("After Gets (Len: %d): %s\n", deq_len(q), s);
    free(s);

    //put data back in
    deq_head_put(q, (void *)(intptr_t)99);
    deq_tail_put(q, (void *)(intptr_t)88);

    //rem data
    deq_head_rem(q, (void *)(intptr_t)99);
    deq_tail_rem(q, (void *)(intptr_t)88);

    //final
    s = deq_str(q, int_to_str);
    printf("Final (Len: %d): %s\n", deq_len(q), s);
    free(s);

    //free it
    deq_del(q, NULL);
    return 0;
}
