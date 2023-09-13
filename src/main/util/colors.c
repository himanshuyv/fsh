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
    fprintf(stderr, COLOR_RED);
    va_list arguments;
    va_start(arguments, format);
    fprintf(stderr, ERROR_PREFIX);
    vfprintf(stderr, format, arguments);
    fprintf(stderr, COLOR_RESET);
}