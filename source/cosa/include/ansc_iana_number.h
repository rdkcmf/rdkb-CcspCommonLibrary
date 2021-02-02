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

    module:	ansc_iana_number.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the unique parameter numbers for
        the Internet Protocol. All the number definitions follow
        the latest RFC of "Assigned Numbers" (as of this writing,
        RFC 1700 is the latest).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_IANA_NUMBER_
#define  _ANSC_IANA_NUMBER_


/***********************************************************
     MEDIA TYPE AND SUBTYPE DEFINITIONS FOR APPLICATIONS
***********************************************************/

/*
 * Quoted from RFC 1512: MIME Part One -
 *
 * "The purpose of the Content-Type field is to describe the data contained in the body fully
 * enough that the receiving user agent can pick an appropriate agent or mechanism to present
 * the data to the user, or otherwise deal with the data in an appropriate manner."
 */
#define  IANA_MEDIA_TYPE_TEXT_TEXT                  "text"
#define  IANA_MEDIA_TYPE_TEXT_MULTIPART             "multipart"
#define  IANA_MEDIA_TYPE_TEXT_MESSAGE               "message"
#define  IANA_MEDIA_TYPE_TEXT_APPLICATION           "application"
#define  IANA_MEDIA_TYPE_TEXT_IMAGE                 "image"
#define  IANA_MEDIA_TYPE_TEXT_AUDIO                 "audio"
#define  IANA_MEDIA_TYPE_TEXT_VIDEO                 "video"
#define  IANA_MEDIA_TYPE_TEXT_MODEL                 "model"
#define  IANA_MEDIA_TYPE_TEXT_STAR                  "*"

#define  IANA_MEDIA_TYPE_CODE_RESERVED              0
#define  IANA_MEDIA_TYPE_CODE_TEXT                  1
#define  IANA_MEDIA_TYPE_CODE_MULTIPART             2
#define  IANA_MEDIA_TYPE_CODE_MESSAGE               3
#define  IANA_MEDIA_TYPE_CODE_APPLICATION           4
#define  IANA_MEDIA_TYPE_CODE_IMAGE                 5
#define  IANA_MEDIA_TYPE_CODE_AUDIO                 6
#define  IANA_MEDIA_TYPE_CODE_VIDEO                 7
#define  IANA_MEDIA_TYPE_CODE_MODEL                 8
#define  IANA_MEDIA_TYPE_CODE_STAR                  0xFFFFFFFF

