#include "../header/headers.h"

void addSpacesAround(char* haystack, char* match, char* ignore, char* buffer) {
    int matchLength = strlen(match);
    int ignoreLength = strlen(ignore);
    if (matchLength == 0) {
        strcpy(buffer, haystack);
        return;
    }

    int bufferPtr = 0;
    for (int i = 0; haystack[i] != '\0'; i++) {
        if (ignoreLength != 0) {
            char* firstOccurence = strstr(haystack + i, ignore);
            if (firstOccurence == haystack + i) {
                for (int j = 0; j < ignoreLength; j++) {
                    buffer[bufferPtr++] = ignore[j];
                } 

                i += ignoreLength - 1;
                continue;
            }
        } 
        char* firstOccurence = strstr(haystack + i, match);
        if (firstOccurence == haystack + i) {
            buffer[bufferPtr++] = ' ';
            for (int j = 0; j < matchLength; j++) {
                buffer[bufferPtr++] = match[j];
            }
            buffer[bufferPtr++] = ' ';
            i += matchLength - 1;
        } else {
            buffer[bufferPtr++] = haystack[i];
        }
    }   

    buffer[bufferPtr] = '\0';
}

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