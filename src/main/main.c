#include "../header/headers.h"

void initialize() {
    initPrompt();
    initHistory();
    initStandardIO();
    initSignalHandlers();
}

void destruct() { 
    destructHistory(); 
    destructBackground();
}

char longestCommand[DIRECTORY_BUFFER_SIZE] = "";
time_t longestTime = 0;
bool commandIsRunning = false;
int main() {
    initialize();
    while (1) {
        printFinishedProcesses();
        commandIsRunning = false;
        prompt();
        // fgets(input, INPUT_BUFFER_SIZE, stdin);
        readInput();
        // int len = strlen(input);
        // input[len - 1] = '\0';
        commandIsRunning = true;
        char spacedInput[INPUT_BUFFER_SIZE];
        addSpacesAround(input, "|", "", spacedInput);
        addSpacesAround(spacedInput, ">", ">>", input);
        addSpacesAround(input, "<", "", spacedInput);
        addSpacesAround(spacedInput, ">>", "", input);
        char rawInput[INPUT_BUFFER_SIZE];
        strcpy(rawInput, input);
#ifdef DEBUG
        printf("[MAIN_DEBUG] Input recieved: %s\n", input);
#endif
        Command buffer[MAX_COMMAND_PER_LINE];
        int commandCt = parseInput(buffer, MAX_COMMAND_PER_LINE, input);
        addEvent(buffer, commandCt, rawInput);
        strcpy(longestCommand, "");
        longestTime = 0;
        for (int i = 0; i < commandCt; i++) {
            time_t startTime = time(NULL);
            executeCommand(&buffer[i]);
            time_t endTime = time(NULL);
            time_t timeTaken = endTime - startTime;

            if (buffer[i].argc && timeTaken > longestTime) {
                longestTime = timeTaken;
                strcpy(longestCommand, buffer[i].argv[0]);
            }
        }
        for (int i = 0; i < commandCt; i++) freeCommand(&buffer[i]);
    }

    destruct();
}
