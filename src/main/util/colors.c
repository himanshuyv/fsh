#include "../../header/headers.h"

void colorPrintf(char* color, char* format, ...) {
    printf(color);
    va_list arguments;
    va_start(arguments, format);
    vprintf(format, arguments);
    printf(COLOR_RESET);
}

char* ERROR_PREFIX = "[ERROR]: ";

void errorPrintf(char* format, ...) {
    // colorPrintf(COLOR_RED, ERROR_PREFIX);
    // colorPrintf(COLOR_RED, format, arguments);
    printf(COLOR_RED);
    va_list arguments;
    va_start(arguments, format);
    printf(ERROR_PREFIX);
    vprintf(format, arguments);
    printf(COLOR_RESET);
}