// Test all kinds of page faults
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int signal_handler(int signo, siginfo_t *info, void *context) {
    printf("Received signal %d\n", signo);
    printf("Address: %p\n", info->si_addr);
    printf("Error code: %d\n", info->si_code);
    printf("Faulting instruction address: %p\n", info->si_addr);
    printf("Faulting memory address: %p\n", info->si_addr);
    return 0;
}

int main(int argc, char *argv[]) {
    char *p = (char *)mmap(NULL, PAGE_SIZE, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    struct sigaction act;
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, NULL);

    // Read from a page that is not mapped
    printf("Reading from a page that is not mapped\n");
    printf("%c\n", p[0]);

    // Write to a page that is not mapped
    printf("Writing to a page that is not mapped\n");
    p[0] = 'a';

    // Read from a page that is not readable
    printf("Reading from a page that is not readable\n");
    mprotect(p, PAGE_SIZE, PROT_NONE);
    printf("%c\n", p[0]);

    // Write to a page that is not writable
    printf("Writing to a page that is not writable\n");
    mprotect(p, PAGE_SIZE, PROT_READ);
    p[0] = 'a';

    // Read from a page that is not present
    printf("Reading from a page that is not present\n");
    munmap(p, PAGE_SIZE);
    printf("%c\n", p[0]);

    // Write to a page that is not present
    printf("Writing to a page that is not present\n");
    p[0] = 'a';

    return 0;
}