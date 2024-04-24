#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int fd;
    struct stat sb;
    char *buffer;

    // Open the file
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Get the file size
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        exit(1);
    }

    // Map the file into memory
    buffer = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Read the file using mmap
    printf("%s", buffer);

    // Unmap the file
    if (munmap(buffer, sb.st_size) == -1) {
        perror("munmap");
        exit(1);
    }

    // Close the file
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    return 0;
}