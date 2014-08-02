
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_varentity.la
#bin_PROGRAMS =

libccsp_varentity_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_varentity_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_varentity_la_SOURCES = \
    dslh_vareo_base.c \
    dslh_vareo_interface.c \
    dslh_vareo_states.c \
    dslh_vareo_tokens.c \

libccsp_varentity_la_LDFLAGS =

