AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmc2reqcontroller.la
#bin_PROGRAMS =

libcosa_bmc2reqcontroller_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmc2reqcontroller_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmc2/include $(CPPFLAGS)

libcosa_bmc2reqcontroller_la_SOURCES = \
    bmc2_reqco_base.c \
    bmc2_reqco_control.c \
    bmc2_reqco_interface.c \
    bmc2_reqco_management.c \
    bmc2_reqco_operation.c \
    bmc2_reqco_pecif.c \
    bmc2_reqco_states.c \

libcosa_bmc2reqcontroller_la_LDFLAGS =
