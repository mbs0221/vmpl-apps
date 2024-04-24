#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/syslog.h>
#include <sys/resource.h>
#include <syslog.h>

void test_user_ids() {
    uid_t ruid, euid;

    // 获取实际用户 ID 和有效用户 ID
    ruid = getuid();
    euid = geteuid();

    // 输出用户 ID 信息
    printf("Real User ID: %d\n", ruid);
    printf("Effective User ID: %d\n", euid);
}

void test_group_ids() {
    int ret;
    gid_t rgid, egid, pgid;

    // 获取实际组 ID 和有效组 ID, 以及进程组 ID
    rgid = getgid();
    egid = getegid();
    pgid = getpgrp();

    // 输出组 ID 信息
    printf("Real Group ID: %d\n", rgid);
    printf("Effective Group ID: %d\n", egid);
    printf("Process Group ID: %d\n", pgid);

    ret = getresgid(&rgid, &egid, &pgid);
    assert(ret == 0);
    printf("Real Group ID: %d, Effective Group ID: %d, Saved Group ID: %d\n", rgid, egid, pgid);

    ret = setresgid(1000, 1000, 1000);
    assert(ret != 0);
}

void test_process_ids() {
    pid_t pid, ppid;

    // 获取进程 ID 和父进程 ID
    pid = getpid();
    ppid = getppid();

    // 输出进程 ID 信息
    printf("Process ID: %d\n", pid);
    printf("Parent Process ID: %d\n", ppid);
}

void test_session_ids() {
    pid_t sid;

    // 获取会话 ID
    sid = getsid(0);

    // 输出会话 ID 信息
    printf("Session ID: %d\n", sid);
}

void test_logging() {
    // 使用 syslog 打印系统日志
    syslog(LOG_INFO, "Testing syslog");
    printf("Syslog message sent.\n");
}

void test_process_group() {
    pid_t pgid;

    // 获取进程组 ID
    pgid = getpgid(0);

    // 输出进程组 ID 信息
    printf("Process Group ID: %d\n", pgid);
}

void test_process_control() {
    pid_t ret;

    // 设置进程组 ID
    ret = setpgid(0, 0);
    if (ret == 0) {
        printf("Process group ID set successfully.\n");
    } else {
        printf("Failed to set process group ID.\n");
    }
}

void test_user_control() {
    // 设置用户 ID
    if (setuid(0) == 0) {
        printf("User ID set successfully.\n");
    } else {
        printf("Failed to set user ID.\n");
    }
}

void test_group_control() {
    // 设置组 ID
    if (setgid(0) == 0) {
        printf("Group ID set successfully.\n");
    } else {
        printf("Failed to set group ID.\n");
    }
}

void test_resource_control() {
    int ret;

    // 设置进程资源限制
    ret = setrlimit(RLIMIT_CPU, NULL);
    if (ret == 0) {
        printf("Process resource limits set successfully.\n");
    } else {
        printf("Failed to set process resource limits.\n");
    }
}

void test_hostname() {
    char buffer[1024];

    // 获取主机名
    gethostname(buffer, 1024);
    printf("Hostname: %s\n", buffer);

    // 获取域名
    getdomainname(buffer, 1024);
    printf("Domainname: %s\n", buffer);
}

void test_file_descriptor() {
    int fd;

    // 创建文件描述符
    fd = syscall(SYS_open, "test.txt", O_CREAT | O_RDWR, 0644);
    if (fd != -1) {
        printf("File descriptor created successfully.\n");
        close(fd);
    } else {
        printf("Failed to create file descriptor.\n");
    }
}

void test_file_system() {
    int ret;

    // 检查文件系统权限
    ret = access("/etc/passwd", F_OK);
    if (ret == 0) {
        printf("File system access allowed.\n");
    } else {
        printf("File system access denied.\n");
    }
}

void test_file_operations() {
    int ret;

    // 删除文件
    ret = unlink("test.txt");
    if (ret == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Failed to delete file.\n");
    }
}

int main() {
    printf("=== Testing User IDs ===\n");
    test_user_ids();

    printf("\n=== Testing Group IDs ===\n");
    test_group_ids();

    printf("\n=== Testing Process IDs ===\n");
    test_process_ids();

    printf("\n=== Testing Session IDs ===\n");
    test_session_ids();

    printf("\n=== Testing Logging ===\n");
    test_logging();

    printf("\n=== Testing Process Group ===\n");
    test_process_group();

    printf("\n=== Testing Process Control ===\n");
    test_process_control();

    printf("\n=== Testing User Control ===\n");
    test_user_control();

    printf("\n=== Testing Group Control ===\n");
    test_group_control();

    printf("\n=== Testing Hostname ===\n");
    test_hostname();

    printf("\n=== Testing Resource Control ===\n");
    test_resource_control();

    printf("\n=== Testing File Descriptor ===\n");
    test_file_descriptor();

    printf("\n=== Testing File System ===\n");
    test_file_system();

    printf("\n=== Testing File Operations ===\n");
    test_file_operations();

    return 0;
}
