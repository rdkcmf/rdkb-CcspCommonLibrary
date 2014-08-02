
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_varrecord.la
#bin_PROGRAMS =

libccsp_varrecord_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_varrecord_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_varrecord_la_SOURCES = \
    dslh_varro_access.c \
    dslh_varro_base.c \
    dslh_varro_config.c \
    dslh_varro_interface.c \
    dslh_varro_states.c \
    dslh_varro_utility.c \

libccsp_varrecord_la_LDFLAGS =

