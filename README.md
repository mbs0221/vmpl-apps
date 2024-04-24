# vmpl-apps

VMPL-Process Applications

```bash
sudo apt install devscripts debhelper
```

```c
// vmpl-apps/syscalls/src/test_syscall.c:146
static void pgflt_handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
    ptent_t *pte;
#if CONFIG_VMPL_DEBUG
    dune_printf("syscall: caught page fault, addr=%ld\n", addr);
#endif
    dune_vm_lookup(pgroot, (void *) addr, 0, &pte);
    *pte |= PTE_P | PTE_W | PTE_U | PTE_A | PTE_D;
}
```

```c
// vmpl-apps/bench/src/bench_dune_ring.c:52
static void pgflt_handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
    ptent_t *pte;

    dune_vm_lookup(pgroot, (void *)addr, 0, &pte);
    *pte |= PTE_P | PTE_W | PTE_U | PTE_A | PTE_D;
}
```

```c
// vmpl-apps/bench/src/bench_dune.c:108
static void benchmark1_handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
    ptent_t *pte;
    int accessed;

    my_time += rdtscllp() - trap_tsc;

    dune_vm_lookup(pgroot, (void *)addr, 0, &pte);
    *pte |= PTE_P | PTE_W | PTE_U | PTE_A | PTE_D;

    dune_vm_lookup(pgroot, (void *)addr + NRPGS * PGSIZE, 0, &pte);
    accessed = *pte & PTE_A;
    *pte = PTE_ADDR(*pte);
    if (accessed)
        dune_flush_tlb_one(addr + NRPGS * PGSIZE);
}
```

```c
// vmpl-apps/bench/src/bench_dune.c:131
static void benchmark2_handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
    ptent_t *pte;

    dune_vm_lookup(pgroot, (void *)addr, 0, &pte);
    *pte |= PTE_P | PTE_W | PTE_U | PTE_A | PTE_D;
}
```

```c
// vmpl-apps/sandbox/src/trap.c:685
static void handler(uintptr_t addr, uint64_t fec, struct dune_tf *tf)
{
#if CONFIG_VMPL_DEBUG
    // TODO: causes a crash -- does printf allocate too much stack space?
    printf("in fault handler: %lx\n", addr);
#endif

    fltaddr = addr;
    ptent_t *pte;
    dune_vm_lookup(pgroot, (void *)addr, 0, &pte);
    *pte |= PTE_W;
}
```
