/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module:	ansc_http_parser.c

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the HTTP message parser component object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.
        03/22/09    a lighter version

    ---------------------------------------------------------------

    last modified:

        03/22/09

**********************************************************************/


#include "ansc_platform.h"
#include "http_definitions.h"

static SLIST_HEADER                    HeaderXList[HTTP_SMPO_HEADER_TABLE_SIZE];
static BOOL                            headerInitialized               = FALSE;


static const ANSC_UTILBOX_IANA_MEDIA_SUBTYPE        g_IanaTextSubtypes[]        =
{
    {
        IANA_MT_TX_STYPE_CODE_PLAIN,
        IANA_MT_TX_STYPE_TEXT_PLAIN
    },

    {
        IANA_MT_TX_STYPE_CODE_RICHTEXT,
        IANA_MT_TX_STYPE_TEXT_RICHTEXT
    },

    {
        IANA_MT_TX_STYPE_CODE_ENRICHED,
        IANA_MT_TX_STYPE_TEXT_ENRICHED
    },

    {
        IANA_MT_TX_STYPE_CODE_TAB_SEP_VALUES,
        IANA_MT_TX_STYPE_TEXT_TAB_SEP_VALUES
    },

    {
        IANA_MT_TX_STYPE_CODE_HTML,
        IANA_MT_TX_STYPE_TEXT_HTML
    },

    {
        IANA_MT_TX_STYPE_CODE_SGML,
        IANA_MT_TX_STYPE_TEXT_SGML
    },

    {
        IANA_MT_TX_STYPE_CODE_URL_LIST,
        IANA_MT_TX_STYPE_TEXT_URL_LIST
    },

    {
        IANA_MT_TX_STYPE_CODE_RFC822_HEADERS,
        IANA_MT_TX_STYPE_TEXT_RFC822_HEADERS
    },

    {
        IANA_MT_TX_STYPE_CODE_CSS,
        IANA_MT_TX_STYPE_TEXT_CSS
    },

    {
        IANA_MT_TX_STYPE_CODE_XML,
        IANA_MT_TX_STYPE_TEXT_XML
    },

    {
        IANA_MT_TX_STYPE_CODE_XML_EPE,
        IANA_MT_TX_STYPE_TEXT_XML_EPE
    },

    {
        IANA_MT_TX_STYPE_CODE_DIRECTORY,
        IANA_MT_TX_STYPE_TEXT_DIRECTORY
    },

    {
        IANA_MT_TX_STYPE_CODE_CALENDAR,
        IANA_MT_TX_STYPE_TEXT_CALENDAR
    },

    {
        IANA_MT_TX_STYPE_CODE_T140,
        IANA_MT_TX_STYPE_TEXT_T140
    },

    {
        IANA_MT_TX_STYPE_CODE_PARITYFEC,
        IANA_MT_TX_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_TX_STYPE_CODE_JAVASCRIPT,
        IANA_MT_TX_STYPE_TEXT_JAVASCRIPT
    },

    {
        IANA_MT_TX_STYPE_CODE_X_MSMS_GSINVITE,
        IANA_MT_TX_STYPE_TEXT_X_MSMS_GSINVITE
    },

    {
        IANA_MT_TX_STYPE_CODE_RESERVED,
        NULL
    }
};

static const ANSC_UTILBOX_IANA_MEDIA                g_IanaMultipartSubtypes[]   =
{
    {
        IANA_MT_MP_STYPE_CODE_MIXED,
        IANA_MT_MP_STYPE_TEXT_MIXED
    },

    {
        IANA_MT_MP_STYPE_CODE_ALTERNATIVE,
        IANA_MT_MP_STYPE_TEXT_ALTERNATIVE
    },

    {
        IANA_MT_MP_STYPE_CODE_DIGEST,
        IANA_MT_MP_STYPE_TEXT_DIGEST
    },

    {
        IANA_MT_MP_STYPE_CODE_PARALLEL,
        IANA_MT_MP_STYPE_TEXT_PARALLEL
    },

    {
        IANA_MT_MP_STYPE_CODE_APPLEDOUBLE,
        IANA_MT_MP_STYPE_TEXT_APPLEDOUBLE
    },

    {
        IANA_MT_MP_STYPE_CODE_HEADER_SET,
        IANA_MT_MP_STYPE_TEXT_HEADER_SET
    },

    {
        IANA_MT_MP_STYPE_CODE_FORM_DATA,
        IANA_MT_MP_STYPE_TEXT_FORM_DATA
    },

    {
        IANA_MT_MP_STYPE_CODE_RELATED,
        IANA_MT_MP_STYPE_TEXT_RELATED
    },

    {
        IANA_MT_MP_STYPE_CODE_REPORTED,
        IANA_MT_MP_STYPE_TEXT_REPORTED
    },

    {
        IANA_MT_MP_STYPE_CODE_VOICE_MESSAGE,
        IANA_MT_MP_STYPE_TEXT_VOICE_MESSAGE
    },

    {
        IANA_MT_MP_STYPE_CODE_SIGNED,
        IANA_MT_MP_STYPE_TEXT_SIGNED
    },

    {
        IANA_MT_MP_STYPE_CODE_ENCRYPTED,
        IANA_MT_MP_STYPE_TEXT_ENCRYPTED
    },

    {
        IANA_MT_MP_STYPE_CODE_BYTERANGES,
        IANA_MT_MP_STYPE_TEXT_BYTERANGES
    },

    {
        IANA_MT_MP_STYPE_CODE_RESERVED,
        NULL
    }
};

static const ANSC_UTILBOX_IANA_MEDIA                g_IanaMessageSubtypes[]     =
{
    {
        IANA_MT_MG_STYPE_CODE_RFC822,
        IANA_MT_MG_STYPE_TEXT_RFC822
    },

    {
        IANA_MT_MG_STYPE_CODE_PARTIAL,
        IANA_MT_MG_STYPE_TEXT_PARTIAL
    },

    {
        IANA_MT_MG_STYPE_CODE_EXTERNAL_BODY,
        IANA_MT_MG_STYPE_TEXT_EXTERNAL_BODY
    },

    {
        IANA_MT_MG_STYPE_CODE_NEWS,
        IANA_MT_MG_STYPE_TEXT_NEWS
    },

    {
        IANA_MT_MG_STYPE_CODE_HTTP,
        IANA_MT_MG_STYPE_TEXT_HTTP
    },

    {
        IANA_MT_MG_STYPE_CODE_DELIVERY_STATUS,
        IANA_MT_MG_STYPE_TEXT_DELIVERY_STATUS
    },

    {
        IANA_MT_MG_STYPE_CODE_DISPOSITION_NF,
        IANA_MT_MG_STYPE_TEXT_DISPOSITION_NF
    },

    {
        IANA_MT_MG_STYPE_CODE_S_HTTP,
        IANA_MT_MG_STYPE_TEXT_S_HTTP
    },

    {
        IANA_MT_MG_STYPE_CODE_SIPFRAG,
        IANA_MT_MG_STYPE_TEXT_SIPFRAG
    },

    {
        IANA_MT_MG_STYPE_CODE_RESERVED,
        NULL
    }
};

static const ANSC_UTILBOX_IANA_MEDIA                g_IanaApplicationSubtypes[] =
{
    {
        IANA_MT_AP_STYPE_CODE_OCTET_STREAM,
        IANA_MT_AP_STYPE_TEXT_OCTET_STREAM
    },

    {
        IANA_MT_AP_STYPE_CODE_POSTSCRIPT,
        IANA_MT_AP_STYPE_TEXT_POSTSCRIPT
    },

    {
        IANA_MT_AP_STYPE_CODE_ODA,
        IANA_MT_AP_STYPE_TEXT_ODA
    },

    {
        IANA_MT_AP_STYPE_CODE_ATOMICMAIL,
        IANA_MT_AP_STYPE_TEXT_ATOMICMAIL
    },

    {
        IANA_MT_AP_STYPE_CODE_ANDREW_INSET,
        IANA_MT_AP_STYPE_TEXT_ANDREW_INSET
    },

    {
        IANA_MT_AP_STYPE_CODE_SLATE,
        IANA_MT_AP_STYPE_TEXT_SLATE
    },

    {
        IANA_MT_AP_STYPE_CODE_WITA,
        IANA_MT_AP_STYPE_TEXT_WITA
    },

    {
        IANA_MT_AP_STYPE_CODE_DEC_DX,
        IANA_MT_AP_STYPE_TEXT_DEC_DX
    },

    {
        IANA_MT_AP_STYPE_CODE_DCA_RFT,
        IANA_MT_AP_STYPE_TEXT_DCA_RFT
    },

    {
        IANA_MT_AP_STYPE_CODE_ACTIVEMESSAGE,
        IANA_MT_AP_STYPE_TEXT_ACTIVEMESSAGE
    },

    {
        IANA_MT_AP_STYPE_CODE_RTF,
        IANA_MT_AP_STYPE_TEXT_RTF
    },

    {
        IANA_MT_AP_STYPE_CODE_APPLEFILE,
        IANA_MT_AP_STYPE_TEXT_APPLEFILE
    },

    {
        IANA_MT_AP_STYPE_CODE_MAC_BINHEX40,
        IANA_MT_AP_STYPE_TEXT_MAC_BINHEX40
    },

    {
        IANA_MT_AP_STYPE_CODE_NEWS_MESSAGE_ID,
        IANA_MT_AP_STYPE_TEXT_NEWS_MESSAGE_ID
    },

    {
        IANA_MT_AP_STYPE_CODE_NEWS_TRANSMISSION,
        IANA_MT_AP_STYPE_TEXT_NEWS_TRANSMISSION
    },

    {
        IANA_MT_AP_STYPE_CODE_WORDPERFECT51,
        IANA_MT_AP_STYPE_TEXT_WORDPERFECT51
    },

    {
        IANA_MT_AP_STYPE_CODE_PDF,
        IANA_MT_AP_STYPE_TEXT_PDF
    },

    {
        IANA_MT_AP_STYPE_CODE_ZIP,
        IANA_MT_AP_STYPE_TEXT_ZIP
    },

    {
        IANA_MT_AP_STYPE_CODE_MACWRITEII,
        IANA_MT_AP_STYPE_TEXT_MACWRITEII
    },

    {
        IANA_MT_AP_STYPE_CODE_MSWORD,
        IANA_MT_AP_STYPE_TEXT_MSWORD
    },

    {
        IANA_MT_AP_STYPE_CODE_REMOTE_PRINTING,
        IANA_MT_AP_STYPE_TEXT_REMOTE_PRINTING
    },

    {
        IANA_MT_AP_STYPE_CODE_ESHOP,
        IANA_MT_AP_STYPE_TEXT_ESHOP
    },

    {
        IANA_MT_AP_STYPE_CODE_X400_BP,
        IANA_MT_AP_STYPE_TEXT_X400_BP
    },

    {
        IANA_MT_AP_STYPE_CODE_SGML,
        IANA_MT_AP_STYPE_TEXT_SGML
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_ENCRYPTED,
        IANA_MT_AP_STYPE_TEXT_PGP_ENCRYPTED
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_SIGNATURE,
        IANA_MT_AP_STYPE_TEXT_PGP_SIGNATURE
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_KEYS,
        IANA_MT_AP_STYPE_TEXT_PGP_KEYS
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS7_MIME,
        IANA_MT_AP_STYPE_TEXT_PKCS7_MIME
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS7_SIGNATURE,
        IANA_MT_AP_STYPE_TEXT_PKCS7_SIGNATURE
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS10,
        IANA_MT_AP_STYPE_TEXT_PKCS10
    },

    {
        IANA_MT_AP_STYPE_CODE_EDIFACT,
        IANA_MT_AP_STYPE_TEXT_EDIFACT
    },

    {
        IANA_MT_AP_STYPE_CODE_EDI_X12,
        IANA_MT_AP_STYPE_TEXT_EDI_X12
    },

    {
        IANA_MT_AP_STYPE_CODE_EDI_CONSENT,
        IANA_MT_AP_STYPE_TEXT_EDI_CONSENT
    },

    {
        IANA_MT_AP_STYPE_CODE_XML,
        IANA_MT_AP_STYPE_TEXT_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_XML_EPE,
        IANA_MT_AP_STYPE_TEXT_XML_EPE
    },

    {
        IANA_MT_AP_STYPE_CODE_XML_DTD,
        IANA_MT_AP_STYPE_TEXT_XML_DTD
    },

    {
        IANA_MT_AP_STYPE_CODE_BATCH_SMTP,
        IANA_MT_AP_STYPE_TEXT_BATCH_SMTP
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIXCMP,
        IANA_MT_AP_STYPE_TEXT_PKIXCMP
    },

    {
        IANA_MT_AP_STYPE_CODE_IPP,
        IANA_MT_AP_STYPE_TEXT_IPP
    },

    {
        IANA_MT_AP_STYPE_CODE_OCSP_REQUEST,
        IANA_MT_AP_STYPE_TEXT_OCSP_REQUEST
    },

    {
        IANA_MT_AP_STYPE_CODE_OCSP_RESPONSE,
        IANA_MT_AP_STYPE_TEXT_OCSP_RESPONSE
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIX_CERT,
        IANA_MT_AP_STYPE_TEXT_PKIX_CERT
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIX_CRL,
        IANA_MT_AP_STYPE_TEXT_PKIX_CRL
    },

    {
        IANA_MT_AP_STYPE_CODE_HTTP,
        IANA_MT_AP_STYPE_TEXT_HTTP
    },

    {
        IANA_MT_AP_STYPE_CODE_SDP,
        IANA_MT_AP_STYPE_TEXT_SDP
    },

    {
        IANA_MT_AP_STYPE_CODE_IOTP,
        IANA_MT_AP_STYPE_TEXT_IOTP
    },

    {
        IANA_MT_AP_STYPE_CODE_PARITYFEC,
        IANA_MT_AP_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_AP_STYPE_CODE_ISUP,
        IANA_MT_AP_STYPE_TEXT_ISUP
    },

    {
        IANA_MT_AP_STYPE_CODE_QSIG,
        IANA_MT_AP_STYPE_TEXT_QSIG
    },

    {
        IANA_MT_AP_STYPE_CODE_TIMESTAMP_QUERY,
        IANA_MT_AP_STYPE_TEXT_TIMESTAMP_QUERY
    },

    {
        IANA_MT_AP_STYPE_CODE_TIMESTAMP_REPLY,
        IANA_MT_AP_STYPE_TEXT_TIMESTAMP_REPLY
    },

    {
        IANA_MT_AP_STYPE_CODE_REGINFO_XML,
        IANA_MT_AP_STYPE_TEXT_REGINFO_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_SIMPLE_MSG_SUMMARY,
        IANA_MT_AP_STYPE_TEXT_SIMPLE_MSG_SUMMARY
    },

    {
        IANA_MT_AP_STYPE_CODE_PIDF_XML,
        IANA_MT_AP_STYPE_TEXT_PIDF_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_WATCHERINFO_XML,
        IANA_MT_AP_STYPE_TEXT_WATCHERINFO_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED,
        IANA_MT_AP_STYPE_TEXT_X_FORM_URLENCODED
    },

    {
        IANA_MT_AP_STYPE_CODE_X_GZIP,
        IANA_MT_AP_STYPE_TEXT_X_GZIP
    },

    {
        IANA_MT_AP_STYPE_CODE_X_TAR,
        IANA_MT_AP_STYPE_TEXT_X_TAR
    },

    {
        IANA_MT_AP_STYPE_CODE_X_COMPRESSED,
        IANA_MT_AP_STYPE_TEXT_X_COMPRESSED
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_ARCHIVE,
        IANA_MT_AP_STYPE_TEXT_X_JAVA_ARCHIVE
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_SO,
        IANA_MT_AP_STYPE_TEXT_X_JAVA_SO
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_VM,
        IANA_MT_AP_STYPE_TEXT_X_JAVA_VM
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVASCRIPT,
        IANA_MT_AP_STYPE_TEXT_X_JAVASCRIPT
    },

    {
        IANA_MT_AP_STYPE_CODE_RESERVED,
        NULL
    }
};

static const ANSC_UTILBOX_IANA_MEDIA                g_IanaImageSubtypes[]       =
{
    {
        IANA_MT_IM_STYPE_CODE_JPEG,
        IANA_MT_IM_STYPE_TEXT_JPEG
    },

    {
        IANA_MT_IM_STYPE_CODE_GIF,
        IANA_MT_IM_STYPE_TEXT_GIF
    },

    {
        IANA_MT_IM_STYPE_CODE_IEF,
        IANA_MT_IM_STYPE_TEXT_IEF
    },

    {
        IANA_MT_IM_STYPE_CODE_G3FAX,
        IANA_MT_IM_STYPE_TEXT_G3FAX
    },

    {
        IANA_MT_IM_STYPE_CODE_TIFF,
        IANA_MT_IM_STYPE_TEXT_TIFF
    },

    {
        IANA_MT_IM_STYPE_CODE_CGM,
        IANA_MT_IM_STYPE_TEXT_CGM
    },

    {
        IANA_MT_IM_STYPE_CODE_NAPLPS,
        IANA_MT_IM_STYPE_TEXT_NAPLPS
    },

    {
        IANA_MT_IM_STYPE_CODE_PNG,
        IANA_MT_IM_STYPE_TEXT_PNG
    },

    {
        IANA_MT_IM_STYPE_CODE_SVG_XML,
        IANA_MT_IM_STYPE_TEXT_SVG_XML
    },

    {
        IANA_MT_IM_STYPE_CODE_RESERVED,
        NULL
    }
};

static const ANSC_UTILBOX_IANA_MEDIA                g_IanaAudioSubtypes[]       =
{
    {
        IANA_MT_AD_STYPE_CODE_BASIC,
        IANA_MT_AD_STYPE_TEXT_BASIC
    },

    {
        IANA_MT_AD_STYPE_CODE_32KADPCM,
        IANA_MT_AD_STYPE_TEXT_32KADPCM
    },

    {
        IANA_MT_AD_STYPE_CODE_L16,
        IANA_MT_AD_STYPE_TEXT_L16
    },

    {
        IANA_MT_AD_STYPE_CODE_TELEPHONE_EVENT,
        IANA_MT_AD_STYPE_TEXT_TELEPHONE_EVENT
    },

    {
        IANA_MT_AD_STYPE_CODE_TONE,
        IANA_MT_AD_STYPE_TEXT_TONE
    },

    {
        IANA_MT_AD_STYPE_CODE_MPEG,
        IANA_MT_AD_STYPE_TEXT_MPEG
    },

    {
        IANA_MT_AD_STYPE_CODE_PARITYFEC,
        IANA_MT_AD_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_AD_STYPE_CODE_MP4A_LATM,
        IANA_MT_AD_STYPE_TEXT_MP4A_LATM
    },

    {
        IANA_MT_AD_STYPE_CODE_G722_1,
        IANA_MT_AD_STYPE_TEXT_G722_1
    },

    {
        IANA_MT_AD_STYPE_CODE_MPA_ROBUST,
        IANA_MT_AD_STYPE_TEXT_MPA_ROBUST
    },

    {
        IANA_MT_AD_STYPE_CODE_DAT12,
        IANA_MT_AD_STYPE_TEXT_DAT12
    },

    {
        IANA_MT_AD_STYPE_CODE_L20,
        IANA_MT_AD_STYPE_TEXT_L20
    },

    {
        IANA_MT_AD_STYPE_CODE_L24,
        IANA_MT_AD_STYPE_TEXT_L24
    },

    {
        IANA_MT_AD_STYPE_CODE_RESERVED,
        NULL
    }
};


static const ANSC_UTILBOX_IANA_MEDIA                g_IanaVideoSubtypes[]       =
{
    {
        IANA_MT_VD_STYPE_CODE_MPEG,
        IANA_MT_VD_STYPE_TEXT_MPEG
    },

    {
        IANA_MT_VD_STYPE_CODE_QUICKTIME,
        IANA_MT_VD_STYPE_TEXT_QUICKTIME
    },

    {
        IANA_MT_VD_STYPE_CODE_POINTER,
        IANA_MT_VD_STYPE_TEXT_POINTER
    },

    {
        IANA_MT_VD_STYPE_CODE_PARITYFEC,
        IANA_MT_VD_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_VD_STYPE_CODE_MP4V_ES,
        IANA_MT_VD_STYPE_TEXT_MP4V_ES
    },

    {
        IANA_MT_VD_STYPE_CODE_RESERVED,
        NULL
    }
};


static const ANSC_UTILBOX_IANA_MEDIA                g_IanaModelSubtypes[]       =
{
    {
        IANA_MT_MD_STYPE_CODE_IGES,
        IANA_MT_MD_STYPE_TEXT_IGES
    },

    {
        IANA_MT_MD_STYPE_CODE_VRML,
        IANA_MT_MD_STYPE_TEXT_VRML
    },

    {
        IANA_MT_MD_STYPE_CODE_MESH,
        IANA_MT_MD_STYPE_TEXT_MESH
    },

    {
        IANA_MT_MD_STYPE_CODE_RESERVED,
        NULL
    }
};


