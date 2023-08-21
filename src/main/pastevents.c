#include "../header/headers.h"

#define RADIX 10

int purge() {
    int queueSize = getHistorySize();
    for (int i = 0; i < queueSize; i++) {
        pop(eventQueue);
    }

    writeHistory();
    return EXEC_FAILURE;
}

int pastEventsExecute(Command* command) {
    if (command->argc != 3) {
        fprintf(stderr, "[ERROR]: Invalid number of arguments for pastevents execute: %ld", command->argc);
        return EXEC_FAILURE;
    } 

    if (!isNum(command->argv[2])) {
        fprintf(stderr, "[ERROR]: pastevents execute: expected index, found: \'%s\'", command->argv[2]);
        return EXEC_FAILURE;
    }
 
    char* endPointer;
    int k = (int) strtoll(command->argv[2], &endPointer, RADIX);

    if (k > MAX_HISTORY_SIZE || k <= 0) {
        return EXEC_FAILURE;
    }
    

    char* kthEvent = getKthLastEvent(k);
    int length = strlen(kthEvent);
    char kthEventCopy[length + 1];

#ifdef DEBUG
    printf("[PAST_EVENT_EXEC_DEBUG]: k = %d\n", k);
#endif

    char kthEventCopyCopy[length + 1];
    strcpy(kthEventCopy, kthEvent);
    strcpy(kthEventCopyCopy, kthEvent);

#ifdef DEBUG
    printf("[PAST_EVENT_EXEC_DEBUG]: kth event is: \'%s\'\n", kthEvent);
#endif

    Command buffer[MAX_COMMAND_PER_LINE];
    int commandCt = parseInput(buffer, MAX_COMMAND_PER_LINE, kthEventCopy);
    addEvent(buffer, commandCt, kthEventCopyCopy);
    for (int i = 0; i < commandCt; i++) executeCommand(&buffer[i]);
    for (int i = 0; i < commandCt; i++) freeCommand(&buffer[i]);
    return EXEC_SUCCESS;
}