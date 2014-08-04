AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscatomtable.la
#bin_PROGRAMS =

lib_utilapi_anscatomtable_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscatomtable_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscatomtable_la_SOURCES = \
    ansc_ato_base.c \
    ansc_ato_interface.c \
    ansc_ato_management.c \
    ansc_ato_operation.c \
    ansc_ato_states.c \

lib_utilapi_anscatomtable_la_LDFLAGS =
