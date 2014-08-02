
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_objcontroller.la
#bin_PROGRAMS =

libccsp_objcontroller_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_objcontroller_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_objcontroller_la_SOURCES = \
    dslh_objco_access.c \
    dslh_objco_base.c \
    dslh_objco_interface.c \
    dslh_objco_states.c \

libccsp_objcontroller_la_LDFLAGS =

