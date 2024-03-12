#include <stdio.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

int main() {
    printf("Hello from parent process!\n");
    VMPL_ENTER;
    printf("Hello from parent process after VMPL_ENTER!\n");
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hello from child process!\n");
        VMPL_ENTER;
        printf("Hello from child process after VMPL_ENTER!\n");
    } else {
        // Parent process
        printf("Hello from parent process!\n");
    }

    return 0;
}