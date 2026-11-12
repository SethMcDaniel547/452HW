#include <stdlib.h>
#include <time.h>

#include "lawn.h"
#include "mole.h"
#include "mtq.h"
#include "threads.h"

typedef struct {
  Mtq queue;
  Lawn lawn;
} ProduceArg;

typedef struct {
  Mtq queue;
} ConsumeArg;

static void *produce(void *argument) {
  ProduceArg *arg=(ProduceArg *)argument;
  mtq_tail_put(arg->queue,mole_new(arg->lawn,0,0));
  return 0;
}

static void *consume(void *argument) {
  ConsumeArg *arg=(ConsumeArg *)argument;
  mole_whack(mtq_head_get(arg->queue));
  return 0;
}

int main() {
  srandom(time(0));
  const int n=10;
  Lawn lawn=lawn_new(0,0);
  Mtq queue=mtq_new(4);
  ProduceArg produce_args[n];
  ConsumeArg consume_args[n];
  void *producer_arguments[n];
  void *consumer_arguments[n];

  for (int i=0; i<n; i++) {
    produce_args[i]=(ProduceArg){queue,lawn};
    consume_args[i]=(ConsumeArg){queue};
    producer_arguments[i]=&produce_args[i];
    consumer_arguments[i]=&consume_args[i];
  }

  Threads producers=threads_new(n,produce,producer_arguments);
  Threads consumers=threads_new(n,consume,consumer_arguments);
  threads_join(producers);
  threads_join(consumers);
  threads_free(producers);
  threads_free(consumers);
  mtq_del(queue,0);
  lawn_free(lawn);
}
