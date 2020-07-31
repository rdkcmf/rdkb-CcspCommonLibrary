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


#include "ansc_platform.h"
#include "ansc_utilbox_def.h"
#include "ansc_utilbox_api.h"


static const ANSC_UTILBOX_IANA_MEDIA                g_IanaMedias[]              =
{
    {
        IANA_MEDIA_TYPE_CODE_TEXT,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_TEXT
    },

    {
        IANA_MEDIA_TYPE_CODE_MULTIPART,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_MULTIPART
    },

    {
        IANA_MEDIA_TYPE_CODE_MESSAGE,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_MESSAGE
    },

    {
        IANA_MEDIA_TYPE_CODE_APPLICATION,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_APPLICATION
    },

    {
        IANA_MEDIA_TYPE_CODE_IMAGE,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_IMAGE
    },

    {
        IANA_MEDIA_TYPE_CODE_AUDIO,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_AUDIO
    },

    {
        IANA_MEDIA_TYPE_CODE_VIDEO,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_VIDEO
    },

    {
        IANA_MEDIA_TYPE_CODE_MODEL,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_MODEL
    },

    {
        IANA_MEDIA_TYPE_CODE_STAR,
        (PUCHAR)IANA_MEDIA_TYPE_TEXT_STAR
    },

    {
        IANA_MEDIA_TYPE_CODE_RESERVED,
        NULL
    }
};


