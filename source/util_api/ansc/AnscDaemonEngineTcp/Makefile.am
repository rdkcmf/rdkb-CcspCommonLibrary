AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscdaemonenginetcp.la
#bin_PROGRAMS =

lib_utilapi_ladir = $(top_builddir)/source/.libs

lib_utilapi_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_la_SOURCES = \
    ansc_deto_base.c \
    ansc_deto_interface.c \
    ansc_deto_management.c \
    ansc_deto_operation.c \
    ansc_deto_process.c \
    ansc_deto_recv.c \
    ansc_deto_send.c \
    ansc_deto_states.c \

lib_utilapi_la_LDFLAGS =
