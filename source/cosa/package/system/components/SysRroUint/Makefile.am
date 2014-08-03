AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrrouint.la
#bin_PROGRAMS =

libcosa_sysrrouint_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrrouint_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrrouint_la_SOURCES = \
    sys_rrouint_access.c \
    sys_rrouint_base.c \
    sys_rrouint_interface.c \

libcosa_sysrrouint_la_LDFLAGS =
