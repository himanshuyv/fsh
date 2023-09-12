#include "../../header/headers.h"

int changeDirectory(char* path) {
    char pathCpy[DIRECTORY_BUFFER_SIZE];    
    strcpy(pathCpy, path);
    int pathLength = strlen(pathCpy);
    int errorCode;
    if (pathLength == 0) {
        errorCode = chdir(homeDirectory);
        if (errorCode == -1) {
            fprintf(stderr, "[ERROR]: Could not warp to \'%s\'\n", pathCpy);
            return EXEC_FAILURE;
        }
    } else if (pathCpy[0] == '~') {
        replaceTildaWithHome(pathCpy);
        errorCode = chdir(pathCpy);
        if (errorCode == -1) {
            fprintf(stderr, "[ERROR]: Could not warp to \'%s\'\n", pathCpy);
            return EXEC_FAILURE;
        }
    } else if (pathLength == 1) {
        if (pathCpy[0] == '-') {
            if (strlen(previousDirectory) == 0) {
                fprintf(stderr, "[ERROR]: OLD_PWD not set\n");
                return 3;
            } else {
                errorCode = chdir(previousDirectory);
            }
        } else {
            errorCode = chdir(pathCpy);
            if (errorCode == -1) {
                fprintf(stderr, "[ERROR]: Could not warp to \'%s\'\n", pathCpy);
                return EXEC_FAILURE;
            }           
        }
    } else {                    
        errorCode = chdir(path);
        if (errorCode == -1) {
            fprintf(stderr, "[ERROR]: Could not warp to \'%s\'\n", path);
            return EXEC_FAILURE;
        }
    }

    if (errorCode == -1) {
        if (errno == 2) {
            fprintf(stderr, "[ERROR]: Directory \'%s\' not found\n", path);
            return NO_SUCH_DIRECTORY;
        }
    } else if (errno == 0) {
        strcpy(previousDirectory, absolutePath);
        if (getcwd(absolutePath, DIRECTORY_BUFFER_SIZE) == NULL) {
            fprintf(stderr, "[ERROR]: Warp: Could not get current directory\n");
            return EXEC_FAILURE;
        } else {
            printf("%s\n", absolutePath);
        }
    }

    return 0;
}

int warp(Subcommand command) {
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