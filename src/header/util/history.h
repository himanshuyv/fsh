#ifndef __HISTORY_H
#define __HISTORY_H

#include "command.h"
#include "queue.h"
#define MAX_HISTORY_SIZE 15
void initHistory();

/*
    Returns true if event was added
*/
bool addEvent(Command* commands, int commandCt, char* inputString);

/*
    Returns size of command array (buffer)
    Returns NULL on unsuccessful call
*/
char* getKthLastEvent(int k);

void destructHistory();

/*
    Returns size of history
*/
size_t getHistorySize();
void writeHistory();
extern Queue eventQueue;
#endif