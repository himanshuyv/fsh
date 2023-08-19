#include "../header/headers.h"

#define USERNAME_BUFFER_SIZE 64
#define SYSNAME_BUFFER_SIZE 64
#define DIRECTORY_BUFFER_SIZE 1024

char userName[USERNAME_BUFFER_SIZE];
char sysName[SYSNAME_BUFFER_SIZE];
char homeDirectory[DIRECTORY_BUFFER_SIZE];
char currentDirectory[DIRECTORY_BUFFER_SIZE];

int initPrompt() {
    if (getcwd(homeDirectory, DIRECTORY_BUFFER_SIZE) == NULL) {
        // TODO Handle error
    }
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

void setCurrentDirectory() {
    if (getcwd(currentDirectory, DIRECTORY_BUFFER_SIZE) == NULL) {
        // TODO Handle error
    }

    int homeDirectoryLength = strlen(homeDirectory);
    bool isHomeDirPrefix = true;
    for (int i = 0; i < homeDirectoryLength; i++) {
        if (homeDirectory[i] != currentDirectory[i]) {
            isHomeDirPrefix = false;
            break;
        }
    }

    if (isHomeDirPrefix) {
        int currentDirectoryLength = strlen(currentDirectory);
        currentDirectory[0] = '~';
        for (int i = 1; i - 1 + homeDirectoryLength <= currentDirectoryLength; i++) {
            currentDirectory[i] = currentDirectory[i - 1 + homeDirectoryLength]; 
        }    
    }
}

void prompt() {
    getUsername(userName);
    getSysname(sysName);
    setCurrentDirectory();
    printf("<%s@%s:%s>", userName, sysName, currentDirectory);
}
