#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    // Allocate a memory region
    size_t size = 4096;
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Perform madvise operations
    int result = madvise(addr, size, MADV_RANDOM);
    if (result == -1) {
        perror("madvise");
        exit(1);
    }

    // Print the result
    printf("madvise operation completed successfully\n");

    // Free the memory region
    result = munmap(addr, size);
    if (result == -1) {
        perror("munmap");
        exit(1);
    }

    return 0;
}