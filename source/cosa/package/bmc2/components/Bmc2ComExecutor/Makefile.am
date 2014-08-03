AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmc2comexecutor.la
#bin_PROGRAMS =

libcosa_bmc2comexecutor_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmc2comexecutor_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmc2/include $(CPPFLAGS)

libcosa_bmc2comexecutor_la_SOURCES = \
    bmc2_comeo_base.c \
    bmc2_comeo_helper_base.c \
    bmc2_comeo_helper.c \
    bmc2_comeo_helper_ext.c \
    bmc2_comeo_interface.c \
    bmc2_comeo_operation.c \
    bmc2_comeo_process.c \
    bmc2_comeo_soaif.c \
    bmc2_comeo_state.c \

libcosa_bmc2comexecutor_la_LDFLAGS =
