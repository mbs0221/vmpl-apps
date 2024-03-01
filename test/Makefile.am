AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -g -MD -O2  -I$(srcdir)/include
AM_LDFLAGS = -lvmpl -lpthread
MODULE_LIB = /lib/modules/$(shell uname -r)/build

bin_PROGRAMS = hello \
            mprotect \
            fork \
            timetest \
            test \
            test_sandbox \
            test_syscall \
            test_hotcalls \
            test_mmap \
            test_heap \
            test_mem \
            test_brk \
            test_maps \
            test_pagemap \
            # hugepages

hello_SOURCES = src/hello.c
hello_LDADD = -lvmpl
hello_LDFLAGS = $(AM_LDFLAGS)

mprotect_SOURCES = src/mprotect.c
mprotect_LDADD = -lvmpl
mprotect_LDFLAGS = $(AM_LDFLAGS)

fork_SOURCES = src/fork.c
fork_LDADD = -lvmpl -lpthread
fork_LDFLAGS = $(AM_LDFLAGS)

timetest_SOURCES = src/timetest.c
timetest_LDADD = -lvmpl
timetest_LDFLAGS = $(AM_LDFLAGS)

test_SOURCES = src/test.c
test_LDADD = -lvmpl -lpthread
test_LDFLAGS = $(AM_LDFLAGS)

test_sandbox_SOURCES = src/test_sandbox.c
test_sandbox_LDFLAGS = -pthread
test_sandbox_LDADD = $(AM_LDFLAGS)

# hugepages_SOURCES = src/hugepages.c
# hugepages_LDADD = -lvmpl
# hugepages_LDFLAGS = $(AM_LDFLAGS)

test_syscall_SOURCES = src/test_syscall.c
test_syscall_LDFLAGS = -lvmpl
test_syscall_LDADD = $(AM_LDFLAGS)

test_hotcalls_SOURCES = src/test_hotcalls.c
test_hotcalls_LDADD = -lvmpl -lhotcalls
test_hotcalls_LDFLAGS = $(AM_LDFLAGS)

# Test programs without libvmpl
test_mmap_SOURCES = src/test_mmap.c
test_heap_SOURCES = src/test_heap.c
test_mem_SOURCES = src/test_mem.c
test_brk_SOURCES = src/test_brk.c
test_maps_SOURCES = src/test_maps.c
test_pagemap_SOURCES = src/test_pagemap.c

CLEANFILES = *.o test *.d $(bin_PROGRAMS) $(noinst_PROGRAMS)