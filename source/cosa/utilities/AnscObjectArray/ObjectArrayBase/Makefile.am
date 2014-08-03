AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_anscobjarray.la
#bin_PROGRAMS =

libcosa_anscobjarray_ladir = $(top_builddir)/source/cosa/.libs

libcosa_anscobjarray_la_CPPFLAGS = -I$(top_builddir)/source/cosa/utilities/include $(CPPFLAGS)

libcosa_anscobjarray_la_SOURCES = \
    ansc_obj_array_co_base.c \
    ansc_obj_array_co_interface.c \
    ansc_obj_array_co_process.c \

libcosa_anscobjarray_la_LDFLAGS =
