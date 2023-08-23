#include "../../header/headers.h"

int changeDirectory(char* path) {
    int pathLength = strlen(path);
    int errorCode;
    if (pathLength == 0) {
        errorCode = chdir(homeDirectory);
    } else if (path[0] == '~') {
        replaceTildaWithHome(path);
        errorCode = chdir(path);
    } else if (pathLength == 1) {
        if (path[0] == '-') {
            if (strlen(previousDirectory) == 0) {
                fprintf(stderr, "[ERROR]: OLD_PWD not set\n");
                return 3;
            } else {
                errorCode = chdir(previousDirectory);
            }
        }
    } else {
        errorCode = chdir(path);
    }

    if (errorCode == -1) {
        if (errno == 2) {
            fprintf(stderr, "[ERROR]: Directory \'%s\' not found\n", path);
            return NO_SUCH_DIRECTORY;
        }
    } else if (errno == 0) {
        strcpy(previousDirectory, absolutePath);
        getcwd(absolutePath, DIRECTORY_BUFFER_SIZE);
        printf("%s\n", absolutePath);
    }

    return 0;
}

int warp(Command* command) {
    if (command->argc == 1 && strcmp(command->argv[0], "warp") != 0) {
        fprintf(stderr, "[ERROR]: Warp command does not begin with \'warp\'\n");
        return EXEC_FAILURE;
    }

    int exitCode;
    if (command->argc == 1) {
        exitCode = changeDirectory("");
    } else {
        exitCode = 0;
        for (int i = 1; i < command->argc; i++)
            exitCode |= (changeDirectory(command->argv[i]) > 0);
    }

    if (exitCode > 0)
        return EXEC_FAILURE;
    else
        return EXEC_SUCCESS;
}