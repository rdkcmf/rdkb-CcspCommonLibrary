AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscsimpleclienttcp.la
#bin_PROGRAMS =

lib_utilapi_anscsimpleclienttcp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscsimpleclienttcp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscsimpleclienttcp_la_SOURCES = \
    ansc_scto_base.c \
    ansc_scto_interface.c \
    ansc_scto_operation.c \
    ansc_scto_process.c \
    ansc_scto_recv.c \
    ansc_scto_states.c \
    ansc_scto_tsaif.c \

lib_utilapi_anscsimpleclienttcp_la_LDFLAGS =
