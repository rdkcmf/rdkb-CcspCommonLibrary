AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_cpecontroller.la
#bin_PROGRAMS =

libccsp_cpecontroller_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_cpecontroller_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_cpecontroller_la_SOURCES = \
    dslh_cpeco_access.c \
    dslh_cpeco_base.c \
    dslh_cpeco_control.c \
    dslh_cpeco_interface.c \
    dslh_cpeco_operation.c \
    dslh_cpeco_staif.c \
    dslh_cpeco_states.c \

libccsp_cpecontroller_la_LDFLAGS =

