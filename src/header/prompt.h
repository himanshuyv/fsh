#ifndef __PROMPT_H
#define __PROMPT_H

#define DIRECTORY_BUFFER_SIZE 1024
extern char absolutePath[DIRECTORY_BUFFER_SIZE];

void prompt();
int initPrompt();
#endif