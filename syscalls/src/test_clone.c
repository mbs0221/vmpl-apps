#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vmpl/vmpl.h>

int main() {
    printf("Hello from parent process!\n");
    VMPL_ENTER;
    printf("Parent process: PID = %d\n", getpid());
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child process
        printf("Child process: PID = %d\n", getpid());
        printf("Child process: Parent PID = %d\n", getppid());
        VMPL_ENTER;
        printf("Hello from child process!\n");
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent process: PID = %d\n", getpid());
        printf("Parent process: Child PID = %d\n", child_pid);
        wait(NULL);
    }

    return 0;
}