#include "../../header/headers.h"

void freeCommand(Command* c) {
    free(c->argv);
}

bool isEqual(Command* a, Command* b) {
    if (a->argc != b->argc) return false;
    for (int i = 0; i < a->argc; i++) {
        if (strcmp(a->argv[i], b->argv[i]) != 0) return false;
    }

    return true;
}