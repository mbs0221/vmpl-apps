#include <stdio.h>
#include <stdlib.h>

int main() {
    int num = 42;
    float f = 3.14;
    char str[] = "Hello, world!";
    double d = 2.71828;

    num = abs(f/10.2 - 1.0) > 0.1 ? num : num + 1;
    printf("Integer: %d\n", num);
    printf("Float: %.2f\n", f);
    printf("String: %s\n", str);
    printf("Double: %.5lf\n", d);

    return 0;
}
