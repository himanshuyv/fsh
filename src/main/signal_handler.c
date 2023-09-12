#include "../header/headers.h"

void handleCtrlC(int sig) {
    if (!commandIsRunning) {
        pt = 0;
        input[0] = '\0';
        printf("\n");
        prompt();
    }
}

void initSignalHandlers() {
    signal(SIGTSTP, SIG_IGN);  // Ctrl Z -> SIGIGN
    struct sigaction act;
    act.sa_flags = SA_RESTART;
    act.sa_handler = handleCtrlC;
    sigaction(SIGINT, &act, NULL);
}