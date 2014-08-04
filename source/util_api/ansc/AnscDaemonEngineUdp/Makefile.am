AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscdaemonengineudp.la
#bin_PROGRAMS =

lib_utilapi_anscdaemonengineudp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscdaemonengineudp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscdaemonengineudp_la_SOURCES = \
    ansc_deuo_base.c \
    ansc_deuo_interface.c \
    ansc_deuo_management.c \
    ansc_deuo_operation.c \
    ansc_deuo_process.c \
    ansc_deuo_recv.c \
    ansc_deuo_send.c \
    ansc_deuo_states.c \

lib_utilapi_anscdaemonengineudp_la_LDFLAGS =
