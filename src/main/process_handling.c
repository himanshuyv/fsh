#include "../header/headers.h"

size_t backgroundListSize;
ProcessList backgroundList;

void initBackground() {
    backgroundListSize = 0;    
    backgroundList = NULL;
}

void destructBackground() {
    freeProcessList(backgroundList);
    backgroundListSize = 0;
}

void printFinishedProcesses() {
    for (Process itr = backgroundList; itr != NULL;) {
        int status;
        int ret = waitpid(itr->processID, &status, WNOHANG);
        if (ret == 0) {
            itr = itr->next;
            continue;
        }
#ifdef DEBUG
        printf("status: %b\n", status);
        printf("\nProcess Name: %s\n", itr->processName);
        printf("Process Number: %d\n", itr->processID);
#endif

        if (WIFEXITED(status)) {
            printf("%s exited normally (%d)\n", itr->processName, itr->processID);
            itr = removeFromList(&backgroundList, itr);
            backgroundListSize--;
        } else {
            itr = itr->next;
        }
    }
}

int addProcess(char* processName, pid_t processID) {
    Process new = newProcess(processName, processID);
    if (new == NULL) return 1;
    addToList(&backgroundList, new);
    return 0;    
}