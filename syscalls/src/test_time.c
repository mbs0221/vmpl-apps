// Path: vmpl-apps/test/src/test_time.c
// test_gettime()
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

int test_gettime() {
    // 获取当前时间
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
        return 1;
    }
    printf("Current time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

    return 0;
}

int test_clock_getres() {
    // 获取时钟精度
    struct timespec res;
    if (clock_getres(CLOCK_REALTIME, &res) == -1) {
        perror("clock_getres");
        return 1;
    }
    printf("Clock resolution: %ld.%09ld\n", res.tv_sec, res.tv_nsec);

    return 0;
}

int test_clock_nanosleep() {
    // 休眠
    struct timespec req = {1, 0};
    struct timespec rem;
    if (clock_nanosleep(CLOCK_REALTIME, 0, &req, &rem) == -1) {
        perror("clock_nanosleep");
        return 1;
    }
    printf("Slept for %ld.%09ld\n", rem.tv_sec, rem.tv_nsec);

    return 0;
}

int test_clock() {
    // 获取进程时间
    clock_t c = clock();
    if (c == (clock_t)-1) {
        perror("clock");
        return 1;
    }
    printf("Process time: %ld\n", c);

    return 0;
}

int test_time() {
    // 获取当前时间
    time_t t = time(NULL);
    if (t == (time_t)-1) {
        perror("time");
        return 1;
    }
    printf("Current time: %ld\n", t);

    return 0;
}

int test_difftime() {
    // 计算时间差
    time_t t1 = 0;
    time_t t2 = 1000000000;
    double diff = difftime(t2, t1);
    printf("Time difference: %f\n", diff);

    return 0;
}

char buf[64];

int test_strftime() {
    // 格式化时间
    time_t t = time(NULL);
    if (t == (time_t)-1) {
        perror("time");
        return 1;
    }
    struct tm *tm = localtime(&t);
    if (tm == NULL) {
        perror("localtime");
        return 1;
    }
    
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm) == 0) {
        perror("strftime");
        return 1;
    }
    printf("Formatted time: %s\n", buf);

    return 0;
}

int test_strptime() {
    // 解析时间
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    char *s = "2021-01-01 00:00:00";
    char *r = strptime(s, "%Y-%m-%d %H:%M:%S", &tm);
    if (r == NULL) {
        perror("strptime");
        return 1;
    }
    time_t t = mktime(&tm);
    if (t == (time_t)-1) {
        perror("mktime");
        return 1;
    }
    printf("Parsed time: %ld\n", t);

    return 0;
}

int main() {
    int ret = 0;
    ret = test_gettime();
    assert(ret == 0);

    ret = test_clock_getres();
    assert(ret == 0);

    ret = test_clock_nanosleep();
    assert(ret == 0);

    ret = test_clock();
    assert(ret == 0);

    ret = test_time();
    assert(ret == 0);

    ret = test_difftime();
    assert(ret == 0);

    ret = test_strftime();
    assert(ret == 0);

    ret = test_strptime();
    assert(ret == 0);

    printf("Tests passed\n");

    return 0;
}