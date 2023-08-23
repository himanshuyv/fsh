#include "../../header/headers.h"

#define PROCESS_STATUS_INDEX 2
#define PROCESS_GROUP_INDEX 4
#define PROCESS_VIRTUAL_MEMORY_INDEX 22
#define READ_BUFFER_SIZE 256
const char* STAT_FILE_PATH_FORMAT = "/proc/%d/stat";
const char* EXE_FILE_PATH_FORMAT = "/proc/%d/exe";
int proclore(pid_t pid) {
    char statFilePath[DIRECTORY_BUFFER_SIZE];
    sprintf(statFilePath, STAT_FILE_PATH_FORMAT, pid);
    FILE* statFile = fopen(statFilePath, "r");
    if (statFile == NULL) {
        fprintf(stderr, "[ERROR]: Error opening %s\n", statFilePath);
        return EXEC_FAILURE;
    }
    
    char readBuffer[1024];
    char status = '\0';
    int group = -1;
    int virtualMemory = 0;
    bool isBackground = false;
    for (int i = 0; i < 23; i++) {
        fscanf(statFile, "%s", readBuffer);
        if (i == PROCESS_STATUS_INDEX) {
            status = readBuffer[0];
        } else if (i == PROCESS_GROUP_INDEX) {
            group = atoi(readBuffer);
        } else if (i == PROCESS_VIRTUAL_MEMORY_INDEX) {
            virtualMemory = atoi(readBuffer);
        }
    }

    if (status == '\0') {
        fprintf(stderr, "[ERROR]: Unable to read %s\n", statFilePath);
        return EXEC_FAILURE;
    }

    int processGroupID = getpgrp();
    isBackground = (processGroupID != group);
    char statusString[3] = {status, '\0', '\0'};
    char executablePath[READ_BUFFER_SIZE];
    sprintf(executablePath, EXE_FILE_PATH_FORMAT, pid);
    char exe[READ_BUFFER_SIZE] = {'\0'};
    readlink(executablePath, exe, READ_BUFFER_SIZE);
    if (!isBackground) statusString[1] = '+';

    printf("pid: %d\n", pid);
    printf("process status: %s\n", statusString);
    printf("Process Group: %d\n", group);
    printf("Virtual memory: %d\n", virtualMemory);
    printf("executable Path: %s\n", exe);
    return EXEC_SUCCESS;
}