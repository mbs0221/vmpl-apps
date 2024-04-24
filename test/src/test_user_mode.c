#include <stdio.h>
#include <vmpl/dune.h>

/* Utility functions. */
static int dune_call_user(void *func)
{
	int ret;
	unsigned long sp;
	struct dune_tf *tf = malloc(sizeof(struct dune_tf));
	if (!tf)
		return -ENOMEM;

	asm ("movq %%rsp, %0" : "=r" (sp));
	tf->rip = (unsigned long) &__entry;
	tf->rsp = sp;
	tf->rflags = 0x0;

	ret = dune_jump_to_user(tf);

__entry:
	return ret;
}
