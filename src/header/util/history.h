#ifndef __HISTORY_H
#define __HISTORY_H

#include "command.h"

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
#endif