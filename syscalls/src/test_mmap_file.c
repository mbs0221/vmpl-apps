#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_SIZE (64 * 1024 * 1024) // 64MB

#define FILE0_SIZE 0x4100014
#define FILE1_SIZE 0x3938714
#define FILE2_SIZE 0x493e14
#define PAGE_SIZE (4096)

struct file_test {
    char *name;
    size_t size;
};

struct file_test files[] = {
    {"default", FILE_SIZE},
    {"file0", FILE0_SIZE},
    {"file1", FILE1_SIZE},
    {"file2", FILE2_SIZE},
    {NULL, 0}
};

int main(int argc, char* argv[]) {
    struct file_test *file;
    for (file = files; file->name != NULL; file++) {
        create_test_file(file->name, file->size);
        test_mmap_file(file->name, file->size);
    }

    return 0;
}

int test_mmap_file(const char* file_path, size_t file_size) {
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 将文件映射到内存中
    char* addr = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 访问并修改每一个页面
    for (size_t i = 0; i < file_size; i += PAGE_SIZE) {
        addr[i] = 'A';
    }

    // 关闭文件映射
    if (munmap(addr, FILE_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}

void create_test_file(const char* file_path, size_t file_size) {
    printf("Creating file %s with size %zu bytes\n", file_path, file_size);
    FILE* fp = fopen(file_path, "wb");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // 写入空字节，创建指定大小的文件
    ftruncate(fileno(fp), file_size);
    fclose(fp);

    printf("File created successfully\n");
}