static const ANSC_UTILBOX_IANA_MEDIA_SUBTYPE        g_IanaTextSubtypes[]        =
{
    {
        IANA_MT_TX_STYPE_CODE_PLAIN,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_PLAIN
    },

    {
        IANA_MT_TX_STYPE_CODE_RICHTEXT,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_RICHTEXT
    },

    {
        IANA_MT_TX_STYPE_CODE_ENRICHED,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_ENRICHED
    },

    {
        IANA_MT_TX_STYPE_CODE_TAB_SEP_VALUES,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_TAB_SEP_VALUES
    },

    {
        IANA_MT_TX_STYPE_CODE_HTML,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_HTML
    },

    {
        IANA_MT_TX_STYPE_CODE_SGML,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_SGML
    },

    {
        IANA_MT_TX_STYPE_CODE_URL_LIST,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_URL_LIST
    },

    {
        IANA_MT_TX_STYPE_CODE_RFC822_HEADERS,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_RFC822_HEADERS
    },

    {
        IANA_MT_TX_STYPE_CODE_CSS,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_CSS
    },

    {
        IANA_MT_TX_STYPE_CODE_XML,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_XML
    },

    {
        IANA_MT_TX_STYPE_CODE_XML_EPE,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_XML_EPE
    },

    {
        IANA_MT_TX_STYPE_CODE_DIRECTORY,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_DIRECTORY
    },

    {
        IANA_MT_TX_STYPE_CODE_CALENDAR,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_CALENDAR
    },

    {
        IANA_MT_TX_STYPE_CODE_T140,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_T140
    },

    {
        IANA_MT_TX_STYPE_CODE_PARITYFEC,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_TX_STYPE_CODE_JAVASCRIPT,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_JAVASCRIPT
    },

    {
        IANA_MT_TX_STYPE_CODE_X_MSMS_GSINVITE,
        (PUCHAR)IANA_MT_TX_STYPE_TEXT_X_MSMS_GSINVITE
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
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_MIXED
    },

    {
        IANA_MT_MP_STYPE_CODE_ALTERNATIVE,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_ALTERNATIVE
    },

    {
        IANA_MT_MP_STYPE_CODE_DIGEST,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_DIGEST
    },

    {
        IANA_MT_MP_STYPE_CODE_PARALLEL,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_PARALLEL
    },

    {
        IANA_MT_MP_STYPE_CODE_APPLEDOUBLE,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_APPLEDOUBLE
    },

    {
        IANA_MT_MP_STYPE_CODE_HEADER_SET,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_HEADER_SET
    },

    {
        IANA_MT_MP_STYPE_CODE_FORM_DATA,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_FORM_DATA
    },

    {
        IANA_MT_MP_STYPE_CODE_RELATED,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_RELATED
    },

    {
        IANA_MT_MP_STYPE_CODE_REPORTED,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_REPORTED
    },

    {
        IANA_MT_MP_STYPE_CODE_VOICE_MESSAGE,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_VOICE_MESSAGE
    },

    {
        IANA_MT_MP_STYPE_CODE_SIGNED,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_SIGNED
    },

    {
        IANA_MT_MP_STYPE_CODE_ENCRYPTED,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_ENCRYPTED
    },

    {
        IANA_MT_MP_STYPE_CODE_BYTERANGES,
        (PUCHAR)IANA_MT_MP_STYPE_TEXT_BYTERANGES
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
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_RFC822
    },

    {
        IANA_MT_MG_STYPE_CODE_PARTIAL,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_PARTIAL
    },

    {
        IANA_MT_MG_STYPE_CODE_EXTERNAL_BODY,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_EXTERNAL_BODY
    },

    {
        IANA_MT_MG_STYPE_CODE_NEWS,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_NEWS
    },

    {
        IANA_MT_MG_STYPE_CODE_HTTP,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_HTTP
    },

    {
        IANA_MT_MG_STYPE_CODE_DELIVERY_STATUS,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_DELIVERY_STATUS
    },

    {
        IANA_MT_MG_STYPE_CODE_DISPOSITION_NF,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_DISPOSITION_NF
    },

    {
        IANA_MT_MG_STYPE_CODE_S_HTTP,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_S_HTTP
    },

    {
        IANA_MT_MG_STYPE_CODE_SIPFRAG,
        (PUCHAR)IANA_MT_MG_STYPE_TEXT_SIPFRAG
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
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_OCTET_STREAM
    },

    {
        IANA_MT_AP_STYPE_CODE_POSTSCRIPT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_POSTSCRIPT
    },

    {
        IANA_MT_AP_STYPE_CODE_ODA,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ODA
    },

    {
        IANA_MT_AP_STYPE_CODE_ATOMICMAIL,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ATOMICMAIL
    },

    {
        IANA_MT_AP_STYPE_CODE_ANDREW_INSET,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ANDREW_INSET
    },

    {
        IANA_MT_AP_STYPE_CODE_SLATE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_SLATE
    },

    {
        IANA_MT_AP_STYPE_CODE_WITA,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_WITA
    },

    {
        IANA_MT_AP_STYPE_CODE_DEC_DX,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_DEC_DX
    },

    {
        IANA_MT_AP_STYPE_CODE_DCA_RFT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_DCA_RFT
    },

    {
        IANA_MT_AP_STYPE_CODE_ACTIVEMESSAGE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ACTIVEMESSAGE
    },

    {
        IANA_MT_AP_STYPE_CODE_RTF,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_RTF
    },

    {
        IANA_MT_AP_STYPE_CODE_APPLEFILE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_APPLEFILE
    },

    {
        IANA_MT_AP_STYPE_CODE_MAC_BINHEX40,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_MAC_BINHEX40
    },

    {
        IANA_MT_AP_STYPE_CODE_NEWS_MESSAGE_ID,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_NEWS_MESSAGE_ID
    },

    {
        IANA_MT_AP_STYPE_CODE_NEWS_TRANSMISSION,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_NEWS_TRANSMISSION
    },

    {
        IANA_MT_AP_STYPE_CODE_WORDPERFECT51,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_WORDPERFECT51
    },

    {
        IANA_MT_AP_STYPE_CODE_PDF,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PDF
    },

    {
        IANA_MT_AP_STYPE_CODE_ZIP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ZIP
    },

    {
        IANA_MT_AP_STYPE_CODE_MACWRITEII,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_MACWRITEII
    },

    {
        IANA_MT_AP_STYPE_CODE_MSWORD,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_MSWORD
    },

    {
        IANA_MT_AP_STYPE_CODE_REMOTE_PRINTING,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_REMOTE_PRINTING
    },

    {
        IANA_MT_AP_STYPE_CODE_ESHOP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ESHOP
    },

    {
        IANA_MT_AP_STYPE_CODE_X400_BP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X400_BP
    },

    {
        IANA_MT_AP_STYPE_CODE_SGML,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_SGML
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_ENCRYPTED,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PGP_ENCRYPTED
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_SIGNATURE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PGP_SIGNATURE
    },

    {
        IANA_MT_AP_STYPE_CODE_PGP_KEYS,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PGP_KEYS
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS7_MIME,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKCS7_MIME
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS7_SIGNATURE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKCS7_SIGNATURE
    },

    {
        IANA_MT_AP_STYPE_CODE_PKCS10,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKCS10
    },

    {
        IANA_MT_AP_STYPE_CODE_EDIFACT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_EDIFACT
    },

    {
        IANA_MT_AP_STYPE_CODE_EDI_X12,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_EDI_X12
    },

    {
        IANA_MT_AP_STYPE_CODE_EDI_CONSENT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_EDI_CONSENT
    },

    {
        IANA_MT_AP_STYPE_CODE_XML,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_XML_EPE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_XML_EPE
    },

    {
        IANA_MT_AP_STYPE_CODE_XML_DTD,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_XML_DTD
    },

    {
        IANA_MT_AP_STYPE_CODE_BATCH_SMTP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_BATCH_SMTP
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIXCMP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKIXCMP
    },

    {
        IANA_MT_AP_STYPE_CODE_IPP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_IPP
    },

    {
        IANA_MT_AP_STYPE_CODE_OCSP_REQUEST,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_OCSP_REQUEST
    },

    {
        IANA_MT_AP_STYPE_CODE_OCSP_RESPONSE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_OCSP_RESPONSE
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIX_CERT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKIX_CERT
    },

    {
        IANA_MT_AP_STYPE_CODE_PKIX_CRL,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PKIX_CRL
    },

    {
        IANA_MT_AP_STYPE_CODE_HTTP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_HTTP
    },

    {
        IANA_MT_AP_STYPE_CODE_SDP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_SDP
    },

    {
        IANA_MT_AP_STYPE_CODE_IOTP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_IOTP
    },

    {
        IANA_MT_AP_STYPE_CODE_PARITYFEC,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_AP_STYPE_CODE_ISUP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_ISUP
    },

    {
        IANA_MT_AP_STYPE_CODE_QSIG,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_QSIG
    },

    {
        IANA_MT_AP_STYPE_CODE_TIMESTAMP_QUERY,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_TIMESTAMP_QUERY
    },

    {
        IANA_MT_AP_STYPE_CODE_TIMESTAMP_REPLY,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_TIMESTAMP_REPLY
    },

    {
        IANA_MT_AP_STYPE_CODE_REGINFO_XML,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_REGINFO_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_SIMPLE_MSG_SUMMARY,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_SIMPLE_MSG_SUMMARY
    },

    {
        IANA_MT_AP_STYPE_CODE_PIDF_XML,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_PIDF_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_WATCHERINFO_XML,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_WATCHERINFO_XML
    },

    {
        IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_FORM_URLENCODED
    },

    {
        IANA_MT_AP_STYPE_CODE_X_GZIP,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_GZIP
    },

    {
        IANA_MT_AP_STYPE_CODE_X_TAR,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_TAR
    },

    {
        IANA_MT_AP_STYPE_CODE_X_COMPRESSED,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_COMPRESSED
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_ARCHIVE,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_JAVA_ARCHIVE
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_SO,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_JAVA_SO
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVA_VM,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_JAVA_VM
    },

    {
        IANA_MT_AP_STYPE_CODE_X_JAVASCRIPT,
        (PUCHAR)IANA_MT_AP_STYPE_TEXT_X_JAVASCRIPT
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
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_JPEG
    },

    {
        IANA_MT_IM_STYPE_CODE_GIF,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_GIF
    },

    {
        IANA_MT_IM_STYPE_CODE_IEF,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_IEF
    },

    {
        IANA_MT_IM_STYPE_CODE_G3FAX,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_G3FAX
    },

    {
        IANA_MT_IM_STYPE_CODE_TIFF,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_TIFF
    },

    {
        IANA_MT_IM_STYPE_CODE_CGM,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_CGM
    },

    {
        IANA_MT_IM_STYPE_CODE_NAPLPS,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_NAPLPS
    },

    {
        IANA_MT_IM_STYPE_CODE_PNG,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_PNG
    },

    {
        IANA_MT_IM_STYPE_CODE_SVG_XML,
        (PUCHAR)IANA_MT_IM_STYPE_TEXT_SVG_XML
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
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_BASIC
    },

    {
        IANA_MT_AD_STYPE_CODE_32KADPCM,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_32KADPCM
    },

    {
        IANA_MT_AD_STYPE_CODE_L16,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_L16
    },

    {
        IANA_MT_AD_STYPE_CODE_TELEPHONE_EVENT,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_TELEPHONE_EVENT
    },

    {
        IANA_MT_AD_STYPE_CODE_TONE,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_TONE
    },

    {
        IANA_MT_AD_STYPE_CODE_MPEG,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_MPEG
    },

    {
        IANA_MT_AD_STYPE_CODE_PARITYFEC,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_AD_STYPE_CODE_MP4A_LATM,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_MP4A_LATM
    },

    {
        IANA_MT_AD_STYPE_CODE_G722_1,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_G722_1
    },

    {
        IANA_MT_AD_STYPE_CODE_MPA_ROBUST,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_MPA_ROBUST
    },

    {
        IANA_MT_AD_STYPE_CODE_DAT12,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_DAT12
    },

    {
        IANA_MT_AD_STYPE_CODE_L20,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_L20
    },

    {
        IANA_MT_AD_STYPE_CODE_L24,
        (PUCHAR)IANA_MT_AD_STYPE_TEXT_L24
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
        (PUCHAR)IANA_MT_VD_STYPE_TEXT_MPEG
    },

    {
        IANA_MT_VD_STYPE_CODE_QUICKTIME,
        (PUCHAR)IANA_MT_VD_STYPE_TEXT_QUICKTIME
    },

    {
        IANA_MT_VD_STYPE_CODE_POINTER,
        (PUCHAR)IANA_MT_VD_STYPE_TEXT_POINTER
    },

    {
        IANA_MT_VD_STYPE_CODE_PARITYFEC,
        (PUCHAR)IANA_MT_VD_STYPE_TEXT_PARITYFEC
    },

    {
        IANA_MT_VD_STYPE_CODE_MP4V_ES,
        (PUCHAR)IANA_MT_VD_STYPE_TEXT_MP4V_ES
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
        (PUCHAR)IANA_MT_MD_STYPE_TEXT_IGES
    },

    {
        IANA_MT_MD_STYPE_CODE_VRML,
        (PUCHAR)IANA_MT_MD_STYPE_TEXT_VRML
    },

    {
        IANA_MT_MD_STYPE_CODE_MESH,
        (PUCHAR)IANA_MT_MD_STYPE_TEXT_MESH
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


static ULONG
AnscGetIanaTypeByName
    (
        PANSC_UTILBOX_IANA_MEDIA    pSet,
        ULONG                       ulCount,
        PUCHAR                      pName,
        ULONG                       ulNameLen
    )
{
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
                (ulNameLen == AnscSizeOfString((char *)pInfo->Name) && 
                AnscEqualString2((char *)pName, (char *)pInfo->Name, ulNameLen, FALSE)) )
        {
            return pInfo->Type;
        }
    }

    return 0;
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


ULONG
AnscGetIanaMediaTypeByName
    (
        PUCHAR                      pMediaType
    )
{
    return 
        AnscGetIanaTypeByName
            (
                (PANSC_UTILBOX_IANA_MEDIA)g_IanaMedias, 
                sizeof(g_IanaMedias)/sizeof(g_IanaMedias[0]), 
                pMediaType,
                AnscSizeOfString((char *)pMediaType)
            );
}


ULONG
AnscGetIanaMediaTypeByName2
    (
        PUCHAR                      pMediaType,
        ULONG                       ulLen
    )
{
    return 
        AnscGetIanaTypeByName
            (
                (PANSC_UTILBOX_IANA_MEDIA)g_IanaMedias, 
                sizeof(g_IanaMedias)/sizeof(g_IanaMedias[0]), 
                pMediaType,
                ulLen
            );
}


PUCHAR
AnscGetIanaMediaNameByType
    (
        ULONG                       ulMediaType
    )
{
    return 
        AnscGetIanaNameByType
            (
                (PANSC_UTILBOX_IANA_MEDIA)g_IanaMedias, 
                sizeof(g_IanaMedias)/sizeof(g_IanaMedias[0]), 
                ulMediaType
            );
}


ULONG
AnscGetIanaMediaSubtypeByName
    (
        ULONG                       ulMediaType,
        PUCHAR                      pMediaSubtype
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
                        AnscSizeOfString((char *)pMediaSubtype)
                    );
        }
    }

    return 0;
}


