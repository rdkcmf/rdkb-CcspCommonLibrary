AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysctlvprocessor.la
#bin_PROGRAMS =

libcosa_sysctlvprocessor_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysctlvprocessor_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysctlvprocessor_la_SOURCES = \
    sys_cpo_base.c \
    sys_cpo_interface.c \
    sys_cpo_operation.c \
    sys_cpo_provision.c \
    sys_cpo_serialize.c \
    sys_cpo_sfcif.c \
    sys_cpo_states.c \

libcosa_sysctlvprocessor_la_LDFLAGS =

