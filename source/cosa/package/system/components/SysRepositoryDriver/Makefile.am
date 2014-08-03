AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrepositorydriver.la
#bin_PROGRAMS =

libcosa_sysrepositorydriver_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrepositorydriver_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrepositorydriver_la_SOURCES = \
    sys_rdo_access.c \
    sys_rdo_base.c \
    sys_rdo_interface.c \
    sys_rdo_operation.c \
    sys_rdo_states.c \

libcosa_sysrepositorydriver_la_LDFLAGS =