static const ANSC_UTILBOX_IANA_MEDIA_SUBTYPE_LIST   g_IanaMediaSubtypeLists[]   =
{
    {
        IANA_MEDIA_TYPE_CODE_TEXT,
        g_IanaTextSubtypes,
        sizeof(g_IanaTextSubtypes)/sizeof(g_IanaTextSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_MULTIPART,
        g_IanaMultipartSubtypes,
        sizeof(g_IanaMultipartSubtypes)/sizeof(g_IanaMultipartSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_MESSAGE,
        g_IanaMessageSubtypes,
        sizeof(g_IanaMessageSubtypes)/sizeof(g_IanaMessageSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_APPLICATION,
        g_IanaApplicationSubtypes,
        sizeof(g_IanaApplicationSubtypes)/sizeof(g_IanaApplicationSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_IMAGE,
        g_IanaImageSubtypes,
        sizeof(g_IanaImageSubtypes)/sizeof(g_IanaImageSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_AUDIO,
        g_IanaAudioSubtypes,
        sizeof(g_IanaAudioSubtypes)/sizeof(g_IanaAudioSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_VIDEO,
        g_IanaVideoSubtypes,
        sizeof(g_IanaVideoSubtypes)/sizeof(g_IanaVideoSubtypes[0])
    },

    {
        IANA_MEDIA_TYPE_CODE_MODEL,
        g_IanaModelSubtypes,
        sizeof(g_IanaModelSubtypes)/sizeof(g_IanaModelSubtypes[0])
    }
};


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrBufGetCharNum
            (
                PUCHAR                      pString,
                ULONG                       ulStrLen,
                UCHAR                       uChar
            )

    description:

        This function is called to count the specified character
        in the given string.

    argument:   PUCHAR                      pString
                The string to be processed.

                ULONG                       ulStrLen
                The length of string.

                UCHAR                       uChar
                The character to be counted.

    return:     the number of characters in the string.

**********************************************************************/

static ULONG
//ULONG Tom Modified
AnscStrBufGetCharNum
    (
        PUCHAR                      pString,
        ULONG                       ulStrLen,
        UCHAR                       uChar
    )
{
    ULONG                           i;
    ULONG                           ulCount = 0;
    PUCHAR                          pBuf;

    if (pString && ulStrLen != 0)
    {
        pBuf    = pString;
        for (i = 0; i < ulStrLen; i ++)
        {
            if (*pBuf == uChar)
                ulCount ++;

            pBuf ++;
        }
    }

    return ulCount;
}

/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoDecodeUrlGetHex
            (
                UCHAR                       uc,
                PUCHAR                      pHex
            )

    description:

        This function is called to the given character into hex
        value.

    argument:   UCHAR                       uc
                The character to be converted.

                PUCHAR                      pHex
                The buffer for result.

    return:     status of operation.

**********************************************************************/

static BOOL
HttpSmpoDecodeUrlGetHex
    (
        UCHAR                       uc,
        PUCHAR                      pHex
    )
{
    UCHAR                           ucHex;

    if (uc >= '0' && uc <= '9')
        ucHex = uc - '0';
    else
        if (uc >= 'a' && uc <= 'f')
            ucHex = uc - 'a' + 10;
        else
            if (uc >= 'A' && uc <= 'F')
                ucHex = uc - 'A' + 10;
            else
            {
                return FALSE;
            }

    *pHex   = ucHex;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscHttpParseQueryString
            (
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP status
        line in given buffer and then save the information
        into an internal structure.

    argument:
                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                status line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to the status line object.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseQueryString
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_FORM_INPUT                pFormInput  = NULL;
    ULONG                           ulCount;
    ULONG                           ulBufSize;
    PUCHAR                          pAmp;
    PUCHAR                          pBuf, pName, pValue;
    ULONG                           ulBuf, ulName, ulValue;
    PHTTP_FORM_ELEMENT              pElement;
    ULONG                           ulNameLen;

    /* get the count of elements */
    ulCount = AnscStrBufGetCharNum(buffer, ulSize, '&');

    /*
    if (ulCount == 0)
        return NULL;
     */

    ulCount ++;

    ulBufSize   = sizeof(HTTP_FORM_INPUT) + ulCount * sizeof(HTTP_FORM_ELEMENT);
    pFormInput  = (PHTTP_FORM_INPUT)AnscAllocateMemory(ulBufSize);

    if (pFormInput)
    {
        pBuf        = buffer;
        ulBuf       = ulSize;

        ulNameLen   = 0;

        while (pBuf && ulBuf != 0)
        {
            if (pFormInput->ElementCount >= ulCount)
            {
                /* to prevent invalid input */
                break;
            }

            /* find next item */
            pAmp    = (PUCHAR)_ansc_memchr(pBuf, '&', ulBuf);

            if (pAmp)
            {
                /* we got an item */
                pName   = pBuf;
                ulName  = pAmp - pBuf;

                pBuf    = pAmp + 1;
                ulBuf   -= (ulName + 1);
            }
            else
            {
                pName   = pBuf;
                ulName  = ulBuf;

                pBuf    = NULL;
                ulBuf   = 0;
            }

            if (ulName == 0)
            {
                continue;
            }

            pElement    = &pFormInput->ElementArray[pFormInput->ElementCount ++];
            HttpInitFormElement(pElement);

            /* find '=' */
            pValue  = (PUCHAR)_ansc_memchr(pName, '=', ulName);
            if (!pValue)
            {
                /* only name is present */
                pElement->Name = (char *)AnscAllocateMemory(ulName + 1);

                AnscCopyMemory(pElement->Name, pName, ulName);
                pElement->Name[ulName]  = 0;

                ulNameLen   = ulName;
            }
            else
            {
                /* get name & value */
                ulNameLen   = pValue - pName;
                ulValue     = ulName - ulNameLen - 1;
                pValue  ++;

                if (ulNameLen == 0)
                {
                    /* no name specified, could the query string be malformed? */
                    pFormInput->ElementCount --;
                    continue;
                }

                if (ulNameLen != 0)
                {
                    pElement->Name = (char *)AnscAllocateMemory(ulNameLen + 1);

                    AnscCopyMemory(pElement->Name, pName, ulNameLen);
                    pElement->Name[ulNameLen]  = 0;
                }

                if (ulValue != 0)
                {
                    pElement->Value = (char *)AnscAllocateMemory(ulValue + 1);

                    AnscCopyMemory(pElement->Value, pValue, ulValue);
                    ((char *)pElement->Value)[ulValue]  = 0;

                    /* decode the string if it contains characters like "%xx" */
                    HttpSmpoDecodeUrlEncode(pElement->Value, ulValue, pElement->Value, &ulValue);

                    pElement->Size = ulValue;
                }
            }

            /* decode query name */
            HttpSmpoDecodeUrlEncode(pElement->Name, ulNameLen, pElement->Name, &ulNameLen);
        }
    }

    if (pFormInput->ElementCount == 0)
    {
        /* the query string might be empty or malformed */
        AnscFreeMemory(pFormInput);
        pFormInput  = NULL;
    }

    return pFormInput;
}

/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoUtilLinearWhiteSpace
            (
                PUCHAR                      pBuf,
                ULONG                       ulBufLen
            )

    description:

        This function is called to remove all leading
        linear-white-space characters.

    argument:
                ANSC_HANDLE                 hUri
                HTTP Uri object.

                PUCHAR                      pBuf
                Buffer of Uri string.

                ULONG                       ulBufLen
                The size of Uri string buffer.

    return:     the position of the first non
                liear-white-space character.

**********************************************************************/

static PUCHAR
HttpSmpoUtilLinearWhiteSpace
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pNext   = pBuf;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    BOOL                            bCRLF   = FALSE;
    UCHAR                           c;
    PUCHAR                          pAfter;

    while (pNext <= pLast)
    {
        c   = *pNext;

        if (c != HTTP_SMPO_CHAR_SPACE && c != HTTP_SMPO_CHAR_HTAB && c != HTTP_SMPO_CHAR_CR)
        {
            return pNext;
        }
        else
        {
            bCRLF = (c == HTTP_SMPO_CHAR_CR && *(pNext + 1) == HTTP_SMPO_CHAR_LF);

            if (!bCRLF)
            {
                pNext ++;
            }
            else
            {
                pAfter = pNext + 2;

                if (pAfter >= pLast)
                {
                    return pNext;
                }

                c   = *pAfter;
                if (c != HTTP_SMPO_CHAR_SPACE && c != HTTP_SMPO_CHAR_HTAB)
                {
                    return pNext;
                }
                else
                {
                    pNext   = pAfter + 1;
                }
            }
        }
    }

    return pNext;
}

static void
HttpSmpoUtilCopyHeaderString
    (
        PUCHAR                      pString,
        ULONG                       ulStringLen,
        char                        *pBuf,
        ULONG                       ulBufSize
    )
{
    if ( ulStringLen == 0 )
    {
        return;
    }

    if (ulStringLen >= ulBufSize)
    {
        ulStringLen = ulBufSize - 1;
    }

    AnscCopyMemory(pBuf, pString, ulStringLen);
    pBuf[ulStringLen] = 0;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscHttpGetHeaderParam
            (
                char*                       parameters,
                char*                       name
            )

    description:

        This function is called to extract the value of given
        parameter from the given buffer.

    argument:
                char*                       parameters
                The buffer to parameters.

                char*                       name
                Parameter's name.

    return:     the value of the parameter. None if the parameter
                is not present.

**********************************************************************/

char*
AnscHttpGetHeaderParam
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        char*                       parameters,
        char*                       name
    )
{
    PUCHAR                          pLast   = parameters + AnscSizeOfString(parameters) - 1;
    PUCHAR                          pName;
    PUCHAR                          pValue;
    ULONG                           ulValue;
    PUCHAR                          pParamValue = NULL;
    PUCHAR                          pCur, pNext;
    ULONG                           ulLen;

    pCur    = parameters;

    while (pCur)
    {
        pCur    = HttpSmpoUtilLinearWhiteSpace(pCur, pLast - pCur + 1);

        pNext   = _ansc_memchr(pCur, HTTP_SMPO_CHAR_SEMICOLON, pLast - pCur + 1);

        /* check current parameter */
        if (pNext)
        {
            ulLen   = pNext - pCur;
        }
        else
        {
            ulLen   = pLast - pCur + 1;
        }

        pName   = _ansc_strstr(pCur, name);

        if (pName && pName <= pCur + ulLen)
        {
            pValue  = pName + AnscSizeOfString(name);

            if (pValue < pCur + ulLen)
            {
                if (pNext)
                {
                    ulValue = pNext - pValue;
                }
                else
                {
                    ulValue = pLast - pValue + 1;
                }

                pValue  = HttpSmpoUtilLinearWhiteSpace(pValue, ulValue);

                if (pValue < pCur + ulLen && *pValue == HTTP_SMPO_CHAR_EQUAL)
                {
                    pValue ++;

                    if (pNext)
                    {
                        ulValue = pNext - pValue;
                    }
                    else
                    {
                        ulValue = pLast - pValue + 1;
                    }

                    if (ulValue != 0 && pValue < pCur + ulLen)
                    {
                        pParamValue = (PUCHAR)AnscAllocateMemory(ulValue + 1);

                        if (pParamValue)
                        {
                            HttpSmpoUtilCopyHeaderString(pValue, ulValue, pParamValue, ulValue + 1);
                        }

                        return (char *)pParamValue;
                    }
                }
            }
        }

        /* check next parameter */

        if (pNext)
        {
            pCur    = pNext + 1;
        }
        else
        {
            pCur    = NULL;
        }
    }

    return NULL;
}

static
ULONG
HttpSmpoUtilGetSizeBasicCredential
    (
        PHTTP_CREDENTIAL_BASIC      pBasic
    )
{
    ULONG                           ulSize  = 0;
    ULONG                           ulUser, ulPass, ulBufLen;
    PUCHAR                          pBuf, pEncBuf;

    ulUser      =  AnscSizeOfString(pBasic->UserName);
    ulPass      =  AnscSizeOfString(pBasic->Password);
    ulBufLen    =  ulUser;

    if (ulPass != 0)
    {
        ulBufLen    += ulPass + 1;                  /* ':' */
    }

    ulBufLen    ++;                                 /* '\0' */

    pBuf        = (PUCHAR)AnscAllocateMemory(ulBufLen);

    if (pBuf)
    {
        if (ulUser != 0)
        {
            AnscCopyMemory(pBuf, pBasic->UserName, ulUser);
        }

        if (ulPass != 0)
        {
            pBuf[ulUser]    = ':';
            AnscCopyMemory(pBuf + ulUser + 1, pBasic->Password, ulPass);
        }

        pBuf[ulBufLen]      = 0;

        /* Encode the USER:PASS */
        pEncBuf = AnscBase64Encode(pBuf, ulBufLen - 1);
        AnscFreeMemory(pBuf);

        /* Basic */
        ulSize  += HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH;

        /* Space */
        ulSize  += HTTP_SMPO_SPACE_LENGTH;

        /* basic-cookie */
        ulSize  += AnscSizeOfString(pEncBuf);

        AnscFreeMemory(pEncBuf);
    }

    return ulSize;
}


static ULONG
HttpSmpoUtilGetSizeAuthorization
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Authorization = "Authorization" ":" credentials
     */
    PHTTP_HFO_AUTHORIZATION         pAuthorization  = (PHTTP_HFO_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize          = 0;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    ulSize      += HTTP_SMPO_AUTHORIZATION_LENGTH;                  /* "Authorization" */
    ulSize      += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;               /* ": " */

    pCredential = &pAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        ulSize  += HttpSmpoUtilGetSizeBasicCredential(&pCredential->Credential.Basic);
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        break;

    case HTTP_AUTH_TYPE_NONE:
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        break;
    }

    return ulSize;
}

static ULONG
HttpSmpoUtilGetSizeConnection
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Connection = Connection-header  ; added to remove orphan Connection-header
     *  Connection-header = "Connection" ":" 1#( connection-token )
     *  connection-token = token
     */
    PHTTP_HFO_CONNECTION            pConnection = (PHTTP_HFO_CONNECTION)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulTokenCount, i;

    ulSize          += HTTP_SMPO_CONNECTION_LENGTH;             /* "Connection" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTokenCount    = pConnection->TokenCount;

    for (i = 0; i <ulTokenCount; i ++)
    {
        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += AnscSizeOfString(pConnection->TokenArray[i].Token);
    }

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetContentCodingLength
    (
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize = 0;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        break;

    case IANA_CCODING_CODE_IDENTITY:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        break;
    }

    return ulSize;
}



static ULONG
HttpSmpoUtilGetSizeContentEncoding
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Encoding = "Content-Encoding" ":" 1#content-coding
     *  content-coding = token
     */
    PHTTP_HFO_CONTENT_ENCODING      pContentEncoding    = (PHTTP_HFO_CONTENT_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;

    ulSize          += HTTP_SMPO_CONTENT_ENCODING_LENGTH;       /* "Content-Encoding" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulEncodingCount = pContentEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pContentEncoding->EncodingArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        ulSize      += HttpSmpoUtilGetContentCodingLength(pEncoding->Type);
    }

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetUlongStringLength
    (
        ULONG                       ulValue
    )
{
    UCHAR                       buf[16];
    ULONG                       ulSize;

    AnscInt2String(ulValue, buf, 10);
    ulSize   = AnscSizeOfString(buf);

    return ulSize;
}


static ULONG
HttpSmpoUtilGetSizeContentLength
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Length = "Content-Length" ":" 1*DIGIT
     */
    PHTTP_HFO_CONTENT_LENGTH        pContentLength  = (PHTTP_HFO_CONTENT_LENGTH)hHttpHfo;
    ULONG                           ulSize          = 0;

    ulSize  += HTTP_SMPO_CONTENT_LENGTH_LENGTH;                             /* "Content-Length" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                           /* ": " */
    ulSize  += HttpSmpoUtilGetUlongStringLength(pContentLength->BodySize);    /* body size */

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetMediaTypeLength
    (
        ULONG                       ulMediaType
    )
{
    ULONG                           ulSize = 0;

    switch (ulMediaType)
    {
    case IANA_MEDIA_TYPE_CODE_TEXT:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT);
        break;

    case IANA_MEDIA_TYPE_CODE_MULTIPART:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART);
        break;

    case IANA_MEDIA_TYPE_CODE_MESSAGE:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE);
        break;

    case IANA_MEDIA_TYPE_CODE_APPLICATION:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION);
        break;

    case IANA_MEDIA_TYPE_CODE_IMAGE:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE);
        break;

    case IANA_MEDIA_TYPE_CODE_AUDIO:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO);
        break;

    case IANA_MEDIA_TYPE_CODE_VIDEO:
        ulSize = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO);
        break;

    default:
        ulSize = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
        break;
    }

    return ulSize;
}

static PUCHAR
AnscGetIanaNameByType
    (
        PANSC_UTILBOX_IANA_MEDIA    pSet,
        ULONG                       ulCount,
        ULONG                       Type
    )
{
    ULONG                           i;
    PANSC_UTILBOX_IANA_MEDIA        pInfo;
    PUCHAR                          pName   = NULL;

    for (i = 0; i < ulCount; i ++)
    {
        pInfo   = (PANSC_UTILBOX_IANA_MEDIA)&pSet[i];

        if (pInfo->Name == NULL)
        {
            return NULL;
        }

        if (pInfo->Type == Type)
        {
            return pInfo->Name;
        }
    }

    return NULL;
}



static PUCHAR
AnscGetIanaMediaSubtypeNameByType
    (
        ULONG                       ulMediaType,
        ULONG                       ulMediaSubtype
    )
{
    ULONG                           ulCount, i;

    for (i = 0; i < sizeof(g_IanaMediaSubtypeLists)/sizeof(g_IanaMediaSubtypeLists[0]); i ++)
    {
        if (ulMediaType == g_IanaMediaSubtypeLists[i].MediaType)
        {
            ulCount = g_IanaMediaSubtypeLists[i].Count;

            return
                AnscGetIanaNameByType
                    (
                        (PANSC_UTILBOX_IANA_MEDIA)g_IanaMediaSubtypeLists[i].pSubtypes,
                        ulCount,
                        ulMediaSubtype
                    );
        }
    }

    return NULL;
}


static
ULONG
HttpSmpoUtilGetMediaSubTypeLength
    (
        ULONG                       ulType,
        ULONG                       ulSubType
    )
{
    ULONG                           ulSize = 0;
    PUCHAR                          pName;

    pName   = AnscGetIanaMediaSubtypeNameByType(ulType, ulSubType);

    if (pName)
    {
        ulSize = AnscSizeOfString(pName);
    }
    else
    {
        ulSize = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
    }

    return ulSize;
}


static
ULONG
HttpSmpoUtilGetSizeContentType
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Content-Type = "Content-Type" ":" media-type
     *  media-type = type "/" subtype *( ";" parameter )
     */
    PHTTP_HFO_CONTENT_TYPE          pContentType    = (PHTTP_HFO_CONTENT_TYPE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulParams;

    ulSize  += HTTP_SMPO_CONTENT_TYPE_LENGTH;                               /* "Content-Type" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;                           /* ": " */

    ulSize  += HttpSmpoUtilGetMediaTypeLength(pContentType->MediaType);     /* media type */
    ulSize  += 1;                                                           /* "/" */
    ulSize  += HttpSmpoUtilGetMediaSubTypeLength(pContentType->MediaType, pContentType->SubType);    /* subtype */

    ulParams = AnscSizeOfString(pContentType->Parameters);
    if (ulParams != 0)
    {
        ulSize  += 1;                       /* ";" */
        ulSize  += ulParams;                /* parameter */
    }

    return ulSize;
}

static ULONG
HttpSmpoUtilGetSizeHost
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Host = "Host" ":" host [":" port]  ; Section 3.2.2
     */
    PHTTP_HFO_HOST                  pHost   = (PHTTP_HFO_HOST)hHttpHfo;
    ULONG                           ulSize  = 0;

    ulSize  += HTTP_SMPO_HEADER_HOST_LENGTH;            /* "Host" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulSize  += AnscSizeOfString(pHost->HostName);       /* host name */
    if (pHost->HostPort != 80)
    {
        ulSize  += 1;                                   /* ":" */
        ulSize  += HttpSmpoUtilGetUlongStringLength(pHost->HostPort);   /* port */
    }

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetTransferCodingLength
    (
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize = 0;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        break;

    case IANA_CCODING_CODE_IDENTITY:
        ulSize = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        break;

    case IANA_TCODING_CODE_CHUNKED:
        ulSize = AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED);
        break;

    case IANA_TCODING_CODE_TRAILERS:
        ulSize = AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS);
        break;
    }

    return ulSize;
}


static ULONG
HttpSmpoUtilGetSizeTransferEncoding
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding
     *  transfer-coding = "chunked" | transfer-extension
     *  transfer-extension = token
     */
    PHTTP_HFO_TRANSFER_ENCODING     pTransferEncoding   = (PHTTP_HFO_TRANSFER_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;

    ulSize          += HTTP_SMPO_TRANSFER_ENCODING_LENGTH;      /* "Transfer-Encoding" */
    ulSize          += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulEncodingCount = pTransferEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTransferEncoding->EncodingArray[i];

        if (i != 0)
            ulSize  += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;

        ulSize      += HttpSmpoUtilGetTransferCodingLength(pEncoding->Type);
    }

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetSizeBasicChallenge
    (
        PHTTP_CHALLENGE_BASIC       pBasic
    )
{
    ULONG                           ulSize  = 0;

    /* "Basic" */
    ulSize  += HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH;

    /* Space */
    ulSize  += HTTP_SMPO_SPACE_LENGTH;

    /* "realm" */
    ulSize  += HTTP_SMPO_REALM_LENGTH;

    /* "=" */
    ulSize  ++;

    /* realm value */
    ulSize  += AnscSizeOfString(pBasic->Realm)  + 2;    /* two '"' (double quote) */

    return ulSize;
}


static ULONG
HttpSmpoUtilGetSizeWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge
     */
    PHTTP_HFO_WWW_AUTHENTICATE      pWWWAuthenticate    = (PHTTP_HFO_WWW_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;

    ulSize              += HTTP_SMPO_WWW_AUTHENTICATE_LENGTH;       /* "WWW-Authenticate" */
    ulSize              += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulChallengeCount    = pWWWAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pWWWAuthenticate->ChallengeArray[i];
        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            ulSize  += HttpSmpoUtilGetSizeBasicChallenge(&pChallenge->Challenge.Basic);
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return ulSize;
}

static ULONG
HttpSmpoUtilGetSizeProxyConnection
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Proxy-Connection        = "Proxy-Connection" ":" 1#(proxy-connection-token)
     *  proxy-connection-token  = token
     */
    PHTTP_HFO_PROXY_CONNECTION      pProxyConnection    = (PHTTP_HFO_PROXY_CONNECTION)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulTokenCount, i;
    PHTTP_CONNECTION_TOKEN          pToken;

    ulSize              += HTTP_SMPO_PROXY_CONNECTION_LENGTH;       /* "Proxy-Connection" */
    ulSize              += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulTokenCount        = pProxyConnection->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pToken          = &pProxyConnection->TokenArray[i];
        if (i != 0)
            ulSize      += HTTP_SMPO_FIELD_SEPARATOR_LENGTH;        /* ", " */

        ulSize          += AnscSizeOfString(pToken->Token);         /* token */
    }

    return ulSize;
}

static ULONG
HttpSmpoUtilGetSizeCookie
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     *  cookie-value = cookie-name "=" value [";" cookie-path] [";" cookie-domain]
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulCount, i;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    ULONG                           ulVersion;

    ulSize  += HTTP_SMPO_COOKIE_LENGTH;                 /* "Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;       /* ": " */

    ulCount = pCookie->CookieCount;

    if (ulCount != 0)
    {
        ulVersion   = pCookie->CookieArray[0].Version;

        if (ulVersion != 1)
        {
            ulSize      += HTTP_SMPO_SET_COOKIE_ATTR_PREFIX_LENGTH;
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulVersion);
            ulSize      += HTTP_SMPO_SET_COOKIE_VERSION_LENGTH;
        }

        for (i = 0; i < ulCount; i ++)
        {
            if (i != 0)
                ulSize  ++;         /* ";" or "," */

            pCookieContent  = &pCookie->CookieArray[i];

            /* name */
            pValue      = pCookieContent->Name;
            ulValueLen  = AnscSizeOfString(pValue);
            ulSize      += ulValueLen;

            /* "=" */
            ulSize  ++;

            /* value */
            pValue      = pCookieContent->Value;
            ulValueLen  = AnscSizeOfString(pValue);
            ulSize      += ulValueLen;

            /* path */
            pValue      = pCookieContent->Path;
            ulValueLen  = AnscSizeOfString(pValue);

            if (ulValueLen != 0)
            {
                ulSize      ++;     /* ";" */
                ulSize      += ulValueLen;
            }

            /* domain */
            pValue      = pCookieContent->Domain;
            ulValueLen  = AnscSizeOfString(pValue);

            if (ulValueLen != 0)
            {
                ulSize      ++;     /* ";" */
                ulSize      += ulValueLen;
            }
        }
    }

    return ulSize;
}

