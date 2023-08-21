#ifndef __PROCESS_HANDLING_H
#define __PROCESS_HANDLING_H

#include <stdlib.h>
#include "util/process.h"
extern size_t backgroundListSize;
extern ProcessList backgroundList;

void initBackground();
void destructBackground();
void printFinishedProcesses();
void addProcess(char* processName, pid_t processID);

#endif