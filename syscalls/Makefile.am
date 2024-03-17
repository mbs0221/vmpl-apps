AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -g -MD -O2  -I$(srcdir)/include
AM_LDFLAGS = -ldune -lvmpl -lpthread
MODULE_LIB = /lib/modules/$(shell uname -r)/build

bin_PROGRAMS = test_syscall \
            test_hotcalls \
            test_mmap \
            test_heap \
            test_mem \
            test_brk \
            test_maps \
            test_pagemap \
            test_procmaps \
            test_mprotect \
            test_cpuid \
            test_msr \
            test_atexit \
            test_file \
            test_proc \
            test_hello \
            test_time \
            test_timer \
            test_pipe \
            test_signal \
            test_sem \
            test_shm \
            test_msg \
            test_socket \
            test_epoll \
            test_aio \
            test_clone \
            test_fork \
            test_futex


test_syscall_SOURCES = src/test_syscall.c
test_hotcalls_SOURCES = src/test_hotcalls.c
test_hotcalls_LDFLAGS = $(AM_LDFLAGS) -lhotcalls
test_mmap_SOURCES = src/test_mmap.c
test_heap_SOURCES = src/test_heap.c
test_mem_SOURCES = src/test_mem.c
test_brk_SOURCES = src/test_brk.c
test_maps_SOURCES = src/test_maps.c
test_pagemap_SOURCES = src/test_pagemap.c
test_procmaps_SOURCES = src/test_procmaps.c
test_mprotect_SOURCES = src/test_mprotect.c
test_cpuid_SOURCES = src/test_cpuid.c
test_msr_SOURCES = src/test_msr.c
test_atexit_SOURCES = src/test_atexit.c
test_file_SOURCES = src/test_file.c
test_proc_SOURCES = src/test_proc.c
test_hello_SOURCES = src/test_hello.c
test_time_SOURCES = src/test_time.c
test_timer_SOURCES = src/test_timer.c
test_pipe_SOURCES = src/test_pipe.c
test_signal_SOURCES = src/test_signal.c
test_sem_SOURCES = src/test_sem.c
test_shm_SOURCES = src/test_shm.c
test_msg_SOURCES = src/test_msg.c
test_socket_SOURCES = src/test_socket.c
test_epoll_SOURCES = src/test_epoll.c
test_aio_SOURCES = src/test_aio.c
test_clone_SOURCES = src/test_clone.c
test_fork_SOURCES = src/test_fork.c
test_futex_SOURCES = src/test_futex.c

CLEANFILES = *.o test *.d $(bin_PROGRAMS) $(noinst_PROGRAMS)