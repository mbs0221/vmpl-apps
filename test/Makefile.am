AUTOMAKE_OPTIONS = foreign subdir-objects
CC = gcc
AM_CFLAGS = -Wall -g -MD -O2
AM_LDFLAGS = -ldune -lpthread
MODULE_LIB = /lib/modules/$(shell uname -r)/build

bin_PROGRAMS = hello posted_ipi test timetest test_sandbox hugepages
noinst_PROGRAMS = hugepages_mod.ko

hello_SOURCES = hello.o
hello_LDADD = libvmpl.a
hello_LDFLAGS = $(AM_LDFLAGS)

posted_ipi_SOURCES = posted_ipi.o
posted_ipi_LDADD = libvmpl.a
posted_ipi_LDFLAGS = $(AM_LDFLAGS)

timetest_SOURCES = timetest.o
timetest_LDADD = libvmpl.a
timetest_LDFLAGS = $(AM_LDFLAGS)

test_SOURCES = test.o
test_LDADD = libvmpl.a
test_LDFLAGS = $(AM_LDFLAGS)

test_sandbox_SOURCES = test_sandbox.o
test_sandbox_LDFLAGS = -pthread

hugepages_SOURCES = hugepages.o
hugepages_LDADD = libvmpl.a
hugepages_LDFLAGS = $(AM_LDFLAGS)

CLEANFILES = *.o test *.d $(bin_PROGRAMS) $(noinst_PROGRAMS)