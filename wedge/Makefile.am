AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_OBJDIR = .obj
AM_CPPFLAGS = -Iinclude
AM_CFLAGS = -Wall -g -MD -O3 -z noexecstack
AM_LDFLAGS = -L. -lvmpl -lhotcalls -ldict

bin_PROGRAMS = bench httpd test
bench_SOURCES = src/bench.c
httpd_SOURCES = src/httpd.c
test_SOURCES = src/test.c

# Add this line to compile sthread as a library
lib_LTLIBRARIES = libsthread.la
libsthread_la_SOURCES = src/sthread.c
libsthread_la_LDFLAGS = -version-info 1:0:0

# Specify headers to be installed
pkgincludedir = $(includedir)/sthread
pkginclude_HEADERS = include/sthread.h