static ULONG
HttpSmpoUtilGetSizeSetCookie
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     *      Set-Cookie      =       "Set-Cookie:" 1#Cookie-Entity
     *      Cookie-Entity   =       cookie-name "=" value *(";" cookie-av)
     *      cookie-av       =       "Comment"   "="     comment
     *                      |       "Domain"    "="     domain
     *                      |       "Max-Age"   "="     delta-seconds
     *                      |       "Path"      "="     ( "/" | path )
     *                      |       "Secure"
     *                      |       "Version"   "="     1*DIGIT
     *                      |       "expires"   "="     HTTP-date
     *                      |       token       "="     value
     *
     *
     */
    PHTTP_HFO_SET_COOKIE            pSetCookie          = (PHTTP_HFO_SET_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulValue;

    ulSize  += HTTP_SMPO_SET_COOKIE_LENGTH;                 /* "Set-Cookie" */
    ulSize  += HTTP_SMPO_HEADER_SEPARATOR_LENGTH;           /* ": " */

    ulCount = pSetCookie->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        pCookieContent  = &pSetCookie->CookieArray[i];

        /* name */
        pValue          = pCookieContent->Name;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* "=" */
        ulSize ++;

        /* value */
        pValue          = pCookieContent->Value;
        ulValueLen      = AnscSizeOfString(pValue);
        ulSize          += ulValueLen;

        /* comment */
        pValue          = pCookieContent->Comment;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            ulSize ++;  /* ";" */
            ulSize      += HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* path */
        pValue          = pCookieContent->Path;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_PATH_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* domain */
        pValue          = pCookieContent->Domain;
        ulValueLen      = AnscSizeOfString(pValue);

        if (ulValueLen != 0)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += ulValueLen;
        }

        /* version */
        ulValue         = pCookieContent->Version;
        if (ulValue != 0)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_VERSION_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* max-age */
        ulValue         = pCookieContent->MaxAgeInSeconds;
        if (ulValue != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH;
            ulSize ++;  /* "=" */
            ulSize      += HttpSmpoUtilGetUlongStringLength(ulValue);
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            /* ";" */
            ulSize ++;
            ulSize      += HTTP_SMPO_SET_COOKIE_SECURE_LENGTH;
        }
    }

    return ulSize;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoUtilGetSizeHeader
            (
                ANSC_HANDLE         hHttpHfo
            )

    description:

        This function is called to get size of HTTP header field.

    argument:
                ANSC_HANDLE         hHttpHfo
                Handle to HTTP header field object.

    return:     the string length.

**********************************************************************/

static ULONG
HttpSmpoUtilGetSizeHeader
    (
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    PHTTP_HEADER_FIELD              pHeader;
    ULONG                           ulSize = 0;

    pHeader = (PHTTP_HEADER_FIELD)hHttpHfo;

    if (!pHeader)
        return 0;

    if (pHeader->Flags & HTTP_FIELD_FLAG_LINE_PRESENT)
    {
        /* this header is still valid, so return the size of HeaderLine */
        ulSize = AnscSizeOfString(pHeader->HeaderLine);
    }
    else
    {
        switch (pHeader->HeaderId)
        {
        case HTTP_HEADER_ID_AUTHORIZATION:
            ulSize = HttpSmpoUtilGetSizeAuthorization(hHttpHfo);
            break;

        case HTTP_HEADER_ID_CONNECTION:
            ulSize = HttpSmpoUtilGetSizeConnection(hHttpHfo);
            break;

        case HTTP_HEADER_ID_CONTENT_ENCODING:
            ulSize = HttpSmpoUtilGetSizeContentEncoding(hHttpHfo);
            break;

        case HTTP_HEADER_ID_CONTENT_LENGTH:
            ulSize = HttpSmpoUtilGetSizeContentLength(hHttpHfo);
            break;

        case HTTP_HEADER_ID_CONTENT_TYPE:
            ulSize = HttpSmpoUtilGetSizeContentType(hHttpHfo);
            break;

        case HTTP_HEADER_ID_HOST:
            ulSize = HttpSmpoUtilGetSizeHost(hHttpHfo);
            break;

        case HTTP_HEADER_ID_TRANSFER_ENCODING:
            ulSize = HttpSmpoUtilGetSizeTransferEncoding(hHttpHfo);
            break;

        case HTTP_HEADER_ID_WWW_AUTHENTICATE:
            ulSize = HttpSmpoUtilGetSizeWWWAuthenticate(hHttpHfo);
            break;

        case HTTP_HEADER_ID_PROXY_CONNECTION:
            ulSize = HttpSmpoUtilGetSizeProxyConnection(hHttpHfo);
            break;

        case HTTP_HEADER_ID_COOKIE:
            ulSize = HttpSmpoUtilGetSizeCookie(hHttpHfo);
            break;

        case HTTP_HEADER_ID_SET_COOKIE:
            ulSize = HttpSmpoUtilGetSizeSetCookie(hHttpHfo);
            break;
        }

    }

    return ulSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSmpoGetHeaderSize
            (
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the header field length and
        CRLF is not included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP header

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
AnscHttpGetHeaderSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    )
{
    /* build header and save it into header structure */

    AnscHttpBuildHeader(NULL, hHttpHfo, NULL, 0);

    return HttpSmpoUtilGetSizeHeader(hHttpHfo);
}


static
ULONG
HttpSmpoUtilGetMethodLength
    (
        ULONG                       ulMethod,
        PUCHAR                      pMethodName
    )
{
    ULONG                           ulLen;

    ulLen = 0;

    switch (ulMethod)
    {
    case HTTP_METHOD_OPTIONS:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS);
        break;

    case HTTP_METHOD_GET:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_GET);
        break;

    case HTTP_METHOD_HEAD:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_HEAD);
        break;

    case HTTP_METHOD_POST:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_POST);
        break;

    case HTTP_METHOD_PUT:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_PUT);
        break;

    case HTTP_METHOD_DELETE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_DELETE);
        break;

    case HTTP_METHOD_TRACE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_TRACE);
        break;

    case HTTP_METHOD_CONNECT:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_CONNECT);
        break;

    case HTTP_METHOD_NOTIFY:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY);
        break;

    case HTTP_METHOD_SEARCH:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_SEARCH);
        break;

    case HTTP_METHOD_M_SEARCH:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH);
        break;

    case HTTP_METHOD_M_POST:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_M_POST);
        break;

    case HTTP_METHOD_SUBSCRIBE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE);
        break;

    case HTTP_METHOD_UNSUBSCRIBE:
        ulLen = AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE);
        break;

    default:
        if (pMethodName)
        {
            ulLen   = AnscSizeOfString(pMethodName);
        }
        break;
    }

    return ulLen;
}

static
ULONG
HttpSmpoUtilGetSizeRequestUri
    (
        PHTTP_REQUEST_URI           pUri
    )
{
    ULONG                           ulSize = 0;
    BOOL                            bHttps = FALSE;

    bHttps  = ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) != 0 );

    switch (pUri->Type)
    {
    case HTTP_URI_TYPE_ASTERISK:
        ulSize  = HTTP_SMPO_REQUEST_URI_ASTERISK_LENGTH;
        break;

    case HTTP_URI_TYPE_ABSOLUTE_URI:
        /*
         *     absoluteURI = scheme ":" *( uchar | reserved )
         */

        if (!bHttps)
        {
            ulSize  += HTTP_SMPO_SCHEME_LENGTH;
        }
        else
        {
            ulSize  += HTTP_SMPO_SCHEME_HTTPS_LENGTH;
        }

        ulSize  += AnscSizeOfString(pUri->HostName);

        if (TRUE)
        {
            ULONG                   ulHostPort;
            UCHAR                   pHostPort[8];
            BOOL                    bCountPort;

            AnscInt2String(pUri->HostPort, (PUCHAR)pHostPort, 10);

            if (bHttps)
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bCountPort)
            {
                ulHostPort = AnscSizeOfString(pHostPort);

                ulSize  += 1;                   /* ":" */
                ulSize  += ulHostPort;
            }
        }

        /*
         * check path segments
         */
        if (pUri->PathLevel >= 2)
        {
            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;

            ulSize      += 1;               /* "/" */
            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                if (
                       i == ulLevels - 1       &&
                       AnscEqualString(pUri->PathArray[i], "/", TRUE)
                   )
                {
                    /* already counted */
                    break;
                }

                /* count each (f)segment */
                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        else
        {
            ulSize  += 1;               /* '/' */
        }
        break;

    case HTTP_URI_TYPE_ABS_PATH:
        {
            /*
             *  abs_path = "/" rel_path
             *  rel_path = [path] [";" params] ["?" query]
             *  path = fsegment *( "/" segment )
             */

            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;
            BOOL                    bIsSlash;

            ulSize      = 1;            /* "/" */
            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                /* count each (f)segment */
                bIsSlash    = AnscEqualString(pUri->PathArray[i], "/", TRUE);
                if (bIsSlash && i == ulLevels - 1)
                    break;

                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1 && !bIsSlash)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        break;

    case HTTP_URI_TYPE_RELATIVE_URI:
        {
            /*
             *  relativeURI = net_path | abs_path | rel_path
             *  net_path = "//" net_loc [abs_path]
             */
            ULONG                   ulLevels, i;
            ULONG                   ulQuerySize;
            ULONG                   ulPathRoot;
/*            ULONG                   ulHostName; */

            ulSize      = 0;

#if 0   /* ??? */
            ulHostName  = AnscSizeOfString(pUri->HostName);

            if (ulHostName != 0)
            {
                ulSize  += 2;           /* "//" */
                ulSize  += ulHostName;  /* net_loc */
            }
#endif

            ulPathRoot  = AnscSizeOfString(pUri->PathArray[0]);

            if (ulPathRoot != 0)
                ulSize      += 1;            /* "/" */

            ulLevels    = pUri->PathLevel;

            /* ignore the first path because it's always a slash ("/") */
            for (i = 1; i < ulLevels; i ++)
            {
                /* count each (f)segment */
                ulSize += AnscSizeOfString(pUri->PathArray[i]);

                /* with a slash if a segment is not the last one */
                if (i < ulLevels - 1)
                    ulSize ++;
            }

            ulQuerySize = AnscSizeOfString(pUri->QueryParams);
            if (ulQuerySize != 0)
            {
                ulSize += 1;            /* the question mark */
                ulSize += ulQuerySize;
            }
        }
        break;

    case HTTP_URI_TYPE_AUTHORITY:
        {
            /*
             *  authority = authority-server | reg_name
             *  authority-server = [ [ userinfo "@" ] hostport ]
             *  hostport = host [ ":" port ]
             *  reg_name = 1*( uri-unreserved | escape | "$" | "," |
             *                 ";" | ":" | "@" | "&" | "=" | "+" )
             */

            ULONG                   ulHostName;
            ULONG                   ulHostPort;
            UCHAR                   pHostPort[8];
            BOOL                    bCountPort;

            ulHostName = AnscSizeOfString(pUri->HostName);
            AnscInt2String(pUri->HostPort, (PUCHAR)pHostPort, 10);
            ulSize  = ulHostName;

            if (bHttps)
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bCountPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bCountPort)
            {
                ulHostPort = AnscSizeOfString(pHostPort);

                ulSize  += 1;                   /* ":" */
                ulSize  += ulHostPort;
            }
        }
        break;
    }

    return ulSize;
}

static
ULONG
HttpSmpoUtilGetHttpVersionLength
    (
        ULONG                       MajorVersion,
        ULONG                       MinorVersion
    )
{
    /*
     *  HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     */
    UCHAR                           buf[80];
    ULONG                           ulSize;

    ulSize  = HTTP_SMPO_NAME_LENGTH;            /* "HTTP" --- protocol name */
    ulSize  += 1;                               /* "/" */

    _ansc_ultoa(MajorVersion, buf, 10);
    ulSize  += AnscSizeOfString(buf);           /* major version number length */

    ulSize  += 1;                               /* "." */

    _ansc_ultoa(MinorVersion, buf, 10);
    ulSize  += AnscSizeOfString(buf);           /* minor version number length */

    return ulSize;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscHttpGetRequestLineSize
            (
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the request line length and
        CRLF is not included.

    argument:
                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP request
                line.

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
AnscHttpGetRequestLineSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * Even the definition of "Request-Line" requires a CRLF at
     * the end, but here we don't count it.
     *
     * Request-Line = Method SP Request-URI SP HTTP-Version CRLF
     */

    PHTTP_REQUEST_INFO              pRequestInfo;
    ULONG                           ulSize = 0;

    pRequestInfo = (PHTTP_REQUEST_INFO)hHttpHfo;

    if (!pRequestInfo)
        return 0;

    ulSize  = HttpSmpoUtilGetMethodLength(pRequestInfo->Method, pRequestInfo->MethodName);
    ulSize  += 1;                   /* space */

    ulSize  += HttpSmpoUtilGetSizeRequestUri(&pRequestInfo->RequestUri);

    ulSize  += 1;                   /* space */
    ulSize  += HttpSmpoUtilGetHttpVersionLength(pRequestInfo->MajorVersion, pRequestInfo->MinorVersion);

    return ulSize;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscHttpGetStatusLineSize
            (
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the status line length and
        CRLF is not included.

    argument:
                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP status
                line.

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
AnscHttpGetStatusLineSize
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo
    )
{
    /*
     * Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
     */
    ULONG                           ulSize = 0;
    PHTTP_RESPONSE_INFO             pResponseInfo;
    UCHAR                           buf[8];

    pResponseInfo   = (PHTTP_RESPONSE_INFO)hHttpHfo;
    if (!pResponseInfo)
        return 0;

    /* HTTP-Version */
    ulSize  += HttpSmpoUtilGetHttpVersionLength(pResponseInfo->MajorVersion, pResponseInfo->MinorVersion);
    ulSize  += 1;                                   /* space */

    _ansc_ultoa(pResponseInfo->StatusCode, buf, 10);
    ulSize  += AnscSizeOfString(buf);               /* status code */

    ulSize  += 1;                                   /* space */
    ulSize  += AnscSizeOfString(pResponseInfo->ReasonPhrase);

    return ulSize;
}


static
BOOL
HttpSmpoUtilCopyString
    (
        PUCHAR                      pDest,
        ULONG                       ulDestSize,
        ULONG                       *pulDestStart,        /* where copy begins, 0-based */
        PUCHAR                      pSrc,
        ULONG                       ulSrcSize
    )
{
    BOOL                            bBufExhausted   = FALSE;
    ULONG                           ulCopySize, ulDestLeft;
    ULONG                           ulDestStart     = *pulDestStart;

    ulCopySize      = ulSrcSize;
    ulDestLeft      = ulDestSize - ulDestStart;

    if (ulDestLeft < ulSrcSize)
    {
        ulCopySize      = ulDestLeft;
        bBufExhausted   = TRUE;
    }

    if (ulCopySize != 0)
    {
        AnscCopyMemory(&pDest[ulDestStart], pSrc, ulCopySize);
        ulDestStart += ulCopySize;
    }

    *pulDestStart   = ulDestStart;

    return !bBufExhausted;
}


static
BOOL
HttpSmpoUtilPrebuildHeader
    (
        PUCHAR                      buffer,
        ULONG                       ulBufSize,
        ULONG                       *pulStart,        /* where copy begins, 0-based */
        PUCHAR                      pHeaderName,
        ULONG                       HeaderNameLength
    )
{
    BOOL                            bCopySucc;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         pHeaderName,
                         HeaderNameLength
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_HEADER_SEPARATOR,
                         HTTP_SMPO_HEADER_SEPARATOR_LENGTH
                     );
    if (!bCopySucc)
        return FALSE;

    return TRUE;
}

static
BOOL
HttpSmpoUtilBuildBasicCredentials
    (
        PHTTP_CREDENTIAL_BASIC      pBasic,
        PUCHAR                      buffer,
        ULONG                       ulBufSize,
        ULONG                       *pulStart       /* where copy begins, 0-based */
    )
{
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pBuf;
    ULONG                           ulBufLen, ulUser, ulPass;
    PUCHAR                          pEncBuf;

    ulUser      =  AnscSizeOfString(pBasic->UserName);
    ulPass      =  AnscSizeOfString(pBasic->Password);
    ulBufLen    =  ulUser;

    if (ulPass != 0)
    {
        ulBufLen    += ulPass + 1;                  /* ':' */
    }

    ulBufLen    ++;                                 /* '\0' */

    pBuf        = (PUCHAR)AnscAllocateMemory(ulBufLen);

    if (pBuf)
    {
        if (ulUser != 0)
        {
            AnscCopyMemory(pBuf, pBasic->UserName, ulUser);
        }

        if (ulPass != 0)
        {
            pBuf[ulUser]    = ':';
            AnscCopyMemory(pBuf + ulUser + 1, pBasic->Password, ulPass);
        }

        pBuf[ulBufLen-1]      = 0;

        /* Encode the USER:PASS */
        pEncBuf = AnscBase64Encode(pBuf, ulBufLen - 1);
        if ( pEncBuf )
        {
            ULONG                   ulLen   = AnscSizeOfString(pEncBuf);

            if ( ulLen >= 2 )
            {
                if ( pEncBuf[ulLen - 1] == '\r' || pEncBuf[ulLen - 1] == '\n' )
                {
                    pEncBuf[ulLen - 1]  = 0;
                }

                if ( pEncBuf[ulLen - 2] == '\r' || pEncBuf[ulLen - 2] == '\n' )
                {
                    pEncBuf[ulLen - 2]  = 0;
                }
            }
        }
        AnscFreeMemory(pBuf);

        /* copy "Basic" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             HTTP_SMPO_CREDENTIAL_BASIC_SCHEME,
                             HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        /* SPACE */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             HTTP_SMPO_SPACE,
                             HTTP_SMPO_SPACE_LENGTH
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        /* basic-cookie */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             pulStart,
                             pEncBuf,
                             AnscSizeOfString(pEncBuf)
                         );

        if (!bCopySucc)
        {
            AnscFreeMemory(pEncBuf);
            return FALSE;
        }

        AnscFreeMemory(pEncBuf);
    }

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildAuthorization
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Authorization = "Authorization" ":" credentials
     */
    PHTTP_HFO_AUTHORIZATION         pAuthorization  = (PHTTP_HFO_AUTHORIZATION)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CREDENTIAL           pCredential;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_AUTHORIZATION,
                         HTTP_SMPO_AUTHORIZATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    pCredential = &pAuthorization->Credential;

    switch (pCredential->AuthType)
    {
    case HTTP_AUTH_TYPE_BASIC:
        {
            PHTTP_CREDENTIAL_BASIC  pBasic;

            pBasic      = &pCredential->Credential.Basic;
            bCopySucc   = HttpSmpoUtilBuildBasicCredentials
                             (
                                 pBasic,
                                 buffer,
                                 ulBufSize,
                                 &ulStart
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
        break;

    case HTTP_AUTH_TYPE_DIGEST:
        {
            PHTTP_CREDENTIAL_DIGEST pDigest;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME,
                                 HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SPACE,
                                 HTTP_SMPO_SPACE_LENGTH
                             );

            if (!bCopySucc)
            {
                return FALSE;
            }

            pDigest     = &pCredential->Credential.Digest;

            ulSize      = AnscSizeOfString(pDigest->UserName);

            if ( ulSize != 0 )
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     pDigest->UserName,
                                     ulSize
                                 );
            }

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
        break;

    case HTTP_AUTH_TYPE_NONE:
        /* do nothing */
        break;

    case HTTP_AUTH_TYPE_RESERVED:
    default:
        /* do nothing */
        break;
    }

    return status;
}


static ANSC_STATUS
HttpSmpoUtilBuildConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Connection = Connection-header  ; added to remove orphan Connection-header
     *  Connection-header = "Connection" ":" 1#( connection-token )
     *  connection-token = token
     */
    PHTTP_HFO_CONNECTION            pConnection = (PHTTP_HFO_CONNECTION)hHttpHfo;
    ULONG                           ulSize      = 0;
    ULONG                           ulTokenCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONNECTION,
                         HTTP_SMPO_CONNECTION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTokenCount    = pConnection->TokenCount;
    for (i = 0; i <ulTokenCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pConnection->TokenArray[i].Token);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pConnection->TokenArray[i].Token,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static
BOOL
HttpSmpoUtilBuildContentCoding
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        pSrc    = IANA_CCODING_TEXT_GZIP;
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        pSrc    = IANA_CCODING_TEXT_COMPRESS;
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        pSrc    = IANA_CCODING_TEXT_DEFLATE;
        break;

    case IANA_CCODING_CODE_IDENTITY:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        pSrc    = IANA_CCODING_TEXT_IDENTITY;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildContentEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Encoding = "Content-Encoding" ":" 1#content-coding
     *  content-coding = token
     */
    PHTTP_HFO_CONTENT_ENCODING      pContentEncoding    = (PHTTP_HFO_CONTENT_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_ENCODING,
                         HTTP_SMPO_CONTENT_ENCODING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulEncodingCount = pContentEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pContentEncoding->EncodingArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildContentCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static
BOOL
HttpSmpoUtilBuildUlong
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulValue
    )
{
    UCHAR                       buf[16];
    ULONG                       ulSize;
    BOOL                        bCopySucc = TRUE;

    AnscInt2String(ulValue, buf, 10);
    ulSize      = AnscSizeOfString(buf);

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         pulStart,
                         buf,
                         ulSize
                     );

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildContentLength
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Length = "Content-Length" ":" 1*DIGIT
     */
    PHTTP_HFO_CONTENT_LENGTH        pContentLength  = (PHTTP_HFO_CONTENT_LENGTH)hHttpHfo;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_LENGTH,
                         HTTP_SMPO_CONTENT_LENGTH_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentLength->BodySize

                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}

static
BOOL
HttpSmpoUtilBuildMediaType
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulMediaType
    )
{
    ULONG                           ulSize      = 0;
    ULONG                           ulStart     = *pulStart;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc;

    switch (ulMediaType)
    {
    case IANA_MEDIA_TYPE_CODE_TEXT:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT);
        pSrc    = IANA_MEDIA_TYPE_TEXT_TEXT;
        break;

    case IANA_MEDIA_TYPE_CODE_MULTIPART:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART);
        pSrc    = IANA_MEDIA_TYPE_TEXT_MULTIPART;
        break;

    case IANA_MEDIA_TYPE_CODE_MESSAGE:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE);
        pSrc    = IANA_MEDIA_TYPE_TEXT_MESSAGE;
        break;

    case IANA_MEDIA_TYPE_CODE_APPLICATION:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION);
        pSrc    = IANA_MEDIA_TYPE_TEXT_APPLICATION;
        break;

    case IANA_MEDIA_TYPE_CODE_IMAGE:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE);
        pSrc    = IANA_MEDIA_TYPE_TEXT_IMAGE;
        break;

    case IANA_MEDIA_TYPE_CODE_AUDIO:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO);
        pSrc    = IANA_MEDIA_TYPE_TEXT_AUDIO;
        break;

    case IANA_MEDIA_TYPE_CODE_VIDEO:
        ulSize  = AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO);
        pSrc    = IANA_MEDIA_TYPE_TEXT_VIDEO;
        break;

    default:
        ulSize  = HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH;
        pSrc    = HTTP_SMPO_MEDIA_TYPE_ANY;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             &ulStart,
                             pSrc,
                             ulSize
                         );
    }

    *pulStart   = ulStart;

    return bCopySucc;
}


