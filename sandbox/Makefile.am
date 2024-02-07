AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_OBJDIR = .obj
AM_CPPFLAGS = -Iinclude
AM_CFLAGS = -Wall -g -MD -O3 -z noexecstack
AM_LDFLAGS = -L. -lvmpl -lhotcalls -ldict -lsandbox

lib_LTLIBRARIES = libsandbox.la
libsandbox_la_SOURCES = src/main.c src/umm.c src/trap.c src/exec.c src/sandbox.c
libsandbox_la_LDFLAGS = -version-info 1:0:0 -no-undefined

bin_PROGRAMS = sandbox sandbox_wrap
sandbox_SOURCES = src/nullsandbox.c
sandbox_LDADD = libsandbox.la
sandbox_wrap_SOURCES = src/sandbox_wrap.sh
sandbox_wrap_LDADD = libsandbox.la

# Specify headers to be installed
pkgincludedir = $(includedir)/sandbox
pkginclude_HEADERS = include/sandbox.h