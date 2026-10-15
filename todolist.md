Phase 0: Setup & Preparation

    [ ] Replace the binary-only double-ended queue library with your own implementation from HW#1.

    [ ] Explore the skeletal code structure in pub/hw3 (scanner, parser, tree-walking interpreter, and regression tester run).

    [ ] Configure the regression tester (run) to point to your shell executable.

Phase 1: Simple Foreground Commands

    [ ] Process Management (Fork/Exec):

        [ ] Implement fork() and execvp() in the tree-walking interpreter for non-builtin commands.

        [ ] Ensure the parent process blocks using waitpid() until the child process exits.

    [ ] Basic Builtin Commands:

        [ ] Implement exit: Terminate the shell normally.

        [ ] Implement pwd: Print the current working directory from the shell's process.

        [ ] Implement cd <dir>: Change the current working directory, supporting absolute paths and "-" for the previous working directory.

Phase 2: Sequences of Commands

    [ ] Semicolon Sequencing (;):

        [ ] Traverse and execute sequence nodes where commands are separated by semicolons.

        [ ] Ensure sequential execution order (the shell must wait for the left command/pipeline to finish completely before executing the command following the ;).

Phase 3: I/O Redirection

    [ ] Input Redirection (<):

        [ ] Open the specified input file with appropriate read flags.

        [ ] Redirect standard input (STDIN_FILENO) using dup2() in the child process before execution.

    [ ] Output Redirection (>):

        [ ] Open/create the specified output file with write/create/truncate flags.

        [ ] Redirect standard output (STDOUT_FILENO) using dup2() in the child process.

    [ ] Combined Redirection (< and >):

        [ ] Support commands that utilize both input and output redirection simultaneously.

        [ ] Ensure proper file descriptor cleanup and error handling if files fail to open.

Phase 4: Background Commands

    [ ] Ampersand Execution (&):

        [ ] Recognize when a pipeline or command is suffixed with &.

        [ ] Execute the command in a child process without blocking the parent shell (skip waitpid() in the main loop).

    [ ] Zombie Process Prevention:

        [ ] Track background jobs and implement reaping logic (via signal handlers like SIGCHLD or periodic checks) so background processes do not become zombies.

Phase 5: Pipelines

    [ ] Pipe Setup (|):

        [ ] Create pipes using pipe() for commands linked by vertical bars.

        [ ] Fork separate child processes for each command in the pipeline.

    [ ] File Descriptor Wiring:

        [ ] Use dup2() to wire the write end of the pipe to STDOUT_FILENO for the upstream command and the read end to STDIN_FILENO for the downstream command.

        [ ] Close unused pipe ends in all processes to prevent hanging reads/writes.

    [ ] Pipeline Testing:

        [ ] Test complex sequences combining pipes, semicolons, and background operators (e.g., pwd & cd /etc ; pwd).

Phase 6: Remaining Builtins & Robustness

    [ ] History Builtin:

        [ ] Implement history using the GNU History library function history_list.

    [ ] Exit Cleanup:

        [ ] Update exit so it waits for any active background jobs to finish before finally shutting down the shell.

    [ ] Testing & Debugging:

        [ ] Add comprehensive test cases to the regression tester (run).

        [ ] Run valgrind (using the provided vg script) to identify and fix all memory leaks and invalid reads/writes.