static
BOOL
HttpSmpoUtilBuildMediaSubType
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulType,
        ULONG                       ulSubType
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    pSrc    = AnscGetIanaMediaSubtypeNameByType(ulType, ulSubType);

    if (!pSrc)
    {
        ulSize  = HTTP_SMPO_MEDIA_SUBTYPE_ANY_LENGTH;
        pSrc    = HTTP_SMPO_MEDIA_SUBTYPE_ANY;
    }
    else
    {
        ulSize  = AnscSizeOfString(pSrc);
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildContentType
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Content-Type = "Content-Type" ":" media-type
     *  media-type = type "/" subtype *( ";" parameter )
     */
    PHTTP_HFO_CONTENT_TYPE          pContentType    = (PHTTP_HFO_CONTENT_TYPE)hHttpHfo;
    ULONG                           ulSize          = 0;
    ULONG                           ulParams;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_CONTENT_TYPE,
                         HTTP_SMPO_CONTENT_TYPE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildMediaType
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentType->MediaType
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         "/",
                         1
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildMediaSubType
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pContentType->MediaType,
                         pContentType->SubType
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulParams = AnscSizeOfString(pContentType->Parameters);
    if (ulParams != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             ";",
                            1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pContentType->Parameters,
                             ulParams
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static ANSC_STATUS
HttpSmpoUtilBuildHost
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Host = "Host" ":" host [":" port]  ; Section 3.2.2
     */
    PHTTP_HFO_HOST                  pHost   = (PHTTP_HFO_HOST)hHttpHfo;
    ULONG                           ulSize  = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_HEADER_HOST,
                         HTTP_SMPO_HEADER_HOST_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulSize      = AnscSizeOfString(pHost->HostName);       /* host name */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         pHost->HostName,
                         ulSize
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    if (pHost->HostPort != 80)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             ":",
                             1
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        bCopySucc   = HttpSmpoUtilBuildUlong
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pHost->HostPort
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static
BOOL
HttpSmpoUtilBuildTransferCoding
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulCoding
    )
{
    ULONG                           ulSize      = 0;
    PUCHAR                          pSrc        = NULL;
    BOOL                            bCopySucc   = TRUE;

    switch (ulCoding)
    {
    case IANA_CCODING_CODE_GZIP:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_GZIP);
        pSrc    = IANA_CCODING_TEXT_GZIP;
        break;

    case IANA_CCODING_CODE_COMPRESS:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS);
        pSrc    = IANA_CCODING_TEXT_COMPRESS;
        break;

    case IANA_CCODING_CODE_DEFLATE:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE);
        pSrc    = IANA_CCODING_TEXT_DEFLATE;
        break;

    case IANA_CCODING_CODE_IDENTITY:
        ulSize  = AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY);
        pSrc    = IANA_CCODING_TEXT_IDENTITY;
        break;
        break;

    case IANA_TCODING_CODE_CHUNKED:
        ulSize  = AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED);
        pSrc    = IANA_TCODING_TEXT_CHUNKED;
        break;

    case IANA_TCODING_CODE_TRAILERS:
        ulSize  = AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS);
        pSrc    = IANA_TCODING_TEXT_TRAILERS;
        break;
    }

    if (ulSize != 0)
    {
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             pulStart,
                             pSrc,
                             ulSize
                         );
    }

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildTransferEncoding
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding
     *  transfer-coding = "chunked" | transfer-extension
     *  transfer-extension = token
     */
    PHTTP_HFO_TRANSFER_ENCODING     pTransferEncoding   = (PHTTP_HFO_TRANSFER_ENCODING)hHttpHfo;
    ULONG                           ulSize              = 0;
    PHTTP_ENCODING                  pEncoding;
    ULONG                           ulEncodingCount, i;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_TRANSFER_ENCODING,
                         HTTP_SMPO_TRANSFER_ENCODING_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulEncodingCount = pTransferEncoding->EncodingCount;
    for (i = 0; i < ulEncodingCount; i ++)
    {
        pEncoding   = &pTransferEncoding->EncodingArray[i];

        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        bCopySucc   = HttpSmpoUtilBuildTransferCoding
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pEncoding->Type
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static
BOOL
HttpSmpoUtilBuildBasicChallenge
    (
        PHTTP_CHALLENGE_BASIC       pBasic,
        PUCHAR                      buffer,
        ULONG                       ulBufSize,
        ULONG                       *pulStart       /* where copy begins, 0-based */
    )
{
    BOOL                            bCopySucc   = TRUE;

    /* copy "Basic" */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_CREDENTIAL_BASIC_SCHEME,
                         HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* SPACE */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_SPACE,
                         HTTP_SMPO_SPACE_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* copy "realm" */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         HTTP_SMPO_REALM,
                         HTTP_SMPO_REALM_LENGTH
                     );

    if (!bCopySucc)
        return FALSE;

    /* '=' */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         "=",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    /* copy realm value */
    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         pulStart,
                         pBasic->Realm,
                         AnscSizeOfString(pBasic->Realm)
                     );

    if (!bCopySucc)
        return FALSE;

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildWWWAuthenticate
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge
     */
    PHTTP_HFO_WWW_AUTHENTICATE      pWWWAuthenticate    = (PHTTP_HFO_WWW_AUTHENTICATE)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulChallengeCount, i;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_WWW_AUTHENTICATE,
                         HTTP_SMPO_WWW_AUTHENTICATE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulChallengeCount    = pWWWAuthenticate->ChallengeCount;
    for (i = 0; i < ulChallengeCount; i ++)
    {
        pChallenge      = &pWWWAuthenticate->ChallengeArray[i];
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        switch (pChallenge->AuthType)
        {
        case HTTP_AUTH_TYPE_BASIC:
            {
                bCopySucc   = HttpSmpoUtilBuildBasicChallenge
                                 (
                                     &pChallenge->Challenge.Basic,
                                     buffer,
                                     ulBufSize,
                                     &ulStart
                                 );

                if (!bCopySucc)
                    return ANSC_STATUS_FAILURE;
            }
            break;

        case HTTP_AUTH_TYPE_DIGEST:
            {
                PHTTP_CHALLENGE_DIGEST  pDigestAuth = &pChallenge->Challenge.Digest;

                /* copy "Basic" */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME,
                                     HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME_LENGTH
                                 );

                if (!bCopySucc)
                    return FALSE;

                /* SPACE */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     HTTP_SMPO_SPACE,
                                     HTTP_SMPO_SPACE_LENGTH
                                 );

                if (!bCopySucc)
                    return FALSE;

                /* copy "Basic" */
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     buffer,
                                     ulBufSize,
                                     &ulStart,
                                     pDigestAuth->Realm,
                                     AnscSizeOfString(pDigestAuth->Realm)
                                 );

                if (!bCopySucc)
                    return FALSE;
            }
            break;

        case HTTP_AUTH_TYPE_NONE:
            break;

        case HTTP_AUTH_TYPE_RESERVED:
        default:
            break;
        }
    }

    return status;
}

static ANSC_STATUS
HttpSmpoUtilBuildProxyConnection
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Proxy-Connection    = "Proxy-Connection" ":" 1#(proxy-connection-token
     */
    PHTTP_HFO_PROXY_CONNECTION      pProxyConnection    = (PHTTP_HFO_PROXY_CONNECTION)hHttpHfo;
    ULONG                           ulSize              = 0;
    ULONG                           ulTokenCount, i;
    PHTTP_CONNECTION_TOKEN          pToken;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_PROXY_CONNECTION,
                         HTTP_SMPO_PROXY_CONNECTION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulTokenCount    = pProxyConnection->TokenCount;
    for (i = 0; i < ulTokenCount; i ++)
    {
        pToken          = &pProxyConnection->TokenArray[i];
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_FIELD_SEPARATOR,
                                 HTTP_SMPO_FIELD_SEPARATOR_LENGTH
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        ulSize      = AnscSizeOfString(pToken->Token); /* challenge */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             pToken->Token,
                             ulSize
                         );
        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;
    }

    return status;
}

static ANSC_STATUS
HttpSmpoUtilBuildCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           ulCount, i;
    ULONG                           ulVersion;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    UCHAR                           buf[8];
    ULONG                           bufLen;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE,
                         HTTP_SMPO_COOKIE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount     = pCookie->CookieCount;

    if(ulCount == 0)
        return ANSC_STATUS_FAILURE;

    ulVersion   = pCookie->CookieArray[0].Version;

    /*
     * Don't write version into a cookie right now if
     * the version is 1 (default version or no version
     * specified). If we write version into a cookie as
     * metioned by RFC 2109, some HTTP server can not
     * respond (Browser will timeout).
     */
    if (ulVersion != 0)
    {
        AnscInt2String(ulVersion, buf, 10);
        bufLen      = AnscSizeOfString(buf);

        if (bufLen != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_ATTR_PREFIX,
                                 HTTP_SMPO_SET_COOKIE_ATTR_PREFIX_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );
            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    /* write cookie-value(s) */
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pCookie->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_PATH,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;


            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_DOMAIN,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* port */
        pValue      = pCookieContent->Port;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_STRING_COOKIE_PORT,
                                 AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT)
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "=",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}

static PUCHAR
HttpSmpoUtilGetWeekDayName
    (
        USHORT                      usWkDay
    )
{
    PUCHAR                          pSrc    = HTTP_SMPO_WKDAY_SUN;

    switch (usWkDay)
    {
    case 1:
        pSrc    = HTTP_SMPO_WKDAY_MON;
        break;

    case 2:
        pSrc    = HTTP_SMPO_WKDAY_TUE;
        break;

    case 3:
        pSrc    = HTTP_SMPO_WKDAY_WED;
        break;

    case 4:
        pSrc    = HTTP_SMPO_WKDAY_THU;
        break;

    case 5:
        pSrc    = HTTP_SMPO_WKDAY_FRI;
        break;

    case 6:
        pSrc    = HTTP_SMPO_WKDAY_SAT;
        break;
    }

    return pSrc;
}

static PUCHAR
HttpSmpoUtilGetMonthName
    (
        USHORT                      usMonth
    )
{
    PUCHAR                          pSrc    = HTTP_SMPO_MONTH_JAN;

    switch (usMonth)
    {
    case 2:
        pSrc    = HTTP_SMPO_MONTH_FEB;
        break;

    case 3:
        pSrc    = HTTP_SMPO_MONTH_MAR;
        break;

    case 4:
        pSrc    = HTTP_SMPO_MONTH_APR;
        break;

    case 5:
        pSrc    = HTTP_SMPO_MONTH_MAY;
        break;

    case 6:
        pSrc    = HTTP_SMPO_MONTH_JUN;
        break;

    case 7:
        pSrc    = HTTP_SMPO_MONTH_JUL;
        break;

    case 8:
        pSrc    = HTTP_SMPO_MONTH_AUG;
        break;

    case 9:
        pSrc    = HTTP_SMPO_MONTH_SEP;
        break;

    case 10:
        pSrc    = HTTP_SMPO_MONTH_OCT;
        break;

    case 11:
        pSrc    = HTTP_SMPO_MONTH_NOV;
        break;

    case 12:
        pSrc    = HTTP_SMPO_MONTH_DEC;
        break;
    }

    return pSrc;
}


static ANSC_STATUS
HttpSmpoUtilBuildSetCookie
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *      Set-Cookie      =       "Set-Cookie:" 1#Cookie-Entity
     *      Cookie-Entity   =       cookie-name "=" value *(";" cookie-av)
     *      cookie-av       =       "Comment"   "="     comment
     *                      |       "Domain"    "="     domain
     *                      |       "Max-Age"   "="     delta-seconds
     *                      |       "Path"      "="     ( "/" | path )
     *                      |       "Secure"
     *                      |       "Version"   "="     1*DIGIT
     *                      |       "expires"   "="     HTTP-date
     *                      |       token       "="     value
     *
     *
     */
    PHTTP_HFO_SET_COOKIE            pSetCookie      = (PHTTP_HFO_SET_COOKIE)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    UCHAR                           buf[16];
    ULONG                           bufLen;
    ULONG                           ulValue;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_SET_COOKIE,
                         HTTP_SMPO_SET_COOKIE_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount = pSetCookie->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   &= HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pSetCookie->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment */
        pValue      = pCookieContent->Comment;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PATH,
                                 HTTP_SMPO_SET_COOKIE_PATH_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* version */
        ulValue      = pCookieContent->Version;

        if (ulValue != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->Version, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* expires */
        if (pCookieContent->bPermanent)
        {
            UCHAR                   expires[48];
            PUCHAR                  pWkDay;
            PUCHAR                  pMonth;
            PANSC_UNIVERSAL_TIME    pTime;

            pTime   = &pCookieContent->Expires;
            pWkDay  = HttpSmpoUtilGetWeekDayName(pTime->DayOfWeek);
            pMonth  = HttpSmpoUtilGetMonthName(pTime->Month);

            _ansc_sprintf
                (
                    expires,
                    "; expires=%s, %.2d-%s-%.4d %.2d:%.2d:%.2d GMT",
                    pWkDay,
                    pTime->DayOfMonth,
                    pMonth,
                    pTime->Year,
                    pTime->Hour,
                    pTime->Minute,
                    pTime->Second
                );

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 expires,
                                 AnscSizeOfString(expires)
                             );
        }

        /* max-age */
        ulValue     = pCookieContent->MaxAgeInSeconds;

        if (pCookieContent->bPermanent && ulValue != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->MaxAgeInSeconds, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_SECURE,
                                 HTTP_SMPO_SET_COOKIE_SECURE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}

static ANSC_STATUS
HttpSmpoUtilBuildCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value )
     */
    PHTTP_HFO_COOKIE                pCookie             = (PHTTP_HFO_COOKIE)hHttpHfo;
    ULONG                           ulSize              = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE2,
                         HTTP_SMPO_COOKIE2_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_COOKIE2_CONTENT,
                         HTTP_SMPO_COOKIE2_CONTENT_LENGTH
                     );

    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}

static ANSC_STATUS
HttpSmpoUtilBuildSetCookie2
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *   cookie-av       =   "Comment"   "="     comment
     *               |       "Domain"    "="     set-cookie-domain
     *               |       "Max-Age"   "="     delta-seconds
     *               |       "Path"      "="     set-cookie-path
     *               |       "Secure"
     *               |       "Version"   "="     1*DIGIT
     *               |       "expires"   "="     cookie-expires-date
     *               |       token       "="     value
     *
     *
     */
    PHTTP_HFO_SET_COOKIE2           pSetCookie2     = (PHTTP_HFO_SET_COOKIE2)hHttpHfo;
    ULONG                           ulSize          = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount, i;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;
    UCHAR                           buf[16];
    ULONG                           bufLen;
    ULONG                           ulValue;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_SET_COOKIE2,
                         HTTP_SMPO_SET_COOKIE2_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    ulCount = pSetCookie2->CookieCount;
    for (i = 0; i < ulCount; i ++)
    {
        if (i != 0)
        {
            bCopySucc   &= HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "; ",
                                 2
                             );


            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        pCookieContent  = &pSetCookie2->CookieArray[i];

        /* name */
        pValue      = pCookieContent->Name;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* "=" */
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             buffer,
                             ulBufSize,
                             &ulStart,
                             HTTP_SMPO_ATTR_SEPARATOR,
                             HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                         );

        if (!bCopySucc)
            return ANSC_STATUS_FAILURE;

        /* value */
        pValue      = pCookieContent->Value;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment */
        pValue      = pCookieContent->Comment;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* comment url */
        pValue      = pCookieContent->CommentUrl;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_URL,
                                 HTTP_SMPO_SET_COOKIE_COMMENT_URL_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* path */
        pValue      = pCookieContent->Path;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PATH,
                                 HTTP_SMPO_SET_COOKIE_PATH_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* domain */
        pValue      = pCookieContent->Domain;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN,
                                 HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* port */
        pValue      = pCookieContent->Port;
        ulValueLen  = AnscSizeOfString(pValue);

        if (ulValueLen)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_PORT,
                                 HTTP_SMPO_SET_COOKIE_PORT_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 pValue,
                                 ulValueLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* version */
        ulValue      = pCookieContent->Version;

        if (ulValue != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_VERSION,
                                 HTTP_SMPO_SET_COOKIE_VERSION_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->Version, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 "\"",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* expires */
        if (pCookieContent->bPermanent)
        {
            UCHAR                   expires[48];
            PUCHAR                  pWkDay;
            PUCHAR                  pMonth;
            PANSC_UNIVERSAL_TIME    pTime;

            pTime   = &pCookieContent->Expires;
            pWkDay  = HttpSmpoUtilGetWeekDayName(pTime->DayOfWeek);
            pMonth  = HttpSmpoUtilGetMonthName(pTime->Month);

            _ansc_sprintf
                (
                    expires,
                    "; expires=%s, %.2d-%s-%.4d %.2d:%.2d:%.2d GMT",
                    pWkDay,
                    pTime->DayOfMonth,
                    pMonth,
                    pTime->Year,
                    pTime->Hour,
                    pTime->Minute,
                    pTime->Second
                );

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 expires,
                                 AnscSizeOfString(expires)
                             );
        }

        /* max-age */
        ulValue     = pCookieContent->MaxAgeInSeconds;

        if (pCookieContent->bPermanent && ulValueLen != HTTP_COOKIE_MAX_AGE_ABSENT)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE,
                                 HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            /* "=" */
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_ATTR_SEPARATOR,
                                 HTTP_SMPO_ATTR_SEPARATOR_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            AnscInt2String(pCookieContent->MaxAgeInSeconds, buf, 10);
            bufLen      = AnscSizeOfString(buf);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 buf,
                                 bufLen
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* secure */
        if (pCookieContent->bSecure)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_SECURE,
                                 HTTP_SMPO_SET_COOKIE_SECURE_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }

        /* discard */
        if (pCookieContent->bDiscard)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 ";",
                                 1
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 buffer,
                                 ulBufSize,
                                 &ulStart,
                                 HTTP_SMPO_SET_COOKIE_DISCARD,
                                 HTTP_SMPO_SET_COOKIE_DISCARD_LENGTH
                             );

            if (!bCopySucc)
                return ANSC_STATUS_FAILURE;
        }
    }

    return status;
}


static
BOOL
HttpSmpoUtilBuildRequestUri
    (
        PVOID                       buffer,
        ULONG                       ulDestSize,
        ULONG                       *pulBufStart,
        PHTTP_REQUEST_URI           pUri
    )
{
    /*
     *  Request-URI = "*" | absoluteURI | abs_path | authority
     */
    BOOL                            bCopySucc       = FALSE;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PUCHAR                          pBuf            = (PUCHAR)buffer;
    ULONG                           ulStart         = *pulBufStart;
    ULONG                           ulSize;
    BOOL                            bAppendPort     = FALSE;
    BOOL                            bHttps          = FALSE;

    bHttps  = ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) != 0 );

    switch (pUri->Type)
    {
    case HTTP_URI_TYPE_ASTERISK:
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             &ulStart,
                             HTTP_SMPO_REQUEST_URI_ASTERISK,
                             HTTP_SMPO_REQUEST_URI_ASTERISK_LENGTH
                         );
        break;

    case HTTP_URI_TYPE_ABSOLUTE_URI:
        /*
         *     absoluteURI = scheme ":" *( uchar | reserved )
         */

        if ( (pUri->Flag & HTTP_URI_FLAG_HTTPS) == 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 HTTP_SMPO_SCHEME,
                                 HTTP_SMPO_SCHEME_LENGTH
                             );
        }
        else
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 HTTP_SMPO_SCHEME_HTTPS,
                                 HTTP_SMPO_SCHEME_HTTPS_LENGTH
                             );
        }

        if (!bCopySucc)
            return FALSE;

        ulSize      = AnscSizeOfString(pUri->HostName);
        bCopySucc   = HttpSmpoUtilCopyString
                         (
                             pBuf,
                             ulDestSize,
                             &ulStart,
                             pUri->HostName,
                             ulSize
                         );

        /*  port number */
        if (bHttps)
        {
            bAppendPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
        }
        else
        {
            bAppendPort = (pUri->HostPort != HTTP_SERVER_PORT);
        }

        if (bAppendPort)
        {
            UCHAR                   portNum[8];

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 (PUCHAR)":",
                                 1
                             );
            if (!bCopySucc)
                return FALSE;

            _ansc_sprintf(portNum, "%d", pUri->HostPort);

            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 portNum,
                                 AnscSizeOfString(portNum)
                             );
            if (!bCopySucc)
                return FALSE;
        }

        if (pUri->PathLevel >= 2)
        {
            ULONG                   ulPathLevel, i;

            ulPathLevel     = pUri->PathLevel;
            for (i = 1; i < ulPathLevel; i ++)
            {
                ulSize      = 1;
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "/",
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;

                if (
                       AnscEqualString(pUri->PathArray[i], "/", TRUE)
                   )
                {
                    /* already counted */
                    continue;
                }

                ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[i],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        else
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 "/",
                                 1
                             );
        }
        break;

    case HTTP_URI_TYPE_ABS_PATH:
        {
            /*
             *  abs_path = "/" rel_path
             *  rel_path = [path] [";" params] ["?" query]
             *  path = fsegment *( "/" segment )
             */
            ULONG                   ulPathLevel, i;

            ulPathLevel     = pUri->PathLevel;

            if (
                   (ulPathLevel == 1 && AnscEqualString(pUri->PathArray[0], "/", TRUE)) ||
                   ulPathLevel == 0
               )
            {
                ulSize      = 1;
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "/",
                                     ulSize
                                 );
            }
            else
            {
                for (i = 1; i < ulPathLevel; i ++)
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     (
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;

                    if (
                           AnscEqualString(pUri->PathArray[i], "/", TRUE)
                       )
                    {
                        /* already counted */
                        continue;
                    }

                    ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                    bCopySucc   = HttpSmpoUtilCopyString
                                     (
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         pUri->PathArray[i],
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;
                }
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        break;

    case HTTP_URI_TYPE_RELATIVE_URI:
        {
            /*
             *  relativeURI = net_path | abs_path | rel_path
             *  net_path = "//" net_loc [abs_path]
             */
            ULONG                   i, ulPathLevel;

#if 0   /* ??? */
            ulSize      = AnscSizeOfString(pUri->HostName);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "//",
                                     2
                                 );
                if (!bCopySucc)
                    return FALSE;

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->HostName,
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }
#endif

            ulPathLevel     = pUri->PathLevel;

            if (ulPathLevel == 1)
            {
                ulSize      = AnscSizeOfString(pUri->PathArray[0]);

                if (ulSize != 1 || pUri->PathArray[0][0] != '/')
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     (
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                }

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[0],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            for (i = 1; i < ulPathLevel; i ++)
            {
                ULONG               ulPathRoot;
                BOOL                bAddSlash;

                bAddSlash   = TRUE;

                if (i == 1)
                {
                    ulPathRoot  = AnscSizeOfString(pUri->PathArray[0]);
                    bAddSlash   = (ulPathRoot != 0);
                }

                if (bAddSlash)
                {
                    ulSize      = 1;
                    bCopySucc   = HttpSmpoUtilCopyString
                                     (
                                         pBuf,
                                         ulDestSize,
                                         &ulStart,
                                         "/",
                                         ulSize
                                     );
                    if (!bCopySucc)
                        return FALSE;
                }

                ulSize      = AnscSizeOfString(pUri->PathArray[i]);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->PathArray[i],
                                     ulSize
                                 );
                if (!bCopySucc)
                    return FALSE;
            }

            ulSize  = AnscSizeOfString(pUri->QueryParams);
            if (ulSize != 0)
            {
                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     "?",
                                     1
                                 );



                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     pUri->QueryParams,
                                     ulSize
                                 );
            }
        }
        break;

    case HTTP_URI_TYPE_AUTHORITY:
        /*
         *  authority = authority-server | reg_name
         *  authority-server = [ [ userinfo "@" ] hostport ]
         *  hostport = host [ ":" port ]
         *  reg_name = 1*( uri-unreserved | escape | "$" | "," |
         *                 ";" | ":" | "@" | "&" | "=" | "+" )
         */
        ulSize  = AnscSizeOfString(pUri->HostName);
        if (ulSize != 0)
        {
            bCopySucc   = HttpSmpoUtilCopyString
                             (
                                 pBuf,
                                 ulDestSize,
                                 &ulStart,
                                 pUri->HostName,
                                 ulSize
                             );

            if (!bCopySucc)
                return FALSE;

            if (bHttps)
            {
                bAppendPort = (pUri->HostPort != HTTP_SERVER_TLS_PORT);
            }
            else
            {
                bAppendPort = (pUri->HostPort != HTTP_SERVER_PORT);
            }

            if (bAppendPort)
            {
                UCHAR               buf[8];

                AnscInt2String(pUri->HostPort, buf, 10);

                ulSize  = AnscSizeOfString(buf);

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     ":",
                                     1
                                 );
                if (!bCopySucc)
                    return FALSE;

                bCopySucc   = HttpSmpoUtilCopyString
                                 (
                                     pBuf,
                                     ulDestSize,
                                     &ulStart,
                                     buf,
                                     ulSize
                                 );

                if (!bCopySucc)
                    return FALSE;
            }
        }
        break;
    }

    *pulBufStart    = ulStart;

    return bCopySucc;
}


