#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <vmpl/dune.h>

/* Handlers. */
static void pgflt_handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
	ptent_t *pte;
#if CONFIG_VMPL_DEBUG
	dune_printf("syscall: caught page fault, addr=%ld\n", addr);
#endif
	dune_vm_lookup(pgroot, (void *) addr, 0, &pte);
	*pte |= PTE_P | PTE_W | PTE_U | PTE_A | PTE_D;
}

static void syscall_handler(struct dune_tf *tf)
{
	dune_printf("syscall: caught syscall, num=%ld\n", tf->rax);
	dune_passthrough_syscall(tf);
}

/* Utility functions. */
static int dune_call_user(void *func)
{
	int ret;
	unsigned long sp;
	struct dune_tf *tf = malloc(sizeof(struct dune_tf));
	if (!tf)
		return -ENOMEM;

	asm ("movq %%rsp, %0" : "=r" (sp));
	tf->rip = (unsigned long) func;
	tf->rsp = sp - 10000;
	tf->rflags = 0x0;

	ret = dune_jump_to_user(tf);

	return ret;
}

static void user_mmap_test()
{
	int rc;
	void *addr, *tmp_addr;
	pte_t *ptep;
	pte_t *new_root;

	// Test mmap
	dune_printf("Test mmap");
	addr = mmap(NULL, PGSIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
	assert(addr != MAP_FAILED);
	dune_printf("Test mmap passed");

	// Test access to the mapped page
	dune_printf("Test access to the mapped page");
	*(uint64_t *)(addr) = 0xdeadbeef;
	assert(*(uint64_t *)addr == 0xdeadbeef);
	dune_printf("Test access to the mapped page passed");

	// Test mmap at a specific address that is already mapped with MAP_FIXED_NOREPLACE.
	tmp_addr = addr;
	dune_printf("Test mmap at a specific address that is already mapped with MAP_FIXED_NOREPLACE");
	addr = mmap(tmp_addr, PGSIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
	assert(addr == MAP_FAILED);
	assert(errno == EEXIST);
	dune_printf("Test mmap at a specific address that is already mapped with MAP_FIXED_NOREPLACE passed");

	// Test mmap at a specific address that is already mapped with MAP_FIXED.
	tmp_addr = addr;
	dune_printf("Test mmap at a specific address that is already mapped with MAP_FIXED");
	addr = mmap(tmp_addr, PGSIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	assert(addr != MAP_FAILED);
	assert(addr == tmp_addr);
	dune_printf("Test mmap at a specific address that is already mapped with MAP_FIXED passed");

	// Test mprotect
	dune_printf("Test mprotect");
	rc = mprotect(addr, PGSIZE, PROT_READ);
	assert(rc == 0);
	dune_printf("Test mprotect passed");

	// Test pkey_alloc
	dune_printf("Test pkey_alloc");
	int pkey = pkey_alloc(0, PKEY_DISABLE_WRITE);
	assert(pkey >= 0);
	dune_printf("Test pkey_alloc passed");

	// Test pkey_free
	dune_printf("Test pkey_free");
	rc = pkey_free(pkey);
	assert(rc == 0);
	dune_printf("Test pkey_free passed");

	// Test pkey_mprotect
	dune_printf("Test pkey_mprotect");
	pkey = pkey_alloc(0, PKEY_DISABLE_WRITE);
	rc = pkey_mprotect(addr, PGSIZE, PROT_READ, pkey);
	assert(rc == 0);
	dune_printf("Test pkey_mprotect passed");

	// Test mremap
	dune_printf("Test mremap");
	addr = mremap(addr, PGSIZE, PGSIZE * 2, MREMAP_MAYMOVE, NULL);
	assert(addr != MAP_FAILED);
	dune_printf("Test mremap passed");

	// Test munmap
	dune_printf("Test munmap");
	rc = munmap(addr, PGSIZE * 2);
	assert(rc == 0);
	dune_printf("Test munmap passed");
}

/* User code with system calls. */
 void user_main()
{
	int ret = syscall(SYS_gettid);
	assert(ret > 0);
	dune_printf("syscall: gettid=%d\n", ret);
	dune_ret_from_user(ret);
}

int main(int argc, char *argv[])
{
	int ret;

	printf("syscall: not running dune yet\n");

	ret = dune_init_and_enter();
	if (ret) {
		printf("failed to initialize dune\n");
		return ret;
	}

	dune_printf("syscall: now printing from dune mode\n");

	dune_register_pgflt_handler(pgflt_handler);
	dune_register_syscall_handler(syscall_handler);

	dune_printf("syscall: about to call user code\n");
	return dune_call_user(&user_main);
}

