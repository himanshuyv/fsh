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

    int status;
    waitpid(pid, &status, WNOHANG | WUNTRACED);
    if (status)
    {
        printf("%i exited normally\n", pid);
    }
    for (Process itr = backgroundList; itr != NULL;) {
        int status;
        waitpid(itr->processID, &status, WNOHANG | WUNTRACED);

#ifdef DEBUG
        printf("\nProcess Name: %s\n", itr->processName);
        printf("Process Number: %d\n", itr->processID);
        printf("Process Status: %d\n", *(itr->statusPtr));
#endif

    if (WIFEXITED(status)) {
        printf("%s exited normally (killed) (%d)\n", itr->processName, itr->processID);
        itr = removeFromList(&backgroundList, itr);
        backgroundListSize--;
    } else if (WIFSTOPPED(status)) {
        printf("%s exited normally (suspended) (%d)\n", itr->processName, itr->processID);
        itr = removeFromList(&backgroundList, itr);
        backgroundListSize--;
    } else {
        itr = itr->next;
    }
    }
}

void addProcess(char* processName, pid_t processID) {
    Process new = newProcess(processName, processID);
    addToList(&backgroundList, new);    
}