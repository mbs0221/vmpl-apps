#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <liburing.h>

void test_io_uring() {
    // 创建io_uring实例
    struct io_uring ring;
    if (io_uring_queue_init(8, &ring, 0) != 0) {
        perror("io_uring_queue_init");
        return;
    }

    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 准备读取文件
    char buf[1024];
    struct iovec iov = {
        .iov_base = buf,
        .iov_len = sizeof(buf),
    };
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_readv(sqe, fd, &iov, 1, 0);

    // 提交读取请求
    io_uring_submit(&ring);

    // 等待读取完成
    struct io_uring_cqe *cqe;
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    // 打印读取的内容
    write(STDOUT_FILENO, buf, cqe->res);

    // 关闭文件描述符
    close(fd);

    // 关闭io_uring实例
    io_uring_queue_exit(&ring);
}

void test_io_uring_register_files() {
    // 创建io_uring实例
    struct io_uring ring;
    if (io_uring_queue_init(8, &ring, 0) != 0) {
        perror("io_uring_queue_init");
        return;
    }

    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 注册文件到io_uring实例
    if (io_uring_register_files(&ring, &fd, 1) != 0) {
        perror("io_uring_register_files");
        close(fd);
        io_uring_queue_exit(&ring);
        return;
    }

    // 准备读取文件
    char buf[1024];
    struct iovec iov = {
        .iov_base = buf,
        .iov_len = sizeof(buf),
    };
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_readv(sqe, fd, &iov, 1, 0);

    // 提交读取请求
    io_uring_submit(&ring);

    // 等待读取完成
    struct io_uring_cqe *cqe;
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    // 打印读取的内容
    write(STDOUT_FILENO, buf, cqe->res);

    // 关闭文件描述符
    close(fd);

    // 关闭io_uring实例
    io_uring_queue_exit(&ring);
}

int main() {
    test_io_uring();
    test_io_uring_register_files();
    return 0;
}