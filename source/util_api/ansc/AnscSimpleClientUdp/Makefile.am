AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscsimpleclientudp.la
#bin_PROGRAMS =

lib_utilapi_anscsimpleclientudp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscsimpleclientudp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscsimpleclientudp_la_SOURCES = \
    ansc_scuo_base.c \
    ansc_scuo_interface.c \
    ansc_scuo_operation.c \
    ansc_scuo_process.c \
    ansc_scuo_recv.c \
    ansc_scuo_states.c \

lib_utilapi_anscsimpleclientudp_la_LDFLAGS =
