#include <stdio.h>
#include "common.h"

void run_test_cases(struct test_case *cases) {
    printf("Running test cases\n");
    for (int i = 0; cases[i].name != NULL; i++)
    {
        printf("Running test: %s\n", cases[i].name);
        cases[i].func();
    }
    printf("Tests passed\n");
}