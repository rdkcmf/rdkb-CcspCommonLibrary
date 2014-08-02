
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_objentity.la
#bin_PROGRAMS =

libccsp_objentity_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_objentity_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_objentity_la_SOURCES = \
    dslh_objeo_base.c \
    dslh_objeo_interface.c \
    dslh_objeo_management.c \
    dslh_objeo_states.c \

libccsp_objentity_la_LDFLAGS =

