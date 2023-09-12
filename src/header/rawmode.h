#ifndef __RAWMODE_H
#define __RAWMODE_H

#define INPUT_BUFFER_SIZE 2048
extern char input[INPUT_BUFFER_SIZE];

void disableRawMode();
void enableRawMode();
void readInput();
#endif