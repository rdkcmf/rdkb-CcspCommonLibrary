AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_dmapi.la
#bin_PROGRAMS =

libccsp_dmapi_ladir = $(top_builddir)/source/.libs

libccsp_dmapi_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

libccsp_dmapi_la_SOURCES = \
    ccsp_dm_api.c

libccsp_dmapi_la_LDFLAGS =
