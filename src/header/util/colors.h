#ifndef __COLOR_H
#define __COLOR_H

#define COLOR_RESET "\033[0m"

#define COLOR_BLACK "\033[30;1m"
#define COLOR_RED "\033[31;1m"
#define COLOR_GREEN "\033[32;1m"
#define COLOR_YELLOW "\033[33;1m"
#define COLOR_BLUE "\033[34;1m"
#define COLOR_MAGENTA "\033[35;1m"
#define COLOR_CYAN "\033[36;1m"
#define COLOR_WHITE "\033[37;1m"

#define EXECUTABLE_COLOR COLOR_GREEN
#define FILE_COLOR COLOR_WHITE
#define DIRECTORY_COLOR COLOR_BLUE

#define USERNAME_COLOR COLOR_BLUE
#define SYSNAME_COLOR COLOR_CYAN
void colorPrintf(char* color, char* format, ...);

#endif