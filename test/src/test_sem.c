// test_sem

// Path: vmpl-apps/test/src/test_sem.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <vmpl/vmpl.h>

int test_sem()
{
    int semid;
    key_t key;
    struct sembuf sops;

    key = ftok(".", 'm');
    if (key == -1) {
        perror("ftok");
        return -1;
    }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return -1;
    }

    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    if (semop(semid, &sops, 1) == -1) {
        perror("semop");
        return -1;
    }

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        return -1;
    }

    return 0;
}

int main()
{
    VMPL_ENTER;
    test_sem();
    return 0;
}