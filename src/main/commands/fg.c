#include "../../header/headers.h"

int fg(Subcommand command) {
    if (command->argc != 2) {
        errorPrintf("fg expects 1 argument found %ld\n",
                command->argc - 1);
        return EXEC_FAILURE;
    }

    if (!isNum(command->argv[1])) {
        errorPrintf("Argument \'%s\' for fg is not valid\n",
                command->argv[1]);
        return EXEC_FAILURE;
    }

    if (bg(command)) {
        return EXEC_FAILURE;
    }
    char* endPointer;
    pid_t pid = (int)strtoll(command->argv[1], &endPointer, RADIX);

    if (kill(pid, 0) == -1) {
        errorPrintf("Process %d not found\n", pid);
        return EXEC_FAILURE;
    }

    if (kill(pid, SIGCONT) == -1) {
        errorPrintf("Could not bring process %d to background\n",
                pid);
        return EXEC_FAILURE;
    }
    setpgid(pid, 0);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, pid);


    int status;
    waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    return EXEC_SUCCESS;
}