AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_anscuintarray.la
#bin_PROGRAMS =

libcosa_anscuintarray_ladir = $(top_builddir)/source/cosa/.libs

libcosa_anscuintarray_la_CPPFLAGS = -I$(top_builddir)/source/cosa/utilities/include $(CPPFLAGS)

libcosa_anscuintarray_la_SOURCES = \
    ansc_uint_array_co_base.c \
    ansc_uint_array_co_interface.c \
    ansc_uint_array_co_process.c \

libcosa_anscuintarray_la_LDFLAGS =