static ANSC_STATUS
HttpSmpoUtilBuildLocation
    (
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulBufSize
    )
{
    /*
     *  Location = "Location" ":" absoluteURI
     */
    PHTTP_HFO_LOCATION              pLocation   = (PHTTP_HFO_LOCATION)hHttpHfo;
    ULONG                           ulSize      = 0;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;

    bCopySucc   = HttpSmpoUtilPrebuildHeader
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         HTTP_SMPO_LOCATION,
                         HTTP_SMPO_LOCATION_LENGTH
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildRequestUri
                     (
                         buffer,
                         ulBufSize,
                         &ulStart,
                         &pLocation->PreferredUri
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscHttpBuildHeader
            (
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP header specified
        by hHttpHfo and save it into specified buffer. If the buffer
        size is not big enough, the constructed HTTP header will be
        trunked.

    argument:
                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP header
                in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP header.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
AnscHttpBuildHeader
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     * This function is responsible to call corresponding
     * "GetSize" function to a specific header which is
     * identified by the header id.
     */
    PHTTP_HEADER_FIELD              pHeader;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    BOOL                            bCopySucc   = TRUE;
    ULONG                           i, ulStart     = 0;

    pHeader = (PHTTP_HEADER_FIELD)hHttpHfo;

    if (!pHeader)
        return 0;

    if (pHeader->Flags & HTTP_FIELD_FLAG_LINE_PRESENT)
    {
        if (buffer && buffer != pHeader->HeaderLine)
        {
            /* this header is still valid, so return the size of HeaderLine */
            ULONG                   ulHeaderLineSize;

            ulHeaderLineSize    = AnscSizeOfString(pHeader->HeaderLine);
            bCopySucc           = HttpSmpoUtilCopyString
                                     (
                                         buffer,
                                         ulSize,
                                         &ulStart,
                                         pHeader->HeaderLine,
                                         ulHeaderLineSize
                                     );

            if (!bCopySucc)
                return ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        BOOL                        bConstructHeaderLine    = FALSE;

        if (!buffer)
        {
            buffer  = pHeader->HeaderLine;
            ulSize  = HTTP_HEADER_LINE_SIZE;
            /* construct header line */
            bConstructHeaderLine    = TRUE;
            /*
             * zero out header line, so we don't need to care
             * about where it ends
             */
            AnscZeroMemory(buffer, ulSize);
        }

        switch (pHeader->HeaderId)
        {
        case HTTP_HEADER_ID_AUTHORIZATION:
        	status = HttpSmpoUtilBuildAuthorization(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_CONNECTION:
        	status = HttpSmpoUtilBuildConnection(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_CONTENT_ENCODING:
        	status = HttpSmpoUtilBuildContentEncoding(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_CONTENT_LENGTH:
        	status = HttpSmpoUtilBuildContentLength(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_CONTENT_TYPE:
        	status = HttpSmpoUtilBuildContentType(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_HOST:
        	status = HttpSmpoUtilBuildHost(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_LOCATION:
        	status = HttpSmpoUtilBuildLocation(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_TRANSFER_ENCODING:
        	status = HttpSmpoUtilBuildTransferEncoding(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_WWW_AUTHENTICATE:
        	status = HttpSmpoUtilBuildWWWAuthenticate(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_PROXY_CONNECTION:
        	status = HttpSmpoUtilBuildProxyConnection(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_COOKIE:
        	status = HttpSmpoUtilBuildCookie(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_SET_COOKIE:
        	status = HttpSmpoUtilBuildSetCookie(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_COOKIE2:
        	status = HttpSmpoUtilBuildCookie2(hHttpHfo, buffer, ulSize);
            break;

        case HTTP_HEADER_ID_SET_COOKIE2:
            status = HttpSmpoUtilBuildSetCookie2(hHttpHfo, buffer, ulSize);
            break;

        default:
            return ANSC_STATUS_FAILURE;

        }

        if (status == ANSC_STATUS_SUCCESS && ulSize != 0)
        {
            if (bConstructHeaderLine)
            {
                /*
                pHeader->HeaderLine[ulSize - 1] = 0;
                */
                pHeader->Flags  = HTTP_FIELD_FLAG_LINE_PRESENT;
            }
            else
            {
                ULONG               ulCopySize;
                BOOL                bTrunked    = FALSE;
                PUCHAR              pCurHeaderLine  = pHeader->HeaderLine;
                ULONG               ulCurHeaderLine;
                PUCHAR              pNewHeaderLine  = (PUCHAR)buffer;

                if (pCurHeaderLine)
                {
                    ulCurHeaderLine = AnscSizeOfString(pCurHeaderLine);
                    if (ulCurHeaderLine != 0)
                    {
                        if (ulCurHeaderLine != ulSize)
                        {
                            int     a = 0;
                        }
                    }
                }

                ulCopySize  = ulSize;
                if (ulCopySize >= HTTP_HEADER_LINE_SIZE)
                {
                    ulCopySize  = HTTP_HEADER_LINE_SIZE - 1;
                    bTrunked    = TRUE;
                }

                /*
                 * if the message is complete, we also put them
                 * into header line so that we don't need to
                 * re-construct this header later on.
                 */
                if (!bTrunked)
                {
                    AnscCopyMemory(pHeader->HeaderLine, buffer, ulCopySize);
                    pHeader->HeaderLine[ulCopySize] = 0;
                    pHeader->Flags  = HTTP_FIELD_FLAG_LINE_PRESENT;
                }
            }
        }
        else
        {
        	AnscTrace("AnscHttpBuildHeader - build header failed!\n");
            for (i = 0; i < ulSize; i ++)
            {
                AnscTrace("    %c", ((char *)buffer) + i);
            }

            AnscTrace("\n\n");
        }
    }

    return status;
}

static
BOOL
HttpSmpoUtilBuildMethod
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       ulMethod,
        PUCHAR                      pMethodName
    )
{
    ULONG                           ulLen, ulBufLeft;
    BOOL                            bCopySucc   = TRUE;
    PUCHAR                          pSrc        = NULL;
    ULONG                           ulStart     = *pulStart;

    ulLen = 0;

    switch (ulMethod)
    {
    case HTTP_METHOD_OPTIONS:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS);
        pSrc    = HTTP_METHOD_NAME_OPTIONS;
        break;

    case HTTP_METHOD_GET:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_GET);
        pSrc    = HTTP_METHOD_NAME_GET;
        break;

    case HTTP_METHOD_HEAD:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_HEAD);
        pSrc    = HTTP_METHOD_NAME_HEAD;
        break;

    case HTTP_METHOD_POST:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_POST);
        pSrc    = HTTP_METHOD_NAME_POST;
        break;

    case HTTP_METHOD_PUT:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_PUT);
        pSrc    = HTTP_METHOD_NAME_PUT;
        break;

    case HTTP_METHOD_DELETE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_DELETE);
        pSrc    = HTTP_METHOD_NAME_DELETE;
        break;

    case HTTP_METHOD_TRACE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_TRACE);
        pSrc    = HTTP_METHOD_NAME_TRACE;
        break;

    case HTTP_METHOD_CONNECT:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_CONNECT);
        pSrc    = HTTP_METHOD_NAME_CONNECT;
        break;

    case HTTP_METHOD_NOTIFY:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY);
        pSrc    = HTTP_METHOD_NAME_NOTIFY;
        break;

    case HTTP_METHOD_SEARCH:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_SEARCH);
        pSrc    = HTTP_METHOD_NAME_SEARCH;
        break;

    case HTTP_METHOD_M_SEARCH:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH);
        pSrc    = HTTP_METHOD_NAME_M_SEARCH;
        break;

    case HTTP_METHOD_M_POST:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_M_POST);
        pSrc    = HTTP_METHOD_NAME_M_POST;
        break;

    case HTTP_METHOD_SUBSCRIBE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE);
        pSrc    = HTTP_METHOD_NAME_SUBSCRIBE;
        break;

    case HTTP_METHOD_UNSUBSCRIBE:
        ulLen   = AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE);
        pSrc    = HTTP_METHOD_NAME_UNSUBSCRIBE;
        break;

    default:
        ulLen   = pMethodName ? AnscSizeOfString(pMethodName) : 0;
        pSrc    = pMethodName;
        break;
    }

    if (ulLen != 0)
    {
        ulBufLeft   = ulDestSize - ulStart;

        if (ulLen > ulBufLeft)
        {
            bCopySucc   = FALSE;
            ulLen       = ulBufLeft;
        }

        AnscCopyMemory(&pBuf[ulStart], pSrc, ulLen);

        ulStart += ulLen;
    }

    *pulStart   = ulStart;

    return bCopySucc;
}

static
BOOL
HttpSmpoUtilBuildHttpVersion
    (
        PUCHAR                      pBuf,
        ULONG                       ulDestSize,
        ULONG                       *pulStart,
        ULONG                       MajorVersion,
        ULONG                       MinorVersion
    )
{
    /*
     *  HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     */
    UCHAR                           buf[80];
    ULONG                           ulSize;
    BOOL                            bCopySucc;
    ULONG                           ulStart = *pulStart;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         HTTP_SMPO_NAME,
                         HTTP_SMPO_NAME_LENGTH
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         "/",
                         1
                     );
    if (!bCopySucc)
        return FALSE;

    AnscInt2String(MajorVersion, buf, 10);
    ulSize  = AnscSizeOfString(buf);           /* major version number length */

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         buf,
                         ulSize
                     );
    if (!bCopySucc)
        return FALSE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         ".",
                         1
                     );

    if (!bCopySucc)
        return FALSE;

    AnscInt2String(MinorVersion, buf, 10);
    ulSize  = AnscSizeOfString(buf);           /* minor version number length */

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         pBuf,
                         ulDestSize,
                         &ulStart,
                         buf,
                         ulSize
                     );

    if (!bCopySucc)
        return FALSE;

    *pulStart   = ulStart;

    return bCopySucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscHttpBuildRequestLine
            (
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP request line
        specified by hHttpHfo and save it into specified buffer.
        If the buffer size is not big enough, the constructed HTTP
        request line will be trunked.

    argument:
                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP request
                line in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP
                request line.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
AnscHttpBuildRequestLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     *  Request-Line = Method SP Request-URI SP HTTP-Version
     */
    PHTTP_REQUEST_INFO              pRequestInfo    = (PHTTP_REQUEST_INFO)hHttpHfo;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           ulStart         = 0;
    BOOL                            bCopySucc;

    if (!hHttpHfo)
        return ANSC_STATUS_FAILURE;

    /* method */
    bCopySucc   = HttpSmpoUtilBuildMethod(buffer, ulSize, &ulStart, pRequestInfo->Method, pRequestInfo->MethodName);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* space */
    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* request uri */
    bCopySucc   = HttpSmpoUtilBuildRequestUri(buffer, ulSize, &ulStart, &pRequestInfo->RequestUri);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* space */
    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    /* version */
    bCopySucc   = HttpSmpoUtilBuildHttpVersion(buffer, ulSize, &ulStart, pRequestInfo->MajorVersion, pRequestInfo->MinorVersion);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscHttpBuildStatusLine
            (
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP status line
        specified by hHttpHfo and save it into specified buffer.
        If the buffer size is not big enough, the constructed HTTP
        status line will be trunked.

    argument:
                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP status
                line in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP
                status line.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
AnscHttpBuildStatusLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    /*
     *  Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase
     */
    PHTTP_RESPONSE_INFO             pResponseInfo   = (PHTTP_RESPONSE_INFO)hHttpHfo;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    BOOL                            bCopySucc;
    ULONG                           ulStart         = 0;

    if (!hHttpHfo)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildHttpVersion
                     (
                         buffer,
                         ulSize,
                         &ulStart,
                         pResponseInfo->MajorVersion,
                         pResponseInfo->MinorVersion
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilBuildUlong(buffer, ulSize, &ulStart, pResponseInfo->StatusCode);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString(buffer, ulSize, &ulStart, " ", 1);
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    bCopySucc   = HttpSmpoUtilCopyString
                     (
                         buffer,
                         ulSize,
                         &ulStart,
                         pResponseInfo->ReasonPhrase,
                         AnscSizeOfString(pResponseInfo->ReasonPhrase)
                     );
    if (!bCopySucc)
        return ANSC_STATUS_FAILURE;

    return status;
}



/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoRegisterHeaderXInfo2
            (
                PUCHAR                      pName,
                ULONG                       ulHeaderId,
                PFN_SMPO_PARSE_HDR          xFunc
            )

    description:

        This function is called to register a header parsing function
        to a local hash table.

    argument:
                PUCHAR                      pName
                HTTP Header name.

                ULONG                       ulHeaderId
                The ID of the HTTP header.

                PFN_SMPO_PARSE_HDR          xFunc
                The parsing function for the header.

    return:     status of operation.

**********************************************************************/

static ANSC_STATUS
HttpSmpoRegisterHeaderXInfo2
    (
        PUCHAR                      pName,
        ULONG                       ulHeaderId,
        PFN_SMPO_PARSE_HDR1          xFunc
    )
{
    PHTTP_SMPO_HEADER_XINFO1         pXinfo      = NULL;
    ULONG                           ulHashIndex = 0;

    pXinfo  = (PHTTP_SMPO_HEADER_XINFO1)AnscAllocateMemory(sizeof(HTTP_SMPO_HEADER_XINFO1));

    if (!pXinfo)
    {
    	AnscTrace("HttpSmpoRegisterHeaderXInfo2 - can't allocate header xinfo object!\n");
        return ANSC_STATUS_RESOURCES;
    }

    AnscCopyString(pXinfo->Name, pName);
    pXinfo->xFunc       = xFunc;
    pXinfo->HeaderId    = ulHeaderId;

    ulHashIndex = AnscHashString(pName, AnscSizeOfString(pName), HTTP_SMPO_HEADER_TABLE_SIZE);

    AnscSListPushEntry(&HeaderXList[ulHashIndex], &pXinfo->Linkage);

     return ANSC_STATUS_SUCCESS;
}

static BOOL
HttpSmpoUtilParseDigestCredentials
    (
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* auth-scheme = token */
    /* auth-param = token "=" quoted-string */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PHTTP_CREDENTIAL_DIGEST         pDigest         = &pCredentials->Credential.Digest;

    HttpSmpoUtilCopyHeaderString(pBuf, ulSize, pDigest->UserName, HTTP_MAX_USERNAME_SIZE);

    return TRUE;
}

static BOOL
HttpSmpoUtilParseBasicCredentials
    (
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* basic-credentials = "Basic" SP basic-cookie */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PHTTP_CREDENTIAL_BASIC          pBasicCookie    = &pCredentials->Credential.Basic;
    PUCHAR                          pToken          = pBuf;
    BOOL                            bSucc           = TRUE;
    PUCHAR                          pDecodedString;
    PUCHAR                          pString         = NULL;
    ULONG                           ulDecodedStringLen;

    pString = (PUCHAR)AnscAllocateMemory(ulSize + 1);

    if (!pString)
    {
        return FALSE;
    }

    AnscCopyMemory(pString, pBuf, ulSize);
    pString[ulSize] = 0;

    pDecodedString  = AnscBase64Decode((PUCHAR)pString, &ulDecodedStringLen);

    if (pDecodedString)
    {
        PUCHAR                      pPos;
        ULONG                       ulCopySize;

        /* parse the decoded string to get user & password */
        pPos = _ansc_memchr(pDecodedString, HTTP_SMPO_CHAR_COLON, ulDecodedStringLen);

        if (!pPos)
        {
            ulCopySize  = ulDecodedStringLen;
            if (ulCopySize >= HTTP_MAX_USERNAME_SIZE)
                ulCopySize  = HTTP_MAX_USERNAME_SIZE - 1;

            AnscCopyMemory(pBasicCookie->UserName, pDecodedString, ulCopySize);
            pBasicCookie->UserName[ulCopySize]  = 0;
        }
        else
        {
            PUCHAR                  pUser, pPass;
            ULONG                   ulUser, ulPass;

            pPass   = pPos + 1;
            *pPos   = 0;
            pUser   = pDecodedString;

            ulUser  = AnscSizeOfString(pUser);
            ulPass  = ulDecodedStringLen - ulUser - 1;

            ulCopySize  = ulUser;
            if (ulCopySize >= HTTP_MAX_USERNAME_SIZE)
                ulCopySize  = HTTP_MAX_USERNAME_SIZE - 1;

            AnscCopyMemory(pBasicCookie->UserName, pUser, ulCopySize);
            pBasicCookie->UserName[ulCopySize]  = 0;

            ulCopySize  = ulPass;
            if (ulCopySize >= HTTP_MAX_PASSWORD_SIZE)
                ulCopySize  = HTTP_MAX_PASSWORD_SIZE - 1;

            AnscCopyMemory(pBasicCookie->Password, pPass, ulCopySize);
            pBasicCookie->Password[ulCopySize]  = 0;
        }

        AnscFreeMemory(pDecodedString);
    }

    AnscFreeMemory(pString);

    return TRUE;
}


static BOOL
HttpSmpoUtilParseCredentials
    (
        ANSC_HANDLE                 hCredentials,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* credentials = basic-credentials | auth-scheme #auth-param */
    /* basic-credentials = "Basic" SP basic-cookie */
    /* auth-scheme = token */
    /* auth-param = token "=" quoted-string */

    PHTTP_AUTH_CREDENTIAL           pCredentials    = (PHTTP_AUTH_CREDENTIAL)hCredentials;
    PUCHAR                          pToken          = pBuf;
    PUCHAR                          pLast           = pBuf + ulSize - 1;
    PUCHAR                          pNext;
    ULONG                           ulTokenSize;
    BOOL                            bSucc           = TRUE;

    /* scheme */
    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SPACE, pLast - pToken + 1);

    if (!pNext)
    {
        return FALSE;
    }

    ulTokenSize = pNext - pToken;

    pNext ++;

    if (AnscSizeOfString(HTTP_AUTH_TEXT_NONE) == ulTokenSize &&
        AnscEqualString2(pToken, HTTP_AUTH_TEXT_NONE, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_NONE;
    }
    else
    if (AnscSizeOfString(HTTP_AUTH_TEXT_BASIC) == ulTokenSize &&
        AnscEqualString2(pToken, HTTP_AUTH_TEXT_BASIC, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_BASIC;

        HttpSmpoUtilParseBasicCredentials(hCredentials, pNext, pLast - pNext + 1);
    }
    else
    if (AnscSizeOfString(HTTP_AUTH_TEXT_DIGEST) == ulTokenSize &&
        AnscEqualString2(pToken, HTTP_AUTH_TEXT_DIGEST, ulTokenSize, FALSE))
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_DIGEST;

        HttpSmpoUtilParseDigestCredentials(hCredentials, pNext, pLast - pNext + 1);
    }
    else
    {
        pCredentials->AuthType  = HTTP_AUTH_TYPE_RESERVED;
    }

    return bSucc;
}


static ANSC_HANDLE
HttpSmpoParseAuthorization
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Authorization = "Authorization" ":" credentials */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_AUTHORIZATION         pHfoAuthorization   = NULL;
    PHTTP_AUTH_CREDENTIAL           pCredentials;

    pHfoAuthorization  = (PHTTP_HFO_AUTHORIZATION)AnscAllocateMemory(sizeof(HTTP_HFO_AUTHORIZATION));

    if (pHfoAuthorization)
    {
        pCredentials = &pHfoAuthorization->Credential;

        HttpSmpoUtilParseCredentials((ANSC_HANDLE)pCredentials, pMsg, ulSize);
    }

    return (ANSC_HANDLE)pHfoAuthorization;
}

static ANSC_HANDLE
HttpSmpoParseConnection
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pMsg            = (PUCHAR)pBuf;
    PHTTP_HFO_CONNECTION            pHfoConnection  = (PHTTP_HFO_CONNECTION)NULL;
    PUCHAR                          pLast           = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    ULONG                           ulTokenIndex    = 0;
    PHTTP_CONNECTION_TOKEN          pConnToken;
    ULONG                           ulTokenType;

    pHfoConnection = (PHTTP_HFO_CONNECTION)AnscAllocateMemory(sizeof(HTTP_HFO_CONNECTION));

    if (pHfoConnection)
    {
        pHfoConnection->TokenCount = 0;

        /* parse connection tokens */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoConnection->TokenCount >= HTTP_MAX_CONNECTION_TOKEN_NUMBER)
            {
                break;
            }

            pConnToken  = &pHfoConnection->TokenArray[pHfoConnection->TokenCount ++];

            HttpSmpoUtilCopyHeaderString
                (
                    pToken,
                    ulTokenSize,
                    pConnToken->Token,
                    HTTP_MAX_CONNECTION_TOKEN_SIZE
                );

            ulTokenType = IANA_CONNECTION_CODE_RESERVED;

            if (AnscEqualString2(pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_CLOSE;
            }
            else
            if (AnscEqualString2(pToken, IANA_CONNECTION_TEXT_KEEP_ALIVE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_KEEP_ALIVE;
            }

            pConnToken->OptionType  = ulTokenType;

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoConnection;
}

static ULONG
HttpSmpoUtilGetEncodingType
    (
        PUCHAR                      pEncType,
        ULONG                       ulSize
    )
{
    ULONG                           ulType  = IANA_CCODING_CODE_RESERVED;

    if (AnscSizeOfString(IANA_CCODING_TEXT_GZIP) == ulSize &&
        AnscEqualString2(pEncType, IANA_CCODING_TEXT_GZIP, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_GZIP;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_COMPRESS) == ulSize &&
        AnscEqualString2(pEncType, IANA_CCODING_TEXT_COMPRESS, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_COMPRESS;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_DEFLATE) == ulSize &&
        AnscEqualString2(pEncType, IANA_CCODING_TEXT_DEFLATE, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_DEFLATE;
    else
    if (AnscSizeOfString(IANA_CCODING_TEXT_IDENTITY) == ulSize &&
        AnscEqualString2(pEncType, IANA_CCODING_TEXT_IDENTITY, ulSize, FALSE))
        ulType = IANA_CCODING_CODE_IDENTITY;

    return ulType;
}

static ULONG
HttpSmpoUtilGetQuality
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulInt, ulFrac;

    ulInt   = ulFrac    = 0;

    pToken  = pBuf;
    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_DOT, ulSize);

    ulInt   = _ansc_atoi(pToken);

    if (pNext)
    {
        ulFrac  = _ansc_atoi(pNext + 1);

        if (ulFrac <= 9)
        {
            ulFrac  *= 100;
        }
        else
        if (ulFrac <= 99)
        {
            ulFrac  *= 10;
        }
    }

    return (ulInt * 1000 + ulFrac);
}


static ANSC_HANDLE
HttpSmpoParseContentEncoding
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Content-Encoding = "Content-Encoding" ":" 1#content-coding */
    /* content-coding = token  */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_CONTENT_ENCODING      pHfoContentEncoding = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PUCHAR                          pQuality;
    ULONG                           ulQualitySize;
    ULONG                           ulQuality;
    ULONG                           ulLen;
    PHTTP_ENCODING                  pEncoding;

    pHfoContentEncoding  = (PHTTP_HFO_CONTENT_ENCODING)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_ENCODING));

    if (pHfoContentEncoding)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pQuality    = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, ulTokenSize);

            if (pQuality)
            {
                ulLen   = pQuality - pToken;
            }
            else
            {
                ulLen   = ulTokenSize;
            }

            if (pHfoContentEncoding->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding    = &pHfoContentEncoding->EncodingArray[pHfoContentEncoding->EncodingCount ++];

            pEncoding->Type  = HttpSmpoUtilGetEncodingType(pToken, ulLen);

            pQuality    = _ansc_strstr(pToken, HTTP_SMPO_STRING_QUALITY);
            ulQuality   = HTTP_ENCODING_QUALITY_BEST;
            if (pQuality >= pToken + ulTokenSize)
            {
                pQuality    = NULL;
            }

            if (pQuality)
            {
                pQuality        += AnscSizeOfString(HTTP_SMPO_STRING_QUALITY);

                if (pNext)
                {
                    ulQualitySize   = pNext - pQuality;
                }
                else
                {
                    ulQualitySize   = pLast - pQuality;
                }

                ulQuality       = HttpSmpoUtilGetQuality(pQuality, ulQualitySize);
            }

            pEncoding->Quality   = ulQuality;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoContentEncoding;
}

