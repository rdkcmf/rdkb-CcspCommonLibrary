AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_CFLAGS += -DCCSP_INC_no_asm_sigcontext_h
AM_LDFLAGS = -L$(top_srcdir)/../CcspCommonLibrary/lib
AM_LDFLAGS += -lccsp_common
AM_LDFLAGS += -lpthread
AM_LDFLAGS += -lrt
AM_LDFLAGS +=
AM_LDFLAGS +=
AM_LDFLAGS += -lz

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu
lib_LTLIBRARIES = libwebconfig_framework.la 
libwebconfig_framework_la_LIBADD = ${top_builddir}/source/libccsp_common.la
libwebconfig_framework_la_CPPFLAGS = -I$(top_srcdir)/source/ccsp/custom -I$(top_srcdir)/source/ccsp/include -I$(top_srcdir)/source/debug_api/include -I$(top_srcdir)/source/cosa/include -I$(top_srcdir)/source/cosa/include/linux -I$(top_srcdir)/source/ccsp/components/include -I$(top_srcdir)/source/cosa/package/slap/include $(CPPFLAGS)
libwebconfig_framework_la_SOURCES = webconfig_framework.c
libwebconfig_framework_la_LDFLAGS = -lpthread -lrt -lz

libwebconfig_framework_la_DEPENDENCIES = ${top_builddir}/source/libccsp_common.la