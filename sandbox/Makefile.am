AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_OBJDIR = .obj
AM_CPPFLAGS = -Iinclude
AM_CFLAGS = -Wall -g -MD -O3 -z noexecstack
AM_LDFLAGS = -L. -lvmpl -lhotcalls -ldict

lib_LTLIBRARIES = libsandbox.la
libsandbox_la_SOURCES = src/exec.c src/main.c src/nullsandbox.c src/sandbox.c src/trap.c src/umm.c
libsandbox_la_LDFLAGS = -version-info 1:0:0 -no-undefined

bin_PROGRAMS = sandbox_wrap
sandbox_wrap_SOURCES = src/sandbox_wrap.sh
sandbox_wrap_LDADD = libsandbox.la