/*
 * Definitions of "text" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_TX_STYPE_TEXT_PLAIN                "plain"
#define  IANA_MT_TX_STYPE_TEXT_RICHTEXT             "richtext"
#define  IANA_MT_TX_STYPE_TEXT_ENRICHED             "enriched"
#define  IANA_MT_TX_STYPE_TEXT_TAB_SEP_VALUES       "tab-separated-values"
#define  IANA_MT_TX_STYPE_TEXT_HTML                 "html"
#define  IANA_MT_TX_STYPE_TEXT_SGML                 "sgml"
#define  IANA_MT_TX_STYPE_TEXT_URL_LIST             "url-list"
#define  IANA_MT_TX_STYPE_TEXT_RFC822_HEADERS       "rfc822-headers"
#define  IANA_MT_TX_STYPE_TEXT_CSS                  "css"
#define  IANA_MT_TX_STYPE_TEXT_XML                  "xml"
#define  IANA_MT_TX_STYPE_TEXT_XML_EPE              "xml-external-parsed-entity"
#define  IANA_MT_TX_STYPE_TEXT_DIRECTORY            "directory"
#define  IANA_MT_TX_STYPE_TEXT_CALENDAR             "calendar"
#define  IANA_MT_TX_STYPE_TEXT_T140                 "t140"
#define  IANA_MT_TX_STYPE_TEXT_PARITYFEC            "parityfec"
#define  IANA_MT_TX_STYPE_TEXT_JAVASCRIPT           "javascript"
#define  IANA_MT_TX_STYPE_TEXT_X_MSMS_GSINVITE      "x-msmsgsinvite"

#define  IANA_MT_TX_STYPE_CODE_RESERVED             0
#define  IANA_MT_TX_STYPE_CODE_PLAIN                IANA_MT_TX_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_TX_STYPE_CODE_RICHTEXT             IANA_MT_TX_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_TX_STYPE_CODE_ENRICHED             IANA_MT_TX_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_TX_STYPE_CODE_TAB_SEP_VALUES       IANA_MT_TX_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_TX_STYPE_CODE_HTML                 IANA_MT_TX_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_TX_STYPE_CODE_SGML                 IANA_MT_TX_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_TX_STYPE_CODE_URL_LIST             IANA_MT_TX_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_TX_STYPE_CODE_RFC822_HEADERS       IANA_MT_TX_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_TX_STYPE_CODE_CSS                  IANA_MT_TX_STYPE_CODE_RESERVED  + 9
#define  IANA_MT_TX_STYPE_CODE_XML                  IANA_MT_TX_STYPE_CODE_RESERVED  + 10
#define  IANA_MT_TX_STYPE_CODE_XML_EPE              IANA_MT_TX_STYPE_CODE_RESERVED  + 11
#define  IANA_MT_TX_STYPE_CODE_DIRECTORY            IANA_MT_TX_STYPE_CODE_RESERVED  + 12
#define  IANA_MT_TX_STYPE_CODE_CALENDAR             IANA_MT_TX_STYPE_CODE_RESERVED  + 13
#define  IANA_MT_TX_STYPE_CODE_T140                 IANA_MT_TX_STYPE_CODE_RESERVED  + 14
#define  IANA_MT_TX_STYPE_CODE_PARITYFEC            IANA_MT_TX_STYPE_CODE_RESERVED  + 15
#define  IANA_MT_TX_STYPE_CODE_JAVASCRIPT           IANA_MT_TX_STYPE_CODE_RESERVED  + 16
#define  IANA_MT_TX_STYPE_CODE_X_MSMS_GSINVITE      IANA_MT_TX_STYPE_CODE_RESERVED  + 17

/*
 * Definitions of "multipart" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_MP_STYPE_TEXT_MIXED                "mixed"
#define  IANA_MT_MP_STYPE_TEXT_ALTERNATIVE          "alternative"
#define  IANA_MT_MP_STYPE_TEXT_DIGEST               "digest"
#define  IANA_MT_MP_STYPE_TEXT_PARALLEL             "parallel"
#define  IANA_MT_MP_STYPE_TEXT_APPLEDOUBLE          "appledouble"
#define  IANA_MT_MP_STYPE_TEXT_HEADER_SET           "header-set"
#define  IANA_MT_MP_STYPE_TEXT_FORM_DATA            "form-data"
#define  IANA_MT_MP_STYPE_TEXT_RELATED              "related"
#define  IANA_MT_MP_STYPE_TEXT_REPORTED             "reported"
#define  IANA_MT_MP_STYPE_TEXT_VOICE_MESSAGE        "voice-message"
#define  IANA_MT_MP_STYPE_TEXT_SIGNED               "signed"
#define  IANA_MT_MP_STYPE_TEXT_ENCRYPTED            "encrypted"
#define  IANA_MT_MP_STYPE_TEXT_BYTERANGES           "byteranges"

#define  IANA_MT_MP_STYPE_CODE_RESERVED             1000
#define  IANA_MT_MP_STYPE_CODE_MIXED                IANA_MT_MP_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_MP_STYPE_CODE_ALTERNATIVE          IANA_MT_MP_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_MP_STYPE_CODE_DIGEST               IANA_MT_MP_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_MP_STYPE_CODE_PARALLEL             IANA_MT_MP_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_MP_STYPE_CODE_APPLEDOUBLE          IANA_MT_MP_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_MP_STYPE_CODE_HEADER_SET           IANA_MT_MP_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_MP_STYPE_CODE_FORM_DATA            IANA_MT_MP_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_MP_STYPE_CODE_RELATED              IANA_MT_MP_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_MP_STYPE_CODE_REPORTED             IANA_MT_MP_STYPE_CODE_RESERVED  + 9
#define  IANA_MT_MP_STYPE_CODE_VOICE_MESSAGE        IANA_MT_MP_STYPE_CODE_RESERVED  + 10
#define  IANA_MT_MP_STYPE_CODE_SIGNED               IANA_MT_MP_STYPE_CODE_RESERVED  + 11
#define  IANA_MT_MP_STYPE_CODE_ENCRYPTED            IANA_MT_MP_STYPE_CODE_RESERVED  + 12
#define  IANA_MT_MP_STYPE_CODE_BYTERANGES           IANA_MT_MP_STYPE_CODE_RESERVED  + 13

/*
 * Definitions of "message" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_MG_STYPE_TEXT_RFC822               "rfc822"
#define  IANA_MT_MG_STYPE_TEXT_PARTIAL              "partial"
#define  IANA_MT_MG_STYPE_TEXT_EXTERNAL_BODY        "external-body"
#define  IANA_MT_MG_STYPE_TEXT_NEWS                 "news"
#define  IANA_MT_MG_STYPE_TEXT_HTTP                 "http"
#define  IANA_MT_MG_STYPE_TEXT_DELIVERY_STATUS      "delivery-status"
#define  IANA_MT_MG_STYPE_TEXT_DISPOSITION_NF       "disposition-notification"
#define  IANA_MT_MG_STYPE_TEXT_S_HTTP               "s-http"
#define  IANA_MT_MG_STYPE_TEXT_SIPFRAG              "sipfrag"

#define  IANA_MT_MG_STYPE_CODE_RESERVED             2000
#define  IANA_MT_MG_STYPE_CODE_RFC822               IANA_MT_MG_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_MG_STYPE_CODE_PARTIAL              IANA_MT_MG_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_MG_STYPE_CODE_EXTERNAL_BODY        IANA_MT_MG_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_MG_STYPE_CODE_NEWS                 IANA_MT_MG_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_MG_STYPE_CODE_HTTP                 IANA_MT_MG_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_MG_STYPE_CODE_DELIVERY_STATUS      IANA_MT_MG_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_MG_STYPE_CODE_DISPOSITION_NF       IANA_MT_MG_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_MG_STYPE_CODE_S_HTTP               IANA_MT_MG_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_MG_STYPE_CODE_SIPFRAG              IANA_MT_MG_STYPE_CODE_RESERVED  + 9

/*
 * Definitions of "application" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_AP_STYPE_TEXT_OCTET_STREAM         "octet-stream"
#define  IANA_MT_AP_STYPE_TEXT_POSTSCRIPT           "postscript"
#define  IANA_MT_AP_STYPE_TEXT_ODA                  "oda"
#define  IANA_MT_AP_STYPE_TEXT_ATOMICMAIL           "atomicmail"
#define  IANA_MT_AP_STYPE_TEXT_ANDREW_INSET         "andrew-inset"
#define  IANA_MT_AP_STYPE_TEXT_SLATE                "slate"
#define  IANA_MT_AP_STYPE_TEXT_WITA                 "wita"
#define  IANA_MT_AP_STYPE_TEXT_DEC_DX               "dec-dx"
#define  IANA_MT_AP_STYPE_TEXT_DCA_RFT              "dca-rft"
#define  IANA_MT_AP_STYPE_TEXT_ACTIVEMESSAGE        "activemessage"
#define  IANA_MT_AP_STYPE_TEXT_RTF                  "rtf"
#define  IANA_MT_AP_STYPE_TEXT_APPLEFILE            "applefile"
#define  IANA_MT_AP_STYPE_TEXT_MAC_BINHEX40         "mac-binhex40"
#define  IANA_MT_AP_STYPE_TEXT_NEWS_MESSAGE_ID      "news-message-id"
#define  IANA_MT_AP_STYPE_TEXT_NEWS_TRANSMISSION    "news-transmission"
#define  IANA_MT_AP_STYPE_TEXT_WORDPERFECT51        "wordperfect5.1"
#define  IANA_MT_AP_STYPE_TEXT_PDF                  "pdf"
#define  IANA_MT_AP_STYPE_TEXT_ZIP                  "zip"
#define  IANA_MT_AP_STYPE_TEXT_MACWRITEII           "macwriteii"
#define  IANA_MT_AP_STYPE_TEXT_MSWORD               "msword"
#define  IANA_MT_AP_STYPE_TEXT_REMOTE_PRINTING      "remote-printing"
#define  IANA_MT_AP_STYPE_TEXT_ESHOP                "eshop"
#define  IANA_MT_AP_STYPE_TEXT_X400_BP              "x400-bp"
#define  IANA_MT_AP_STYPE_TEXT_SGML                 "sgml"
#define  IANA_MT_AP_STYPE_TEXT_PGP_ENCRYPTED        "pgp-encrypted"
#define  IANA_MT_AP_STYPE_TEXT_PGP_SIGNATURE        "pgp-signature"
#define  IANA_MT_AP_STYPE_TEXT_PGP_KEYS             "pgp-keys"
#define  IANA_MT_AP_STYPE_TEXT_PKCS7_MIME           "pkcs7-mime"
#define  IANA_MT_AP_STYPE_TEXT_PKCS7_SIGNATURE      "pkcs7-signature"
#define  IANA_MT_AP_STYPE_TEXT_PKCS10               "pkcs10"
#define  IANA_MT_AP_STYPE_TEXT_EDIFACT              "EDIFACT"
#define  IANA_MT_AP_STYPE_TEXT_EDI_X12              "EDI-X12"
#define  IANA_MT_AP_STYPE_TEXT_EDI_CONSENT          "EDI-Consent"
#define  IANA_MT_AP_STYPE_TEXT_XML                  "xml"
#define  IANA_MT_AP_STYPE_TEXT_XML_EPE              "xml-external-parsed-entity"
#define  IANA_MT_AP_STYPE_TEXT_XML_DTD              "xml-dtd"
#define  IANA_MT_AP_STYPE_TEXT_BATCH_SMTP           "batch-SMTP"
#define  IANA_MT_AP_STYPE_TEXT_PKIXCMP              "pkixcmp"
#define  IANA_MT_AP_STYPE_TEXT_IPP                  "ipp"
#define  IANA_MT_AP_STYPE_TEXT_OCSP_REQUEST         "ocsp-request"
#define  IANA_MT_AP_STYPE_TEXT_OCSP_RESPONSE        "ocsp-response"
#define  IANA_MT_AP_STYPE_TEXT_PKIX_CERT            "pkix-cert"
#define  IANA_MT_AP_STYPE_TEXT_PKIX_CRL             "pkix-crl"
#define  IANA_MT_AP_STYPE_TEXT_HTTP                 "http"
#define  IANA_MT_AP_STYPE_TEXT_SDP                  "sdp"
#define  IANA_MT_AP_STYPE_TEXT_IOTP                 "iotp"
#define  IANA_MT_AP_STYPE_TEXT_PARITYFEC            "parityfec"
#define  IANA_MT_AP_STYPE_TEXT_ISUP                 "isup"
#define  IANA_MT_AP_STYPE_TEXT_QSIG                 "qsig"
#define  IANA_MT_AP_STYPE_TEXT_TIMESTAMP_QUERY      "timestamp-query"
#define  IANA_MT_AP_STYPE_TEXT_TIMESTAMP_REPLY      "timestamp-reply"
#define  IANA_MT_AP_STYPE_TEXT_REGINFO_XML          "reginfo+xml"
#define  IANA_MT_AP_STYPE_TEXT_SIMPLE_MSG_SUMMARY   "simple-message-summary"
#define  IANA_MT_AP_STYPE_TEXT_PIDF_XML             "pidf_xml"
#define  IANA_MT_AP_STYPE_TEXT_WATCHERINFO_XML      "watcherinfo+xml"

#define  IANA_MT_AP_STYPE_TEXT_X_FORM_URLENCODED    "x-www-form-urlencoded"
#define  IANA_MT_AP_STYPE_TEXT_X_GZIP               "x-gzip"
#define  IANA_MT_AP_STYPE_TEXT_X_TAR                "x-tar"
#define  IANA_MT_AP_STYPE_TEXT_X_COMPRESSED         "x-compressed"
#define  IANA_MT_AP_STYPE_TEXT_X_JAVA_ARCHIVE       "x-java-archive"
#define  IANA_MT_AP_STYPE_TEXT_X_JAVA_SO            "x-java-serialized-object"
#define  IANA_MT_AP_STYPE_TEXT_X_JAVA_VM            "x-java-vm"
#define  IANA_MT_AP_STYPE_TEXT_X_JAVASCRIPT         "x-javascript"

#define  IANA_MT_AP_STYPE_CODE_RESERVED             3000
#define  IANA_MT_AP_STYPE_CODE_OCTET_STREAM         IANA_MT_AP_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_AP_STYPE_CODE_POSTSCRIPT           IANA_MT_AP_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_AP_STYPE_CODE_ODA                  IANA_MT_AP_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_AP_STYPE_CODE_ATOMICMAIL           IANA_MT_AP_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_AP_STYPE_CODE_ANDREW_INSET         IANA_MT_AP_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_AP_STYPE_CODE_SLATE                IANA_MT_AP_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_AP_STYPE_CODE_WITA                 IANA_MT_AP_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_AP_STYPE_CODE_DEC_DX               IANA_MT_AP_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_AP_STYPE_CODE_DCA_RFT              IANA_MT_AP_STYPE_CODE_RESERVED  + 9
#define  IANA_MT_AP_STYPE_CODE_ACTIVEMESSAGE        IANA_MT_AP_STYPE_CODE_RESERVED  + 10
#define  IANA_MT_AP_STYPE_CODE_RTF                  IANA_MT_AP_STYPE_CODE_RESERVED  + 11
#define  IANA_MT_AP_STYPE_CODE_APPLEFILE            IANA_MT_AP_STYPE_CODE_RESERVED  + 12
#define  IANA_MT_AP_STYPE_CODE_MAC_BINHEX40         IANA_MT_AP_STYPE_CODE_RESERVED  + 13
#define  IANA_MT_AP_STYPE_CODE_NEWS_MESSAGE_ID      IANA_MT_AP_STYPE_CODE_RESERVED  + 14
#define  IANA_MT_AP_STYPE_CODE_NEWS_TRANSMISSION    IANA_MT_AP_STYPE_CODE_RESERVED  + 15
#define  IANA_MT_AP_STYPE_CODE_WORDPERFECT51        IANA_MT_AP_STYPE_CODE_RESERVED  + 16
#define  IANA_MT_AP_STYPE_CODE_PDF                  IANA_MT_AP_STYPE_CODE_RESERVED  + 17
#define  IANA_MT_AP_STYPE_CODE_ZIP                  IANA_MT_AP_STYPE_CODE_RESERVED  + 18
#define  IANA_MT_AP_STYPE_CODE_MACWRITEII           IANA_MT_AP_STYPE_CODE_RESERVED  + 19
#define  IANA_MT_AP_STYPE_CODE_MSWORD               IANA_MT_AP_STYPE_CODE_RESERVED  + 20
#define  IANA_MT_AP_STYPE_CODE_REMOTE_PRINTING      IANA_MT_AP_STYPE_CODE_RESERVED  + 21
#define  IANA_MT_AP_STYPE_CODE_ESHOP                IANA_MT_AP_STYPE_CODE_RESERVED  + 22
#define  IANA_MT_AP_STYPE_CODE_X400_BP              IANA_MT_AP_STYPE_CODE_RESERVED  + 23
#define  IANA_MT_AP_STYPE_CODE_SGML                 IANA_MT_AP_STYPE_CODE_RESERVED  + 24
#define  IANA_MT_AP_STYPE_CODE_PGP_ENCRYPTED        IANA_MT_AP_STYPE_CODE_RESERVED  + 25
#define  IANA_MT_AP_STYPE_CODE_PGP_SIGNATURE        IANA_MT_AP_STYPE_CODE_RESERVED  + 26
#define  IANA_MT_AP_STYPE_CODE_PGP_KEYS             IANA_MT_AP_STYPE_CODE_RESERVED  + 27
#define  IANA_MT_AP_STYPE_CODE_PKCS7_MIME           IANA_MT_AP_STYPE_CODE_RESERVED  + 28
#define  IANA_MT_AP_STYPE_CODE_PKCS7_SIGNATURE      IANA_MT_AP_STYPE_CODE_RESERVED  + 29
#define  IANA_MT_AP_STYPE_CODE_PKCS10               IANA_MT_AP_STYPE_CODE_RESERVED  + 30
#define  IANA_MT_AP_STYPE_CODE_EDIFACT              IANA_MT_AP_STYPE_CODE_RESERVED  + 31
#define  IANA_MT_AP_STYPE_CODE_EDI_X12              IANA_MT_AP_STYPE_CODE_RESERVED  + 32
#define  IANA_MT_AP_STYPE_CODE_EDI_CONSENT          IANA_MT_AP_STYPE_CODE_RESERVED  + 33
#define  IANA_MT_AP_STYPE_CODE_XML                  IANA_MT_AP_STYPE_CODE_RESERVED  + 34
#define  IANA_MT_AP_STYPE_CODE_XML_EPE              IANA_MT_AP_STYPE_CODE_RESERVED  + 35
#define  IANA_MT_AP_STYPE_CODE_XML_DTD              IANA_MT_AP_STYPE_CODE_RESERVED  + 36
#define  IANA_MT_AP_STYPE_CODE_BATCH_SMTP           IANA_MT_AP_STYPE_CODE_RESERVED  + 37
#define  IANA_MT_AP_STYPE_CODE_PKIXCMP              IANA_MT_AP_STYPE_CODE_RESERVED  + 38
#define  IANA_MT_AP_STYPE_CODE_IPP                  IANA_MT_AP_STYPE_CODE_RESERVED  + 39
#define  IANA_MT_AP_STYPE_CODE_OCSP_REQUEST         IANA_MT_AP_STYPE_CODE_RESERVED  + 40
#define  IANA_MT_AP_STYPE_CODE_OCSP_RESPONSE        IANA_MT_AP_STYPE_CODE_RESERVED  + 41
#define  IANA_MT_AP_STYPE_CODE_PKIX_CERT            IANA_MT_AP_STYPE_CODE_RESERVED  + 42
#define  IANA_MT_AP_STYPE_CODE_PKIX_CRL             IANA_MT_AP_STYPE_CODE_RESERVED  + 43
#define  IANA_MT_AP_STYPE_CODE_HTTP                 IANA_MT_AP_STYPE_CODE_RESERVED  + 44
#define  IANA_MT_AP_STYPE_CODE_SDP                  IANA_MT_AP_STYPE_CODE_RESERVED  + 45
#define  IANA_MT_AP_STYPE_CODE_IOTP                 IANA_MT_AP_STYPE_CODE_RESERVED  + 46
#define  IANA_MT_AP_STYPE_CODE_PARITYFEC            IANA_MT_AP_STYPE_CODE_RESERVED  + 47
#define  IANA_MT_AP_STYPE_CODE_ISUP                 IANA_MT_AP_STYPE_CODE_RESERVED  + 48
#define  IANA_MT_AP_STYPE_CODE_QSIG                 IANA_MT_AP_STYPE_CODE_RESERVED  + 49
#define  IANA_MT_AP_STYPE_CODE_TIMESTAMP_QUERY      IANA_MT_AP_STYPE_CODE_RESERVED  + 50
#define  IANA_MT_AP_STYPE_CODE_TIMESTAMP_REPLY      IANA_MT_AP_STYPE_CODE_RESERVED  + 51
#define  IANA_MT_AP_STYPE_CODE_REGINFO_XML          IANA_MT_AP_STYPE_CODE_RESERVED  + 52
#define  IANA_MT_AP_STYPE_CODE_SIMPLE_MSG_SUMMARY   IANA_MT_AP_STYPE_CODE_RESERVED  + 53
#define  IANA_MT_AP_STYPE_CODE_PIDF_XML             IANA_MT_AP_STYPE_CODE_RESERVED  + 54
#define  IANA_MT_AP_STYPE_CODE_WATCHERINFO_XML      IANA_MT_AP_STYPE_CODE_RESERVED  + 55

#define  IANA_MT_AP_STYPE_CODE_EXTENSION            IANA_MT_AP_STYPE_CODE_RESERVED  + 256
#define  IANA_MT_AP_STYPE_CODE_X_FORM_URLENCODED    IANA_MT_AP_STYPE_CODE_EXTENSION + 1
#define  IANA_MT_AP_STYPE_CODE_X_GZIP               IANA_MT_AP_STYPE_CODE_EXTENSION + 2
#define  IANA_MT_AP_STYPE_CODE_X_TAR                IANA_MT_AP_STYPE_CODE_EXTENSION + 3
#define  IANA_MT_AP_STYPE_CODE_X_COMPRESSED         IANA_MT_AP_STYPE_CODE_EXTENSION + 4
#define  IANA_MT_AP_STYPE_CODE_X_JAVA_ARCHIVE       IANA_MT_AP_STYPE_CODE_EXTENSION + 5
#define  IANA_MT_AP_STYPE_CODE_X_JAVA_SO            IANA_MT_AP_STYPE_CODE_EXTENSION + 6
#define  IANA_MT_AP_STYPE_CODE_X_JAVA_VM            IANA_MT_AP_STYPE_CODE_EXTENSION + 7
#define  IANA_MT_AP_STYPE_CODE_X_JAVASCRIPT         IANA_MT_AP_STYPE_CODE_EXTENSION + 8

/*
 * Definitions of "image" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_IM_STYPE_TEXT_JPEG                 "jpeg"
#define  IANA_MT_IM_STYPE_TEXT_GIF                  "gif"
#define  IANA_MT_IM_STYPE_TEXT_IEF                  "ief"
#define  IANA_MT_IM_STYPE_TEXT_G3FAX                "g3fax"
#define  IANA_MT_IM_STYPE_TEXT_TIFF                 "tiff"
#define  IANA_MT_IM_STYPE_TEXT_CGM                  "cgm"
#define  IANA_MT_IM_STYPE_TEXT_NAPLPS               "naplps"
#define  IANA_MT_IM_STYPE_TEXT_PNG                  "png"
#define  IANA_MT_IM_STYPE_TEXT_SVG_XML              "svg+xml"

#define  IANA_MT_IM_STYPE_CODE_RESERVED             4000
#define  IANA_MT_IM_STYPE_CODE_JPEG                 IANA_MT_IM_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_IM_STYPE_CODE_GIF                  IANA_MT_IM_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_IM_STYPE_CODE_IEF                  IANA_MT_IM_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_IM_STYPE_CODE_G3FAX                IANA_MT_IM_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_IM_STYPE_CODE_TIFF                 IANA_MT_IM_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_IM_STYPE_CODE_CGM                  IANA_MT_IM_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_IM_STYPE_CODE_NAPLPS               IANA_MT_IM_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_IM_STYPE_CODE_PNG                  IANA_MT_IM_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_IM_STYPE_CODE_SVG_XML              IANA_MT_IM_STYPE_CODE_RESERVED  + 9

/*
 * Definitions of "audio" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_AD_STYPE_TEXT_BASIC                "basic"
#define  IANA_MT_AD_STYPE_TEXT_32KADPCM             "32kadpcm"
#define  IANA_MT_AD_STYPE_TEXT_L16                  "L16"
#define  IANA_MT_AD_STYPE_TEXT_TELEPHONE_EVENT      "telephone-event"
#define  IANA_MT_AD_STYPE_TEXT_TONE                 "tone"
#define  IANA_MT_AD_STYPE_TEXT_MPEG                 "mpeg"
#define  IANA_MT_AD_STYPE_TEXT_PARITYFEC            "parityfec"
#define  IANA_MT_AD_STYPE_TEXT_MP4A_LATM            "MP4A-LATM"
#define  IANA_MT_AD_STYPE_TEXT_G722_1               "G.722.1"
#define  IANA_MT_AD_STYPE_TEXT_MPA_ROBUST           "mpa-robust"
#define  IANA_MT_AD_STYPE_TEXT_DAT12                "DAT12"
#define  IANA_MT_AD_STYPE_TEXT_L20                  "L20"
#define  IANA_MT_AD_STYPE_TEXT_L24                  "L24"

#define  IANA_MT_AD_STYPE_CODE_RESERVED             5000
#define  IANA_MT_AD_STYPE_CODE_BASIC                IANA_MT_AD_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_AD_STYPE_CODE_32KADPCM             IANA_MT_AD_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_AD_STYPE_CODE_L16                  IANA_MT_AD_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_AD_STYPE_CODE_TELEPHONE_EVENT      IANA_MT_AD_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_AD_STYPE_CODE_TONE                 IANA_MT_AD_STYPE_CODE_RESERVED  + 5
#define  IANA_MT_AD_STYPE_CODE_MPEG                 IANA_MT_AD_STYPE_CODE_RESERVED  + 6
#define  IANA_MT_AD_STYPE_CODE_PARITYFEC            IANA_MT_AD_STYPE_CODE_RESERVED  + 7
#define  IANA_MT_AD_STYPE_CODE_MP4A_LATM            IANA_MT_AD_STYPE_CODE_RESERVED  + 8
#define  IANA_MT_AD_STYPE_CODE_G722_1               IANA_MT_AD_STYPE_CODE_RESERVED  + 9
#define  IANA_MT_AD_STYPE_CODE_MPA_ROBUST           IANA_MT_AD_STYPE_CODE_RESERVED  + 10
#define  IANA_MT_AD_STYPE_CODE_DAT12                IANA_MT_AD_STYPE_CODE_RESERVED  + 11
#define  IANA_MT_AD_STYPE_CODE_L20                  IANA_MT_AD_STYPE_CODE_RESERVED  + 12
#define  IANA_MT_AD_STYPE_CODE_L24                  IANA_MT_AD_STYPE_CODE_RESERVED  + 13

/*
 * Definitions of "video" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_VD_STYPE_TEXT_MPEG                 "mpeg"
#define  IANA_MT_VD_STYPE_TEXT_QUICKTIME            "quicktime"
#define  IANA_MT_VD_STYPE_TEXT_POINTER              "pointer"
#define  IANA_MT_VD_STYPE_TEXT_PARITYFEC            "parityfec"
#define  IANA_MT_VD_STYPE_TEXT_MP4V_ES              "MP4V-ES"

#define  IANA_MT_VD_STYPE_CODE_RESERVED             6000
#define  IANA_MT_VD_STYPE_CODE_MPEG                 IANA_MT_VD_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_VD_STYPE_CODE_QUICKTIME            IANA_MT_VD_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_VD_STYPE_CODE_POINTER              IANA_MT_VD_STYPE_CODE_RESERVED  + 3
#define  IANA_MT_VD_STYPE_CODE_PARITYFEC            IANA_MT_VD_STYPE_CODE_RESERVED  + 4
#define  IANA_MT_VD_STYPE_CODE_MP4V_ES              IANA_MT_VD_STYPE_CODE_RESERVED  + 5

/*
 * Definitions of "model" sub-types as defined by the latest document at
 *
 *      http://www.isi.edu/in-notes/iana/assignments/media-types/media-types
 */
