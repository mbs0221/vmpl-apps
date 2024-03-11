// Path: vmpl-apps/test/src/test_msg.c
#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <vmpl/vmpl.h>

#define MAXSIZE     128

key_t get_key()
{
    key_t key;
    key = ftok(".", 'm');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}

int test_msg()
{
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = get_key();

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        return -1;
    }

    if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0) {
        perror("msgrcv");
        return -1;
    }

    printf("%s\n", rcvbuffer.mtext);
    return 0;
}

int test_msg2()
{
    int msqid;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = get_key();

    if ((msqid = msgget(key, IPC_CREAT| 0666)) < 0) {
        perror("msgget");
        return -1;
    }

    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Did you get this?");
    buflen = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) {
        perror("msgsnd");
        return -1;
    }

    return 0;
}

int on_exit()
{
    key_t key;
    key = get_key();

    // query the info of the message queue
    struct msqid_ds buf;
    if (msgctl(key, IPC_STAT, &buf) < 0) {
        perror("msgctl");
        return -1;
    }

    // remove the message queue
    if (msgctl(key, IPC_RMID, NULL) < 0) {
        perror("msgctl");
        return -1;
    }

    // check if the message queue is removed
    if (msgctl(key, IPC_STAT, &buf) == 0) {
        printf("The message queue is not removed\n");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    if (argc < 2) {
        printf("Usage: %s <test_num>\n", argv[0]);
        return -1;
    }

    atexit(on_exit);
    switch (atoi(argv[1])) {
    case 1:
        VMPL_ENTER;
        ret = test_msg();
        break;
    case 2:
        VMPL_ENTER;
        ret = test_msg2();
        break;
    default:
        printf("Invalid test number\n");
        return -1;
    }

    return 0;
}