AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2breespo.la
#bin_PROGRAMS =

libcosa_bmw2breespo_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2breespo_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2breespo_la_SOURCES = \
    bree_spo_base.c \
    bree_spo_cooked_page_api.c \
    bree_spo_helper_base.c \
    bree_spo_helper.c \
    bree_spo_helper_ext.c \
    bree_spo_interface.c \
    bree_spo_operation.c \
    bree_spo_process.c \
    bree_spo_soaif.c \
    bree_spo_srm.c \

libcosa_bmw2breespo_la_LDFLAGS =
