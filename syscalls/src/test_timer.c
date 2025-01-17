#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/signal.h>
#include <sys/timerfd.h>

#define TIMER_SIG SIGRTMAX

void timer_handler(int sig, siginfo_t *si, void *uc) {
    // 当定时器到期时，打印一条消息
    printf("Timer expired\n");
}

int test_timer() {
    // 设置定时器到期时的处理函数
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(TIMER_SIG, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // 创建定时器
    timer_t timerid;
    struct sigevent sev;
    memset(&sev, 0, sizeof(struct sigevent));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMER_SIG;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }

    // 设置定时器的时间
    struct itimerspec its;
    its.it_value.tv_sec = 1; // 1秒后定时器到期
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1; // 每1秒定时器到期一次
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }

    // 输出定时器的时间
    if (timer_gettime(timerid, &its) == -1) {
        perror("timer_gettime");
        return 1;
    }

    // 主循环
    int count = 0;
    while (count < 5) {
        pause(); // 等待定时器到期
        count++;
    }

    // 输出定时器的溢出次数
    int overrun;
    if (timer_getoverrun(timerid) == -1) {
        perror("timer_getoverrun");
        return 1;
    }


    // 删除定时器
    if (timer_delete(timerid) == -1) {
        perror("timer_delete");
        return 1;
    }

    return 0;
}

int test_timerfd() {
    // 创建定时器
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1) {
        perror("timerfd_create");
        return 1;
    }

    // 设置定时器的时间
    struct itimerspec its;
    its.it_value.tv_sec = 1; // 1秒后定时器到期
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1; // 每1秒定时器到期一次
    its.it_interval.tv_nsec = 0;
    if (timerfd_settime(fd, 0, &its, NULL) == -1) {
        perror("timerfd_settime");
        return 1;
    }

    // 主循环
    int count = 0;
    while (count < 5) {
        uint64_t exp;
        if (read(fd, &exp, sizeof(uint64_t)) == -1) {
            perror("read");
            return 1;
        }
        printf("Timer expired\n");
        count++;
    }

    // 删除定时器
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}

int main() {
    printf("Timer test:\n");
    if (test_timer() != 0) {
        return 1;
    }

    printf("Timerfd test:\n");
    if (test_timerfd() != 0)
    {
        return 1;
    }

    printf("All tests passed\n");
    return 0;
}