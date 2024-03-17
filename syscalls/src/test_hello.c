#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main() {
    // 获取当前时间
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    // 将时间戳转换为本地时间
    struct tm *local_time = localtime(&current_time.tv_sec);

    // 格式化时间为字符串，包含毫秒
    char formatted_time[50];
    strftime(formatted_time, sizeof(formatted_time), "%d %b %Y %H:%M:%S", local_time);
    sprintf(formatted_time + strlen(formatted_time), ".%03ld", current_time.tv_usec / 1000);

    // 打印格式化后的时间
    printf("Formatted Time: %s\n", formatted_time);

    return 0;
}
