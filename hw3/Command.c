#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <readline/history.h>

#include "Command.h"
#include "error.h"

typedef struct {
  char *file;
  char **argv;
  char *input;
  char *output;
} *CommandRep;

#define BIARGS CommandRep r, int *eof, Jobs jobs
#define BINAME(name) bi_##name
#define BIDEFN(name) static void BINAME(name) (BIARGS)
#define BIENTRY(name) {#name,BINAME(name)}

static char *owd=0;
static char *cwd=0;
static void bi_history(BIARGS);

static void builtin_args(CommandRep r, int n) {
  char **argv=r->argv;
  for (n++; *argv++; n--);
  if (n)
    ERROR("wrong number of arguments to builtin command");
}

BIDEFN(exit) {
  builtin_args(r,0);
  *eof=1;
}

BIDEFN(pwd) {
  //pwd in the shell process
  builtin_args(r,0);
  char *path=getcwd(0,0);
  if (!path) ERROR("getcwd() failed");
  printf("%s\n",path);
  free(path);
}

BIDEFN(cd) {
  //cd and the previous-directory switch
  builtin_args(r,1);
  char *old=getcwd(0,0);
  char *target;
  if (strcmp(r->argv[1],"-")==0) {
    target=owd ? strdup(owd) : 0;
  } else {
    target=strdup(r->argv[1]);
  }
  if (!target || chdir(target)) {
    free(old); free(target);
    ERROR("chdir() failed");
  }
  free(owd); owd=old;
  free(cwd); cwd=target;
}

static int builtin(BIARGS) {
  typedef struct {
    char *s;
    void (*f)(BIARGS);
  } Builtin;
  static const Builtin builtins[]={
    BIENTRY(exit),
    BIENTRY(pwd),
    BIENTRY(cd),
    BIENTRY(history),
    {0,0}
  };
  int i;
  for (i=0; builtins[i].s; i++)
    if (!strcmp(r->file,builtins[i].s)) {
      builtins[i].f(r,eof,jobs);
      return 1;
    }

  return 0;
}

BIDEFN(history) {
  //print GNU History entries with history_list
  builtin_args(r,0);
  HIST_ENTRY **entries=history_list();
  if (entries)
    for (int i=0; entries[i]; i++)
      printf("%d %s\n",i+history_base,entries[i]->line);
}

static int is_builtin(CommandRep r) {
  return !strcmp(r->file,"exit") || !strcmp(r->file,"pwd") ||
         !strcmp(r->file,"cd") || !strcmp(r->file,"history");
}

static char **getargs(T_words words) {
  int n=0;
  T_words p=words;
  while (p) {
    p=p->words;
    n++;
  }
  char **argv=(char **)malloc(sizeof(char *)*(n+1));
  if (!argv)
    ERROR("malloc() failed");
  p=words;
  int i=0;
  while (p) {
    argv[i++]=strdup(p->word->s);
    p=p->words;
  }
  argv[i]=0;
  return argv;
}

extern Command newCommand(T_command command) {
  CommandRep r=(CommandRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->argv=getargs(command->words);
  r->file=r->argv[0];
  r->input=command->input ? strdup(command->input) : 0;
  r->output=command->output ? strdup(command->output) : 0;
  return r;
}

static void child(CommandRep r, int fg) {
  int eof=0;
  Jobs jobs=newJobs();
  if (builtin(r,&eof,jobs)) {
    fflush(stdout);
    fflush(stderr);
    freeJobs(jobs);
    _exit(0);
  }
  execvp(r->argv[0],r->argv);
  fprintf(stderr,"%s: %s\n",r->file,strerror(errno));
  _exit(127);
}

static int redirect(CommandRep r, int input_fd, int output_fd) {
  int fd;
  if (input_fd != STDIN_FILENO && dup2(input_fd,STDIN_FILENO) < 0) return -1;
  if (output_fd != STDOUT_FILENO && dup2(output_fd,STDOUT_FILENO) < 0) return -1;
  if (r->input) {
    fd=open(r->input,O_RDONLY);
    if (fd < 0) return -1;
    if (dup2(fd,STDIN_FILENO) < 0) { close(fd); return -1; }
    close(fd);
  }
  if (r->output) {
    fd=open(r->output,O_WRONLY|O_CREAT|O_TRUNC,0666);
    if (fd < 0) return -1;
    if (dup2(fd,STDOUT_FILENO) < 0) { close(fd); return -1; }
    close(fd);
  }
  return 0;
}

extern int execCommand(Command command, Pipeline pipeline, Jobs jobs,
                       int input_fd, int output_fd, int *eof, int fg) {
  CommandRep r=command;
  //builtins in the shell process
  if (fg && input_fd == STDIN_FILENO && output_fd == STDOUT_FILENO &&
      !r->input && !r->output &&
      builtin(r,eof,jobs))
    return 0;
  if (fg && is_builtin(r)) {
    int saved_in=dup(STDIN_FILENO), saved_out=dup(STDOUT_FILENO);
    if (saved_in < 0 || saved_out < 0 || redirect(r,input_fd,output_fd) < 0)
      ERROR("redirection failed");
    builtin(r,eof,jobs);
    dup2(saved_in,STDIN_FILENO); dup2(saved_out,STDOUT_FILENO);
    close(saved_in); close(saved_out);
    return 0;
  }
  fflush(NULL);
  int pid=fork();
  if (pid==-1)
    ERROR("fork() failed");
  if (pid==0) {
    // apply redirections to the child before exec
    if (redirect(r,input_fd,output_fd) < 0) {
      fprintf(stderr,"%s: redirection failed: %s\n",r->file,strerror(errno));
      _exit(126);
    }
    child(r,fg);
  }
  return pid;
}

extern char *commandInput(Command command) { return ((CommandRep)command)->input; }
extern char *commandOutput(Command command) { return ((CommandRep)command)->output; }

extern void freeCommand(Command command) {
  CommandRep r=command;
  char **argv=r->argv;
  while (*argv)
    free(*argv++);
  free(r->argv);
  free(r->input);
  free(r->output);
  free(r);
}

extern void freestateCommand() {
  if (cwd) free(cwd);
  if (owd) free(owd);
}
