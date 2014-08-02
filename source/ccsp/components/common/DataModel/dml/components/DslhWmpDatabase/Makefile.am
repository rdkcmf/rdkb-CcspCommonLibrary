
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_wmpdatabase.la
#bin_PROGRAMS =

libccsp_wmpdatabase_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_wmpdatabase_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_wmpdatabase_la_SOURCES = \
    dslh_wmpdo_base.c \
    dslh_wmpdo_control.c \
    dslh_wmpdo_interface.c \
    dslh_wmpdo_mpaif.c \
    dslh_wmpdo_mprif.c \
    dslh_wmpdo_mprif_priv.c \
    dslh_wmpdo_operation.c \
    dslh_wmpdo_states.c \
    dslh_wmpdo_utilities.c \

libccsp_wmpdatabase_la_LDFLAGS =

