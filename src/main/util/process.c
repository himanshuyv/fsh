#include "../../header/headers.h"

void linkProcess(Process a, Process b) {
    if (a) a->next = b;
    if (b) b->prev = a;
}

Process newProcess(pid_t processID, int* statusPtr) {
    Process ret = (Process) malloc(sizeof(ProcessStruct));
    ret->next = NULL;
    ret->processID = processID;
    ret->statusPtr = statusPtr;
    return ret;
}

void freeProcess(Process p) {
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

void removeFromList(ProcessList* list, Process p) {
    if (p == *list) {
        *list =  p->next;
    } else {
        link(p->prev, p->next);
    }
    freeProcess(p);
}


