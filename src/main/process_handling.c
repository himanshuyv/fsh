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

Process findMiddle(ProcessList list) {
    Process slow = list, fast = list;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

bool compareProcess(Process a, Process b) {
    return a->processID < b->processID;
}

ProcessList mergeSorted(ProcessList list1, ProcessList list2) {
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    if (compareProcess(list1, list2)) {
        Process ret = mergeSorted(list1->next, list2);
        list1->next = ret;
        ret->prev = list1;
        return list1;
    } else {
        Process ret = mergeSorted(list1, list2->next);
        list2->next = ret;
        ret->prev = list2;
        return list2;
    }
}

ProcessList sortProcessList(ProcessList list) {
    if (list == NULL) return NULL;
    if (list->next == NULL) return list;
    Process middle = findMiddle(list);
    ProcessList list1 = list;
    ProcessList list2 = middle;
    if (middle->prev) middle->prev->next = NULL;
    middle->prev = NULL;
    list1 = sortProcessList(list1);
    list2 = sortProcessList(list2);
    return mergeSorted(list1, list2);
}