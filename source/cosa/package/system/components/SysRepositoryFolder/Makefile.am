AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrepositoryfolder.la
#bin_PROGRAMS =

libcosa_sysrepositoryfolder_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrepositoryfolder_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrepositoryfolder_la_SOURCES = \
    sys_rfo_access.c \
    sys_rfo_base.c \
    sys_rfo_interface.c \
    sys_rfo_management.c \
    sys_rfo_operation.c \
    sys_rfo_rcaif.c \
    sys_rfo_rcgif.c \
    sys_rfo_rcmif.c \
    sys_rfo_sorting.c \
    sys_rfo_states.c \

libcosa_sysrepositoryfolder_la_LDFLAGS =
