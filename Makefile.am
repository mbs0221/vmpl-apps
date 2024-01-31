AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_OBJDIR = .obj
SUBDIRS = sandbox ufirewall wedge

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = -Wall -g -MD -O3 -z noexecstack
AM_LDFLAGS = -L. -lvmpl -lhotcalls -ldict

bin_PROGRAMS = test
test_SOURCES = src/test.c
test_LDADD = sandbox/libsandbox.a
