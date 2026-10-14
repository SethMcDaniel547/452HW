#include "Jobs.h"
#include "deq.h"
#include "error.h"
#include <sys/wait.h>
#include <unistd.h>

static void freePid(Data data) {
  free(data);
}

extern Jobs newJobs() {
  return deq_new();
}

extern void addJobs(Jobs jobs, Pipeline pipeline) {
  deq_tail_put(jobs,pipeline);
}

/* Phase 4, task: retain background process IDs and reap them without zombies. */
extern void addJobPid(Jobs jobs, int pid) {
  int *stored=malloc(sizeof *stored);
  if (!stored) ERROR("malloc() failed");
  *stored=pid;
  deq_tail_put(jobs,stored);
}

extern void reapJobs(Jobs jobs) {
  int *pid;
  for (int i=0; i<deq_len(jobs);) {
    pid=deq_head_ith(jobs,i);
    if (waitpid(*pid,0,WNOHANG) > 0) {
      free(deq_head_rem(jobs,pid));
    } else i++;
  }
}

extern void waitJobs(Jobs jobs) {
  /* Phase 6, task: wait for every active background process on exit. */
  while (deq_len(jobs)) {
    int *pid=deq_head_get(jobs);
    while (waitpid(*pid,0,0) < 0) {}
    free(pid);
  }
}

extern int sizeJobs(Jobs jobs) {
  return deq_len(jobs);
}

extern void freeJobs(Jobs jobs) {
  deq_del(jobs,freePid);
}
