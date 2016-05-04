ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AM_CFLAGS = -std=gnu11 -O3 -flto -fuse-linker-plugin

#SOURCE_FILES = src/benchmark.c src/cJSON.c

AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = benchmark
benchmark_SOURCES = src/benchmark.c src/cJSON.c
benchmark_LDADD = -lclo -ljansson -lm

MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in
maintainer-clean-local:; rm -rf autotools m4
