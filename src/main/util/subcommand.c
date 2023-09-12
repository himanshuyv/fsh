#include "../../header/headers.h"

Subcommand newSubcommand(size_t argc, char** argv, int inputFd, int outputFd, bool isBackground) {
    Subcommand ret = (Subcommand) malloc(sizeof(SubcommandStruct));
    ret->argc = argc;
    ret->argv = (char**) malloc(sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        ret->argv[i] = argv[i];
    }

    ret->argv[argc] = NULL;
    ret->inputFd = inputFd;
    ret->outputFd = outputFd;
    ret->isBackground = isBackground;
    return ret;
}

void freeSubcommand(Subcommand subcommand) {
    free(subcommand->argv);
    free(subcommand);
}

SubcommandNode newSubcommandNode(Subcommand subcommand) {
    SubcommandNode ret = (SubcommandNode) malloc(sizeof(SubcommandNodeStruct));
    ret->next = NULL;
    ret->prev = NULL;
    ret->subcommand = subcommand;
    return ret;
}

void freeSubcommandNode(SubcommandNode subcommandNode) {
    if (subcommandNode == NULL) return;
    freeSubcommandNode(subcommandNode->next);
    freeSubcommand(subcommandNode->subcommand);
    free(subcommandNode);
}

void linkSubcommand(SubcommandNode a, SubcommandNode b) {
    if (a) a->next = b;
    if (b) b->prev = a;
}

SubcommandList newSubcommandList() {
    SubcommandList ret = (SubcommandList) malloc(sizeof(SubcommandListStruct));
    ret->front = NULL;
    ret->rear = NULL;
    ret->listSize = 0;
    return ret;
}

void freeSubcommandList(SubcommandList list) {
    freeSubcommandNode(list->front);
    free(list);
}

void appendSubcommand(Subcommand subcommand, SubcommandList list) {
    SubcommandNode newNode = newSubcommandNode(subcommand);
    if (list->listSize == 0) {
        list->front = newNode;
        list->rear = newNode;
    } else {
        linkSubcommand(list->rear, newNode);
        list->rear = newNode;
    }

    list->listSize++;
}

#ifdef DEBUG
void debugSubcommand(Subcommand subcommand) {
    printf("[SUBCOMMAND_DEBUG] argc = %lu\n", subcommand->argc);
    printf("[SUBCOMMAND_DEBUG] arguments: ");
    for (int i = 0; i < subcommand->argc; i++) {
        printf("%s, ", subcommand->argv[i]);
    }
    printf("\n");
    printf("[SUBCOMMAND_DEBUG] inputFd = %d\n", subcommand->inputFd);
    printf("[SUBCOMMAND_DEBUG] outputFd = %d\n", subcommand->outputFd);
    printf("[SUBCOMMAND_DEBUG] isBackground = %d\n", subcommand->isBackground);
    printf("\n");
}

void debugSubcommandList(SubcommandList list) {
    printf("[SUBCOMMAND_DEBUG] SubcommandList size = %lu\n\n", list->listSize);
    SubcommandNode itr = list->front;
    for (int i = 0; i < list->listSize; i++, itr = itr->next) {
        debugSubcommand(itr->subcommand);
    }
}
#endif