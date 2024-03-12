AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -g -MD -O2  -I$(srcdir)/include
AM_LDFLAGS = -ldune -lvmpl -lpthread
MODULE_LIB = /lib/modules/$(shell uname -r)/build

bin_PROGRAMS = hello \
            mprotect \
            fork \
            timetest \
            test \
            test_sandbox \
            # hugepages

hello_SOURCES = src/hello.c
mprotect_SOURCES = src/mprotect.c
fork_SOURCES = src/fork.c
timetest_SOURCES = src/timetest.c
test_SOURCES = src/test.c
test_sandbox_SOURCES = src/test_sandbox.c
# hugepages_SOURCES = src/hugepages.c

CLEANFILES = *.o test *.d $(bin_PROGRAMS) $(noinst_PROGRAMS)