static ANSC_HANDLE
HttpSmpoParseContentLength
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Content-Length = "Content-Length" ":" 1*DIGIT */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_CONTENT_LENGTH        pHfoContentLength   = NULL;

    pHfoContentLength  = (PHTTP_HFO_CONTENT_LENGTH)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LENGTH));

    if (pHfoContentLength)
    {
        pHfoContentLength->BodySize = _ansc_atoi(pBuf);
    }

    return (ANSC_HANDLE)pHfoContentLength;
}

static ULONG
HttpSmpoUtilGetMediaType
    (
        PUCHAR                      pType,
        ULONG                       ulTypeSize
    )
{
    ULONG                       ulMediaType;

    ulMediaType = IANA_MEDIA_TYPE_CODE_RESERVED;

    if (ulTypeSize == 1 && *pType == HTTP_SMPO_CHAR_ASTERISK)
    {
        return IANA_MEDIA_TYPE_CODE_STAR;
    }

    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_TEXT) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_TEXT, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_TEXT;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MULTIPART) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_MULTIPART, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MULTIPART;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MESSAGE) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_MESSAGE, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MESSAGE;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_APPLICATION) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_APPLICATION, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_APPLICATION;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_IMAGE) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_IMAGE, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_IMAGE;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_AUDIO) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_AUDIO, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_AUDIO;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_VIDEO) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_VIDEO, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_VIDEO;
    else
    if (AnscSizeOfString(IANA_MEDIA_TYPE_TEXT_MODEL) == ulTypeSize &&
        AnscEqualString2(pType, IANA_MEDIA_TYPE_TEXT_MODEL, ulTypeSize, FALSE))
        ulMediaType = IANA_MEDIA_TYPE_CODE_MODEL;

    return ulMediaType;
}


static ULONG
AnscGetIanaTypeByName
    (
        PANSC_UTILBOX_IANA_MEDIA    pSet,
        ULONG                       ulCount,
        PUCHAR                      pName,
        ULONG                       ulNameLen
    )
{
    ULONG                           Type    = 0;
    ULONG                           i;
    PANSC_UTILBOX_IANA_MEDIA        pInfo;

    if (!pName || ulNameLen == 0)
    {
        return 0;
    }

    for (i = 0; i < ulCount; i ++)
    {
        pInfo   = (PANSC_UTILBOX_IANA_MEDIA)&pSet[i];

        if ( pInfo->Name == NULL ||
                (ulNameLen == AnscSizeOfString(pInfo->Name) &&
                AnscEqualString2(pName, pInfo->Name, ulNameLen, FALSE)) )
        {
            return pInfo->Type;
        }
    }

    return 0;
}


static ULONG
AnscGetIanaMediaSubtypeByName2
    (
        ULONG                       ulMediaType,
        PUCHAR                      pMediaSubtype,
        ULONG                       ulLen
    )
{
    ULONG                           ulCount, i;

    for (i = 0; i < sizeof(g_IanaMediaSubtypeLists)/sizeof(g_IanaMediaSubtypeLists[0]); i ++)
    {
        if (ulMediaType == g_IanaMediaSubtypeLists[i].MediaType)
        {
            ulCount = g_IanaMediaSubtypeLists[i].Count;

            return
                AnscGetIanaTypeByName
                    (
                        (PANSC_UTILBOX_IANA_MEDIA)g_IanaMediaSubtypeLists[i].pSubtypes,
                        ulCount,
                        pMediaSubtype,
                        ulLen
                    );
        }
    }

    return 0;
}


static ULONG
HttpSmpoUtilGetMediaSubType
    (
        ULONG                       ulMediaType,
        PUCHAR                      pSubType,
        ULONG                       ulSubTypeSize
    )
{

    return
        AnscGetIanaMediaSubtypeByName2
            (
                ulMediaType,
                pSubType,
                ulSubTypeSize
            );
}


static ANSC_HANDLE
HttpSmpoParseContentType
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Content-Type = "Content-Type" ":" media-type */
    /* media-type = type "/" subtype *( ";" parameter ) */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_CONTENT_TYPE          pHfoContentType     = NULL;
    PUCHAR                          pType               = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pParams;
    PUCHAR                          pSubType;
    ULONG                           ulLen;

    pParams = _ansc_memchr(pType, HTTP_SMPO_CHAR_SEMICOLON, ulSize);

    pSubType    = _ansc_memchr(pType, HTTP_SMPO_CHAR_SLASH, ulSize);

    if (!pSubType)
    {
        return (ANSC_HANDLE)NULL;
    }

    pHfoContentType  = (PHTTP_HFO_CONTENT_TYPE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_TYPE));

    if (pHfoContentType)
    {
        pHfoContentType->MediaType = HttpSmpoUtilGetMediaType(pType, pSubType - pType);

        pSubType ++;
        if (pParams)
        {
            ulLen   = pParams - pSubType;
        }
        else
        {
            ulLen   = pLast - pSubType + 1;
        }

        pHfoContentType->SubType    =
            HttpSmpoUtilGetMediaSubType
                (
                    pHfoContentType->MediaType,
                    pSubType,
                    ulLen
                );

        if (pParams)
        {
            pParams ++;
            pParams = HttpSmpoUtilLinearWhiteSpace(pParams, pLast - pParams + 1);
            HttpSmpoUtilCopyHeaderString
                (
                    pParams,
                    pLast - pParams + 1,
                    pHfoContentType->Parameters,
                    HTTP_MAX_HEADER_PARAM_SIZE
               );
        }
    }

    return (ANSC_HANDLE)pHfoContentType;
}

static ANSC_HANDLE
HttpSmpoParseHost
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Host = "Host" ":" host [":" port] */

    PUCHAR                          pMsg        = (PUCHAR)pBuf;
    PHTTP_HFO_HOST                  pHfoHost    = NULL;
    BOOL                            bSucc       = TRUE;
    PUCHAR                          pToken      = pMsg;
    PUCHAR                          pLast       = pMsg + ulSize - 1;
    PUCHAR                          pPort;
    ULONG                           ulTokenSize;

    pHfoHost    = (PHTTP_HFO_HOST)AnscAllocateMemory(sizeof(HTTP_HFO_HOST));

    if (pHfoHost)
    {
        pPort   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, ulSize);

        if (!pPort)
        {
            ulTokenSize = ulSize;
        }
        else
        {
            ulTokenSize = pPort - pToken;
        }

        /* host name */
        HttpSmpoUtilCopyHeaderString(pToken, ulTokenSize, pHfoHost->HostName, ANSC_DOMAIN_NAME_SIZE);

        /* port number */
        if (!pPort)
        {
            pHfoHost->HostPort  = HTTP_SERVER_PORT;
        }
        else
        {
            pPort ++;
            pHfoHost->HostPort  = _ansc_atoi(pPort);
        }
    }

    return (ANSC_HANDLE)pHfoHost;
}

static ULONG
HttpSmpoUtilGetTransferEncoding
    (
        PUCHAR                      pTEncoding,
        ULONG                       ulSize
    )
{
    ULONG                           ulEncoding  = IANA_TCODING_CODE_RESERVED;

    if (AnscSizeOfString(IANA_TCODING_TEXT_GZIP) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_GZIP, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_GZIP;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_COMPRESS) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_COMPRESS, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_COMPRESS;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_DEFLATE) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_DEFLATE, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_DEFLATE;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_IDENTITY) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_IDENTITY, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_IDENTITY;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_CHUNKED) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_CHUNKED, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_CHUNKED;
    else
    if (AnscSizeOfString(IANA_TCODING_TEXT_TRAILERS) == ulSize &&
        AnscEqualString2(pTEncoding, IANA_TCODING_TEXT_TRAILERS, ulSize, FALSE))
        ulEncoding = IANA_TCODING_CODE_TRAILERS;

    return ulEncoding;
}


static ANSC_HANDLE
HttpSmpoParseTransferEncoding
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_TRANSFER_ENCODING     pHfoTransferEncoding= NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_ENCODING                  pEncoding;

    pHfoTransferEncoding  = (PHTTP_HFO_TRANSFER_ENCODING)AnscAllocateMemory(sizeof(HTTP_HFO_TRANSFER_ENCODING));

    if (pHfoTransferEncoding)
    {
        pToken  = pBuf;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoTransferEncoding->EncodingCount >= HTTP_MAX_ENCODING_NUMBER)
            {
                break;
            }

            pEncoding           = &pHfoTransferEncoding->EncodingArray[pHfoTransferEncoding->EncodingCount ++];
            pEncoding->Type     = HttpSmpoUtilGetTransferEncoding(pToken, ulTokenSize);
            pEncoding->Quality  = HTTP_ENCODING_QUALITY_BEST;

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoTransferEncoding;
}

static BOOL
HttpSmpoUtilParseChallenge
    (
        ANSC_HANDLE                 hChallenge,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* challenge = auth-scheme 1*SP realm *( "," auth-param ) */

    PHTTP_AUTH_CHALLENGE            pChallenge  = (PHTTP_AUTH_CHALLENGE)hChallenge;
    PUCHAR                          pScheme     = pBuf;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    PUCHAR                          pRealm;
    ULONG                           ulLen;

    pRealm  = _ansc_memchr(pScheme, HTTP_SMPO_CHAR_SPACE, pLast - pScheme + 1);

    if (!pRealm)
    {
        return FALSE;
    }

    ulLen   = pRealm - pScheme;

    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_NONE) &&
        AnscEqualString2(HTTP_AUTH_TEXT_NONE, pScheme, ulLen, FALSE))
    {
        pChallenge->AuthType    = HTTP_AUTH_TYPE_NONE;
    }
    else
    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_BASIC) &&
        AnscEqualString2(HTTP_AUTH_TEXT_BASIC, pScheme, ulLen, FALSE))
    {
        PHTTP_CHALLENGE_BASIC       pBasicChallenge = &pChallenge->Challenge.Basic;

        pChallenge->AuthType    = HTTP_AUTH_TYPE_BASIC;

        pRealm  = HttpSmpoUtilLinearWhiteSpace(pRealm, pLast - pRealm + 1);
        HttpSmpoUtilCopyHeaderString(pRealm, pLast - pRealm + 1, pBasicChallenge->Realm, HTTP_MAX_REALM_SIZE);
    }
    else
    if (ulLen == AnscSizeOfString(HTTP_AUTH_TEXT_DIGEST) &&
        AnscEqualString2(HTTP_AUTH_TEXT_DIGEST, pScheme, ulLen, FALSE))
    {
        PHTTP_CHALLENGE_DIGEST      pDigestChallenge = &pChallenge->Challenge.Digest;

        pChallenge->AuthType    = HTTP_AUTH_TYPE_DIGEST;

        pRealm  = HttpSmpoUtilLinearWhiteSpace(pRealm, pLast - pRealm + 1);
        HttpSmpoUtilCopyHeaderString(pRealm, pLast - pRealm + 1, pDigestChallenge->Realm, HTTP_MAX_REALM_SIZE);
    }
    else
    {
        pChallenge->AuthType    = HTTP_AUTH_TYPE_RESERVED;
    }

    return TRUE;
}


static ANSC_HANDLE
HttpSmpoParseWwwAuthenticate
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* WWW-Authenticate = "WWW-Authenticate" ":" 1#challenge */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PHTTP_HFO_WWW_AUTHENTICATE      pHfoWwwAuth         = NULL;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    PHTTP_AUTH_CHALLENGE            pChallenge;
    BOOL                            bSucc;

    pHfoWwwAuth  = (PHTTP_HFO_WWW_AUTHENTICATE)AnscAllocateMemory(sizeof(HTTP_HFO_WWW_AUTHENTICATE));

    if (pHfoWwwAuth)
    {
        pToken  = pBuf;

        while (pToken && pToken <= pLast)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
            /* pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1); */
            pNext   = NULL; /* always assume one challenge in each header field */

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoWwwAuth->ChallengeCount >= HTTP_MAX_CHALLENGE_NUMBER)
            {
                break;
            }

            pChallenge  = &pHfoWwwAuth->ChallengeArray[pHfoWwwAuth->ChallengeCount ++];

            bSucc   = HttpSmpoUtilParseChallenge((ANSC_HANDLE)pChallenge, pToken, ulTokenSize);
            if (!bSucc)
            {
                pHfoWwwAuth->ChallengeCount --;
            }

            if (pNext)
            {
                pToken = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoWwwAuth;
}

static ANSC_HANDLE
HttpSmpoParseProxyConnection
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_PROXY_CONNECTION      pHfoProxyConnection = (PHTTP_HFO_PROXY_CONNECTION)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    ULONG                           ulTokenIndex        = 0;
    PHTTP_CONNECTION_TOKEN          pConnToken;
    ULONG                           ulTokenType;

    pHfoProxyConnection = (PHTTP_HFO_PROXY_CONNECTION)AnscAllocateMemory(sizeof(HTTP_HFO_PROXY_CONNECTION));

    if (pHfoProxyConnection)
    {
        pHfoProxyConnection->TokenCount = 0;

        /* parse connection tokens */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if (pHfoProxyConnection->TokenCount >= HTTP_MAX_CONNECTION_TOKEN_NUMBER)
            {
                break;
            }

            pConnToken  = &pHfoProxyConnection->TokenArray[pHfoProxyConnection->TokenCount ++];

            HttpSmpoUtilCopyHeaderString
                (
                    pToken,
                    ulTokenSize,
                    pConnToken->Token,
                    HTTP_MAX_CONNECTION_TOKEN_SIZE
                );

            ulTokenType = IANA_CONNECTION_CODE_RESERVED;

            if (AnscEqualString2(pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_CLOSE;
            }
            else
            if (AnscEqualString2(pToken, IANA_CONNECTION_TEXT_CLOSE, ulTokenSize, FALSE))
            {
                ulTokenType = IANA_CONNECTION_CODE_KEEP_ALIVE;
            }

            pConnToken->OptionType  = ulTokenType;

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoProxyConnection;
}

static PUCHAR
HttpSmpoUtilGetNextCookieNameValuePair
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* find the first cookie name-value pair separated by COMMA or SEMICOLON */
    PUCHAR                          pToken  = pBuf;
    PUCHAR                          pLast   = pToken + ulSize - 1;
    PUCHAR                          pNext   = NULL, pComma, pSemi;
    PUCHAR                          pValue;
    ULONG                           ulNameLen = 0;

    pSemi  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_SEMICOLON, pLast - pToken + 1);
    pComma = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COMMA, pLast - pToken + 1);

    pValue = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, pLast - pToken + 1);

    if ( !pValue || ( pSemi && pValue > pSemi ) || ( pComma && pValue > pComma ) )
    {
        pNext = pSemi;

        if ( pComma && !pSemi )
        {
            pNext = pComma;
        }
        else
        if ( pComma && pSemi && pComma < pSemi )
        {
            pNext = pComma;
        }
    }
    else
    {
        ulNameLen = pValue - pToken;

        pNext = pValue + 1;

        if ( pNext > pLast )
        {
            return NULL;
        }

        if ( *pNext == HTTP_SMPO_CHAR_DQUOTE )
        {
            pNext = _ansc_memchr(pNext + 1, HTTP_SMPO_CHAR_DQUOTE, pLast - pNext);

            if ( pNext )
            {
                if ( pNext + 1 < pLast )
                    pNext ++;
                else
                    pNext = NULL;
            }

            return pNext;
        }

        if ( AnscEqualString2(pToken, HTTP_SMPO_STRING_SCOOKIE_EXPIRES, ulNameLen, FALSE) )
        {
            pNext = _ansc_strstr(pNext, "GMT");

            if ( pNext )
            {
                pNext = pNext + AnscSizeOfString("GMT");

                if ( pNext >= pLast ) pNext = NULL;
            }

            return pNext;
        }

        if ( pSemi && pComma )
        {
            pNext = ( pSemi < pComma ) ? pSemi : pComma;
        }
        else if ( pSemi )
        {
            pNext = pSemi;
        }
        else
        {
            pNext = pComma;
        }
    }

    return pNext;
}


static ANSC_HANDLE
HttpSmpoParseCookie
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Cookie = "Cookie" ":" [ cookie-version ] cookie-value *( ( ";" | "," )  cookie-value ) */
    /* cookie-version  =  "$Version" "=" value ( ";" | "," ) */
    /* cookie-value = cookie-name "=" cookie-name-value [";" cookie-path] [";" cookie-domain] */
    /* value = token | quoted-string  */
    /* cookie-path = "$Path" "=" value */
    /* cookie-domain = "$Domain" "=" value */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_COOKIE                pHfoCookie          = (PHTTP_HFO_COOKIE)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    ULONG                           ulTokenIndex        = 0;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount             = 0;
    PUCHAR                          pValue;
    ULONG                           ulIndex             = 0;
    ULONG                           ulLen;
    PUCHAR                          pName;
    BOOL                            bVersionPresent     = FALSE;
    BOOL                            bVersion, bDomain, bPath, bPort;

    /* get cookie content count */
    pToken  = pMsg;

    ulCount = 0;

    while (pToken)
    {
        pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
        pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

        pName   = pToken;
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        pName   = HttpSmpoUtilLinearWhiteSpace(pName, ulLen);
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            if ( pLast >= pName )
            {
                ulLen   = pLast - pName + 1;
            }
            else
            {
                ulLen   = 0;
            }
        }

        if ( ulLen == 0 )
        {
            break;
        }

        ulCount ++;

        pValue  = _ansc_memchr(pName, HTTP_SMPO_CHAR_EQUAL, ulLen);

        if (pValue)
        {
            ulLen   = pValue - pName;
        }
        else
        {
            ulLen   = pNext - pName;
        }

        /* check the name */
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_VERSION) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_VERSION, pName, ulLen, FALSE))
        {
            bVersionPresent = TRUE;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PATH, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_DOMAIN, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT) &&
            AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PORT, pName, ulLen, FALSE))
        {
            ulCount --;
        }

        if (pNext)
        {
            pToken = pNext + 1;
        }
        else
        {
            pToken  = NULL;
        }
    }

    if (ulCount != 0 && bVersionPresent)
    {
        ulCount --;
    }

    pHfoCookie =
        (PHTTP_HFO_COOKIE)AnscAllocateMemory
            (
                sizeof(HTTP_HFO_COOKIE) + ulCount * sizeof(HTTP_COOKIE_CONTENT)
            );

    if (pHfoCookie)
    {
        pHfoCookie->CookieCount = ulCount;

        pHfoCookie->CookieArray[0].Version = 1;

        /* parse cookie contents */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            pValue  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, ulTokenSize);

            /* cookie name */
            pName   = pToken;
            if (pValue)
            {
                ulLen   = pValue - pName;
            }
            else
            {
                ulLen   = pNext - pName;
            }

            bVersion    = FALSE;
            bDomain     = FALSE;
            bPath       = FALSE;
            bPort       = FALSE;

            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_VERSION) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_VERSION, pName, ulLen, FALSE))
            {
                bVersion = TRUE;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PATH) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PATH, pName, ulLen, FALSE))
            {
                bPath   = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_DOMAIN) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_DOMAIN, pName, ulLen, FALSE))
            {
                bDomain = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_COOKIE_PORT) &&
                AnscEqualString2(HTTP_SMPO_STRING_COOKIE_PORT, pName, ulLen, FALSE))
            {
                bPort   = TRUE;
                ulIndex --;
            }

            if (bVersion)
            {
                pCookieContent  = &pHfoCookie->CookieArray[0];
            }
            else
            {
                if (ulIndex >= ulCount)
                {
                    break;
                }

                pCookieContent  = &pHfoCookie->CookieArray[ulIndex ++];
            }

            if (!bVersion && !bPath && !bDomain && !bPort)
            {
                /* cookie value name */
                HttpSmpoUtilCopyHeaderString(pToken, ulLen, pCookieContent->Name, HTTP_MAX_COOKIE_NAME_SIZE);
            }

            /* cookie value */
            if (bVersion)
            {
                if ( pValue )
                {
                    pValue ++;
                    if ( *pValue == HTTP_SMPO_CHAR_DQUOTE )
                    {
                        pValue ++;
                    }
                    pHfoCookie->CookieArray[0].Version = _ansc_atoi(pValue);
                }
            }
            else
            if (bPath)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Path, HTTP_MAX_PATH_NAME_SIZE);
                }
            }
            else
            if (bDomain)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Domain, ANSC_DOMAIN_NAME_SIZE);
                }
            }
            else
            if (bPort)
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Port, HTTP_MAX_COOKIE_PORT_SIZE);
                }
            }
            else
            {
                if ( pValue )
                {
                    pValue ++;
                    if (pNext)
                    {
                        ulLen   = pNext - pValue;
                    }
                    else
                    {
                        ulLen   = pLast - pValue + 1;
                    }

                    HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Value, HTTP_MAX_COOKIE_VALUE_SIZE);
                }
            }

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoCookie;
}

static USHORT
HttpSmpoUtilGetMonth
    (
        PUCHAR                      pMonth,
        ULONG                       ulSize
    )
{
    USHORT                          usMonth = 1;

    if (ulSize == 3)
    {
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_FEB, ulSize, FALSE))
            usMonth = 2;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_MAR, ulSize, FALSE))
            usMonth = 3;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_APR, ulSize, FALSE))
            usMonth = 4;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_MAY, ulSize, FALSE))
            usMonth = 5;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_JUN, ulSize, FALSE))
            usMonth = 6;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_JUL, ulSize, FALSE))
            usMonth = 7;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_AUG, ulSize, FALSE))
            usMonth = 8;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_SEP, ulSize, FALSE))
            usMonth = 9;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_OCT, ulSize, FALSE))
            usMonth = 10;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_NOV, ulSize, FALSE))
            usMonth = 11;
        else
        if (AnscEqualString2(pMonth, HTTP_SMPO_MONTH_DEC, ulSize, FALSE))
            usMonth = 12;
    }

    return usMonth;
}


