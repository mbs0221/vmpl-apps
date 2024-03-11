#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

void test_fcntl() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用fcntl获取文件状态标志
    int flags = fcntl(fd, F_GETFL);
    assert(flags != -1);

    // 检查文件是否为只读
    assert((flags & O_ACCMODE) == O_RDONLY);

    // 关闭文件描述符
    close(fd);
}

void test_fstat() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用fstat获取文件状态
    struct stat statbuf;
    assert(fstat(fd, &statbuf) == 0);

    // 检查文件是否为普通文件
    assert(S_ISREG(statbuf.st_mode));

    // 关闭文件描述符
    close(fd);
}

void test_access() {
    // 检查文件是否存在
    assert(access("test.txt", F_OK) == 0);
}

void test_dup() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 复制文件描述符
    int newfd = dup(fd);
    assert(newfd != -1);

    // 关闭文件描述符
    close(fd);
    close(newfd);
}

void test_dup2() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 复制文件描述符
    int newfd = dup2(fd, 100);
    assert(newfd != -1);

    // 关闭文件描述符
    close(fd);
    close(newfd);
}

int main() {
    test_fcntl();
    test_fstat();
    test_access();
    test_dup();
    test_dup2();
    printf("Tests passed\n");
    return 0;
}