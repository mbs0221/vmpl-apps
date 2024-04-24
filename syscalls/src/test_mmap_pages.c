#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_ADDR  (0x500000000000)
#define PAGE_SIZE (4096)

void test_mmap_pages(void *addr, int num_pages) {
    printf("addr = %p, num_pages = %d\n", addr, num_pages);

    int i;

    // Allocate virtual pages
    void* pages = mmap(addr, num_pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (pages == MAP_FAILED) {
        perror("Failed to allocate virtual pages");
        return;
    }

    printf("Virtual pages allocated successfully\n");

#if 1
    int sum = 0;
    // Traverse each virtual page
    for (i = 0; i < num_pages; i++) {
        int *addr = pages + (i * PAGE_SIZE);
        sum += *addr;
        *addr = i;
        printf("Traversing virtual page %d at address %p, value = %d, sum = %d\n", i, addr, *addr, sum);
        // Perform any desired operations on the virtual page
    }
#endif

    // Free virtual pages
    if (munmap(pages, num_pages * PAGE_SIZE) == -1) {
        perror("Failed to free virtual pages");
        return;
    }

    printf("Virtual pages freed successfully\n");

    printf("Traversal complete\n");
}

int main(int argc, char *argv[]) {
    const int min_pages = 1 << 14;
    const int max_pages = 1 << 17;

    int pages;
#if 0
    for (pages = min_pages; pages <= max_pages; pages += 512) {
        test_mmap_pages(NULL, pages);
    }
#endif

    test_mmap_pages(MAP_ADDR, max_pages);

    return 0;
}