AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrrobool.la
#bin_PROGRAMS =

libcosa_sysrrobool_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrrobool_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrrobool_la_SOURCES = \
    sys_rrobool_access.c \
    sys_rrobool_base.c \
    sys_rrobool_interface.c \

libcosa_sysrrobool_la_LDFLAGS =
