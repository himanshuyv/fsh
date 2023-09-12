#ifndef __SUBCOMMAND_H
#define __SUBCOMMAND_H

#include <stdlib.h>
typedef struct SubcommandStruct {
    size_t argc;
    char** argv;
    int inputFd;
    int outputFd;
    bool isBackground;
} SubcommandStruct;

typedef SubcommandStruct* Subcommand;

Subcommand newSubcommand(size_t argc, char** argv, int inputFd, int outputFd, bool isBackground);
void freeSubcommand(Subcommand subcommand);

typedef struct SubcommandNodeStruct {
    struct SubcommandNodeStruct* next;
    struct SubcommandNodeStruct* prev;
    Subcommand subcommand;
} SubcommandNodeStruct;

typedef SubcommandNodeStruct* SubcommandNode;

SubcommandNode newSubcommandNode(Subcommand subcommand);
void freeSubcommandNode(SubcommandNode subcommandNode);

typedef struct SubcommandListStruct {
    size_t listSize;
    SubcommandNode front;
    SubcommandNode rear;
} SubcommandListStruct;

typedef SubcommandListStruct* SubcommandList;

SubcommandList newSubcommandList();
void freeSubcommandList(SubcommandList list);
void appendSubcommand(Subcommand subcommand, SubcommandList list);

#ifdef DEBUG
void debugSubcommandList(SubcommandList list);
#endif

#endif