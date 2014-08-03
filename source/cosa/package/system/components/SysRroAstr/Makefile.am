AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrroastr.la
#bin_PROGRAMS =

libcosa_sysrroastr_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrroastr_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrroastr_la_SOURCES = \
    sys_rroastr_access.c \
    sys_rroastr_base.c \
    sys_rroastr_interface.c \

libcosa_sysrroastr_la_LDFLAGS =
