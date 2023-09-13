#include "../../header/headers.h"


#define PROCESS_STATUS_INDEX 2

int activities(Subcommand command) {
    if (command->argc != 1) {
        errorPrintf("Too many arguments for activites\n");
        return EXEC_FAILURE;
    }

    for (Process itr = backgroundList; itr != NULL; itr = itr->next) {
        pid_t pid = itr->processID;
        char statFilePath[DIRECTORY_BUFFER_SIZE];
        sprintf(statFilePath, STAT_FILE_PATH_FORMAT, pid);
        FILE* statFile = fopen(statFilePath, "r");
        if (statFile == NULL) {
            errorPrintf("Error opening %s\n", statFilePath);
            return EXEC_FAILURE;
        }

        char readBuffer[1024];
        char status = '\0';
        for (int i = 0; i < 3; i++) {
            fscanf(statFile, "%s", readBuffer);
            if (i == PROCESS_STATUS_INDEX) {
                status = readBuffer[0];
            }
        }

        if (status == '\0') {
            errorPrintf("Unable to read %s\n", statFilePath);
            return EXEC_FAILURE;
        }


        // Z or T = stopped
        // else runnning
        printf("%d : %s - ", pid, itr->processName);

        if (status == 'Z' || status == 'T') {
            printf("Stopped\n");
        } else {
            printf("Running\n");
        }
    }

    return EXEC_SUCCESS;
}