#define  IANA_MT_MD_STYPE_TEXT_IGES                 "iges"
#define  IANA_MT_MD_STYPE_TEXT_VRML                 "vrml"
#define  IANA_MT_MD_STYPE_TEXT_MESH                 "mesh"

#define  IANA_MT_MD_STYPE_CODE_RESERVED             7000
#define  IANA_MT_MD_STYPE_CODE_IGES                 IANA_MT_MD_STYPE_CODE_RESERVED  + 1
#define  IANA_MT_MD_STYPE_CODE_VRML                 IANA_MT_MD_STYPE_CODE_RESERVED  + 2
#define  IANA_MT_MD_STYPE_CODE_MESH                 IANA_MT_MD_STYPE_CODE_RESERVED  + 3


/***********************************************************
         CHARACTER SET DEFINITIONS FOR APPLICATIONS
***********************************************************/

/*
 * The term "character set" is used in many protocol specifications to refer to a method used with
 * one or more tables to convert a sequence of octets into a sequence of characters. Note that
 * unconditional conversion in the other direction is not required, in that not all characters may
 * be available in a given character set and a character set may provide more than one sequence of
 * octets to represent a particular character. This definition is intended to allow various kinds
 * of character encoding, from simple single-table mappings such as US-ASCII to complex table
 * switching methods such as those that use ISO-2022's techniques. However, the definition
 * associated with a MIME character set name MUST fully specify the mapping to be performed from
 * octets to characters. In particular, use of external profiling information to determine the
 * exact mapping is not permitted.
 */
