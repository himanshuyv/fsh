#ifndef __PROCESS_H
#define __PROCESS_H

#include <sys/types.h>

typedef struct ProcessStruct {
    char* processName;
    pid_t processID;
    struct ProcessStruct* next;
    struct ProcessStruct* prev;
} ProcessStruct;

typedef ProcessStruct* Process;
typedef ProcessStruct* ProcessList;
Process newProcess(char* processName, pid_t processID);
void freeProcess(Process p);
void freeProcessList(ProcessList l);
void addToList(ProcessList* list, Process p);

/*

    Process p is assumed to be in list

*/
Process removeFromList(ProcessList* list, Process p);
#endif