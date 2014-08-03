AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_slapdslhparamtree.la
#bin_PROGRAMS =

libcosa_slapdslhparamtree_ladir = $(top_builddir)/source/cosa/.libs

libcosa_slapdslhparamtree_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/slap/include $(CPPFLAGS)

libcosa_slapdslhparamtree_la_SOURCES = \
    slap_dslh_paramto_access.c \
    slap_dslh_paramto_base.c \
    slap_dslh_paramto_interface.c \
    slap_dslh_paramto_states.c \

libcosa_slapdslhparamtree_la_LDFLAGS =


