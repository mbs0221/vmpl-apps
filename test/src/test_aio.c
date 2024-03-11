
#include <aio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

void test_aio() {
    // 创建异步IO上下文
    struct aiocb cb;
    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = 0;
    cb.aio_buf = malloc(1024);
    cb.aio_nbytes = 1024;
    cb.aio_offset = 0;

    // 读取标准输入
    if (aio_read(&cb) == -1) {
        perror("aio_read");
        return;
    }

    // 等待异步IO完成
    while (aio_error(&cb) == EINPROGRESS) {
        usleep(1000);
    }

    // 打印读取的内容
    int n = aio_return(&cb);
    write(STDOUT_FILENO, cb.aio_buf, n);

    // 释放异步IO缓冲区
    free(cb.aio_buf);
}

void test_aio_suspend() {
    // 创建异步IO上下文
    struct aiocb cb;
    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = 0;
    cb.aio_buf = malloc(1024);
    cb.aio_nbytes = 1024;
    cb.aio_offset = 0;

    // 读取标准输入
    if (aio_read(&cb) == -1) {
        perror("aio_read");
        return;
    }

    // 等待异步IO完成
    struct aiocb *list[1] = {&cb};
    aio_suspend(list, 1, NULL);

    // 打印读取的内容
    int n = aio_return(&cb);
    write(STDOUT_FILENO, cb.aio_buf, n);

    // 释放异步IO缓冲区
    free(cb.aio_buf);
}

void test_aio_cancel() {
    // 创建异步IO上下文
    struct aiocb cb;
    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = 0;
    cb.aio_buf = malloc(1024);
    cb.aio_nbytes = 1024;
    cb.aio_offset = 0;

    // 读取标准输入
    if (aio_read(&cb) == -1) {
        perror("aio_read");
        return;
    }

    // 取消异步IO
    if (aio_cancel(0, &cb) == -1) {
        perror("aio_cancel");
        return;
    }

    // 打印读取的内容
    int n = aio_return(&cb);
    write(STDOUT_FILENO, cb.aio_buf, n);

    // 释放异步IO缓冲区
    free(cb.aio_buf);
}

void test_aio_error() {
    // 创建异步IO上下文
    struct aiocb cb;
    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = 0;
    cb.aio_buf = malloc(1024);
    cb.aio_nbytes = 1024;
    cb.aio_offset = 0;

    // 读取标准输入
    if (aio_read(&cb) == -1) {
        perror("aio_read");
        return;
    }

    // 等待异步IO完成
    while (aio_error(&cb) == EINPROGRESS) {
        usleep(1000);
    }

    // 打印错误码
    printf("Error: %d\n", aio_error(&cb));

    // 释放异步IO缓冲区
    free(cb.aio_buf);
}

int main() {
    VMPL_ENTER;
    // 测试异步IO
    test_aio();
    test_aio_suspend();
    test_aio_cancel();
    test_aio_error();
    printf("Tests passed\n");
    return 0;
}