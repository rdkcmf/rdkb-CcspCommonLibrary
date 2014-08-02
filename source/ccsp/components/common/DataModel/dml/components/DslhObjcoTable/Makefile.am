
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_objcotable.la
#bin_PROGRAMS =

libccsp_objcotable_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_objcotable_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_objcotable_la_SOURCES = \
    dslh_objco_table_access.c \
    dslh_objco_table_base.c \
    dslh_objco_table_interface.c \
    dslh_objco_table_states.c \

libccsp_objcotable_la_LDFLAGS =

