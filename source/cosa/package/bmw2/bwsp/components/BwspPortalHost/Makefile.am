AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwspportalhost.la
#bin_PROGRAMS =

libcosa_bmw2bwspportalhost_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwspportalhost_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwspportalhost_la_SOURCES = \
    bwsp_pho_base.c \
    bwsp_pho_interface.c \
    bwsp_pho_states.c \

libcosa_bmw2bwspportalhost_la_LDFLAGS =
