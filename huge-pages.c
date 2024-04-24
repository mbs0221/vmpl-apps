#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE (2 * 1024 * 1024) // 2MB

int main()
{
    void *addr1 = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (addr1 == MAP_FAILED) {
        perror("Mmap failed");
        exit(1);
    }

    void *addr2 = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (addr2 == MAP_FAILED) {
        perror("Mmap failed");
        exit(1);
    }

    printf("成功在地址 %p 和 %p 处分配了两个巨大页面\n", addr1, addr2);

    munmap(addr1, PAGE_SIZE);
    munmap(addr2, PAGE_SIZE);

    return 0;
}