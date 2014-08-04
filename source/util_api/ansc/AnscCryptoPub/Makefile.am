AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_ansccryptopub.la
#bin_PROGRAMS =

lib_utilapi_ansccryptopub_ladir = $(top_builddir)/source/.libs

lib_utilapi_ansccryptopub_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_ansccryptopub_la_SOURCES = \
    ansc_crypto_pub_base.c \
    ansc_crypto_pub_interface.c \
    ansc_crypto_pub_process.c \

lib_utilapi_ansccryptopub_la_LDFLAGS =
