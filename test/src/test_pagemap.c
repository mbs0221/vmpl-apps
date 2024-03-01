#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 4096

int main() {
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /proc/self/pagemap");
        return 1;
    }

    uint64_t virtual_address = 0x12345678; // Replace with your desired virtual address

    off_t offset = virtual_address / PAGE_SIZE * sizeof(uint64_t);
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Failed to seek /proc/self/pagemap");
        close(fd);
        return 1;
    }

    uint64_t page_frame_number;
    if (read(fd, &page_frame_number, sizeof(uint64_t)) != sizeof(uint64_t)) {
        perror("Failed to read /proc/self/pagemap");
        close(fd);
        return 1;
    }

    close(fd);

    if (page_frame_number & (1ULL << 63)) {
        printf("Page is present\n");
    } else {
        printf("Page is not present\n");
    }

    return 0;
}
