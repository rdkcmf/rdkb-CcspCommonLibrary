AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_syssetupmanager.la
#bin_PROGRAMS =

libcosa_syssetupmanager_ladir = $(top_builddir)/source/cosa/.libs

libcosa_syssetupmanager_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/include $(CPPFLAGS)

libcosa_syssetupmanager_la_SOURCES = \
    sys_smo_audit.c \
    sys_smo_base.c \
    sys_smo_control.c \
    sys_smo_interface.c \
    sys_smo_rcmif.c \
    sys_smo_session.c \
    sys_smo_states.c \
    sys_smo_utility.c \

libcosa_syssetupmanager_la_LDFLAGS =
