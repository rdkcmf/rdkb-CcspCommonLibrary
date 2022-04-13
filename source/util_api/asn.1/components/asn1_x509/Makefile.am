##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
AM_CFLAGS =

ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = libCcspCommonLibrary_asn1_x509.la
libCcspCommonLibrary_asn1_x509_la_CPPFLAGS = -I$(top_srcdir)/source/cosa/include/linux -I$(top_srcdir)/source/debug_api/include -I$(top_srcdir)/source/ccsp/components/common/PoamIrepFolder -I$(top_srcdir)/source/cosa/include -I$(top_srcdir)/source/cosa/utilities/include -I$(top_srcdir)/source/cosa/custom -I$(top_srcdir)/source/cosa/package/bmw2/bwsp/include -I$(top_srcdir)/source/cosa/package/bmw2/include -I$(top_srcdir)/source/cosa/package/bmw2/bree/include -I$(top_srcdir)/source/cosa/package/bmw2/beep/include -I$(top_srcdir)/source/cosa/package/bmw2/bwrm/include -I$(top_srcdir)/source/cosa/package/system/include -I$(top_srcdir)/source/cosa/package/slap/include -I$(top_srcdir)/source/cosa/package/slap/services/dslh/include -I$(top_srcdir)/source/cosa/package/slap/services/bmc2/include -I$(top_srcdir)/source/cosa/package/cli/include -I$(top_srcdir)/source/cosa/package/bmc2/include -I$(top_srcdir)/source/debug_api/include -I$(top_srcdir)/source/ccsp/include -I$(top_srcdir)/source/ccsp/components/include -I$(top_srcdir)/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/source/ccsp/custom -I$(top_srcdir)/source/util_api/tls/include -I$(top_srcdir)/source/util_api/web/include -I$(top_srcdir)/source/util_api/ansc/include -I$(top_srcdir)/source/util_api/stun/include -I$(top_srcdir)/source/util_api/http/include -I$(top_srcdir)/source/util_api/http/utilities/include -I$(top_srcdir)/source/util_api/asn.1/include
libCcspCommonLibrary_asn1_x509_la_SOURCES = ansc_asn1_publickey.c ansc_asn1_certificate.c ansc_asn1_contentinfo.c ansc_asn1_keyagreement.c ansc_asn1_crl.c ansc_asn1_algorithmidentifier.c ansc_asn1_certificaterequest.c ansc_asn1_othername.c ansc_asn1_privatekeyinfo.c pki_export_utils.c pki_cert_attr.c ansc_asn1_extensions.c ansc_asn1_rsadigestinfo.c ansc_asn1_name.c
libCcspCommonLibrary_asn1_x509_la_LDFLAGS =  -lpthread -lrt -lz
