#include "../../header/headers.h"

const char* LOAD_AVG_FILE = "/proc/loadavg";

int repeatInterval = 0;
void printRecentPID(int sig) {
    FILE* loadAvg = fopen(LOAD_AVG_FILE, "r");
    if (loadAvg == NULL) {
        fprintf(stderr, "[ERROR]: Could not open \'%s\'\n", LOAD_AVG_FILE);
    } else {
        char readBuffer[1024];
        for (int i = 0; i < 5; i++) fscanf(loadAvg, "%s", readBuffer);
        printf("%s\n", readBuffer);
        fclose(loadAvg);
    }

    alarm(repeatInterval);
}

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int neonate(Subcommand command) {
    if (command->argc != 3) {
        fprintf(stderr, "[ERROR]: neonate expects 2 arguments, found %lu\n",
                command->argc - 1);
        return EXEC_FAILURE;
    }

    if (strcmp(command->argv[1], "-n") != 0) {
        fprintf(stderr, "[ERROR]: Invalid argument for neonate \'%s\'\n",
                command->argv[1]);
        return EXEC_FAILURE;
    }

    if (!isNum(command->argv[2])) {
        fprintf(stderr, "[ERROR]: Invalid argument for neonate \'%s\'\n",
                command->argv[2]);
        return EXEC_FAILURE;
    }

    char* endPointer;
    repeatInterval = (int)strtoll(command->argv[2], &endPointer, RADIX);
    bool spamFlag = (repeatInterval == 0);
    char input;
    enableRawMode();
    if (spamFlag) {
        while (1) {
            if (kbhit()) {
                if (read(STDIN_FILENO, &input, 1) == 1)
                    if (input == 'x') {
                        break;
                    }
            } else {
                FILE* loadAvg = fopen(LOAD_AVG_FILE, "r");
                if (loadAvg == NULL) {
                    fprintf(stderr, "[ERROR]: Could not open \'%s\'\n",
                            LOAD_AVG_FILE);
                } else {
                    char readBuffer[1024];
                    for (int i = 0; i < 5; i++)
                        fscanf(loadAvg, "%s", readBuffer);
                    printf("%s\n", readBuffer);
                    fclose(loadAvg);
                }
            }
        }
    } else {
        alarm(repeatInterval);
        while (read(STDIN_FILENO, &input, 1) == 1) {
            if (input == 'x') {
                alarm(0);
                break;
            }
        }
    }
    disableRawMode();

    return EXEC_SUCCESS;
}