AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmc2comdomain.la
#bin_PROGRAMS =

libcosa_bmc2comdomain_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmc2comdomain_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmc2/include $(CPPFLAGS)

libcosa_bmc2comdomain_la_SOURCES = \
    bmc2_comdo_base.c \
    bmc2_comdo_interface.c \
    bmc2_comdo_management.c \
    bmc2_comdo_operation.c \
    bmc2_comdo_states.c \

libcosa_bmc2comdomain_la_LDFLAGS =
