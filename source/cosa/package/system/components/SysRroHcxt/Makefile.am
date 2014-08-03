AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrrohcxt.la
#bin_PROGRAMS =

libcosa_sysrrohcxt_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrrohcxt_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrrohcxt_la_SOURCES = \
    sys_rrohcxt_acces \
    sys_rrohcxt_base.c \
    sys_rrohcxt_interface.c \

libcosa_sysrrohcxt_la_LDFLAGS =
