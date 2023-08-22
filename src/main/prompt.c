#include "../header/headers.h"

#define USERNAME_BUFFER_SIZE 64
#define SYSNAME_BUFFER_SIZE 64

char userName[USERNAME_BUFFER_SIZE];
char sysName[SYSNAME_BUFFER_SIZE];
char homeDirectory[DIRECTORY_BUFFER_SIZE];
char absolutePath[DIRECTORY_BUFFER_SIZE];
char currentDirectory[DIRECTORY_BUFFER_SIZE];
char previousDirectory[DIRECTORY_BUFFER_SIZE];

int initPrompt() {
    if (getcwd(homeDirectory, DIRECTORY_BUFFER_SIZE) == NULL) {
        // TODO Handle error

        return 1;
    }

    strcpy(previousDirectory, "");

    return 0;
}

void getUsername(char* buffer) {
    int usernameErrorCode = getlogin_r(buffer, USERNAME_BUFFER_SIZE);
    if (usernameErrorCode) {
        printf("Error retrieving username\n");
        strcpy(buffer, "username");
    }
}

void getSysname(char* buffer) {
    int sysnameError = gethostname(buffer, SYSNAME_BUFFER_SIZE);
    if (sysnameError) {
        printf("Error retrieving hostname\n");
        strcpy(buffer, "hostname");
    }
}

bool isHomeDirPrefix(char* path) {
    int homeDirectoryLength = strlen(homeDirectory);
    int pathLength = strlen(path);
    if (pathLength < homeDirectoryLength) return false;
    for (int i = 0; i < homeDirectoryLength; i++) {
        if (homeDirectory[i] != path[i]) return false;
    }

    return true;
}

void replaceHomeDirPrefix(char* path) {
    if (!isHomeDirPrefix(path)) return;
    int pathLength = strlen(path);
    int homeDirectoryLength = strlen(homeDirectory);
    for (int i = 1; i - 1 + homeDirectoryLength <= pathLength; i++) {
        path[i] = path[i - 1 + homeDirectoryLength];
    }

    path[0] = '~';
    path[pathLength - homeDirectoryLength + 1] = '\0';
}

void setCurrentDirectory() {
    if (getcwd(absolutePath, DIRECTORY_BUFFER_SIZE) == NULL) {
        // TODO Handle error
    }
    
    strcpy(currentDirectory, absolutePath);
    replaceHomeDirPrefix(currentDirectory);
}

void prompt() {
    getUsername(userName);
    getSysname(sysName);
    setCurrentDirectory();

#ifdef DEBUG
    printf("[PROMPT_DEBUG] absolutePath: %s\n", absolutePath);
    printf("[PROMPT_DEBUG] currentDirectory: %s\n", currentDirectory);
    printf("[PROMPT_DEBUG] previousDirectory: %s\n", previousDirectory);
#endif

    printf("<%s@%s:%s>", userName, sysName, currentDirectory);
}