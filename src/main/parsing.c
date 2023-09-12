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

    if (argc == 0) return -1;

    c->argc = argc;
    c->argv = (char**) malloc(sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        c->argv[i] = argv[i];
    }
    c->argv[argc] = NULL;
#ifdef DEBUG
    printf("[TOKENIZE_DEBUG] Token count: %ld, Tokens: ", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s ", c->argv[i]);
    }
    printf(", Is background: %d", c->isBackground);
    printf("\n");
#endif
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

    return commandCt;
}

/*
    Returns number of tokens parsed
*/
int nextSubcommand(char** argv, Subcommand* ret, bool isBackground) {
    *ret = NULL;
    if (argv == NULL || argv[0] == NULL) {
        fprintf(stderr, "[ERROR]: Cannot pipe null command\n");
        return 0;
    }

    if (strcmp(argv[0], "|") == 0 || strcmp(argv[0], ">") == 0 || strcmp(argv[0], "<") == 0 || strcmp(argv[0], ">>") == 0) {
        fprintf(stderr, "[ERROR]: Cannot pipe empty command\n");
        return 0;
    }

    int pipeSize = 0;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) break;
        pipeSize++;
    }

    int argc = 0;
    for (int i = 0; i < pipeSize; i++) {
        if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0 || strcmp(argv[0], "|") == 0) break;
        argc++;
    }

    int inputFd = -1, outputFd = -1;
    int appendCount = 0, outCount = 0, inCount = 0;
    for (int i = argc; i < pipeSize; i++) {
        if (strcmp(argv[i], ">") == 0) {
            outCount++;
            if (outCount > 1 || appendCount > 0) {
                fprintf(stderr, "[ERROR]: Cannot redirect to multiple files\n");
                return 0;
            }

            if (i + 1 >= pipeSize) {
                fprintf(stderr, "[ERROR]: Expected file after \'>\'\n");
                return 0;
            }

            outputFd = open(argv[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (outputFd == -1) {
                fprintf(stderr, "[ERROR]: Cannot open file \'%s\'\n", argv[i + 1]);
                return 0;
            }

            i++;
        } else if (strcmp(argv[i], "<") == 0) {
            inCount++;
            if (inCount > 1) {
                fprintf(stderr, "[ERROR]: Cannot redirect from multiple files\n");
                return 0;
            }

            if (i + 1 >= pipeSize) {
                fprintf(stderr, "[ERROR]: Expected file after \'<\'\n");
                return 0;
            }

            inputFd = open(argv[i + 1], O_RDONLY);
            if (inputFd == -1) {
                fprintf(stderr, "[ERROR]: Cannot open file \'%s\'\n", argv[i + 1]);
                return 0;
            }

            i++;
        } else if (strcmp(argv[i], ">>") == 0) {
            appendCount++;
            if (appendCount > 1 || outCount > 0) {
                fprintf(stderr, "[ERROR]: Cannot redirect to multiple files\n");
                return 0;
            }

            if (i + 1 >= pipeSize) {
                fprintf(stderr, "[ERROR]: Expected file after \'>>\'\n");
                return 0;
            }

            outputFd = open(argv[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (outputFd == -1) {
                fprintf(stderr, "[ERROR]: Cannot open file \'%s\'\n", argv[i + 1]);
                return 0;
            }

            i++;
        }
    }
    *ret = newSubcommand(argc, argv, inputFd, outputFd, isBackground);
    return pipeSize + 1;
}

SubcommandList parseSubcommands(Command* command) {
    SubcommandList ret = newSubcommandList();
    for (int i = 0; i < command->argc;) {
        Subcommand sc;
        i += nextSubcommand(command->argv + i, &sc, command->isBackground);
        if (sc == NULL) {
            freeSubcommandList(ret);
            return NULL;
        }
        appendSubcommand(sc, ret);
    }

#ifdef DEBUG
    debugSubcommandList(ret);
#endif

    return ret;
}

// Test
// int main() {
//     char buffer[1024] = {0};
//     addSpacesAround("abc>|hi hello", ">", "", buffer);
//     assert(strcmp(buffer, "abc > |hi hello") == 0);

//     addSpacesAround("abc > |hi", ">", "", buffer);
//     assert(strcmp(buffer, "abc  >  |hi") == 0);

//     addSpacesAround("abc>>hey", ">>", "", buffer);
//     assert(strcmp(buffer, "abc >> hey") == 0);

//     addSpacesAround(">>", ">>", "", buffer);
//     assert(strcmp(buffer, " >> ") == 0);

//     addSpacesAround(" >>", ">>", "", buffer);
//     assert(strcmp(buffer, "  >> ") == 0);
    
//     addSpacesAround(">>", "|", "", buffer);
//     assert(strcmp(buffer, ">>") == 0);

//     addSpacesAround("a>>>b", ">", ">>", buffer);
//     assert(strcmp(buffer, "a>> > b") == 0);

//     addSpacesAround("a>>>>b", ">", ">>", buffer);
//     assert(strcmp(buffer, "a>>>>b") == 0);

//     addSpacesAround("a>>b", ">", ">>", buffer);
//     assert(strcmp(buffer, "a>>b") == 0);
// }