AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwspwebservlet.la
#bin_PROGRAMS =

libcosa_bmw2bwspwebservlet_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwspwebservlet_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwspwebservlet_la_SOURCES = \
    bwsp_wso_base.c \
    bwsp_wso_do_stuff.c \
    bwsp_wso_do_stuff_upnp.c \
    bwsp_wso_interface.c \
    bwsp_wso_states.c \

libcosa_bmw2bwspwebservlet_la_LDFLAGS =