#define  IANA_CHARSET_TEXT_US_ASCII                 "US-ASCII"
#define  IANA_CHARSET_TEXT_ISO_8859_1               "ISO-8859-1"
#define  IANA_CHARSET_TEXT_ISO_8859_2               "ISO-8859-2"
#define  IANA_CHARSET_TEXT_ISO_8859_3               "ISO-8859-3"
#define  IANA_CHARSET_TEXT_ISO_8859_4               "ISO-8859-4"
#define  IANA_CHARSET_TEXT_ISO_8859_5               "ISO-8859-5"
#define  IANA_CHARSET_TEXT_ISO_8859_6               "ISO-8859-6"
#define  IANA_CHARSET_TEXT_ISO_8859_7               "ISO-8859-7"
#define  IANA_CHARSET_TEXT_ISO_8859_8               "ISO-8859-8"
#define  IANA_CHARSET_TEXT_ISO_8859_9               "ISO-8859-9"

#define  IANA_CHARSET_CODE_RESERVED                 0
#define  IANA_CHARSET_CODE_US_ASCII                 1
#define  IANA_CHARSET_CODE_ISO_8859_1               2
#define  IANA_CHARSET_CODE_ISO_8859_2               3
#define  IANA_CHARSET_CODE_ISO_8859_3               4
#define  IANA_CHARSET_CODE_ISO_8859_4               5
#define  IANA_CHARSET_CODE_ISO_8859_5               6
#define  IANA_CHARSET_CODE_ISO_8859_6               7
#define  IANA_CHARSET_CODE_ISO_8859_7               8
#define  IANA_CHARSET_CODE_ISO_8859_8               9
#define  IANA_CHARSET_CODE_ISO_8859_9               10


