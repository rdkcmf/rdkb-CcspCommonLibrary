AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysinforepository.la
#bin_PROGRAMS =

libcosa_sysinforepository_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysinforepository_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysinforepository_la_SOURCES = \
    sys_iro_base.c \
    sys_iro_interface.c \
    sys_iro_iraif.c \
    sys_iro_operation.c \
    sys_iro_states.c \

libcosa_sysinforepository_la_LDFLAGS =
