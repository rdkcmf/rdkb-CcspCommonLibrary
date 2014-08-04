AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_ansclpcconnector.la
#bin_PROGRAMS =

lib_utilapi_ansclpcconnector_ladir = $(top_builddir)/source/.libs

lib_utilapi_ansclpcconnector_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_ansclpcconnector_la_SOURCES = \
    ansc_lpcco_base.c \
    ansc_lpcco_cpcif.c \
    ansc_lpcco_imcif.c \
    ansc_lpcco_interface.c \
    ansc_lpcco_management.c \
    ansc_lpcco_operation.c \
    ansc_lpcco_states.c \

lib_utilapi_ansclpcconnector_la_LDFLAGS =
