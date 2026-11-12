#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "threads.h"

typedef struct {
  pthread_t *ids;
  size_t count;
  size_t started;
} ThreadsRep;

static ThreadsRep *rep(Threads threads) {
  if (!threads) ERROR("zero pointer");
  return (ThreadsRep *)threads;
}

extern Threads threads_new(size_t count, ThreadFunction function, void *args[]) {
  if (!function) ERROR("zero function pointer");
  if (count && !args) ERROR("zero argument array");

  ThreadsRep *r=calloc(1,sizeof *r);
  if (!r) ERROR("calloc() failed");
  r->count=count;
  r->ids=count ? malloc(count*sizeof *r->ids) : 0;
  if (count && !r->ids) ERROR("malloc() failed");

  for (size_t i=0; i<count; i++) {
    int error=pthread_create(&r->ids[i],0,function,args[i]);
    if (error) {
      for (size_t j=0; j<r->started; j++)
        pthread_join(r->ids[j],0);
      free(r->ids);
      free(r);
      ERROR("pthread_create() failed: %s",strerror(error));
    }
    r->started++;
  }
  return r;
}

extern void threads_join(Threads threads) {
  ThreadsRep *r=rep(threads);
  for (size_t i=0; i<r->started; i++) {
    int error=pthread_join(r->ids[i],0);
    if (error) ERROR("pthread_join() failed: %s",strerror(error));
  }
  r->started=0;
}

extern void threads_free(Threads threads) {
  ThreadsRep *r=rep(threads);
  if (r->started) ERROR("threads still running");
  free(r->ids);
  free(r);
}
