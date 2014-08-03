AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrecyclebin.la
#bin_PROGRAMS =

libcosa_sysrecyclebin_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrecyclebin_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrecyclebin_la_SOURCES = \
    sys_rbo_access.c \
    sys_rbo_base.c \
    sys_rbo_interface.c \
    sys_rbo_management.c \
    sys_rbo_states.c \

libcosa_sysrecyclebin_la_LDFLAGS =
