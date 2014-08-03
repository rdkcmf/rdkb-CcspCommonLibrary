AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_clisclishell.la
#bin_PROGRAMS =

libcosa_clisclishell_ladir = $(top_builddir)/source/cosa/package/.libs

libcosa_clisclishell_la_CPPFLAGS = -I$(top_builddir)/source/cosa/package/cli/include $(CPPFLAGS)

libcosa_clisclishell_la_SOURCES = \
    scli_shell_base.c \
    scli_shell_bic.c \
    scli_shell_buffer.c \
    scli_shell_cenif.c \
    scli_shell_control.c \
    scli_shell_exec.c \
    scli_shell_help.c \
    scli_shell_iceif.c \
    scli_shell_interface.c \
    scli_shell_process.c \
    scli_shell_states.c \

libcosa_clisclishell_la_LDFLAGS =
