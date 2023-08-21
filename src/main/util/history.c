#include "../../header/headers.h"

Queue eventQueue;

void initHistory() {
    eventQueue = newQueue(); // TODO free queue
    // TODO read
}

bool addEvent(Command* commands, int commandCt, char* inputString) {
    if (eventQueue->size == 0) {
        push(eventQueue, inputString);
        return true;
    } 

    // TODO check if commands contains pastevents

    if (strcmp(front(eventQueue), inputString) == 0) {
        return false;
    }

    push(eventQueue, inputString);
    if (eventQueue->size > 15) pop(eventQueue);

    return true;
}

char* getKthLastEvent(int k) {
    if (eventQueue->size < k) {
        fprintf("[ERROR]: eventHistory size is only %d\n", eventQueue->size);
        return NULL;
    }

    Node itr = eventQueue->front;
    for (int i = 1; i < k; i++) {
        itr = itr->next;
    }

    return itr->val;
}