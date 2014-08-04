AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscbrokerservertcp.la
#bin_PROGRAMS =

lib_utilapi_anscbrokerservertcp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscbrokerservertcp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscbrokerservertcp_la_SOURCES = \
    ansc_bsto_accept.c \
    ansc_bsto_base.c \
    ansc_bsto_interface.c \
    ansc_bsto_management.c \
    ansc_bsto_operation.c \
    ansc_bsto_process.c \
    ansc_bsto_states.c \
    ansc_bsto_worker.c \

lib_utilapi_anscbrokerservertcp_la_LDFLAGS =
