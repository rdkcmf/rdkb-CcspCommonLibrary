AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_slapbmc2input.la
#bin_PROGRAMS =

libcosa_slapbmc2input_ladir = $(top_builddir)/source/cosa/.libs

libcosa_slapbmc2input_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/slap/include $(CPPFLAGS)

libcosa_slapbmc2input_la_SOURCES = \
    slap_bmc2ino_access.c \
    slap_bmc2ino_base.c \
    slap_bmc2ino_interface.c \
    slap_bmc2ino_states.c \

libcosa_slapbmc2input_la_LDFLAGS =
