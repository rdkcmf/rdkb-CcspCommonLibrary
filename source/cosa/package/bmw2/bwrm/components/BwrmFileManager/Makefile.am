AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwrmfilemanager.la
#bin_PROGRAMS =

libcosa_bmw2bwrmfilemanager_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwrmfilemanager_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwrmfilemanager_la_SOURCES = \
    bwrm_fmo_base.c \
    bwrm_fmo_interface.c \
    bwrm_fmo_management.c \
    bwrm_fmo_operation.c \
    bwrm_fmo_states.c \

libcosa_bmw2bwrmfilemanager_la_LDFLAGS =
