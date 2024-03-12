#include <stdio.h>
#include <unistd.h>

void show_heap() {
    // Parse /proc/self/maps to show the heap
    char cmd[100];
    sprintf(cmd, "cat /proc/%d/maps | grep -E 'heap|stack'", getpid());
    system(cmd);
}

int test_brk() {
    show_heap();

    void* initial_brk = sbrk(0);
    printf("Initial break value: %p\n", initial_brk);
    show_heap();

    void* new_brk = brk(initial_brk + 4096);
    if (new_brk == (void*)-1) {
        perror("brk");
        return 1;
    }
    printf("New break value: %p\n", new_brk);
    show_heap();

    void* current_brk = sbrk(0);
    printf("Current break value: %p\n", current_brk);
    show_heap();

    return 0;
}

int test_sbrk() {
    show_heap();

    void* initial_brk = sbrk(0);
    printf("Initial break value: %p\n", initial_brk);
    show_heap();

    void* new_brk = sbrk(4096);
    if (new_brk == (void*)-1) {
        perror("sbrk");
        return 1;
    }
    printf("New break value: %p\n", new_brk);
    show_heap();

    void* current_brk = sbrk(0);
    printf("Current break value: %p\n", current_brk);
    show_heap();

    return 0;
}

int main(int argc, char** argv) {
    printf("Testing brk\n");
    test_brk();
    printf("Testing sbrk\n");
    test_sbrk();
    return 0;
}