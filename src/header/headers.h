#ifndef HEADERS_H_
#define HEADERS_H_

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#define MAX_COMMAND_PER_LINE 32

#define DEBUG

#include "executing.h"
#include "parsing.h"
#include "pastevents.h"
#include "peek.h"
#include "process_handling.h"
#include "proclore.h"
#include "prompt.h"
#include "util/colors.h"
#include "util/command.h"
#include "util/history.h"
#include "util/process.h"
#include "util/queue.h"
#include "warp.h"
bool isNum(char* str);
bool isBlank(char* str);
void replaceHomeDirPrefix(char* path);
bool isHomeDirPrefix(char* path);
void replaceTildaWithHome(char* path);
#endif