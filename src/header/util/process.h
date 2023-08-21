#ifndef __PROCESS_H
#define __PROCESS_H

#include <sys/types.h>

typedef struct ProcessStruct {
    pid_t processID;
    int* statusPtr;
    struct ProcessStruct* next;
    struct ProcessStruct* prev;
} ProcessStruct;

typedef ProcessStruct* Process;
typedef ProcessStruct* ProcessList;
Process newProcess(pid_t processID, int* statusPtr);
void freeProcess(Process p);
void freeProcessList(ProcessList l);
void addToList(ProcessList* list, Process p);

/*

    Process p is assumed to be in list

*/
void removeFromList(ProcessList* list, Process p);
#endif
