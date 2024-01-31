AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_OBJDIR = .obj
AM_CPPFLAGS = -Iinclude
AM_CFLAGS = -Wall -g -MD -O3 -z noexecstack
AM_LDFLAGS = -L. -lvmpl -lhotcalls -ldict

bin_PROGRAMS = firewall_test
firewall_test_SOURCES = src/firewall_test.c src/firewall.c
firewall_test_LDADD = -lm
