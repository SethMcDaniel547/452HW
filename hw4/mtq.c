#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "mtq.h"

typedef struct {
  Deq queue;
  size_t capacity;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} MtqRep;

static MtqRep *rep(Mtq queue) {
  if (!queue) ERROR("zero pointer");
  return (MtqRep *)queue;
}

extern Mtq mtq_new(size_t capacity) {
  MtqRep *r=calloc(1,sizeof *r);
  if (!r) ERROR("calloc() failed");
  r->queue=deq_new();
  r->capacity=capacity;
  if (pthread_mutex_init(&r->mutex,0))
    ERROR("pthread_mutex_init() failed");
  if (pthread_cond_init(&r->not_empty,0))
    ERROR("pthread_cond_init() failed");
  if (pthread_cond_init(&r->not_full,0))
    ERROR("pthread_cond_init() failed");
  return r;
}

extern size_t mtq_len(Mtq queue) {
  MtqRep *r=rep(queue);
  if (pthread_mutex_lock(&r->mutex))
    ERROR("pthread_mutex_lock() failed");
  size_t length=(size_t)deq_len(r->queue);
  if (pthread_mutex_unlock(&r->mutex))
    ERROR("pthread_mutex_unlock() failed");
  return length;
}

extern void mtq_tail_put(Mtq queue, Data data) {
  MtqRep *r=rep(queue);
  if (pthread_mutex_lock(&r->mutex))
    ERROR("pthread_mutex_lock() failed");
  while (r->capacity && (size_t)deq_len(r->queue) >= r->capacity) {
    int error=pthread_cond_wait(&r->not_full,&r->mutex);
    if (error) ERROR("pthread_cond_wait() failed: %s",strerror(error));
  }
  deq_tail_put(r->queue,data);
  if (pthread_cond_signal(&r->not_empty))
    ERROR("pthread_cond_signal() failed");
  if (pthread_mutex_unlock(&r->mutex))
    ERROR("pthread_mutex_unlock() failed");
}

extern Data mtq_head_get(Mtq queue) {
  MtqRep *r=rep(queue);
  if (pthread_mutex_lock(&r->mutex))
    ERROR("pthread_mutex_lock() failed");
  while (!deq_len(r->queue)) {
    int error=pthread_cond_wait(&r->not_empty,&r->mutex);
    if (error) ERROR("pthread_cond_wait() failed: %s",strerror(error));
  }
  Data data=deq_head_get(r->queue);
  if (pthread_cond_signal(&r->not_full))
    ERROR("pthread_cond_signal() failed");
  if (pthread_mutex_unlock(&r->mutex))
    ERROR("pthread_mutex_unlock() failed");
  return data;
}

extern void mtq_del(Mtq queue, DeqMapF free_data) {
  MtqRep *r=rep(queue);
  if (pthread_mutex_lock(&r->mutex))
    ERROR("pthread_mutex_lock() failed");
  deq_del(r->queue,free_data);
  r->queue=0;
  if (pthread_mutex_unlock(&r->mutex))
    ERROR("pthread_mutex_unlock() failed");
  if (pthread_mutex_destroy(&r->mutex))
    ERROR("pthread_mutex_destroy() failed");
  if (pthread_cond_destroy(&r->not_empty))
    ERROR("pthread_cond_destroy() failed");
  if (pthread_cond_destroy(&r->not_full))
    ERROR("pthread_cond_destroy() failed");
  free(r);
}
