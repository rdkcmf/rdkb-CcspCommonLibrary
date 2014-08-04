AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscdaemonserverudp.la
#bin_PROGRAMS =

lib_utilapi_anscdaemonserverudp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscdaemonserverudp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscdaemonserverudp_la_SOURCES = \
    ansc_dsuo_accept.c \
    ansc_dsuo_base.c \
    ansc_dsuo_interface.c \
    ansc_dsuo_management.c \
    ansc_dsuo_operation.c \
    ansc_dsuo_process.c \
    ansc_dsuo_states.c \
    ansc_dsuo_worker.c \

lib_utilapi_anscdaemonserverudp_la_LDFLAGS =
