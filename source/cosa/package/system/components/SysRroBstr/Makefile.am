AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrrobstr.la
#bin_PROGRAMS =

libcosa_sysrrobstr_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrrobstr_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrrobstr_la_SOURCES = \
    sys_rrobstr_access.c \
    sys_rrobstr_base.c \
    sys_rrobstr_interface.c \

libcosa_sysrrobstr_la_LDFLAGS =
