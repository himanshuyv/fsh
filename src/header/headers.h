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
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#define MAX_COMMAND_PER_LINE 32

#include "commands/pastevents.h"
#include "commands/peek.h"
#include "commands/proclore.h"
#include "commands/seek.h"
#include "commands/warp.h"
#include "executing.h"
#include "parsing.h"
#include "process_handling.h"
#include "prompt.h"
#include "rawmode.h"
#include "util/colors.h"
#include "util/command.h"
#include "util/file_util.h"
#include "util/history.h"
#include "util/process.h"
#include "util/queue.h"
#include "util/string_util.h"
#include "util/subcommand.h"
extern char longestCommand[DIRECTORY_BUFFER_SIZE];
extern time_t longestTime;
extern int STDIN_FD;
extern int STDOUT_FD;
#endif