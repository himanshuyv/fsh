#include "../header/headers.h"

const char* TOKEN_DELIMETERS = " \r\v\f\t\n";
/*
    -1 return = empty command
*/
int tokenize(Command* c, char* commandStr) {
    size_t argc = 0;
    char* argv[COMMAND_MAX_ARGC];
    char* token;
    token = strtok(commandStr, TOKEN_DELIMETERS);
    while (token != NULL) {
        argv[argc] = token;
        argc++;
        token = strtok(NULL, TOKEN_DELIMETERS);
    }

    if (argc == 0) {
        return -1;
    }
    c->argc = argc;
    c->argv = (char**) malloc(sizeof(char*) * argc);
    printf("%ld ", argc);
    for (int i = 0; i < argc; i++) {
        c->argv[i] = argv[i];
    }

    for (int i = 0; i < argc; i++) {
        printf("%s ", c->argv[i]);
    }
    printf("%d", c->isBackground);
    printf("\n");
    return 0;
}

int parseInput(Command* buffer, size_t bufferSize, char* input) {
    int commandCt = 0;
    int inputLength = strlen(input);
    int inputPtr = 0;
    while (inputPtr < inputLength) {
        size_t commandStrLen = strcspn(input + inputPtr, ";&");
        if (commandStrLen) {
            if (input[inputPtr + commandStrLen] == '&') buffer[commandCt].isBackground = true;
            else buffer[commandCt].isBackground = false;
            input[inputPtr + commandStrLen] = '\0';
            commandCt += tokenize(&buffer[commandCt], input + inputPtr);
            commandCt++;
        }
        inputPtr += commandStrLen + 1;
    }

    return 0;
}