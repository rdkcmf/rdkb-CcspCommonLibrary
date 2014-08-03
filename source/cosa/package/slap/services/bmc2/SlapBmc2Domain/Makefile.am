AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_slapbmc2domain.la
#bin_PROGRAMS =

libcosa_slapbmc2domain_ladir = $(top_builddir)/source/cosa/.libs

libcosa_slapbmc2domain_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/slap/include $(CPPFLAGS)

libcosa_slapbmc2domain_la_SOURCES = \
    slap_bmc2domo_access.c \
    slap_bmc2domo_base.c \
    slap_bmc2domo_interface.c \
    slap_bmc2domo_states.c \

libcosa_slapbmc2domain_la_LDFLAGS =