/***********************************************************
          CONTENT-CODING DEFINITIONS FOR APPLICATIONS
***********************************************************/

/*
 * Content coding values indicate an encoding transformation that has been or can be applied to an
 * entity. Content codings are primarily used to allow a document to be compressed or otherwise
 * usefully transformed without losing the identity of its underlying media type and without loss
 * of information. Frequently, the entity is stored in coded form, transmitted directly, and only
 * decoded by the recipient.
 */
#define  IANA_CCODING_TEXT_GZIP                     "gzip"
#define  IANA_CCODING_TEXT_COMPRESS                 "compress"
#define  IANA_CCODING_TEXT_DEFLATE                  "deflate"
#define  IANA_CCODING_TEXT_IDENTITY                 "identity"

#define  IANA_CCODING_CODE_RESERVED                 0
#define  IANA_CCODING_CODE_GZIP                     1
#define  IANA_CCODING_CODE_COMPRESS                 2
#define  IANA_CCODING_CODE_DEFLATE                  3
#define  IANA_CCODING_CODE_IDENTITY                 4


/***********************************************************
         TRANSFER-CODING DEFINITIONS FOR APPLICATIONS
***********************************************************/

/*
 * Transfer-coding values are used to indicate an encoding transformation that has been, can be,
 * or may need to be applied to an entity-body in order to ensure "safe transport" throught the
 * network. This differs from a content coding in that the transfer-coding is a property of the
 * message, not of the original entity.
 */
