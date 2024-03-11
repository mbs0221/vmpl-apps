// Path: vmpl-apps/test/src/test_pipe.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vmpl/vmpl.h>

int test_pipe()
{
    int pipefd[2];
    pid_t cpid1, cpid2;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    cpid1 = fork();
    if (cpid1 == -1) {
        perror("fork");
        return -1;
    }

    if (cpid1 == 0) {    /* Child 1 writes to pipe */
        VMPL_ENTER;
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], "hello from child 1\n", 19);
        close(pipefd[1]);
        _exit(EXIT_SUCCESS);

    } else {            /* Parent reads from pipe */
        cpid2 = fork();
        if (cpid2 == -1) {
            perror("fork");
            return -1;
        }

        if (cpid2 == 0) {    /* Child 2 reads from pipe */
            VMPL_ENTER;
            close(pipefd[1]);          /* Close unused write end */

            while (read(pipefd[0], &buf, 1) > 0)
                write(STDOUT_FILENO, &buf, 1);

            write(STDOUT_FILENO, "\n", 1);
            close(pipefd[0]);
            _exit(EXIT_SUCCESS);

        } else {    /* Parent waits for both children */
            close(pipefd[0]);          /* Close unused read end */
            close(pipefd[1]);          /* Close unused write end */
            waitpid(cpid1, NULL, 0);
            waitpid(cpid2, NULL, 0);
        }
    }

    return 0;
}

// test pipe2 syscall with two sub processes
int test_pipe2()
{
    int pipefd[2];
    pid_t cpid1, cpid2;
    char buf;

    if (pipe2(pipefd, 0) == -1) {
        perror("pipe2");
        return -1;
    }

    cpid1 = fork();
    if (cpid1 == -1) {
        perror("fork");
        return -1;
    }

    if (cpid1 == 0) {    /* Child 1 writes to pipe */
        VMPL_ENTER;
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], "hello from child 1\n", 19);
        close(pipefd[1]);
        _exit(EXIT_SUCCESS);

    } else {            /* Parent reads from pipe */
        cpid2 = fork();
        if (cpid2 == -1) {
            perror("fork");
            return -1;
        }

        if (cpid2 == 0) {    /* Child 2 reads from pipe */
            VMPL_ENTER;
            close(pipefd[1]);          /* Close unused write end */

            while (read(pipefd[0], &buf, 1) > 0)
                write(STDOUT_FILENO, &buf, 1);

            write(STDOUT_FILENO, "\n", 1);
            close(pipefd[0]);
            _exit(EXIT_SUCCESS);

        } else {    /* Parent waits for both children */
            close(pipefd[0]);          /* Close unused read end */
            close(pipefd[1]);          /* Close unused write end */
            waitpid(cpid1, NULL, 0);
            waitpid(cpid2, NULL, 0);
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;

    ret = test_pipe();
    assert(ret == 0);

    ret = test_pipe2();
    assert(ret == 0);

    return 0;
}