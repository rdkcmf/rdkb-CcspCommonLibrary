AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwrmenvcontroller.la
#bin_PROGRAMS =

libcosa_bmw2bwrmenvcontroller_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwrmenvcontroller_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwrmenvcontroller_la_SOURCES = \
    bwrm_eco_base.c \
    bwrm_eco_interface.c \
    bwrm_eco_operation.c \
    bwrm_eco_ramif.c \
    bwrm_eco_states.c \

libcosa_bmw2bwrmenvcontroller_la_LDFLAGS =
