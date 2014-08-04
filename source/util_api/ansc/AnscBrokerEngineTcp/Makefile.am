AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscbrokerenginetcp.la
#bin_PROGRAMS =

lib_utilapi_anscbrokerenginetcp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscbrokerenginetcp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscbrokerenginetcp_la_SOURCES = \
    ansc_beto_base.c \
    ansc_beto_interface.c \
    ansc_beto_management.c \
    ansc_beto_operation.c \
    ansc_beto_process.c \
    ansc_beto_recv.c \
    ansc_beto_send.c \
    ansc_beto_states.c \

lib_utilapi_anscbrokerenginetcp_la_LDFLAGS =
