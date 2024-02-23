AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = bench_dune bench_dune_ring bench_linux posted_ipi

AM_CFLAGS = -Wall -g -MD -O3 -I$(srcdir)/include
LDADD = -lvmpl -lpthread

bench_dune_SOURCES = src/bench_dune.c
bench_dune_LDADD = -lvmpl
bench_dune_LDFLAGS = -static

bench_dune_ring_SOURCES = src/bench_dune_ring.c
bench_dune_ring_LDADD = -lvmpl
bench_dune_ring_LDFLAGS = -static

bench_linux_SOURCES = src/bench_linux.c

posted_ipi_SOURCES = src/posted_ipi.c
posted_ipi_LDADD = -lvmpl
posted_ipi_LDFLAGS = -static

CLEANFILES = src/*.o test src/*.d $(bin_PROGRAMS)