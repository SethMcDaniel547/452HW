#ifndef MTQ_H
#define MTQ_H

#include <stddef.h>

#include "deq.h"

typedef void *Mtq;

/*
 * Create a thread-safe queue.  A capacity of zero means unbounded; a positive
 * capacity makes put operations wait while the queue is full.
 */
extern Mtq mtq_new(size_t capacity);
extern size_t mtq_len(Mtq queue);
extern void mtq_tail_put(Mtq queue, Data data);
extern Data mtq_head_get(Mtq queue);
/* All queue users must have stopped before this call. */
extern void mtq_del(Mtq queue, DeqMapF free_data);

#endif