#define  IANA_TCODING_TEXT_GZIP                     "gzip"
#define  IANA_TCODING_TEXT_COMPRESS                 "compress"
#define  IANA_TCODING_TEXT_DEFLATE                  "deflate"
#define  IANA_TCODING_TEXT_IDENTITY                 "identity"
#define  IANA_TCODING_TEXT_CHUNKED                  "chunked"
#define  IANA_TCODING_TEXT_TRAILERS                 "trailers"

#define  IANA_TCODING_CODE_RESERVED                 0
#define  IANA_TCODING_CODE_GZIP                     1
#define  IANA_TCODING_CODE_COMPRESS                 2
#define  IANA_TCODING_CODE_DEFLATE                  3
#define  IANA_TCODING_CODE_IDENTITY                 4
#define  IANA_TCODING_CODE_CHUNKED                  5
#define  IANA_TCODING_CODE_TRAILERS                 6


/***********************************************************
     PERSISTENT CONNECTION DEFINITIONS FOR APPLICATIONS
***********************************************************/

/*
 * HTTP/1.1 proxies MUST parse the Connection header field before a message is forwarded and, for
 * each connection-token in this field, remove any header field(s) from the message with the same
 * name as the connection-token. Connection options are signaled by the presence of a connection-
 * token in the Connection header field, not by any corresponding additional header field(s), since
 * the additional header field may not be sent if there're no parameters associated with that con-
 * nection option.
 */
