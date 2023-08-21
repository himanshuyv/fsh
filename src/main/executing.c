#include "../header/headers.h"

bool isNum(char* str) {
    if (str[0] == '\0') return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }

    return true;
}

int executeCommand(Command* command) {
    if (command->argc == 0) {
        fprintf(stderr, "[ERROR]: Command has length 0\n");
        return EXEC_FAILURE;
    }

    char* commandName = command->argv[0];
    if (strcmp(commandName, "warp") == 0) {
        warp(command);
    } else if (strcmp(commandName, "pastevents") == 0) {
        if (command->argc == 1) {
            for (Node itr = eventQueue->front; itr != NULL; itr = itr->next) {
                printf("%s\n", itr->val);
            }
        } else {
            if (strcmp(command->argv[1], "purge") == 0) {
                purge();
            } else if (strcmp(command->argv[1], "execute") == 0) {
                pastEventsExecute(command);
            }
        }

    } else if (strcmp(commandName, "proclore") == 0) {
        if (command->argc == 1) {
            proclore(getpid());
        } else if (command->argc == 2) {
            if (isNum(command->argv[1])) {
                proclore(atoi(command->argv[1]));
            } else {
                fprintf(stderr, "[ERROR]: Expected PID, found: %s\n", command->argv[1]);
                return EXEC_FAILURE;
            }
        }   
    } else {
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "[ERROR]: Unable to fork process, errno = %d",
                    errno);
        } else if (pid == 0) {
            execvp(command->argv[0], command->argv);
            // TODO -1 = error
        } else {
            int status;
            if (command->isBackground) {
                printf("%d\n", pid);
                addProcess(command->argv[0], pid);
            } else {
                waitpid(pid, &status, 0);
            }
        }
    }

    return EXEC_SUCCESS;
}