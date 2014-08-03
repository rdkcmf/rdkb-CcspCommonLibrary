AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwrmcookedpage.la
#bin_PROGRAMS =

libcosa_bmw2bwrmcookedpage_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwrmcookedpage_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwrmcookedpage_la_SOURCES = \
    bwrm_cpo_base.c \
    bwrm_cpo_interface.c \
    bwrm_cpo_operation.c \
    bwrm_cpo_states.c \

libcosa_bmw2bwrmcookedpage_la_LDFLAGS =
