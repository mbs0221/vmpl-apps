#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <dune/dune.h>

int main()
{
	if (dune_init_and_enter() != 0) {
		printf("failed to init dune\n");
		return 1;
	}

	int p = fork();
	if (p < 0) {
		printf("fork error: %d\n", p);
		return 1;
	} else if (p == 0) {
		// asm volatile("int3");
		printf("in child\n");
		usleep(1000 * 1000);
		printf("done sleeping\n");
	} else {
		// asm volatile("int3");
		usleep(1000);
		printf("child=%d\n", p);
		if (waitpid(p, NULL, 0) == -1) {
			printf("waitpid error\n");
			return 1;
		}
		printf("done waiting\n");
	}

	return 0;
}
