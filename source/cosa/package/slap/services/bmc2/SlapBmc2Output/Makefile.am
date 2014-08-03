AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_slapbmc2output.la
#bin_PROGRAMS =

libcosa_slapbmc2output_ladir = $(top_builddir)/source/cosa/.libs

libcosa_slapbmc2output_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/slap/include $(CPPFLAGS)

libcosa_slapbmc2output_la_SOURCES = \
    slap_bmc2outo_access.c \
    slap_bmc2outo_base.c \
    slap_bmc2outo_interface.c \
    slap_bmc2outo_states.c \

libcosa_slapbmc2output_la_LDFLAGS =
