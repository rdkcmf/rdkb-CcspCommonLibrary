AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_slapbmc2command.la
#bin_PROGRAMS =

libcosa_slapbmc2command_ladir = $(top_builddir)/source/cosa/.libs

libcosa_slapbmc2command_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/slap/include $(CPPFLAGS)

libcosa_slapbmc2command_la_SOURCES = \
    slap_bmc2como_access.c \
    slap_bmc2como_base.c \
    slap_bmc2como_collection.c \
    slap_bmc2como_interface.c \
    slap_bmc2como_states.c \

libcosa_slapbmc2command_la_LDFLAGS =
