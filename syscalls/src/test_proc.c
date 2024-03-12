// src/test_proc.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vmpl/vmpl.h>

int main(int argc, char *argv[])
{
    int pid;
    int status;
    char *args[] = {"/bin/ls", "-l", NULL};

    pid = fork();
    if (pid == 0) {
        VMPL_ENTER;
        execv(args[0], args);
        perror("execv");
        exit(1);
    } else if (pid > 0) {
        wait(&status);
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}