#include "../../header/headers.h"

void colorPrintf(char* color, char* format, ...) {
    printf(color);
    va_list arguments;
    va_start(arguments, format);
    vprintf(format, arguments);
    printf(COLOR_RESET);
}