static ANSC_HANDLE
HttpSmpoParseSetCookie
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Set-Cookie  = "Set-Cookie:" 1#Cookie-Entity */
    /* Cookie-Entity = cookie-name "=" cookie-name-value *(";" cookie-av) */
    /* cookie-name-value = cookie-name-value-token | quoted-string */
    /*
        cookie-av       =   "Comment"   "="     comment
                    |       "Domain"    "="     set-cookie-domain
                    |       "Max-Age"   "="     delta-seconds
                    |       "Path"      "="     set-cookie-path
                    |       "Secure"
                    |       "Version"   "="     1*DIGIT
                    |       "expires"   "="     cookie-expires-date
                    |       token       "="     value
    */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_SET_COOKIE            pHfoSetCookie       = (PHTTP_HFO_SET_COOKIE)NULL;
    PUCHAR                          pLast               = pMsg + ulSize - 1;
    PUCHAR                          pToken, pNext;
    ULONG                           ulTokenSize;
    ULONG                           ulTokenIndex        = 0;
    PHTTP_COOKIE_CONTENT            pCookieContent;
    ULONG                           ulCount             = 0;
    PUCHAR                          pValue;
    ULONG                           ulIndex             = 0;
    ULONG                           ulLen;
    BOOL                            bSecure;
    BOOL                            bMaxAge;
    BOOL                            bDomain;
    BOOL                            bPath;
    BOOL                            bComment;
    BOOL                            bExpires;
    PUCHAR                          pName;
    BOOL                            bVersion;
    BOOL                            bDiscard;
    BOOL                            bCommentUrl;
    BOOL                            bPort;

    /* get cookie content count */
    pToken  = pMsg;

    ulCount = 0;

    while (pToken)
    {
        pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);
        pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

        pName   = pToken;
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        if ( ulLen == 0 )
        {
            break;
        }

        ulCount ++;

        pName   = HttpSmpoUtilLinearWhiteSpace(pName, ulLen);
        if (pNext)
        {
            ulLen   = pNext - pName;
        }
        else
        {
            ulLen   = pLast - pName + 1;
        }

        pValue  = _ansc_memchr(pName, HTTP_SMPO_CHAR_EQUAL, ulLen);

        if (pValue)
        {
            ulLen   = pValue - pName;
        }
        else
        {
            ulLen   = pNext ? pNext - pName : pLast - pName + 1;
        }

        /* check the name */
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_COMMENT) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_COMMENT, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_DOMAIN) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_DOMAIN, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_PORT) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_PORT, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_PATH) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_PATH, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_SECURE) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_SECURE, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_DISCARD) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_DISCARD, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_VERSION) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_VERSION, pName, ulLen, FALSE))
        {
            ulCount --;
        }
        else
        if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_EXPIRES) &&
            AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_EXPIRES, pName, ulLen, FALSE))
        {
            ulCount --;
        }

        if (pNext)
        {
            pToken = pNext + 1;
        }
        else
        {
            pToken  = NULL;
        }
    }

    pHfoSetCookie =
        (PHTTP_HFO_SET_COOKIE)AnscAllocateMemory
            (
                sizeof(HTTP_HFO_SET_COOKIE) + ulCount * sizeof(HTTP_COOKIE_CONTENT)
            );

    if (pHfoSetCookie)
    {
        ULONG                       i;

        pHfoSetCookie->CookieCount = ulCount;

        for (i = 0; i < ulCount; i ++)
        {
            pHfoSetCookie->CookieArray[i].bPermanent        = FALSE;
            pHfoSetCookie->CookieArray[i].MaxAgeInSeconds   = HTTP_COOKIE_MAX_AGE_ABSENT;
        }

        /* parse cookie contents */
        pToken  = pMsg;

        while (pToken)
        {
            pToken  = HttpSmpoUtilLinearWhiteSpace(pToken, pLast - pToken + 1);

            pNext   = HttpSmpoUtilGetNextCookieNameValuePair(pToken, pLast - pToken + 1);

            if (pNext)
            {
                ulTokenSize = pNext - pToken;
            }
            else
            {
                ulTokenSize = pLast - pToken + 1;
            }

            if ( ulTokenSize == 0 )
            {
                break;
            }

            pValue  = _ansc_memchr(pToken, HTTP_SMPO_CHAR_EQUAL, ulTokenSize);

            /* cookie name */
            pName   = pToken;
            if (pValue)
            {
                ulLen   = pValue - pName;
            }
            else
            {
                ulLen   = pNext ? pNext - pName : pLast - pName + 1;
            }

            bComment    = FALSE;
            bDomain     = FALSE;
            bPath       = FALSE;
            bSecure     = FALSE;
            bMaxAge     = FALSE;
            bExpires    = FALSE;
            bCommentUrl = FALSE;
            bDiscard    = FALSE;
            bPort       = FALSE;
            bVersion    = FALSE;

            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_COMMENT) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_COMMENT, pName, ulLen, FALSE))
            {
                bComment    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL, pName, ulLen, FALSE))
            {
                bCommentUrl = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_DOMAIN) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_DOMAIN, pName, ulLen, FALSE))
            {
                bDomain     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_PORT) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_PORT, pName, ulLen, FALSE))
            {
                bPort       = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_MAX_AGE, pName, ulLen, FALSE))
            {
                bMaxAge     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_PATH) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_PATH, pName, ulLen, FALSE))
            {
                bPath       = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_SECURE) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_SECURE, pName, ulLen, FALSE))
            {
                bSecure     = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE2_DISCARD) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE2_DISCARD, pName, ulLen, FALSE))
            {
                bDiscard    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_VERSION) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_VERSION, pName, ulLen, FALSE))
            {
                bVersion    = TRUE;
                ulIndex --;
            }
            else
            if (ulLen == AnscSizeOfString(HTTP_SMPO_STRING_SCOOKIE_EXPIRES) &&
                AnscEqualString2(HTTP_SMPO_STRING_SCOOKIE_EXPIRES, pName, ulLen, FALSE))
            {
                bExpires    = TRUE;
                ulIndex --;
            }

            if (ulIndex >= pHfoSetCookie->CookieCount)
            {
                break;
            }

            pCookieContent  = &pHfoSetCookie->CookieArray[ulIndex ++];

            if (!bComment && !bMaxAge && !bSecure && !bPath && !bDomain && !bPort && !bDiscard && !bCommentUrl && !bVersion)
            {
                /* cookie value name */
                HttpSmpoUtilCopyHeaderString(pToken, ulLen, pCookieContent->Name, HTTP_MAX_COOKIE_NAME_SIZE);
            }

            /* cookie value */
            if (bPath)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Path, HTTP_MAX_PATH_NAME_SIZE);
            }
            else
            if (bDomain)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Domain, ANSC_DOMAIN_NAME_SIZE);
            }
            else
            if (bPort)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Port, HTTP_MAX_COOKIE_PORT_SIZE);
            }
            else
            if (bComment)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Comment, HTTP_MAX_COOKIE_COMMENT_SIZE);
            }
            else
            if (bCommentUrl)
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->CommentUrl, ANSC_URI_STRING_SIZE);
            }
            else
            if (bMaxAge)
            {
                pValue ++;

                pCookieContent->MaxAgeInSeconds = _ansc_atoi(pValue);
            }
            else
            if (bVersion)
            {
                pValue ++;

                if (*pValue == '"')
                {
                    pValue ++;
                }

                pCookieContent->Version = _ansc_atoi(pValue);
            }
            else
            if (bSecure)
            {
                pCookieContent->bSecure = TRUE;
            }
            else
            if (bDiscard)
            {
                pCookieContent->bDiscard = TRUE;
            }
            else
            if ( bExpires )
            {
                ANSC_UNIVERSAL_TIME *pTime      = &pCookieContent->Expires;
                char                wday[16];
                char                mon[3];
                char*               pDate;

                pTime->bDayLightSaving = FALSE;

                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                pDate = _ansc_memchr(pValue, HTTP_SMPO_CHAR_COMMA, ulLen);

                if ( pDate )
                {
                    ULONG           ulWdayLen = ((PUCHAR)pDate) - pValue;

                    AnscCopyMemory(wday, pValue, ulWdayLen);
                    wday[ulWdayLen] = 0;

                    pDate ++;
                    if ( *pDate == ' ' )
                    {
                        pDate ++;
                    }
                }

                if ( pDate )
                {
                    _ansc_sscanf
                        (
                            pDate,
                            "%d-%3c-%d %d:%d:%d GMT",
                            &pTime->DayOfMonth,
                            mon,
                            &pTime->Year,
                            &pTime->Hour,
                            &pTime->Minute,
                            &pTime->Second
                        );

                    pCookieContent->bPermanent = TRUE;
                }

                pTime->Month = HttpSmpoUtilGetMonth(mon, 3);

                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_SUNDAY, FALSE))
                    pTime->DayOfWeek = 0;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_MONDAY, FALSE))
                    pTime->DayOfWeek = 1;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_TUESDAY, FALSE))
                    pTime->DayOfWeek = 2;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_WEDNESDAY, FALSE))
                    pTime->DayOfWeek = 3;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_THURSDAY, FALSE))
                    pTime->DayOfWeek = 4;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_FRIDAY, FALSE))
                    pTime->DayOfWeek = 5;
                else
                if (AnscEqualString(wday, HTTP_SMPO_WEEKDAY_SATURDAY, FALSE))
                    pTime->DayOfWeek = 6;
            }
            else if ( pValue )
            {
                pValue ++;
                if (pNext)
                {
                    ulLen   = pNext - pValue;
                }
                else
                {
                    ulLen   = pLast - pValue + 1;
                }

                HttpSmpoUtilCopyHeaderString(pValue, ulLen, pCookieContent->Value, HTTP_MAX_COOKIE_VALUE_SIZE);
            }

            /* try next */
            if (pNext)
            {
                pToken  = pNext + 1;
            }
            else
            {
                pToken  = NULL;
            }
        }
    }

    return (ANSC_HANDLE)pHfoSetCookie;
}

static ANSC_HANDLE
HttpSmpoParseCookie2
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Cookie2: $Version="1" */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_COOKIE2               pHfoCookie2         = (PHTTP_HFO_COOKIE2)NULL;

    pHfoCookie2 = (PHTTP_HFO_COOKIE2)AnscAllocateMemory(sizeof(HTTP_HFO_COOKIE2));

    return (ANSC_HANDLE)pHfoCookie2;
}

static ANSC_HANDLE
HttpSmpoParseSetCookie2
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Set-Cookie  = "Set-Cookie:" 1#Cookie-Entity */
    /* Cookie-Entity = cookie-name "=" cookie-name-value *(";" cookie-av) */
    /* cookie-name-value = cookie-name-value-token | quoted-string */
    /*
     *   cookie-av       =   "Comment"   "="     comment
     *               |       "Domain"    "="     set-cookie-domain
     *               |       "Max-Age"   "="     delta-seconds
     *               |       "Path"      "="     set-cookie-path
     *               |       "Secure"
     *               |       "Version"   "="     1*DIGIT
     *               |       "expires"   "="     cookie-expires-date
     *               |       token       "="     value
     */


    return HttpSmpoParseSetCookie(pBuf, ulSize);
}

static BOOL
HttpSmpoUtilParseRelPath
    (
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* rel_path = [path] [";" params] ["?" query] */

    PHTTP_REQUEST_URI               pUri        = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pToken      = pBuf;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    ULONG                           ulLen;
    PUCHAR                          pNext;
    PUCHAR                          pQuery;
    ULONG                           ulQueryLen;
    PUCHAR                          pPath       = pBuf;
    PUCHAR                          pPathEnd;

    pQuery  = _ansc_memchr(pPath, HTTP_SMPO_CHAR_QUESTION, pLast - pPath + 1);

    if (pQuery)
    {
        ulQueryLen  = pLast - pQuery;
        pPathEnd    = pQuery - 1;
    }
    else
    {
        ulQueryLen  = 0;
        pPathEnd    = pLast;
    }

    /* parsing path segments */
    while (pPath)
    {
        pNext   = _ansc_memchr(pPath, HTTP_SMPO_CHAR_SLASH, pPathEnd - pPath + 1);

        if (pNext)
        {
            ulLen   = pNext - pPath;
        }
        else
        {
            ulLen   = pPathEnd - pPath + 1;
        }

        if (ulLen == 0)
        {
            if (pNext)
            {
                pUri->PathArray[pUri->PathLevel][0] = '/';
                pUri->PathArray[pUri->PathLevel][1] = 0;
            }
            else
            {
                pUri->PathLevel --;
            }
        }
        else
        {
            HttpSmpoUtilCopyHeaderString(pPath, ulLen, pUri->PathArray[pUri->PathLevel], HTTP_MAX_PATH_NAME_SIZE);
        }

        pUri->PathLevel ++;

        if (pNext)
        {
            if (pNext == pPathEnd)
            {
                pUri->PathArray[pUri->PathLevel][0] = '/';
                pUri->PathArray[pUri->PathLevel][1] = 0;
                pUri->PathLevel ++;

                pPath   = NULL;
            }
            else
            {
                pPath   = pNext + 1;
            }
        }
        else
        {
            pPath   = NULL;
        }

		if (pUri->PathLevel >= HTTP_MAX_PATH_LEVEL_NUMBER)
		{
			break;	/* too many paths */
		}
    }

    /* parse query parameters */
    pQuery ++;
    HttpSmpoUtilCopyHeaderString(pQuery, ulQueryLen, pUri->QueryParams, HTTP_MAX_URI_QUERY_SIZE);

    return TRUE;
}



static BOOL
HttpSmpoUtilParseAbsPath
    (
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* abs_path = "/" rel_path */

    PHTTP_REQUEST_URI               pUri        = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pRelPath    = pBuf;
    BOOL                            bSucc       = TRUE;

    if (ulSize == 0 || *pRelPath != HTTP_SMPO_CHAR_SLASH)
    {
        return FALSE;
    }

    pUri->PathArray[0][0]   = '/';
    pUri->PathArray[0][1]   = 0;

    pUri->PathLevel = 1;

    bSucc   = HttpSmpoUtilParseRelPath(hRequestUri, pRelPath + 1, ulSize - 1);

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_ABS_PATH;
    }

    return bSucc;
}



static BOOL
HttpSmpoUtilParseAbsoluteURI
    (
        ANSC_HANDLE                 hRequestUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* absoluteURI = scheme ":" *( uchar | reserved ) */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hRequestUri;
    PUCHAR                          pScheme = pBuf;
    PUCHAR                          pNext;
    ULONG                           ulLen;
    BOOL                            bHttp   = FALSE;
    USHORT                          usPort  = 0;
    PUCHAR                          pHost;
    PUCHAR                          pLast   = pBuf + ulSize - 1;

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_COLON, ulSize);

    if (!pNext)
    {
        return FALSE;
    }

    pHost   = pNext + 1;

    ulLen   = pNext - pBuf;

    if ( ulLen == HTTP_SMPO_PROTOCOL_HTTPS_LENGTH &&
        AnscEqualString2(pScheme, HTTP_SMPO_PROTOCOL_HTTPS, HTTP_SMPO_PROTOCOL_HTTPS_LENGTH, FALSE) )
    {
        pUri->Flag  |= HTTP_URI_FLAG_HTTPS;
        bHttp       = TRUE;
        usPort      = HTTPS_PORT;
    }
    else
        if ( ulLen == 4 && AnscEqualString2(pScheme, "http", 4, FALSE) )
        {
            bHttp   = TRUE;
            usPort  = HTTP_PORT;
        }

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SLASH, pNext - pScheme);

    if (pNext)
    {
        /* scheme name shouldn't have slash in it */

        return FALSE;
    }

    pUri->Type  = HTTP_URI_TYPE_ABSOLUTE_URI;
    HttpSmpoUtilCopyHeaderString(pBuf, ulSize, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

    if (bHttp)
    {
        PUCHAR                      pLast   = pBuf + ulSize - 1;

        pUri->HostPort  = usPort;

        pNext   = pHost;

        if ( *(pNext + 1) == HTTP_SMPO_CHAR_SLASH )
        {
            pHost   = pNext + 2;
        }

        if (pHost)
        {
            BOOL                    bPath       = TRUE;
            PUCHAR                  pPort;
            PUCHAR                  pHostEnd    = NULL;
            PUCHAR                  pPortStart  = NULL;
            ULONG                   ulHostLen   = 0;

            pNext   = _ansc_memchr(pHost, HTTP_SMPO_CHAR_SLASH, pLast - pHost + 1);

            if (!pNext)
            {
                pNext       = _ansc_memchr(pHost, HTTP_SMPO_CHAR_SPACE, pLast - pHost + 1);
                bPath       = FALSE;
                pHostEnd    = pLast;
            }
            else
            {
                pHostEnd    = pNext - 1;
            }

            if (TRUE)
            {
                if (bPath && pNext)
                {
                    HttpSmpoUtilParseAbsPath(hRequestUri, pNext, pLast - pNext + 1);
                    pUri->Type  = HTTP_URI_TYPE_ABSOLUTE_URI;
                }
                else
                {
                    pUri->PathArray[0][0]   = '/';
                    pUri->PathArray[0][1]   = 0;
                }

#ifdef _ANSC_IPV6_COMPATIBLE_
                if ( !(pPortStart = _ansc_strchr(pHost, ']')) )
                {
                    pPortStart = pHost;
                }
#else
                pPortStart = pHost;
#endif

                if (pNext)
                {
                    pPort   = _ansc_memchr(pPortStart/*pHost*/, HTTP_SMPO_CHAR_COLON, pNext - pHost);
                }
                else
                {
                    pPort   = _ansc_memchr(pPortStart/*pHost*/, HTTP_SMPO_CHAR_COLON, pHostEnd - pHost + 1);
                }

                if (pPort)
                {
                    pUri->HostPort = _ansc_atoi(pPort + 1);
                }
                else
                {
                    pUri->HostPort  = usPort;
                }
#ifdef _ANSC_IPV6_COMPATIBLE_
                if ( *pHost == '[' )
                {
                    pHost = pHost + 1;
                    ulHostLen = pPort ? (pPort - pHost - 1) : ( pNext ? (pNext - pHost - 1) : (pLast - pHost) );
                }
                else
                {
                    ulHostLen = pPort ? (pPort - pHost) : ( pNext ? (pNext - pHost) : (pLast - pHost + 1) );
                }
#else
                ulHostLen = pPort ? (pPort - pHost) : ( pNext ? (pNext - pHost) : (pLast - pHost + 1) );
#endif
                HttpSmpoUtilCopyHeaderString
                    (
                        pHost,
                        ulHostLen /*pPort ? (pPort - pHost) : ( pNext ? (pNext - pHost) : (pLast - pHost + 1) )*/,
                        pUri->HostName,
                        ANSC_DOMAIN_NAME_SIZE
                    );
            }
        }
    }

    return TRUE;
}



static ANSC_HANDLE
HttpSmpoParseLocation
    (
        void                        *pBuf,
        ULONG                       ulSize
    )
{
    /* Location = "Location" ":" absoluteURI */

    PUCHAR                          pMsg                = (PUCHAR)pBuf;
    PHTTP_HFO_LOCATION              pHfoLocation        = NULL;
    BOOL                            bSucc               = TRUE;

    pHfoLocation  = (PHTTP_HFO_LOCATION)AnscAllocateMemory(sizeof(HTTP_HFO_LOCATION));

    if (pHfoLocation)
    {
        bSucc   =
            HttpSmpoUtilParseAbsoluteURI
                (
                    (ANSC_HANDLE)&pHfoLocation->PreferredUri,
                    pBuf,
                    ulSize
                );

        if (!bSucc)
        {
            AnscFreeMemory(pHfoLocation);
            pHfoLocation = NULL;
        }
    }

    return (ANSC_HANDLE)pHfoLocation;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        AnscHttpRegisterAllHeadersXInfo
            (
				void
            )

    description:

        This function is called to register parsing functions
        for minimum HTTP headers required by http simple client -
			content_encoding, content_type, host, cookie, user_agent,
			content_length, connection, transfer_encoding, proxy_connection,
			authorization, www_authenticate, set_cookie, set_cookie2,
			transfer_encoding


    argument:   void

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscHttpRegisterAllHeadersXInfo
    (
        void
    )
{
	int           i  = 0;

	if ( headerInitialized )
		return ANSC_STATUS_SUCCESS;

    for (i = 0; i < HTTP_SMPO_HEADER_TABLE_SIZE; i ++)
    {
        AnscInitializeSList(&HeaderXList[i]);
    }

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_AUTHORIZATION,
            HTTP_HEADER_ID_AUTHORIZATION,
            HttpSmpoParseAuthorization
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_CONNECTION,
            HTTP_HEADER_ID_CONNECTION,
            HttpSmpoParseConnection
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_CONTENT_ENCODING,
            HTTP_HEADER_ID_CONTENT_ENCODING,
            HttpSmpoParseContentEncoding
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_CONTENT_LENGTH,
            HTTP_HEADER_ID_CONTENT_LENGTH,
            HttpSmpoParseContentLength
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_CONTENT_TYPE,
            HTTP_HEADER_ID_CONTENT_TYPE,
            HttpSmpoParseContentType
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_HOST,
            HTTP_HEADER_ID_HOST,
            HttpSmpoParseHost
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_LOCATION,
            HTTP_HEADER_ID_LOCATION,
            HttpSmpoParseLocation
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_TRANSFER_ENCODING,
            HTTP_HEADER_ID_TRANSFER_ENCODING,
            HttpSmpoParseTransferEncoding
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_WWW_AUTHENTICATE,
            HTTP_HEADER_ID_WWW_AUTHENTICATE,
            HttpSmpoParseWwwAuthenticate
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_PROXY_CONNECTION,
            HTTP_HEADER_ID_PROXY_CONNECTION,
            HttpSmpoParseProxyConnection
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_COOKIE,
            HTTP_HEADER_ID_COOKIE,
            HttpSmpoParseCookie
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_SET_COOKIE,
            HTTP_HEADER_ID_SET_COOKIE,
            HttpSmpoParseSetCookie
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_COOKIE2,
            HTTP_HEADER_ID_COOKIE2,
            HttpSmpoParseCookie2
        );

    HttpSmpoRegisterHeaderXInfo2
        (
            HTTP_HEADER_NAME_SET_COOKIE2,
            HTTP_HEADER_ID_SET_COOKIE2,
            HttpSmpoParseSetCookie2
        );

    headerInitialized = TRUE;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpGetHeaderXInfo
            (
                PUCHAR                      pName,
                ULONG                       ulSize
            )

    description:

        This function is called to parsing info of the given header.

    argument:
                PUCHAR                      pName
                Header name.

                ULONG                       ulSize
                The length of buffer.

    return:     handle to header parsing info.

**********************************************************************/

ANSC_HANDLE
AnscHttpGetHeaderXInfo
    (
        PUCHAR                      pName,
        ULONG                       ulSize
    )
{
    PHTTP_SMPO_HEADER_XINFO1         pXinfo      = NULL;
    PSINGLE_LINK_ENTRY              pEntry;
    ULONG                           ulHashIndex;

    ulHashIndex = AnscHashString(pName, ulSize, HTTP_SMPO_HEADER_TABLE_SIZE);
    pEntry      = AnscSListGetFirstEntry(&HeaderXList[ulHashIndex]);

    while (pEntry)
    {
        pXinfo  = ACCESS_HTTP_SMPO_HEADER_XINFO1_OBJECT(pEntry);

        if (ulSize == AnscSizeOfString(pXinfo->Name))
        {
            if (AnscEqualString2(pXinfo->Name, pName, ulSize, FALSE))
            {
                return (ANSC_HANDLE)pXinfo;
            }
        }

        pEntry  = AnscSListGetNextEntry(pEntry);
    }

    return (ANSC_HANDLE)NULL;
}



ULONG
AnscHttpGetHeaderCode
    (
        char*                       name
    )
{
    PHTTP_SMPO_HEADER_XINFO1         pXinfo      = NULL;
    PSINGLE_LINK_ENTRY              pEntry;
    ULONG                           ulHashIndex;

    ulHashIndex = AnscHashString(name, AnscSizeOfString(name), HTTP_SMPO_HEADER_TABLE_SIZE);
    pEntry      = AnscSListGetFirstEntry(&HeaderXList[ulHashIndex]);

    while (pEntry)
    {
        pXinfo  = ACCESS_HTTP_SMPO_HEADER_XINFO1_OBJECT(pEntry);

		if (AnscEqualString1(pXinfo->Name, name, FALSE))
		{
			return (ANSC_HANDLE)pXinfo->HeaderId;
		}

        pEntry  = AnscSListGetNextEntry(pEntry);
    }

    AnscTrace("AnscHttpGetHeaderCode - header %s is not supported!\n", name);
    return HTTP_HEADER_ID_RESERVED;
}


static ULONG
HttpSmpoUtilGetMethodId
    (
        PUCHAR                      pMethod,
        ULONG                       ulSize
    )
{
    ULONG                           ulMethod    = HTTP_METHOD_RESERVED;

    if (AnscSizeOfString(HTTP_METHOD_NAME_GET) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_GET, ulSize, FALSE))
        ulMethod = HTTP_METHOD_GET;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_POST) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_POST, ulSize, FALSE))
        ulMethod = HTTP_METHOD_POST;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_OPTIONS) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_OPTIONS, ulSize, FALSE))
        ulMethod = HTTP_METHOD_OPTIONS;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_HEAD) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_HEAD, ulSize, FALSE))
        ulMethod = HTTP_METHOD_HEAD;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_PUT) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_PUT, ulSize, FALSE))
        ulMethod = HTTP_METHOD_PUT;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_DELETE) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_DELETE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_DELETE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_CONNECT) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_CONNECT, ulSize, FALSE))
        ulMethod = HTTP_METHOD_CONNECT;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_TRACE) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_TRACE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_TRACE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_NOTIFY) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_NOTIFY, ulSize, FALSE))
        ulMethod = HTTP_METHOD_NOTIFY;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_SEARCH) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_SEARCH, ulSize, FALSE))
        ulMethod = HTTP_METHOD_SEARCH;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_M_SEARCH) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_M_SEARCH, ulSize, FALSE))
        ulMethod = HTTP_METHOD_M_SEARCH;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_M_POST) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_M_POST, ulSize, FALSE))
        ulMethod = HTTP_METHOD_M_POST;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_SUBSCRIBE) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_SUBSCRIBE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_SUBSCRIBE;
    else
    if (AnscSizeOfString(HTTP_METHOD_NAME_UNSUBSCRIBE) == ulSize &&
        AnscEqualString2(pMethod, HTTP_METHOD_NAME_UNSUBSCRIBE, ulSize, FALSE))
        ulMethod = HTTP_METHOD_UNSUBSCRIBE;

    return ulMethod;
}

