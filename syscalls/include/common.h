#ifndef __COMMON_H__
#define __COMMON_H__

struct test_case {
    const char *name;
    void (*func)();
};

void run_test_cases(struct test_case *cases);

#endif