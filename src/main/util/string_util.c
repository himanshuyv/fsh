#include "../../header/headers.h"

bool isNum(char* str) {
    if (str[0] == '\0') return false;
    for (int i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i])) return false;
    
    return true;
}

bool isBlank(char* str) {
    for (int i = 0; str[i] != '\0'; i++)
        if (!isspace(str[i])) return false;
    
    return true;
}

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