AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrrosint.la
#bin_PROGRAMS =

libcosa_sysrrosint_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrrosint_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrrosint_la_SOURCES = \
    sys_rrosint_access.c \
    sys_rrosint_base.c \
    sys_rrosint_interface.c \

libcosa_sysrrosint_la_LDFLAGS =
