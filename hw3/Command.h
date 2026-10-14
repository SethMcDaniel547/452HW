#ifndef COMMAND_H
#define COMMAND_H

typedef void *Command;

#include "Tree.h"
#include "Jobs.h"
#include "Sequence.h"

extern Command newCommand(T_command command);

extern int execCommand(Command command, Pipeline pipeline, Jobs jobs,
                       int input_fd, int output_fd, int *eof, int fg);
extern char *commandInput(Command command);
extern char *commandOutput(Command command);

extern void freeCommand(Command command);
extern void freestateCommand();

#endif
