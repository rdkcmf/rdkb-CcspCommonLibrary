AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmc2envcontroller.la
#bin_PROGRAMS =

libcosa_bmc2envcontroller_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmc2envcontroller_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmc2/include $(CPPFLAGS)

libcosa_bmc2envcontroller_la_SOURCES = \
    bmc2_envco_base.c \
    bmc2_envco_interface.c \
    bmc2_envco_management.c \
    bmc2_envco_operation.c \
    bmc2_envco_sccif.c \
    bmc2_envco_states.c \

libcosa_bmc2envcontroller_la_LDFLAGS =
