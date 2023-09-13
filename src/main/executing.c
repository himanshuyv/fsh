#include "../header/headers.h"

int executePastEvents(Subcommand command) {
    int exitCode;
    if (command->argc == 1) {
        for (Node itr = eventQueue->front; itr != NULL; itr = itr->next) {
            printf("%s\n", itr->val);
        }
        exitCode = 0;
    } else {
        if (strcmp(command->argv[1], "purge") == 0) {
            if (command->argc == 2)
                exitCode = purge();
            else {
                fprintf(stderr,
                        "[ERROR]: Too many arguments for pastevents purge\n");
            }
        } else if (strcmp(command->argv[1], "execute") == 0) {
            exitCode = pastEventsExecute(command);
        } else {
            fprintf(stderr,
                    "[ERROR]: Unknown second argument for pastevents \'%s\'\n",
                    command->argv[1]);
            exitCode = 1;
        }
    }

    return exitCode > 0;
}

int executeProclore(Subcommand command) {
    int exitCode = 0;
    if (command->argc == 1)
        exitCode = proclore(getpid());
    else if (command->argc == 2) {
        if (isNum(command->argv[1]))
            exitCode = proclore(atoi(command->argv[1]));
        else {
            errorPrintf("Expected PID, found: %s\n", command->argv[1]);
            exitCode = EXEC_FAILURE;
        }
    } else {
        errorPrintf("Too many arguments for proclore\n");
    }

    return exitCode;
}

int executeSys(Subcommand command) {
    int exitCode = 0;
    pid_t pid = fork();
    if (pid == -1) {
        errorPrintf("Unable to fork process, errno = %d\n", errno);
        exitCode = 1;
    } else if (pid == 0) {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (execvp(command->argv[0], command->argv) == -1) {
            errorPrintf("Bad shell command \'%s\'\n", command->argv[0]);
            exit(1);
        }
    } else {
        int status;
        if (command->isBackground) {
            printf("%d\n", pid);
            exitCode = addProcess(command->argv[0], pid);
        } else {
            setpgid(pid, 0);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, pid);

            waitpid(pid, &status, WUNTRACED);
            if (WIFSTOPPED(status)) {
                printf("sent %i to bg\n", pid);
                exitCode = addProcess(command->argv[0], pid);
            }

            tcsetpgrp(STDIN_FILENO, getpgid(0));
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
    }

    return exitCode;
}

int executePeek(Subcommand command) {
    int exitCode = peek(command);
    return exitCode;
}

int executeSeek(Subcommand command) {
    int exitCode = seek(command);
    return exitCode;
}

void executeExit(Subcommand command) {
    destructBackground();
    destructHistory();
    exit(0);
}

int executeSubcommand(Subcommand command) {
    if (command->argc == 0) {
        errorPrintf("Command has length 0\n");
        return EXEC_FAILURE;
    }

    int exitCode = 0;

    char* commandName = command->argv[0];
    if (strcmp(commandName, "warp") == 0)
        exitCode = warp(command);
    else if (strcmp(commandName, "pastevents") == 0)
        exitCode = executePastEvents(command);
    else if (strcmp(commandName, "proclore") == 0)
        exitCode = executeProclore(command);
    else if (strcmp(commandName, "peek") == 0)
        exitCode = executePeek(command);
    else if (strcmp(commandName, "seek") == 0)
        exitCode = executeSeek(command);
    else if (strcmp(commandName, "ping") == 0)
        exitCode = ping(command);
    else if (strcmp(commandName, "bg") == 0)
        exitCode = bg(command);
    else if (strcmp(commandName, "fg") == 0)
        exitCode = fg(command);
    else if (strcmp(commandName, "activities") == 0)
        exitCode = activities(command);
    else if (strcmp(commandName, "neonate") == 0)
        exitCode = neonate(command);
    else if (strcmp(commandName, "iman") == 0)
        exitCode = iman(command);
    else if (strcmp(commandName, "exit") == 0)
        executeExit(command);
    else
        exitCode = executeSys(command);

    return exitCode;
}

void initStandardIO() {
    STDIN_FD = dup(0);
    STDOUT_FD = dup(1);
}

int STDIN_FD;
int STDOUT_FD;
void restoreStandardIO() {
    dup2(STDIN_FD, 0);
    dup2(STDOUT_FD, 1);
    close(STDIN_FD);
    close(STDOUT_FD);
}

int executeCommand(Command* command) {
    SubcommandList subcommandList = parseSubcommands(command);
    if (subcommandList == NULL) return EXEC_FAILURE;
    initStandardIO();
    SubcommandNode itr = subcommandList->front;
    int fd[2];
    int previousOutputCurrentInput = 0;  // current input / previous output
    for (int i = 0; i < subcommandList->listSize; i++, itr = itr->next) {
        if (pipe(fd) == -1) {
            errorPrintf("Could not create pipe\n");
            return EXEC_FAILURE;
        }

        dup2(previousOutputCurrentInput, 0);
        if (itr->subcommand->inputFd != -1) {
            dup2(itr->subcommand->inputFd, 0);
        }

        if (i == subcommandList->listSize - 1) {
            dup2(STDOUT_FD, 1);
        } else {
            dup2(fd[1], 1);
        }

        if (itr->subcommand->outputFd != -1) {
            dup2(itr->subcommand->outputFd, 1);
        }

        if (executeSubcommand(itr->subcommand)) {
            restoreStandardIO();
            freeSubcommandList(subcommandList);
            return EXEC_FAILURE;
        }

        close(fd[1]);
        previousOutputCurrentInput = fd[0];
    }
    freeSubcommandList(subcommandList);
    restoreStandardIO();
    return EXEC_SUCCESS;
}