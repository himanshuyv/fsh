#ifndef __NEONATE_H
#define __NEONATE_H

#include "../util/subcommand.h"

extern int repeatInterval;

int neonate(Subcommand command);
void printRecentPID(int sig);
#endif