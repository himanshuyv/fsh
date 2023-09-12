#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdlib.h>
#define COMMAND_MAX_ARGC 128

typedef struct Command {
        size_t argc;
        char** argv;
    bool isBackground;
} Command;

void freeCommand(Command* c);
bool isEqual(Command* a, Command* b);
#endif