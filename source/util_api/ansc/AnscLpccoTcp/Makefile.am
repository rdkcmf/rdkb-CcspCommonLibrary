AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_ansclpccotcp.la
#bin_PROGRAMS =

lib_utilapi_ansclpccotcp_ladir = $(top_builddir)/source/.libs

lib_utilapi_ansclpccotcp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_ansclpccotcp_la_SOURCES = \
    ansc_lpccotcp_base.c \
    ansc_lpccotcp_bstowo.c \
    ansc_lpccotcp_dstowo.c \
    ansc_lpccotcp_imcif.c \
    ansc_lpccotcp_interface.c \
    ansc_lpccotcp_operation.c \
    ansc_lpccotcp_recv.c \
    ansc_lpccotcp_send.c \
    ansc_lpccotcp_states.c \

lib_utilapi_ansclpccotcp_la_LDFLAGS =
