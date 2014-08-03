AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bmw2bwrmpagemanager.la
#bin_PROGRAMS =

libcosa_bmw2bwrmpagemanager_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bmw2bwrmpagemanager_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/bmw2/include $(CPPFLAGS)

libcosa_bmw2bwrmpagemanager_la_SOURCES = \
    bwrm_pmo_base.c \
    bwrm_pmo_interface.c \
    bwrm_pmo_management.c \
    bwrm_pmo_operation.c \
    bwrm_pmo_states.c \

libcosa_bmw2bwrmpagemanager_la_LDFLAGS =
