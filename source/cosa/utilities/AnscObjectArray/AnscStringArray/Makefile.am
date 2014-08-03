AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_anscstringarray.la
#bin_PROGRAMS =

libcosa_anscstringarray_ladir = $(top_builddir)/source/cosa/.libs

libcosa_anscstringarray_la_CPPFLAGS = -I$(top_builddir)/source/cosa/utilities/include $(CPPFLAGS)

libcosa_anscstringarray_la_SOURCES = \
    ansc_string_array_co_base.c \
    ansc_string_array_co_interface.c \
    ansc_string_array_co_process.c \

libcosa_anscstringarray_la_LDFLAGS =
