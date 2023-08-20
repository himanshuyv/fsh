#ifndef __EXECUTING_H
#define __EXECUTING_H

#include "util/command.h"

#define EXEC_SUCCESS 0
#define EXEC_FAILURE 1

int executeCommand(Command* command);

#endif