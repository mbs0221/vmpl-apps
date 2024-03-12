#include <stdio.h>
#include <signal.h>
#include <vmpl/vmpl.h>

void sigint_handler(int signum) {
    printf("Received SIGINT signal\n");
}

int main() {
    VMPL_ENTER;
    // Register the SIGINT signal handler
    signal(SIGINT, sigint_handler);

    printf("Press Ctrl+C to send SIGINT signal\n");

    // Infinite loop to keep the program running
    while (1) {
        // Do nothing
    }

    return 0;
}