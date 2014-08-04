AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_anscobjectmapper.la
#bin_PROGRAMS =

lib_utilapi_anscobjectmapper_ladir = $(top_builddir)/source/.libs

lib_utilapi_anscobjectmapper_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_anscobjectmapper_la_SOURCES = \
    ansc_object_descriptor_base.c \
    ansc_object_descriptor_interface.c \
    ansc_object_descriptor_management.c \
    ansc_object_descriptor_operation.c \
    ansc_object_mapper_base.c \
    ansc_object_mapper_interface.c \
    ansc_object_mapper_management.c \
    ansc_object_mapper_operation.c \

lib_utilapi_anscobjectmapper_la_LDFLAGS =
