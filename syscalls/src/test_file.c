#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

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

void test_flock() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用flock获取文件锁
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    assert(flock(fd, LOCK_SH) == 0);

    // 关闭文件描述符
    close(fd);
}

void test_fsync() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用fsync刷新文件
    assert(fsync(fd) == 0);

    // 关闭文件描述符
    close(fd);
}

void test_lseek() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 使用lseek获取文件当前偏移
    off_t offset = lseek(fd, 0, SEEK_CUR);
    assert(offset != -1);

    // 关闭文件描述符
    close(fd);
}

void test_stat() {
    // 使用stat获取文件状态
    struct stat statbuf;
    assert(stat("test.txt", &statbuf) == 0);

    // 检查文件是否为普通文件
    assert(S_ISREG(statbuf.st_mode));
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

void test_lstat() {
    // 使用lstat获取文件状态
    struct stat statbuf;
    assert(lstat("test.txt", &statbuf) == 0);

    // 检查文件是否为普通文件
    assert(S_ISREG(statbuf.st_mode));
}

void test_fstatat() {
    // 使用fstatat获取文件状态
    struct stat statbuf;
    assert(fstatat(AT_FDCWD, "test.txt", &statbuf, 0) == 0);

    // 检查文件是否为普通文件
    assert(S_ISREG(statbuf.st_mode));
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

void test_dup3() {
    // 打开一个文件
    int fd = open("test.txt", O_RDONLY);
    assert(fd != -1);

    // 复制文件描述符
    int newfd = dup3(fd, 100, O_CLOEXEC);
    assert(newfd != -1);

    // 关闭文件描述符
    close(fd);
    close(newfd);
}

int test_all() {
    int fd;
    ssize_t n;
    int ret;

    // creat
    fd = creat("testfile", S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return 1;
    }

    // write (note: write is a low-level system call, fwrite is the portable POSIX function)
    n = write(fd, "hello", 5);
    assert(n == 5);

    // fdatasync
    ret = fdatasync(fd);
    assert(ret == 0);

    // link
    ret = link("testfile", "testfile_link");
    assert(ret == 0);

    // rename
    rename("testfile_link", "testfile_renamed");
    assert(ret == 0);

    // mkdir
    mkdir("testdir", S_IRWXU);
    assert(ret == 0);

    // chdir
    chdir("testdir");
    assert(ret == 0);

    // getcwd
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    // fchdir
    fchdir(fd);
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    // rmdir
    chdir("..");
    rmdir("testdir");

    // truncate
    n = truncate("testfile_renamed", 0);
    assert(n == 0);

    // ftruncate
    n = ftruncate(fd, 0);
    assert(n == 0);

    // getdents (note: getdents is a low-level system call, readdir is the portable POSIX function)
    struct dirent *dir;
    DIR *d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // do something with dir
            fprintf(stderr, "Found file: %s\n", dir->d_name);
        }
        closedir(d);
    }

    close(fd);

    return 0;
}

int test_all2() {
    int fd;
    int ret;

    // creat
    fd = creat("testfile", S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return 1;
    }

    // symlink
    ret = symlink("testfile", "testfile_symlink");
    assert(ret == 0);

    // readlink
    char buffer[1024];
    readlink("testfile_symlink", buffer, sizeof(buffer));
    printf("Symlink target: %s\n", buffer);

    // chmod
    ret = chmod("testfile", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    assert(ret == 0);

    // fchmod
    ret = fchmod(fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    assert(ret == 0);

    // chown
    ret = chown("testfile", getuid(), getgid());
    assert(ret == 0);

    // fchown
    ret = fchown(fd, getuid(), getgid());
    assert(ret == 0);

    // lchown
    ret = lchown("testfile_symlink", getuid(), getgid());
    assert(ret == 0);

    // umask
    ret = umask(0022);
    printf("Previous umask: %o\n", ret);

    // unlink
    ret = unlink("testfile_symlink");
    assert(ret == 0);
    ret = unlink("testfile");
    assert(ret == 0);

    close(fd);

    return 0;
}

int test_loop() {
    // 测试文件操作
    test_fcntl();
    test_flock();
    test_fsync();
    test_lseek();
    // 测试文件状态
    test_stat();
    test_lstat();
    test_fstat();
    test_fstatat();
    // 测试文件访问
    test_access();
    // 测试文件描述符
    test_dup();
    test_dup2();
    test_dup3();
    // 测试所有系统调用
    test_all();
    test_all2();

    return 0;
}

int main(int argc, char *argv[]) {
    // 运行测试
    for (size_t i = 0; i < 1000000; i++) {
        test_loop();
        printf("Test %d passed\n", i);
    }

    return 0;
}