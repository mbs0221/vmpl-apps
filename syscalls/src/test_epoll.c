// Path: vmpl-apps/test/src/test_epoll.c
// test_epoll();

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/eventfd.h>
#include <sys/signalfd.h>
#include <fcntl.h>

#define MAX_EVENTS 5

void test_poll() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用poll等待文件可读
    struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    assert(poll(fds, 1, 1000) == 1);

    // 读取文件内容
    char buf[1024];
    int n = read(fd, buf, sizeof(buf));
    assert(n > 0);

    // 打印文件内容
    write(STDOUT_FILENO, buf, n);

    // 关闭文件描述符
    close(fd);
}

void test_epoll() {
    // 创建epoll实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return;
    }

    // 添加标准输入到epoll实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = 0;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event) == -1) {
        perror("epoll_ctl");
        close(epfd);
        return;
    }

    // 创建epoll事件数组
    struct epoll_event events[MAX_EVENTS];

    // 主循环
    int count = 0;
    while (count < 5) {
        // 等待事件
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        // 处理事件
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == 0) {
                // 从标准输入读取一行
                char buf[1024];
                if (fgets(buf, sizeof(buf), stdin) == NULL) {
                    perror("fgets");
                    break;
                }
                printf("Read: %s", buf);
            }
        }

        count++;
    }

    // 关闭epoll实例
    close(epfd);
}

void test_select() {
    // 创建文件描述符集合
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    // 主循环
    int count = 0;
    while (count < 5) {
        // 等待事件
        int ret = select(1, &rfds, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            break;
        }

        // 处理事件
        if (FD_ISSET(0, &rfds)) {
            // 从标准输入读取一行
            char buf[1024];
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                perror("fgets");
                break;
            }
            printf("Read: %s", buf);
        }

        count++;
    }
}

void test_eventfd() {
    // 创建eventfd实例
    int efd = eventfd(0, 0);
    if (efd == -1) {
        perror("eventfd");
        return;
    }

    // 创建epoll实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(efd);
        return;
    }

    // 添加eventfd到epoll实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = efd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &event) == -1) {
        perror("epoll_ctl");
        close(epfd);
        close(efd);
        return;
    }

    // 主循环
    int count = 0;
    while (count < 5) {
        // 等待事件
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        // 处理事件
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == efd) {
                // 读取eventfd的值
                uint64_t value;
                if (read(efd, &value, sizeof(value)) == -1) {
                    perror("read");
                    break;
                }
                printf("Value: %lu\n", value);
            }
        }

        count++;
    }

    // 关闭epoll实例
    close(epfd);

    // 关闭eventfd实例
    close(efd);
}

void test_signalfd() {
    // 创建信号集合
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);

    // 阻塞信号
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        return;
    }

    // 创建signalfd实例
    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        return;
    }

    // 创建epoll实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(sfd);
        return;
    }

    // 添加signalfd到epoll实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event) == -1) {
        perror("epoll_ctl");
        close(epfd);
        close(sfd);
        return;
    }

    // 主循环
    int count = 0;
    while (count < 5) {
        // 等待事件
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        // 处理事件
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sfd) {
                // 读取信号
                struct signalfd_siginfo info;
                if (read(sfd, &info, sizeof(info)) == -1) {
                    perror("read");
                    break;
                }
                printf("Signal: %d\n", info.ssi_signo);
            }
        }

        count++;
    }

    // 关闭epoll实例
    close(epfd);

    // 关闭signalfd实例
    close(sfd);
}

int main() {
    test_poll();
    test_epoll();
    test_select();
    test_eventfd();
    test_signalfd();

    return 0;
}