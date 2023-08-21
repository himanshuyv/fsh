#include "../../header/headers.h"

void linkProcess(Process a, Process b) {
    if (a) a->next = b;
    if (b) b->prev = a;
}

Process newProcess(char* processName, pid_t processID) {
    Process ret = (Process) malloc(sizeof(ProcessStruct));
    ret->processName = (char*) malloc(sizeof(char) * (strlen(processName) + 1));
    strcpy(ret->processName, processName);
    ret->next = NULL;
    ret->processID = processID;
    return ret;
}

void freeProcess(Process p) {
    free(p->processName);
    free(p);
}

void freeProcessList(ProcessList l) {
    if (l == NULL) return;
    freeProcessList(l->next);
    freeProcess(l);
}

void addToList(ProcessList* list, Process p) {
    linkProcess(p, *list);
    *list = p;
}

Process removeFromList(ProcessList* list, Process p) {
    Process ret = p->next;
    if (p == *list) {
        *list =  p->next;
    } else {
        linkProcess(p->prev, p->next);
    }
    freeProcess(p);
    return ret;
}


