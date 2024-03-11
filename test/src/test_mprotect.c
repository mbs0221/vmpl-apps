// Path: vmpl-apps/test/src/test_mprotect.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <vmpl/vmpl.h>

int test_mprotect()
{
    int ret = 0;
    int page_size = getpagesize();
    void *addr = malloc(page_size * 2);
    if (addr == NULL) {
        perror("malloc");
        return -1;
    }

    // Align the address to the page boundary
    addr = (void *)(((unsigned long)addr + page_size - 1) & ~(page_size - 1));

    // Set the first page to be read-only
    ret = mprotect(addr, page_size, PROT_READ|PROT_WRITE);
    if (ret == -1) {
        perror("mprotect");
        return -1;
    }

    // Write to the first page
    strcpy(addr, "Hello, world!");

    // Set the first page to be read-write
    ret = mprotect(addr, page_size, PROT_READ | PROT_WRITE);
    if (ret == -1) {
        perror("mprotect");
        return -1;
    }

    // Write to the first page
    strcpy(addr, "Hello, world!");

    free(addr);
    return 0;
}

int test_pkey_mprotect()
{
    int ret = 0;
    int page_size = getpagesize();
    void *addr = malloc(page_size * 2);
    if (addr == NULL) {
        perror("malloc");
        return -1;
    }

    int pkey = pkey_alloc(PKEY_DISABLE_WRITE, 0);
    if (pkey == -1) {
        perror("pkey_alloc");
        return -1;
    }

    // Align the address to the page boundary
    addr = (void *)(((unsigned long)addr + page_size - 1) & ~(page_size - 1));

    // Set the first page to be read-only
    ret = pkey_mprotect(addr, page_size, PROT_READ|PROT_WRITE, pkey);
    if (ret == -1) {
        perror("pkey_mprotect");
        return -1;
    }

    // Write to the first page
    strcpy(addr, "Hello, world!");

    // Set the first page to be read-write
    ret = pkey_mprotect(addr, page_size, PROT_READ | PROT_WRITE, 0);
    if (ret == -1) {
        perror("pkey_mprotect");
        return -1;
    }

    // Write to the first page
    strcpy(addr, "Hello, world!");

    free(addr);
    return 0;
}


int main(int argc, char *argv[])
{
    VMPL_ENTER;
    int ret;

    ret = test_mprotect();
    assert(ret == 0);

    printf("mprotect test passed\n");

    ret = test_pkey_mprotect();
    assert(ret == 0);

    printf("pkey_mprotect test passed\n");

    return 0;
}