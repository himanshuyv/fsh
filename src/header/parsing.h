#ifndef __PARSING_H
#define __PARSING_H

#include "util/command.h"

void addSpacesAround(char* haystack, char* match, char* ignore, char* buffer);
int parseInput(Command* buffer, size_t bufferSize, char* input);

#endif