#define  IANA_CONNECTION_TEXT_CLOSE                 "close"
#define  IANA_CONNECTION_TEXT_KEEP_ALIVE            "Keep-Alive"

#define  IANA_CONNECTION_CODE_RESERVED              0
#define  IANA_CONNECTION_CODE_CLOSE                 1
#define  IANA_CONNECTION_CODE_KEEP_ALIVE            2


/***********************************************************
      PERMANENT INTERNET MULTICAST ADDRESS DEFINITIONS
***********************************************************/

/*
 * Host Extensions for IP Multicasting [RFC1112] specifies the extensions required of a host
 * implementation of the Internet Protocol (IP) to support multicasting. Current addresses are
 * listed below. Note: all static IP address value appeared in macros and functions MUST be
 * defined in the network order.
 */
#if defined(_ANSC_LITTLE_ENDIAN_)

    #define  IPV4_MCAST_ADDR_BASE                   0x000000E0      /* 224.0.0.0  */
    #define  IPV4_MCAST_ADDR_ALL_SYSTEMS            0x010000E0      /* 224.0.0.1  */
    #define  IPV4_MCAST_ADDR_ALL_ROUTERS            0x020000E0      /* 224.0.0.2  */
    #define  IPV4_MCAST_ADDR_DVMRP_ROUTERS          0x040000E0      /* 224.0.0.4  */
    #define  IPV4_MCAST_ADDR_OSPFIGP_ALL_ROUTERS    0x050000E0      /* 224.0.0.5  */
    #define  IPV4_MCAST_ADDR_OSPFIGP_DSG_ROUTERS    0x060000E0      /* 224.0.0.6  */
    #define  IPV4_MCAST_ADDR_ST_ROUTERS             0x070000E0      /* 224.0.0.7  */
    #define  IPV4_MCAST_ADDR_ST_HOSTS               0x080000E0      /* 224.0.0.8  */
    #define  IPV4_MCAST_ADDR_RIP2_ROUTERS           0x090000E0      /* 224.0.0.9  */
    #define  IPV4_MCAST_ADDR_IGRP_ROUTERS           0x0A0000E0      /* 224.0.0.10 */
    #define  IPV4_MCAST_ADDR_MOBILE_AGENTS          0x0B0000E0      /* 224.0.0.11 */
    #define  IPV4_MCAST_ADDR_IGMP_ROUTERS           0x160000E0      /* 224.0.0.22 */

