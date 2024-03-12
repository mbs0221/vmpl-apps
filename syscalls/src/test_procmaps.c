#include <stdio.h>
#include <stdlib.h>

void parse_proc_maps() {
    FILE *file;
    char line[256];

    // 打开文件
    file = fopen("/proc/self/maps", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // 逐行读取文件内容并解析
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);  // 在这里可以根据需要对每行进行处理
    }

    // 关闭文件
    fclose(file);
}

int main(int argc, char *argv[]) {
    parse_proc_maps();

    return 0;
}
