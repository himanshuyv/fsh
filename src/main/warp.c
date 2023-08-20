#include "../header/headers.h"

int changeDirectory(char* path) {
    int pathLength = strlen(path);
    int errorCode;
    if (pathLength == 0) {
        errorCode = chdir(homeDirectory);
    } else if (path[0] == '~') {
        char temp[DIRECTORY_BUFFER_SIZE] = {'\0'};
        strcpy(temp, homeDirectory);
        strcat(temp, path + 1);
        strcpy(path, temp);
        errorCode = chdir(path);
    } else if (pathLength == 1) {
        if (path[0] == '-') {
            if (strlen(previousDirectory) == 0) {
                printf("[ERROR]: Old PWD not set\n");
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
            printf("[ERROR]: Directory \'%s\' not found\n", path);
            return NO_SUCH_DIRECTORY;
        }
    } else if (errno == 0) {
        strcpy(previousDirectory, absolutePath);
        getcwd(absolutePath, DIRECTORY_BUFFER_SIZE);
    }

    return 0;
}