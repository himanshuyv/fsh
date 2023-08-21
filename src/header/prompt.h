#ifndef __PROMPT_H
#define __PROMPT_H

#define DIRECTORY_BUFFER_SIZE 256
extern char homeDirectory[DIRECTORY_BUFFER_SIZE];
extern char absolutePath[DIRECTORY_BUFFER_SIZE];
extern char previousDirectory[DIRECTORY_BUFFER_SIZE];
void prompt();
int initPrompt();
#endif