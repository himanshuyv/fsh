#include "../header/headers.h"

void initialize() {
    initPrompt();
}

int main() {
    initialize();
    while (1) {
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        int len = strlen(input);
        input[len - 1] = '\0';
        changeDirectory(input);
#ifdef DEBUG
        printf("[MAIN_DEBUG] Input recieved: %s\n", input);
#endif
    }
}
