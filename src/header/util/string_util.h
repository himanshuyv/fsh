#ifndef __STRING_UTIL_H
#define __STRING_UTIL_H

bool isNum(char* str);
bool isBlank(char* str);
void addSpacesAround(char* haystack, char* match, char* ignore, char* buffer);
#endif