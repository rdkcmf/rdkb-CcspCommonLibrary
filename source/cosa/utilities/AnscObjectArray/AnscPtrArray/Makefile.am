AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_anscptrarray.la
#bin_PROGRAMS =

libcosa_anscptrarray_ladir = $(top_builddir)/source/cosa/.libs

libcosa_anscptrarray_la_CPPFLAGS = -I$(top_builddir)/source/cosa/utilities/include $(CPPFLAGS)

libcosa_anscptrarray_la_SOURCES = \
    ansc_ptr_array_co_base.c \
    ansc_ptr_array_co_interface.c \
    ansc_ptr_array_co_process.c \

libcosa_anscptrarray_la_LDFLAGS =
