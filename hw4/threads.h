#ifndef THREADS_H
#define THREADS_H

#include <stddef.h>

typedef void *(*ThreadFunction)(void *);
typedef void *Threads;

/*
 * Start one thread for each argument.  The argument array and its pointed-to
 * values must remain valid until threads_join returns.
 */
extern Threads threads_new(size_t count, ThreadFunction function, void *args[]);
extern void threads_join(Threads threads);
extern void threads_free(Threads threads);

#endif
