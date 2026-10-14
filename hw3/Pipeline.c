#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Pipeline.h"
#include "deq.h"
#include "error.h"

typedef struct {
  Deq processes;
  int fg;			// not "&"
} *PipelineRep;

extern Pipeline newPipeline(int fg) {
  PipelineRep r=(PipelineRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->processes=deq_new();
  r->fg=fg;
  return r;
}

extern void addPipeline(Pipeline pipeline, Command command) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_tail_put(r->processes,command);
}

extern int sizePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  return deq_len(r->processes);
}

static void execute(Pipeline pipeline, Jobs jobs, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  int n=sizePipeline(r), pids[n], previous=STDIN_FILENO;
  for (int i=0; i<n && !*eof; i++) {
    int pipefd[2]={-1,-1};
    if (i+1<n && pipe(pipefd) < 0) ERROR("pipe() failed");
    int pid=execCommand(deq_head_ith(r->processes,i),pipeline,jobs,
                        previous, i+1<n ? pipefd[1] : STDOUT_FILENO,
                        eof,r->fg && n == 1);
    if (previous != STDIN_FILENO) close(previous);
    if (i+1<n) { close(pipefd[1]); previous=pipefd[0]; }
    else previous=STDIN_FILENO;
    if (pid) pids[i]=pid; else pids[i]=0;
  }
  if (previous != STDIN_FILENO) close(previous);
  /* Phase 1/5, tasks: wait for foreground commands and wire pipelines. */
  if (r->fg) {
    for (int i=0; i<n; i++) if (pids[i])
      while (waitpid(pids[i],0,0) < 0) {}
  } else {
    for (int i=0; i<n; i++) if (pids[i]) addJobPid(jobs,pids[i]);
  }
}

extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof) {
  PipelineRep r=(PipelineRep)pipeline;
  execute(pipeline,jobs,eof);
  freePipeline(pipeline);
}

extern void freePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_del(r->processes,freeCommand);
  free(r);
}
