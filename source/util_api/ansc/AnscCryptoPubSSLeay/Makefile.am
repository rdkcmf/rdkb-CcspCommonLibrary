AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_CFLAGS += -D_NO_PKI_KB5_SUPPORT
AM_CFLAGS += -D_ANSC_USE_OPENSSL_
AM_CFLAGS += -D_ANSC_AES_USED_
AM_CFLAGS += -D_CCSP_CWMP_TCP_CONNREQ_HANDLER
AM_CFLAGS += -D_DSLH_STUN_
AM_CFLAGS += -D_BBHM_SSE_FILE_IO
AM_CFLAGS += -DENABLE_SA_KEY
AM_CFLAGS += -D_ANSC_IPV6_COMPATIBLE_
AM_CFLAGS += -DBUILD_WEB
AM_CFLAGS += -D_COSA_SIM_
AM_LDFLAGS = -L/var/tmp/pc-rdkb/lib
AM_LDFLAGS += -lpthread
AM_LDFLAGS += -lrt
AM_LDFLAGS += -lssl
AM_LDFLAGS += -lcrypto
AM_LDFLAGS += -lz

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu

noinst_LTLIBRARIES = libCcspCommonLibrary_AnscCryptoPubSSLeay.la
libCcspCommonLibrary_AnscCryptoPubSSLeay_la_CPPFLAGS = -I$(top_builddir)/source/cosa/include/linux -I/var/tmp/pc-rdkb/include/dbus-1.0 -I$(top_builddir)/source/debug_api/include -I$(top_builddir)/source/ccsp/components/common/PoamIrepFolder -I$(top_builddir)/source/cosa/package/bmw2/beep/include -I$(top_builddir)/source/cosa/package/bmw2/bwsp/include -I$(top_builddir)/source/cosa/package/bmw2/include -I$(top_builddir)/source/cosa/package/bmw2/bree/include -I$(top_builddir)/source/cosa/package/bmw2/bwrm/include -I$(top_builddir)/source/cosa/package/slap/services/dslh/include -I$(top_builddir)/source/cosa/package/slap/services/bmc2/include -I$(top_builddir)/source/cosa/package/slap/include -I$(top_builddir)/source/cosa/package/cli/include -I$(top_builddir)/source/cosa/package/system/include -I$(top_builddir)/source/cosa/package/bmc2/include -I$(top_builddir)/source/cosa/utilities/include -I$(top_builddir)/source/cosa/custom -I$(top_builddir)/source/cosa/include -I$(top_builddir)/source/util_api/tls/include -I$(top_builddir)/source/util_api/stun/include -I$(top_builddir)/source/util_api/ansc/include -I$(top_builddir)/source/util_api/web/include -I$(top_builddir)/source/util_api/asn.1/include -I$(top_builddir)/source/util_api/http/utilities/include -I$(top_builddir)/source/util_api/http/include -I$(top_builddir)/source/debug_api/include -I$(top_builddir)/source/ccsp/custom -I$(top_builddir)/source/ccsp/include -I$(top_builddir)/source/ccsp/components/common/MessageBusHelper/include -I$(top_builddir)/source/ccsp/components/include $(CPPFLAGS)
libCcspCommonLibrary_AnscCryptoPubSSLeay_la_SOURCES = ansc_crypto_pub_ssleay_interface.c ansc_crypto_pub_ssleay_encrypt.c ansc_crypto_pub_ssleay_key.c ansc_crypto_pub_ssleay_util.c ansc_crypto_pub_ssleay_base.c ansc_crypto_pub_ssleay_sign_priv.c ansc_crypto_pub_ssleay_verify.c ansc_crypto_pub_ssleay_dh.c ansc_crypto_pub_ssleay_sign.c ansc_crypto_pub_ssleay_key_gen.c ansc_crypto_pub_ssleay_verify_priv.c ansc_crypto_pub_ssleay_process.c ansc_crypto_pub_ssleay_util_priv.c
libCcspCommonLibrary_AnscCryptoPubSSLeay_la_LDFLAGS =  -L/var/tmp/pc-rdkb/lib -lpthread -lrt -lssl -lcrypto -lz