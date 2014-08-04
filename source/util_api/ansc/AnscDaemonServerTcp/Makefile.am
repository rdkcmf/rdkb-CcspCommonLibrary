AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscdaemonservertcp.la
#bin_PROGRAMS =

lib_utilapi_anscdaemonservertcp_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscdaemonservertcp_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscdaemonservertcp_la_SOURCES = \
    ansc_dsto_accept.c \
    ansc_dsto_base.c \
    ansc_dsto_interface.c \
    ansc_dsto_management.c \
    ansc_dsto_operation.c \
    ansc_dsto_patient.c \
    ansc_dsto_process.c \
    ansc_dsto_states.c \
    ansc_dsto_worker.c \

lib_utilapi_anscdaemonservertcp_la_LDFLAGS =
