#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char path[1035];

    // Open the command for reading
    fp = popen("ls -l", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    // Read the output line by line
    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("%s", path);
    }

    // Close the pipe
    pclose(fp);

    return 0;
}