#else

    #define  IPV4_MCAST_ADDR_BASE                   0xE0000000      /* 224.0.0.0  */
    #define  IPV4_MCAST_ADDR_ALL_SYSTEMS            0xE0000001      /* 224.0.0.1  */
    #define  IPV4_MCAST_ADDR_ALL_ROUTERS            0xE0000002      /* 224.0.0.2  */
    #define  IPV4_MCAST_ADDR_DVMRP_ROUTERS          0xE0000003      /* 224.0.0.4  */
    #define  IPV4_MCAST_ADDR_OSPFIGP_ALL_ROUTERS    0xE0000005      /* 224.0.0.5  */
    #define  IPV4_MCAST_ADDR_OSPFIGP_DSG_ROUTERS    0xE0000006      /* 224.0.0.6  */
    #define  IPV4_MCAST_ADDR_ST_ROUTERS             0xE0000007      /* 224.0.0.7  */
    #define  IPV4_MCAST_ADDR_ST_HOSTS               0xE0000008      /* 224.0.0.8  */
    #define  IPV4_MCAST_ADDR_RIP2_ROUTERS           0xE0000009      /* 224.0.0.9  */
    #define  IPV4_MCAST_ADDR_IGRP_ROUTERS           0xE000000A      /* 224.0.0.10 */
    #define  IPV4_MCAST_ADDR_MOBILE_AGENTS          0xE000000B      /* 224.0.0.11 */
    #define  IPV4_MCAST_ADDR_IGMP_ROUTERS           0xE0000016      /* 224.0.0.22 */

#endif


#endif