ULONG
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


PUCHAR
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


BOOL
AnscIsIanaSubtypeUnknown
    (
        ULONG                       ulMediaType,
        ULONG                       ulMediaSubType
    )
{
    BOOL                            bUnknown    = FALSE;

    switch (ulMediaType)
    {
        case    IANA_MEDIA_TYPE_CODE_TEXT:

                bUnknown = (ulMediaSubType == IANA_MT_TX_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_MULTIPART:
                
                bUnknown = (ulMediaSubType == IANA_MT_MP_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_MESSAGE:
                
                bUnknown = (ulMediaSubType == IANA_MT_MG_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_APPLICATION:
                
                bUnknown = (ulMediaSubType == IANA_MT_AP_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_IMAGE:
                
                bUnknown = (ulMediaSubType == IANA_MT_IM_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_AUDIO:
                
                bUnknown = (ulMediaSubType == IANA_MT_AD_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_VIDEO:
                
                bUnknown = (ulMediaSubType == IANA_MT_VD_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_MODEL:
                
                bUnknown = (ulMediaSubType == IANA_MT_MD_STYPE_CODE_RESERVED);
                
                break;

        case    IANA_MEDIA_TYPE_CODE_STAR:
                
                bUnknown = TRUE;
                
                break;

        default:

                bUnknown = TRUE;

                break;
    }

    return bUnknown;
}


PUCHAR
AnscIsvpGetStringById
    (
        const ANSC_ID_STRING_PAIR   pIsvp[],
        ULONG                       ulIsvpLen,
        ULONG                       Id,
        PUCHAR                      pDefaultValue

    )
{
    ULONG                           i;

    for (i = 0; i < ulIsvpLen; i ++)
    {
        if (pIsvp[i].id == Id)
        {
            return pIsvp[i].string;
        }
    }

    return pDefaultValue;
}


ULONG
AnscIsvpGetIdByString
    (
        const ANSC_ID_STRING_PAIR   pIsvp[],
        ULONG                       ulIsvpLen,
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        BOOL                        bCaseSensitive,
        ULONG                       ulUnmappedValue
    )
{
    ULONG                           i;

    for (i = 0; i < ulIsvpLen; i ++)
    {
        if (AnscSizeOfString((char *)pIsvp[i].string) == ulStrLen &&
            AnscEqualString2((char *)pIsvp[i].string, (char *)pStr, ulStrLen, bCaseSensitive))
        {
            return pIsvp[i].id;
        }
    }

    return ulUnmappedValue;
}

