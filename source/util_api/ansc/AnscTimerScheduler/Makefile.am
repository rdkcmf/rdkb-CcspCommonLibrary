AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_ansctimerscheduler.la
#bin_PROGRAMS =

lib_utilapi_ansctimerscheduler_ladir = $(top_builddir)/source/.libs

lib_utilapi_ansctimerscheduler_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_ansctimerscheduler_la_SOURCES = \
    ansc_tdo_base.c \
    ansc_tdo_interface.c \
    ansc_tdo_operation.c \
    ansc_tdo_states.c \
    ansc_tso_base.c \
    ansc_tso_interface.c \
    ansc_tso_management.c \
    ansc_tso_operation.c \
    ansc_tso_states.c \

lib_utilapi_ansctimerscheduler_la_LDFLAGS =
