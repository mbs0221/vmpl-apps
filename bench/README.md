# bench

## bench_linux

```bash
benshan@sev-snp-guest:~$ bench_linux
TSC overhead is 28
Benchmarking Linux performance...
System call took 340 cycles
Kernel fault took 22015 cycles
User fault took 14255568 cycles
PROT1,TRAP,UNPROT took 909018 cycles
PROTN,TRAP,UNPROT took 881583 cycles
benshan@sev-snp-guest:~$
```

## bench_dune

```bash
benshan@sev-snp-guest:~$ LD_PRELOAD=libdunify.so bench_dune
TSC overhead is 112
Benchmarking dune performance...
System call took 27198 cycles
benshan@sev-snp-guest:~$ LD_PRELOAD=libdunify.so bench_dune
TSC overhead is 112
Benchmarking dune performance...
System call took 19648 cycles
benshan@sev-snp-guest:~$ LD_PRELOAD=libdunify.so bench_dune
TSC overhead is 112
Benchmarking dune performance...
System call took 22752 cycles
Kernel fault took 110694 cycles
User fault took 1844674407370843 cycles
PROT1,TRAP,UNPROT took 2664 cycles
PROTN,TRAP,UNPROT took 2106 cycles
Terminated
```

## bench_dune_ring

```bash
benshan@sev-snp-guest:~$ LD_PRELOAD=libdunify.so bench_dune_ring
testing page fault from G3... [passed]
measuring round-trip G3 syscall performance... [took 22313 cycles]
benshan@sev-snp-guest:~$ LD_PRELOAD=libdunify.so bench_dune_ring
testing page fault from G3... [passed]
measuring round-trip G3 syscall performance... [took 28740 cycles]
Terminated
```
