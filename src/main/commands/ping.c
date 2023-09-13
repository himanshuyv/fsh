#include "../../header/headers.h"

int ping(Subcommand command) {  
    if (command->argc != 3) {
        fprintf(stderr, "[ERROR]: Ping expects 2 arguments found %ld\n", command->argc - 1);
        return EXEC_FAILURE;    
    } 

    if (!isNum(command->argv[1])) {
        fprintf(stderr, "[ERROR]: Argument \'%s\' for ping is not valid\n", command->argv[1]);
        return EXEC_FAILURE;
    }

    if (!isNum(command->argv[2])) {
        fprintf(stderr, "[ERROR]: Argument \'%s\' for ping is not valid\n", command->argv[2]);
        return EXEC_FAILURE;
    }

    char* endPointer;
    pid_t pid = (int) strtoll(command->argv[1], &endPointer, RADIX);
    int signal = (int) strtoll(command->argv[2], &endPointer, RADIX);
    signal %= 32;

    if (kill(pid, 0) == -1) {
        fprintf(stderr, "[ERROR]: Process %d not found\n", pid);
        return EXEC_FAILURE;
    }

    if (kill(pid, signal) == -1) {
        fprintf(stderr, "[ERROR]: Could not ping %d with signal %d\n", pid, signal);
        return EXEC_FAILURE;
    }
    printf("Sent signal %d to process with pid %d\n", signal, pid);
    return EXEC_SUCCESS;
}