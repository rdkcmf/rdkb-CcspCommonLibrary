AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmc2comterminal.la
#bin_PROGRAMS =

libcosa_bmc2comterminal_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmc2comterminal_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmc2/include $(CPPFLAGS)

libcosa_bmc2comterminal_la_SOURCES = \
    bmc2_comto_base.c \
    bmc2_comto_interface.c \
    bmc2_comto_management.c \
    bmc2_comto_operation.c \
    bmc2_comto_states.c \

libcosa_bmc2comterminal_la_LDFLAGS =
