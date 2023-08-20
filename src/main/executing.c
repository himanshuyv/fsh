#include "../header/headers.h"

int executeCommand(Command* command) {
    if (command->argc == 0) {
        fprintf(stderr, "[ERROR]: Command has length 0\n");
        return EXEC_FAILURE;
    }

    char* commandName = command->argv[0];
    if (strcmp(commandName, "warp") == 0) {
        warp(command);
    } 

    return EXEC_SUCCESS;
}