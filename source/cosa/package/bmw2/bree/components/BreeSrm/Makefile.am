AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2breesrm.la
#bin_PROGRAMS =

libcosa_bmw2breesrm_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2breesrm_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2breesrm_la_SOURCES = \
    bree_srm_base.c \
    bree_srm_crc_data.c \
    bree_srm_crc_marshal.c \
    bree_srm_interface.c \
    bree_srm_process.c \

libcosa_bmw2breesrm_la_LDFLAGS =
