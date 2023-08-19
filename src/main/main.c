#include "../header/headers.h"

int initialize() {
    initPrompt();
}

int main() {
    initialize();
    while (1) {
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
    }
}
