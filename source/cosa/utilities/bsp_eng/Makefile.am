AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = libcosa_bspeng.la
#bin_PROGRAMS =

libcosa_bspeng_ladir = $(top_builddir)/source/cosa/.libs

libcosa_bspeng_la_CPPFLAGS = -I$(top_builddir)/source/cosa/utilities/bsp_eng/include $(CPPFLAGS)

libcosa_bspeng_la_SOURCES = \
    bspeng_access_co_base.c \
    bspeng_access_co_process.c \
    bspeng_archive_co_base.c \
    bspeng_archive_co_process.c \
    bspeng_branch_co_base.c \
    bspeng_branch_co_process.c \
    bspeng_co_base.c \
    bspeng_co_execute.c \
    bspeng_co_interface.c \
    bspeng_co_process.c \
    bspeng_list_co_base.c \
    bspeng_list_co_process.c \
    bspeng_outputset_co_base.c \
    bspeng_outputset_co_process.c \
    bspeng_reader_co_base.c \
    bspeng_reader_co_process.c \
    bspeng_runtime_co_base.c \
    bspeng_runtime_co_process.c \
    bspeng_stack_co_base.c \
    bspeng_stack_co_process.c \
    bspeng_temp_co_base.c \
    bspeng_temp_co_process.c \
    bspeng_var_co_base.c \
    bspeng_var_co_process.c \
    bspeng_var_slap_conv.c \
    bspeng_writer_co_base.c \
    bspeng_writer_co_process.c \

libcosa_bspeng_la_LDFLAGS =
