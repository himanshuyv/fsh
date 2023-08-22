#include "../header/headers.h"

bool isNum(char* str) {
    if (str[0] == '\0') return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }

    return true;
}

int executePastEvents(Command* command) {
    int exitCode;
    if (command->argc == 1) {
        for (Node itr = eventQueue->front; itr != NULL; itr = itr->next) {
            printf("%s\n", itr->val);
        }
        exitCode = 0;
    } else {
        if (strcmp(command->argv[1], "purge") == 0) {
            exitCode = purge();
        } else if (strcmp(command->argv[1], "execute") == 0) {
            exitCode = pastEventsExecute(command);
        }
    }

    return exitCode > 0;
}

int executeProclore(Command* command) {
    int exitCode = 0;
    if (command->argc == 1)
        exitCode = proclore(getpid());
    else if (command->argc == 2) {
        if (isNum(command->argv[1]))
            exitCode = proclore(atoi(command->argv[1]));
        else {
            fprintf(stderr, "[ERROR]: Expected PID, found: %s\n",
                    command->argv[1]);
            exitCode = EXEC_FAILURE;
        }
    }

    return exitCode;
}

int executeSys(Command* command) {
    int exitCode = 0;
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "[ERROR]: Unable to fork process, errno = %d", errno);
        exitCode = 1;
    } else if (pid == 0) {
        if (execvp(command->argv[0], command->argv) == -1) {
            fprintf(stderr, "[ERROR]: Bad shell command \'%s\'\n", command->argv[0]);
            exitCode = 1;
        }
    } else {
        int status;
        if (command->isBackground) {
            printf("%d\n", pid);
            exitCode = addProcess(command->argv[0], pid);
        } else
            waitpid(pid, &status, 0);
    }

    return exitCode;
}

int executeCommand(Command* command) {
    if (command->argc == 0) {
        fprintf(stderr, "[ERROR]: Command has length 0\n");
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
    else
        exitCode = executeSys(command);

    return exitCode;
}