AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -g -MD -O2  -I$(srcdir)/include
AM_LDFLAGS = -lvmpl -lpthread
MODULE_LIB = /lib/modules/$(shell uname -r)/build

bin_PROGRAMS = hello posted_ipi test timetest test_sandbox hugepages
noinst_PROGRAMS = hugepages_mod.ko

hello_SOURCES = src/hello.c
hello_LDADD = -lvmpl
hello_LDFLAGS = $(AM_LDFLAGS)

posted_ipi_SOURCES = src/posted_ipi.c
posted_ipi_LDADD = -lvmpl
posted_ipi_LDFLAGS = $(AM_LDFLAGS)

timetest_SOURCES = src/timetest.c
timetest_LDADD = -lvmpl
timetest_LDFLAGS = $(AM_LDFLAGS)

test_SOURCES = src/test.c
test_LDADD = -lvmpl
test_LDFLAGS = $(AM_LDFLAGS)

test_sandbox_SOURCES = src/test_sandbox.c
test_sandbox_LDFLAGS = -pthread

hugepages_SOURCES = src/hugepages.c
hugepages_LDADD = -lvmpl
hugepages_LDFLAGS = $(AM_LDFLAGS)

CLEANFILES = *.o test *.d $(bin_PROGRAMS) $(noinst_PROGRAMS)