AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwspsitecreator.la
#bin_PROGRAMS =

libcosa_bmw2bwspsitecreator_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwspsitecreator_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwspsitecreator_la_SOURCES = \
    bwsp_sco_base.c \
    bwsp_sco_cspif.c \
    bwsp_sco_interface.c \
    bwsp_sco_operation.c \
    bwsp_sco_states.c \
    bwsp_sco_wsrif.c \

libcosa_bmw2bwspsitecreator_la_LDFLAGS =
