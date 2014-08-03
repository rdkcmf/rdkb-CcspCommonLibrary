AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_sysrepositoryrecord.la
#bin_PROGRAMS =

libcosa_sysrepositoryrecord_ladir = $(top_builddir)/source/cosa/.libs

libcosa_sysrepositoryrecord_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_sysrepositoryrecord_la_SOURCES = \
    sys_rro_access.c \
    sys_rro_base.c \
    sys_rro_interface.c \
    sys_rro_states.c \

libcosa_sysrepositoryrecord_la_LDFLAGS =
