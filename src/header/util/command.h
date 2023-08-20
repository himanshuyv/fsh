#ifndef __COMMAND_H
#define __COMAND_H

#include <stdlib.h>

typedef struct Command {
    size_t argc;
    char** argv;
    bool isBackground;
    int pid;
} Command;

#endif