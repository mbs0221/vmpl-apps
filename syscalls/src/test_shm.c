#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

#define SHM_SIZE 1024
#define SLEEP_TIME 1

void test_shm() {
    int shmid;
    key_t key;
    pid_t pid1, pid2;
    char *shm;

    // 创建共享内存
    key = ftok(".", 's');
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // 创建子进程1
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        // 子进程1

        VMPL_ENTER;
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) {
            perror("shmat");
            exit(1);
        }
        printf("shm: %p\n", shm);
        printf("Write to shm\n");
        sprintf(shm, "Hello, world!");
        printf("shm: %s\n", shm);
        shmdt(shm);
        exit(0);
    }

    sleep(SLEEP_TIME); // Wait for child to start

    // 创建子进程2
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    } else if (pid2 == 0) {
        // 子进程2
        
        VMPL_ENTER;
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) {
            perror("shmat");
            exit(1);
        }
        printf("shm: %p\n", shm);
        printf("Read from shm\n");
        printf("shm: %s\n", shm);
        shmdt(shm);
        exit(0);
    }

    // 等待子进程结束
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
}

int main() {
    test_shm();
    return 0;
}