static BOOL
HttpSmpoUtilParseRequestLineVersion
    (
        PUCHAR                      pVersion,
        ULONG                       ulVersionLen,
        PULONG                      pMajorVersion,
        PULONG                      pMinorVersion
    )
{
    PUCHAR                          pMajor, pMinor;

    pMajor  = _ansc_strchr(pVersion, '/');
    if (!pMajor)
        return FALSE;

    pMajor ++;
    *pMajorVersion  = (ULONG)_ansc_atoi(pMajor);

    pMinor  = _ansc_strchr(pMajor, '.');
    if (!pMinor)
        return FALSE;

    pMinor ++;
    *pMinorVersion  = (ULONG)_ansc_atoi(pMinor);

    return TRUE;
}


static BOOL
HttpSmpoUtilParseAuthorityServer
    (
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    /* authority-server = [ [ userinfo "@" ] hostport ] */
    /* hostport = host [ ":" port ] */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken  = pBuf;
    PUCHAR                          pNext;

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_AT, pLast - pToken + 1);

    if (pNext)
    {
        /* userinfo is present */
        pToken  = pNext + 1;
    }

    pNext   = _ansc_memchr(pToken, HTTP_SMPO_CHAR_COLON, pLast - pToken + 1);

    if (pNext)
    {
        /* port is specified */

        HttpSmpoUtilCopyHeaderString(pToken, pNext - pToken, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

        pUri->HostPort  = (USHORT)_ansc_atoi(pNext + 1);
    }
    else
    {
        HttpSmpoUtilCopyHeaderString(pToken, pLast - pToken + 1, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);

        pUri->HostPort  = (USHORT)HTTP_SERVER_PORT;
    }

    return TRUE;
}

static BOOL
HttpSmpoUtilParseRegName
    (
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    PUCHAR                          pLast   = pBuf + ulSize - 1;
    PUCHAR                          pToken  = pBuf;

    HttpSmpoUtilCopyHeaderString(pToken, pLast - pToken + 1, pUri->HostName, ANSC_DOMAIN_NAME_SIZE);
    pUri->HostPort  = (USHORT)HTTP_SERVER_PORT;

    return TRUE;
}


static BOOL
HttpSmpoUtilParseAuthority
    (
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulSize
    )
{
    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    BOOL                            bSucc   = TRUE;

    bSucc   = HttpSmpoUtilParseAuthorityServer(hUri, pBuf, ulSize);

    if (!bSucc)
    {
        bSucc   = HttpSmpoUtilParseRegName(hUri, pBuf, ulSize);
    }

    if (bSucc)
    {
        pUri->Type  = HTTP_URI_TYPE_AUTHORITY;
    }

    return bSucc;
}


static BOOL
HttpSmpoUtilParseRquestLineUri
    (
        ANSC_HANDLE                 hUri,
        PUCHAR                      pBuf,
        ULONG                       ulBufLen
    )
{
    /* Request-URI = "*" | absoluteURI | abs_path | authority */

    PHTTP_REQUEST_URI               pUri    = (PHTTP_REQUEST_URI)hUri;
    BOOL                            bSucc   = TRUE;

    if (AnscEqualString(pBuf, "*", TRUE))
    {
        pUri->Type  = HTTP_URI_TYPE_ASTERISK;
    }
    else
    {
        bSucc   = HttpSmpoUtilParseAbsoluteURI(hUri, pBuf, ulBufLen);

        if (!bSucc)
        {
            bSucc   = HttpSmpoUtilParseAbsPath(hUri, pBuf, ulBufLen);

            if (!bSucc)
            {
                bSucc   = HttpSmpoUtilParseAuthority(hUri, pBuf, ulBufLen);
            }
        }
    }

    return bSucc;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoDecodeUrlEncode
            (
                PUCHAR                      pStr,
                ULONG                       ulStrLen,
                PUCHAR                      pDecodedStr,
                PULONG                      pulDecodedStrLen
            )

    description:

        This function is called to decode URL-Encoded string.

    argument:   PUCHAR                      pStr
                Buffer of URL-Encoded string.

                ULONG                       ulStrSize
                The length of buffer.

                PUCHAR                      pDecodedStr
                The buffer to hold decoded string.

                PULONG                      pulDecodedStrLen
                The length of decoded string.
    
    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoDecodeUrlEncode
    (
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        PUCHAR                      pDecodedStr,
        PULONG                      pulDecodedStrLen
    )
{
    ULONG                           i, ulCount;
    UCHAR                           uc1, uc2;
    BOOL                            bSucc;

    if (!pStr || (!pDecodedStr && !pulDecodedStrLen))
        return ANSC_STATUS_SUCCESS;

    i       = 0;
    ulCount = 0;

    while (i < ulStrLen)
    {
        uc1 = pStr[i];

        /* convert "% Hex Hex" into a hexadecimal value */
        if (uc1 == '%' && i + 2 < ulStrLen)
        {
            uc1 = pStr[i + 1];
            uc2 = pStr[i + 2];

            bSucc   =  HttpSmpoDecodeUrlGetHex(uc1, &uc1);
            bSucc   &= HttpSmpoDecodeUrlGetHex(uc2, &uc2);

            if (!bSucc)
                return ANSC_STATUS_FAILURE;

            uc1 <<= 4;
            uc1 += uc2;

            i += 2;
        }
        else
            if (uc1 == '+')      /* translate plus sign '+' to space */
            {
                uc1 = ' ';
            }

        if (pDecodedStr)
            pDecodedStr[ulCount]  = uc1;

        ulCount ++;
        i ++;
    }

    if (*pulDecodedStrLen)
        *pulDecodedStrLen = ulCount;

    if (pDecodedStr)
        pDecodedStr[ulCount]  = 0;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpParseRequestLine
            (
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse request line.

    argument:
                PVOID               buffer
                Buffer of request line to be parsed.

    return:     handle to request line object.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseRequestLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_HANDLE                     hResult;
    ULONG                           i;
    PHTTP_REQUEST_INFO              pRequestInfo = NULL;
    PUCHAR                          pMethod, pUri, pVersion;
    ULONG                           ulMethod, ulUri, ulVersion;
    PUCHAR                          pBuf;
    PUCHAR                          pMsg;

    hResult             = (ANSC_HANDLE)NULL;
    pMethod             = NULL;
    pUri                = NULL;
    pVersion            = NULL;
    pBuf                = (PUCHAR)buffer;

    /* skip possible leading spaces and tab */
    for (i = 0; i < ulSize; i ++)
    {
        if (pBuf[i] != ' ')
            break;
    }

    pMethod = &pBuf[i];
    pMsg    = _ansc_strchr(pMethod, ' ');

    if (!pMsg)
    {
        return (ANSC_HANDLE)NULL;
    }

    ulMethod        = (ULONG)(pMsg - pMethod);

    /* skip possible leading spaces and tab */
    while (*pMsg == ' ')
    {
        pMsg ++;
    }

    pUri    = pMsg;
    pMsg    = _ansc_strchr(pUri, ' ');

    if (!pMsg)
        return (ANSC_HANDLE)NULL;

    ulUri           = (ULONG)(pMsg - pUri);
    pMsg ++;

    pVersion        = pMsg;
    ulVersion       = ((PUCHAR)buffer + ulSize - pMsg);

    if (pMethod && pUri && pVersion)
    {
        pRequestInfo    = (PHTTP_REQUEST_INFO)AnscAllocateMemory(sizeof(HTTP_REQUEST_INFO));

        /* parse method */
        pRequestInfo->Method    = HttpSmpoUtilGetMethodId(pMethod, ulMethod);

        if (pMethod && ulMethod != 0)
        {
            ULONG                   ulNameLen   = ulMethod;

            if (ulNameLen >= HTTP_MAX_METHOD_STR_SIZE)
            {
                ulNameLen   = HTTP_MAX_METHOD_STR_SIZE - 1;
            }

            AnscCopyMemory(pRequestInfo->MethodName, pMethod, ulNameLen);
            pRequestInfo->MethodName[ulNameLen] = '\0';
        }

        pRequestInfo->RequestUri.PathLevel = 1;

        pRequestInfo->MajorVersion  = 1;
        pRequestInfo->MinorVersion  = 0;

        /* parse version */
        HttpSmpoUtilParseRequestLineVersion
            (
                pVersion,
                ulVersion,
                &pRequestInfo->MajorVersion,
                &pRequestInfo->MinorVersion
            );

        pRequestInfo->RequestUri.HostPort   = HTTP_SERVER_PORT;

        /* parse URI */
        HttpSmpoUtilParseRquestLineUri((ANSC_HANDLE)&pRequestInfo->RequestUri, pUri, ulUri);

        if (pRequestInfo->RequestUri.Type == HTTP_URI_TYPE_ASTERISK ||
            pRequestInfo->RequestUri.Type == HTTP_URI_TYPE_RESERVED)
        {
            pRequestInfo->RequestUri.PathLevel = 0;
        }

        HttpSmpoUtilCopyHeaderString((PUCHAR)buffer, ulSize, pRequestInfo->StartLine, HTTP_MAX_START_LINE_SIZE);
    }

    return (ANSC_HANDLE)pRequestInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpParseStatusLine
            (
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse status line.

    argument:
                PVOID               buffer
                Buffer of status line to be parsed.

    return:     handle to status line object.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseStatusLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PUCHAR                          pLast           = ((PUCHAR)buffer) + ulSize - 1;
    PHTTP_RESPONSE_INFO             pResponseInfo   = NULL;
    ULONG                           ulMajor, ulMinor;
    PUCHAR                          pNext, pBuf;
    ULONG                           ulStatusCode;
    PUCHAR                          pReason;
    ULONG                           ulReason;
    ULONG                           ulCopySize;

    /* version */
    pBuf    = (PUCHAR)buffer;
    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SPACE, ulSize);

    if (!pNext)
    {
        return NULL;
    }

    HttpSmpoUtilParseRequestLineVersion
        (
            pBuf,
            pNext - pBuf,
            &ulMajor,
            &ulMinor
        );

    /* status code */
    pBuf    = pNext + 1;
    ulStatusCode    = _ansc_atoi(pBuf);

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SPACE, pLast - pBuf + 1);
    if (!pNext)
    {
        return NULL;
    }

    /* reason phrase */
    pReason     = pNext + 1;
    ulReason    = pLast - pReason + 1;

    pResponseInfo   = (PHTTP_RESPONSE_INFO)AnscAllocateMemory(sizeof(HTTP_RESPONSE_INFO));

    if (pResponseInfo)
    {
        ulCopySize  = ulSize;
        if (ulCopySize >= HTTP_MAX_START_LINE_SIZE)
        {
            ulCopySize  = HTTP_MAX_START_LINE_SIZE - 1;
        }

        AnscCopyMemory(pResponseInfo->StartLine, buffer, ulCopySize);
        pResponseInfo->StartLine[ulCopySize] = 0;

        pResponseInfo->MajorVersion = ulMajor;
        pResponseInfo->MinorVersion = ulMinor;
        pResponseInfo->StatusCode   = ulStatusCode;

        ulCopySize  = ulReason;
        if (ulCopySize >= HTTP_MAX_REASON_PHRASE_SIZE)
        {
            ulCopySize  = HTTP_MAX_REASON_PHRASE_SIZE - 1;
        }

        AnscCopyMemory(pResponseInfo->ReasonPhrase, pReason, ulCopySize);
        pResponseInfo->ReasonPhrase[ulCopySize] = 0;
    }

    return (ANSC_HANDLE)pResponseInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpParseHeader
            (
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse header field.

    argument:
                PVOID               buffer
                Buffer of header field to be parsed.

    return:     handle to header field object.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseHeader
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PUCHAR                          pLast       = ((PUCHAR)buffer) + ulSize - 1;
    ANSC_HANDLE                     hHeader     = (ANSC_HANDLE)NULL;
    PUCHAR                          pHeader;
    ULONG                           i, ulHeaderSize;
    PUCHAR                          pValue;
    ULONG                           ulValueSize;
    PHTTP_SMPO_HEADER_XINFO1         pXinfo;
    BOOL                            bUnknown    = FALSE;

    /* get header name and call corresponding function to parse it */
    pHeader = (PUCHAR)buffer;

    pValue  = _ansc_memchr(pHeader, HTTP_SMPO_CHAR_COLON, ulSize);

    if (!pValue)
    {
        AnscTrace("AnscHttpParseHeader - Failed to parse header:\n");
        for (i = 0; i < ulSize; i ++)
        {
            AnscTrace("%c", ((char *)buffer) + i);
        }

        AnscTrace("\n\n");

        return  (ANSC_HANDLE)NULL;
    }

    ulHeaderSize    = pValue - pHeader;

    pValue ++;
    pValue          = HttpSmpoUtilLinearWhiteSpace(pValue, pLast - pValue + 1);
    ulValueSize     = pLast - pValue + 1;

    pXinfo          = AnscHttpGetHeaderXInfo(pHeader, ulHeaderSize);

    if (pXinfo)
    {
        if ( !pValue || ulValueSize == 0 )
        {
            hHeader = (ANSC_HANDLE)NULL;
        }
        else
        {
            hHeader = (pXinfo->xFunc)((PVOID)pValue, ulValueSize);
        }
    }

    if (!hHeader)
    {
        /* extension header */
        PHTTP_HEADER_FIELD          pHeader;

        pHeader     = (PHTTP_HEADER_FIELD)AnscAllocateMemory(sizeof(HTTP_HEADER_FIELD));

        hHeader     = (ANSC_HANDLE)pHeader;
        bUnknown    = TRUE;
        /*
        AnscTrace("AnscHttpParseHeader - header not supported.\n");
        for (i = 0; i < ulSize; i ++)
        {
            AnscTrace("    %c", ((char *)buffer) + i);
        }

        AnscTrace("\n\n");
        */
    }

    if (hHeader)
    {
        PHTTP_HEADER_FIELD          pHeaderField    = (PHTTP_HEADER_FIELD)hHeader;

        HttpSmpoUtilCopyHeaderString((PUCHAR)buffer, ulSize, pHeaderField->HeaderLine, HTTP_HEADER_LINE_SIZE);
        pHeaderField->Flags  |= HTTP_FIELD_FLAG_LINE_PRESENT;

        if ( bUnknown )
        {
            pHeaderField->HeaderId      = HTTP_HEADER_ID_EXTENSION;
        }
        else
        {
            pHeaderField->HeaderId      = pXinfo?pXinfo->HeaderId:HTTP_HEADER_ID_EXTENSION;
        }
        pHeaderField->Linkage.Next  = NULL;
        pHeaderField->HashIndex     = 0;
    }

    return hHeader;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpParseChunkedLine
            (
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to chunked line.

    argument:

                PVOID               buffer
                Buffer of chunked line to be parsed.

    return:     handle to chunked line object.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseChunkedLine
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_CHUNK_INFO                pChunkInfo  = NULL;
    PUCHAR                          pBuf        = (PUCHAR)buffer;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    PUCHAR                          pExt        = NULL;
    PUCHAR                          pNext, pCRLF;
    ULONG                           ulChunkSize = 0;
    ULONG                           i;
    UCHAR                           uc;
    ULONG                           ulValue;

    /*  chunk          = chunk-size [ chunk-extension ] CRLF
                        chunk-data CRLF
        chunk-size     = 1*HEX
        chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
     */

    pChunkInfo  = (PHTTP_CHUNK_INFO)AnscAllocateMemory(sizeof(HTTP_CHUNK_INFO));

    if (!pChunkInfo)
    {
        return (ANSC_HANDLE)NULL;
    }

    pNext   = pBuf;

    while (pNext < pLast)
    {
        pCRLF   = _ansc_memchr(pNext, HTTP_SMPO_CHAR_CR, pLast - pNext);

        if (!pCRLF || pCRLF == pLast)
        {
            return (ANSC_HANDLE)NULL;
        }

        if (*(pCRLF + 1) == HTTP_SMPO_CHAR_LF)
        {
            break;
        }

        pNext = pCRLF + 1;
    }

    pExt    = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SEMICOLON, pCRLF - pBuf);

    i = 0;
    pNext   = pBuf;
    while (pNext)
    {
        uc  = *pNext;

        if (uc >= '0' && uc <= '9')
        {
            ulValue = uc - '0';
        }
        else if ((uc >= 'a' && uc <= 'f') || (uc >= 'A' && uc <= 'F'))
        {
            ulValue = uc - 'a' + 10;
        }
        else
        {
            break;
        }

        ulChunkSize = (ulChunkSize << 4) + ulValue;

        pNext ++;
    }

    pChunkInfo->ChunkSize   = ulChunkSize;

    if (!pExt)
    {
        pChunkInfo->ExtCount    = 0;
    }
    else
    {
        PUCHAR                      pExtValue;
        ULONG                       ulExtTokenSize;
        ULONG                       ulExtCount  = 0;

        while (pExt < pCRLF)
        {
            if (ulExtCount >= HTTP_MAX_CHUNK_EXT_NUMBER)
            {
                break;
            }

            pExt ++;
            pExt    = HttpSmpoUtilLinearWhiteSpace(pExt, pCRLF - pExt);

            pNext   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_SEMICOLON, pCRLF - pExt);

            if (pNext)
            {
                pExtValue   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_EQUAL, pNext - pExt);
            }
            else
            {
                pExtValue   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_EQUAL, pCRLF - pExt);
            }

            ulExtTokenSize  = pExtValue ? pExtValue - pExt : (pNext ? pNext - pExt : pCRLF - pExt);

            if (ulExtTokenSize >= HTTP_MAX_CHUNK_TOKEN_SIZE)
            {
                ulExtTokenSize  = HTTP_MAX_CHUNK_TOKEN_SIZE - 1;
            }

            AnscCopyMemory(pChunkInfo->ExtArray[ulExtCount], pExt, ulExtTokenSize);
            pChunkInfo->ExtArray[ulExtCount][ulExtTokenSize]    = 0;

            ulExtCount ++;

            pExt    = pNext;
        }

        pChunkInfo->ExtCount    = ulExtCount;
    }

    return (ANSC_HANDLE)pChunkInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscHttpSmpoParseUrl
            (
                PVOID                       buffer,
                ULONG                       ulSize
            )

    description:

        This function is called to parse URL.

    argument:
                PVOID               buffer
                Buffer of chunked line to be parsed.

                ULONG               ulSize
                The size of URL.

    return:     handle to HTTP Request URL.

**********************************************************************/

ANSC_HANDLE
AnscHttpParseUrl
    (
        ANSC_HANDLE                 unused,   /* left for interface compatibility */
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_REQUEST_URI               pRequestUri;
    BOOL                            bSucc;

    if (!buffer || ulSize == 0)
    {
        return (ANSC_HANDLE)NULL;
    }

    pRequestUri = (PHTTP_REQUEST_URI)AnscAllocateMemory(sizeof(HTTP_REQUEST_URI));

    if (!pRequestUri)
    {
        return (ANSC_HANDLE)NULL;
    }

    bSucc =
        HttpSmpoUtilParseRquestLineUri
            (
                (ANSC_HANDLE)pRequestUri,
                (PUCHAR)buffer,
                ulSize
            );

    if (!bSucc)
    {
        AnscFreeMemory(pRequestUri);
        pRequestUri = NULL;
    }

    return (ANSC_HANDLE)pRequestUri;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        DslhTcpcrhoMemSearch
            (
                PUCHAR                      pBuf,
                ULONG                       ulSize,
                PUCHAR                      pPattern,
                ULONG                       ulPatternLen,
                BOOL                        bCaseSensitive
            );

    description:

        This function is called to find specific 'string'
        pattern in given memory buffer.

    argument:   PUCHAR                      pBuf
                Memory to be searched.

                ULONG                       ulSize
                The size of memory buffer.

                PUCHAR                      pPattern
                The search pattern.

                ULONG                       ulPatternLen
                Size of pattern string.

                BOOL                        bCaseSensitive
                Sensitiveness of string comparison.

    return:     the number of characters in the string.

**********************************************************************/
#if 0
PUCHAR
AnscMemorySearch
    (
        PUCHAR                      pBuf,
        ULONG                       ulSize,
        PUCHAR                      pPattern,
        ULONG                       ulPatternLen,
        BOOL                        bCaseSensitive
    )
{
    PUCHAR                          pBufEnd     = pBuf + ulSize - 1;
    PUCHAR                          pNext, pNext1;
    UCHAR                           fc          = *pPattern;
    UCHAR                           fc1         = 0;

    if ( fc >= 'a' && fc <= 'z')
    {
        fc1 = fc - 'a' + 'A';
    }
    else if ( fc >= 'A' && fc <= 'Z' )
    {
        fc1 = fc - 'A' + 'a';
    }

    pNext1 = NULL;

    while ( pBuf && pBuf <= pBufEnd )
    {
        pNext = _ansc_memchr(pBuf, fc, pBufEnd - pBuf + 1);
        if ( fc1 != 0 )
        {
            pNext1 = _ansc_memchr(pBuf, fc, pBufEnd - pBuf + 1);
        }

        if ( !pNext )
        {
            pNext = pNext1;
        }
        else if ( pNext1 && pNext1 < pNext )
        {
            pNext = pNext1;
        }

        if ( !pNext )
        {
            return  NULL;
        }

        if ( pBufEnd - pNext + 1 >= ulPatternLen &&
             AnscEqualString2(pNext, pPattern, ulPatternLen, bCaseSensitive) )
        {
            return  pNext;
        }

        pBuf = pNext + 1;
    }

    return  NULL;
}
#endif
//#endif
