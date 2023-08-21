#ifndef __HISTORY_H
#define __HISTORY_H

#include "command.h"

/*
    Returns true if event was added
*/
bool addEvent(Command* commands, int commandCt, char* inputString);

/*
    Returns size of command array (buffer)
    Returns NULL on unsuccessful call
*/
char* getKthLastEvent(int k);
#endif