AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libccsp_baseapi.la
#bin_PROGRAMS =

libccsp_baseapi_ladir = $(top_builddir)/source/.libs

libccsp_baseapi_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

libccsp_baseapi_la_SOURCES = \
    ccsp_base_api.c \
    ccsp_base_api_shm.c \
    ccsp_base_api_srv.c \
    ccsp_message_bus.c \

libccsp_baseapi_la_LDFLAGS =
