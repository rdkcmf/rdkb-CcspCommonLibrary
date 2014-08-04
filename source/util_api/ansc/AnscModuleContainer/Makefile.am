AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscmodulecontainer.la
#bin_PROGRAMS =

lib_utilapi_anscmodulecontainer_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscmodulecontainer_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscmodulecontainer_la_SOURCES = \
    ansc_mco_base.c \
    ansc_mco_control.c \
    ansc_mco_interface.c \
    ansc_mco_operation.c \
    ansc_mco_query.c \
    ansc_mco_states.c \

lib_utilapi_anscmodulecontainer_la_LDFLAGS =
