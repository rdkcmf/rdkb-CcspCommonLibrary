AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysirov2.la
#bin_PROGRAMS =

libcosa_sysirov2_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysirov2_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysirov2_la_SOURCES = \
    sys_irov2_base.c \
    sys_irov2_interface.c \
    sys_irov2_iraif.c \
    sys_irov2_management.c \
    sys_irov2_operation.c \
    sys_irov2_states.c \

libcosa_sysirov2_la_LDFLAGS =
