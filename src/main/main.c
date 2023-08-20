#include "../header/headers.h"

void initialize() { initPrompt(); }

#define MAX_COMMAND_PER_LINE 128
#define INPUT_BUFFER 4096

int main() {
    initialize();
    while (1) {
        prompt();
        char input[INPUT_BUFFER];
        fgets(input, INPUT_BUFFER, stdin);
        int len = strlen(input);
        input[len - 1] = '\0';

#ifdef DEBUG
        printf("[MAIN_DEBUG] Input recieved: %s\n", input);
#endif

        Command buffer[MAX_COMMAND_PER_LINE];
        int commandCt = parseInput(buffer, MAX_COMMAND_PER_LINE, input);
        for (int i = 0; i < commandCt; i++) executeCommand(&buffer[i]);
        for (int i = 0; i < commandCt; i++) freeCommand(&buffer[i]);
    }
}
