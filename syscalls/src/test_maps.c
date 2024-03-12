#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char path[100];
    char line[256];

    // Open the /proc/self/maps file
    snprintf(path, sizeof(path), "/proc/self/maps");
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Failed to open /proc/self/maps\n");
        return 1;
    }

    // Read and print each line of the file
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    // Close the file
    fclose(fp);

    return 0;
}
