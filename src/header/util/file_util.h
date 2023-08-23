#ifndef __FILE_UTIL_H
#define __FILE_UTIL_H

#include <sys/stat.h>

void replaceHomeDirPrefix(char* path);
bool isHomeDirPrefix(char* path);
void replaceTildaWithHome(char* path);
void getAbsolutePath(char* prefix, char* fileName, char* buffer);
int getStat(char* prefix, char* fileName, struct stat* fileStat);
void printFileName(struct dirent* file, struct stat fileStat);
void freeDirent(struct dirent** dir, int fileCount);
#endif