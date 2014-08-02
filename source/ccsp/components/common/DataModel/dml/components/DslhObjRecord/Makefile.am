
AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_objrecord.la
#bin_PROGRAMS =

libccsp_objrecord_ladir = $(top_builddir)/source/ccsp/components/common/DataModel/.libs

libccsp_objrecord_la_CPPFLAGS = -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)

libccsp_objrecord_la_SOURCES = \
    dslh_objro_access.c \
    dslh_objro_base.c \
    dslh_objro_control.c \
    dslh_objro_interface.c \
    dslh_objro_management.c \
    dslh_objro_rvqif.c \
    dslh_objro_states.c \

libccsp_objrecord_la_LDFLAGS =

