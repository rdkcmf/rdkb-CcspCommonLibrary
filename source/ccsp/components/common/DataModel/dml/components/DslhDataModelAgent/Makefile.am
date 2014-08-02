
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_datamodelagent.la
#bin_PROGRAMS =

libccsp_datamodelagent_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_datamodelagent_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_datamodelagent_la_SOURCES = \
    ccsp_component_helper.c \
    dslh_dmagnt_base.c \
    dslh_dmagnt_control.c \
    dslh_dmagnt_exported.c \
    dslh_dmagnt_interface.c \
    dslh_dmagnt_mwsif.c \
    dslh_dmagnt_operation.c \
    dslh_dmagnt_utilities.c \

libccsp_datamodelagent_la_LDFLAGS =

