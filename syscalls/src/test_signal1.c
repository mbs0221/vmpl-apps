#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

void signal_handler(int signum) {
    printf("Received signal: %d\n", signum);
}

int main() {
    VMPL_ENTER;

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error setting signal handler");
        return 1;
    }

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("Error setting signal mask");
        return 1;
    }

    printf("Signal handler set. Waiting for signal...\n");

    // Simulating a long-running process
    for (;;) {
        // Do some work here...
    }

    return 0;
}
