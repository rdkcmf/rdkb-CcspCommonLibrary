AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrroenum.la
#bin_PROGRAMS =

libcosa_sysrroenum_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrroenum_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrroenum_la_SOURCES = \
    sys_rroenum_access.c \
    sys_rroenum_base.c \
    sys_rroenum_interface.c \

libcosa_sysrroenum_la_LDFLAGS =
