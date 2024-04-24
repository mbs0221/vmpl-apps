#include <stdio.h>
#include <unistd.h>
#include <vmpl/vmpl.h>

int test_brk() {

    void* initial_brk = sbrk(0);
    printf("Initial break value: %p\n", initial_brk);

    void* new_brk = brk(initial_brk + 4096);
    if (new_brk == (void*)-1) {
        perror("brk");
        return 1;
    }
    printf("New break value: %p\n", new_brk);

    void* current_brk = sbrk(0);
    printf("Current break value: %p\n", current_brk);

    return 0;
}

int test_sbrk() {
    void* initial_brk = sbrk(0);
    printf("Initial break value: %p\n", initial_brk);

    void* new_brk = sbrk(4096);
    if (new_brk == (void*)-1) {
        perror("sbrk");
        return 1;
    }
    printf("New break value: %p\n", new_brk);

    void* current_brk = sbrk(0);
    printf("Current break value: %p\n", current_brk);

    return 0;
}

int main(int argc, char** argv) {
    VMPL_ENTER;
    printf("Testing brk\n");
    test_brk();
    printf("Testing sbrk\n");
    test_sbrk();
    return 0;
}