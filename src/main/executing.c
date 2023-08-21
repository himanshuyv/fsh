#include "../header/headers.h"

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
    } else {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "[ERROR]: Unable to fork process, errno = %d",
                    errno);
        } else if (pid == 0) {
            execvp(command->argv[0], command->argv);
            // TODO -1 = error
        } else {
            int status;
            if (command->isBackground) {
                addProcess(command->argv[0], pid);
            } else {
                waitpid(pid, &status, 0);
            }
        }
    }

    return EXEC_SUCCESS;
}