AM_CFLAGS = 

AM_CPPFLAGS = -Wall -Werror

ACLOCAL_AMFLAGS = -I m4

hardware_platform = i686-linux-gnu

lib_LTLIBRARIES = lib_utilapi_ansccrypto.la
#bin_PROGRAMS =

lib_utilapi_ansccrypto_ladir = $(top_builddir)/source/.libs

lib_utilapi_ansccrypto_la_CPPFLAGS = -I$(top_builddir)/source/util_api/include $(CPPFLAGS)

lib_utilapi_ansccrypto_la_SOURCES = \
    ansc_crypto_base.c \
    ansc_crypto_compression.c \
    ansc_crypto_decompression.c \
    ansc_crypto_decryption.c \
    ansc_crypto_digestion.c \
    ansc_crypto_encryption.c \
    ansc_crypto_hash.c \
    ansc_crypto_interface.c \
    ansc_crypto_key.c \
    ansc_crypto_random.c \
    ansc_crypto_utility.c \

lib_utilapi_ansccrypto_la_LDFLAGS =
