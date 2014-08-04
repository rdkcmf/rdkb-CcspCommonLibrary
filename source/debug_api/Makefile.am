AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_debugapi.la
#bin_PROGRAMS =

lib_debugapi_ladir = $(top_builddir)/source/.libs

lib_debugapi_la_CPPFLAGS = -I$(top_builddir)/source/debug_api/include $(CPPFLAGS)

lib_debugapi_la_SOURCES = \
    ansc_debug.c \
    ansc_memory.c \
    ansc_protection.c \
    user_string.c \

lib_debugapi_la_LDFLAGS =
