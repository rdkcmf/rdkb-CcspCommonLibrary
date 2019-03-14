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

    module:	http_mp_co_util_def.h

        For HTTP message parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the definitions of HTTP
        production names.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/25/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/25/02

**********************************************************************/

#ifndef  _ANSC_HTTPMP_CO_UTIL_DEF
#define  _ANSC_HTTPMP_CO_UTIL_DEF

/*
 * Define the HTTP production names and some structures which are used
 * for collecting information from token tree node object.
 */
#define  HTTP_COOKIE_MAX_AGE_ABSENT                 (ULONG)-1

BOOL
AnscHttpTokenIs
    (
        ANSC_LEX_STRING             tokenName,
        ANSC_LEX_STRING             httpToken
    );

int
AnscStringFindChar
    (
        ANSC_LEX_STRING             pString,
        UCHAR                       aChar
    );

int
AnscMemFindChar
    (
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        UCHAR                       aChar,
        BOOL                        bCaseSensitive
    );

int
AnscStringFindStr
    (
        ANSC_LEX_STRING             pString,
        ANSC_LEX_STRING             pSubString
    );


ULONG
AnscHexString2Ulong
    (
        PUCHAR                      pHexString
    );

ANSC_STATUS
AnscDecodeUrlEncode
    (
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        PUCHAR                      pDecodedStr,
        PULONG                      pulDecodedStrLen
    );

ANSC_STATUS
AnscGetMultipartPart
    (
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        PUCHAR                      pBoundary,
        ANSC_HANDLE                 hPart
    );

ULONG
AnscHttpUtilGetMediaType
    (
        ANSC_LEX_STRING             pType
    );

ULONG
AnscHttpUtilGetMediaSubType
    (
        ANSC_LEX_STRING             pSubType
    );


#define  ANSC_TOKEN_NAME_IS(tokenName, httpToken)   AnscHttpTokenIs(tokenName, httpToken)


#define  ANSC_HTTP_REQUEST_LINE                     "Request-Line-No-CRLF"
#define  ANSC_HTTP_REQUEST_LINE_LENGTH              AnscSizeOfString(ANSC_HTTP_REQUEST_LINE)

#define  ANSC_HTTP_STATUS_LINE                      "Status-Line-No-CRLF"
#define  ANSC_HTTP_STATUS_LINE_LENGTH               AnscSizeOfString(ANSC_HTTP_STATUS_LINE)

#define  ANSC_HTTP_GENERAL_HEADER                   "general-header"
#define  ANSC_HTTP_GENERAL_HEADER_LENGTH            AnscSizeOfStringANSC_HTTP_GENERAL_HEADER()

#define  ANSC_HTTP_REQUEST_HEADER                   "request-header"
#define  ANSC_HTTP_REQUEST_HEADER_LENGTH            AnscSizeOfString(ANSC_HTTP_REQUEST_HEADER)

#define  ANSC_HTTP_RESPONSE_HEADER                  "response-header"
#define  ANSC_HTTP_RESPONSE_HEADER_LENGTH           AnscSizeOfString(ANSC_HTTP_RESPONSE_HEADER)

#define  ANSC_HTTP_ENTITY_HEADER                    "entity-header"
#define  ANSC_HTTP_ENTITY_HEADER_LENGTH             AnscSizeOfString(ANSC_HTTP_ENTITY_HEADER)

#define  ANSC_HTTP_EXTENSION_HEADER                 "extension-header"
#define  ANSC_HTTP_EXTENSION_HEADER_LENGTH          AnscSizeOfString(ANSC_HTTP_EXTENSION_HEADER)

#define  ANSC_HTTP_METHOD                           "Method"
#define  ANSC_HTTP_METHOD_LENGTH                    AnscSizeOfString(ANSC_HTTP_METHOD)      

#define  ANSC_HTTP_REQUEST_URI                      "Request-URI"
#define  ANSC_HTTP_REQUEST_URI_LENGTH               AnscSizeOfString(ANSC_HTTP_REQUEST_URI)

#define  ANSC_HTTP_VERSION                          "HTTP-Version"
#define  ANSC_HTTP_VERSION_LENGTH                   AnscSizeOfString(ANSC_HTTP_VERSION)  

#define  ANSC_HTTP_STATUS_CODE                      "Status-Code"
#define  ANSC_HTTP_STATUS_CODE_LENGTH               AnscSizeOfString(ANSC_HTTP_STATUS_CODE)

#define  ANSC_HTTP_REASON_PHRASE                    "Reason-Phrase"
#define  ANSC_HTTP_REASON_PHRASE_LENGTH             AnscSizeOfString(ANSC_HTTP_REASON_PHRASE)

#define  ANSC_HTTP_DIGIT                            "DIGIT"
#define  ANSC_HTTP_DIGIT_LENGTH                     AnscSizeOfString(ANSC_HTTP_DIGIT)

#define  ANSC_HTTP_METHOD_OPTIONS                   "OPTIONS"
#define  ANSC_HTTP_METHOD_OPTIONS_LENGTH            AnscSizeOfString(ANSC_HTTP_METHOD_OPTIONS)

#define  ANSC_HTTP_METHOD_GET                       "GET"
#define  ANSC_HTTP_METHOD_GET_LENGTH                AnscSizeOfString(ANSC_HTTP_METHOD_GET)

#define  ANSC_HTTP_METHOD_HEAD                      "HEAD"
#define  ANSC_HTTP_METHOD_HEAD_LENGTH               AnscSizeOfString(ANSC_HTTP_METHOD_HEAD)

#define  ANSC_HTTP_METHOD_POST                      "POST"
#define  ANSC_HTTP_METHOD_POST_LENGTH               AnscSizeOfString(ANSC_HTTP_METHOD_POST)              

#define  ANSC_HTTP_METHOD_PUT                       "PUT"
#define  ANSC_HTTP_METHOD_PUT_LENGTH                AnscSizeOfString(ANSC_HTTP_METHOD_PUT)

#define  ANSC_HTTP_METHOD_DELETE                    "DELETE"
#define  ANSC_HTTP_METHOD_DELETE_LENGTH             AnscSizeOfString(ANSC_HTTP_METHOD_DELETE)

#define  ANSC_HTTP_METHOD_TRACE                     "TRACE"
#define  ANSC_HTTP_METHOD_TRACE_LENGTH              AnscSizeOfString(ANSC_HTTP_METHOD_TRACE)

#define  ANSC_HTTP_METHOD_CONNECT                   "CONNECT"
#define  ANSC_HTTP_METHOD_CONNECT_LENGTH            AnscSizeOfString(ANSC_HTTP_METHOD_CONNECT)

#define  ANSC_HTTP_REQUEST_URI_ASTERISK             "*"
#define  ANSC_HTTP_REQUEST_URI_ASTERISK_LENGTH      AnscSizeOfString(ANSC_HTTP_REQUEST_URI_ASTERISK)

#define  ANSC_HTTP_ABSOLUTE_URI                     "absoluteURI"
#define  ANSC_HTTP_ABSOLUTE_URI_LENGTH              AnscSizeOfString(ANSC_HTTP_ABSOLUTE_URI)

#define  ANSC_HTTP_ABS_PATH                         "abs_path"
#define  ANSC_HTTP_ABS_PATH_LENGTH                  AnscSizeOfString(ANSC_HTTP_ABS_PATH)

#define  ANSC_HTTP_AUTHORITY                        "authority"
#define  ANSC_HTTP_AUTHORITY_LENGTH                 AnscSizeOfString(ANSC_HTTP_AUTHORITY)

#define  ANSC_HTTP_REL_PATH                         "rel_path"
#define  ANSC_HTTP_REL_PATH_LENGTH                  AnscSizeOfString(ANSC_HTTP_REL_PATH)

#define  ANSC_HTTP_PATH                             "path"
#define  ANSC_HTTP_PATH_LENGTH                      AnscSizeOfString(ANSC_HTTP_PATH)

#define  ANSC_HTTP_QUERY                            "query"
#define  ANSC_HTTP_QUERY_LENGTH                     AnscSizeOfString(ANSC_HTTP_QUERY)

#define  ANSC_HTTP_FSEGMENT                         "fsegment"
#define  ANSC_HTTP_FSEGMENT_LENGTH                  AnscSizeOfString(ANSC_HTTP_FSEGMENT)

#define  ANSC_HTTP_SEGMENT                          "segment"
#define  ANSC_HTTP_SEGMENT_LENGTH                   AnscSizeOfString(ANSC_HTTP_SEGMENT)

#define  ANSC_HTTP_AUTHORITY_SERVER                 "authority-server"          
#define  ANSC_HTTP_AUTHORITY_SERVER_LENGTH          AnscSizeOfString(ANSC_HTTP_AUTHORITY_SERVER)

#define  ANSC_HTTP_REG_NAME                         "reg_name"
#define  ANSC_HTTP_REG_NAME_LENGTH                  AnscSizeOfString(ANSC_HTTP_REG_NAME)

#define  ANSC_HTTP_HOST                             "host"
#define  ANSC_HTTP_HOST_LENGTH                      AnscSizeOfString(ANSC_HTTP_HOST)

#define  ANSC_HTTP_PORT                             "port"
#define  ANSC_HTTP_PORT_LENGTH                      AnscSizeOfString(ANSC_HTTP_PORT)

#define  ANSC_HTTP_ACCEPT                           "Accept"
#define  ANSC_HTTP_ACCEPT_LENGTH                    AnscSizeOfString(ANSC_HTTP_ACCEPT)

#define  ANSC_HTTP_ACCEPT_CHARSET                   "Accept-Charset"
#define  ANSC_HTTP_ACCEPT_CHARSET_LENGTH            AnscSizeOfString(ANSC_HTTP_ACCEPT_CHARSET)

#define  ANSC_HTTP_ACCEPT_ENCODING                  "Accept-Encoding"
#define  ANSC_HTTP_ACCEPT_ENCODING_LENGTH           AnscSizeOfString(ANSC_HTTP_ACCEPT_ENCODING)

#define  ANSC_HTTP_ACCEPT_LANGUAGE                  "Accept-Language"
#define  ANSC_HTTP_ACCEPT_LANGUAGE_LENGTH           AnscSizeOfString(ANSC_HTTP_ACCEPT_LANGUAGE)

#define  ANSC_HTTP_ACCEPT_RANGES                    "Accept-Ranges"
#define  ANSC_HTTP_ACCEPT_RANGES_LENGTH             AnscSizeOfString(ANSC_HTTP_ACCEPT_RANGES)

#define  ANSC_HTTP_AGE                              "Age"   
#define  ANSC_HTTP_AGE_LENGTH                       AnscSizeOfString(ANSC_HTTP_AGE)

#define  ANSC_HTTP_ALLOW                            "Allow"
#define  ANSC_HTTP_ALLOW_LENGTH                     AnscSizeOfString(ANSC_HTTP_ALLOW)

#define  ANSC_HTTP_AUTHORIZATION                    "Authorization"
#define  ANSC_HTTP_AUTHORIZATION_LENGTH             AnscSizeOfString(ANSC_HTTP_AUTHORIZATION)

#define  ANSC_HTTP_CACHE_CONTROL                    "Cache-Control"
#define  ANSC_HTTP_CACHE_CONTROL_LENGTH             AnscSizeOfString(ANSC_HTTP_CACHE_CONTROL)

#define  ANSC_HTTP_CONNECTION                       "Connection"
#define  ANSC_HTTP_CONNECTION_LENGTH                AnscSizeOfString(ANSC_HTTP_CONNECTION)

#define  ANSC_HTTP_CONTENT_ENCODING                 "Content-Encoding"
#define  ANSC_HTTP_CONTENT_ENCODING_LENGTH          AnscSizeOfString(ANSC_HTTP_CONTENT_ENCODING)

#define  ANSC_HTTP_CONTENT_LANGUAGE                 "Content-Language"
#define  ANSC_HTTP_CONTENT_LANGUAGE_LENGTH          AnscSizeOfString(ANSC_HTTP_CONTENT_LANGUAGE)

#define  ANSC_HTTP_CONTENT_LENGTH                   "Content-Length"
#define  ANSC_HTTP_CONTENT_LENGTH_LENGTH            AnscSizeOfString(ANSC_HTTP_CONTENT_LENGTH)

#define  ANSC_HTTP_CONTENT_LOCATION                 "Content-Location"
#define  ANSC_HTTP_CONTENT_LOCATION_LENGTH          AnscSizeOfString(ANSC_HTTP_CONTENT_LOCATION)
    
#define  ANSC_HTTP_CONTENT_MD5                      "Content-MD5"
#define  ANSC_HTTP_CONTENT_MD5_LENGTH               AnscSizeOfString(ANSC_HTTP_CONTENT_MD5)

#define  ANSC_HTTP_CONTENT_RANGE                    "Content-Range"
#define  ANSC_HTTP_CONTENT_RANGE_LENGTH             AnscSizeOfString(ANSC_HTTP_CONTENT_RANGE)

#define  ANSC_HTTP_CONTENT_TYPE                     "Content-Type"
#define  ANSC_HTTP_CONTENT_TYPE_LENGTH              AnscSizeOfString(ANSC_HTTP_CONTENT_TYPE)

#define  ANSC_HTTP_DATE_HEADER                      "Date"
#define  ANSC_HTTP_DATE_HEADER_LENGTH               AnscSizeOfString(ANSC_HTTP_DATE_HEADER)

#define  ANSC_HTTP_ETAG                             "ETag"
#define  ANSC_HTTP_ETAG_LENGTH                      AnscSizeOfString(ANSC_HTTP_ETAG)

#define  ANSC_HTTP_EXPECT                           "Expect"
#define  ANSC_HTTP_EXPECT_LENGTH                    AnscSizeOfString(ANSC_HTTP_EXPECT)

#define  ANSC_HTTP_EXPIRES                          "Expires"
#define  ANSC_HTTP_EXPIRES_LENGTH                   AnscSizeOfString(ANSC_HTTP_EXPIRES)

#define  ANSC_HTTP_FROM                             "From"
#define  ANSC_HTTP_FROM_LENGTH                      AnscSizeOfString(ANSC_HTTP_FROM)

#define  ANSC_HTTP_HEADER_HOST                      "Host"
#define  ANSC_HTTP_HEADER_HOST_LENGTH               AnscSizeOfString(ANSC_HTTP_HEADER_HOST)

#define  ANSC_HTTP_IF_MATCH                         "If-Match"
#define  ANSC_HTTP_IF_MATCH_LENGTH                  AnscSizeOfString(ANSC_HTTP_IF_MATCH)

#define  ANSC_HTTP_IF_MODIFIED_SINCE                "If-Modified-Since"
#define  ANSC_HTTP_IF_MODIFIED_SINCE_LENGTH         AnscSizeOfString(ANSC_HTTP_IF_MODIFIED_SINCE)

#define  ANSC_HTTP_IF_NONE_MATCH                    "If-None-Match"
#define  ANSC_HTTP_IF_NONE_MATCH_LENGTH             AnscSizeOfString(ANSC_HTTP_IF_NONE_MATCH)

#define  ANSC_HTTP_IF_RANGE                         "If-Range"
#define  ANSC_HTTP_IF_RANGE_LENGTH                  AnscSizeOfString(ANSC_HTTP_IF_RANGE)

#define  ANSC_HTTP_IF_UNMODIFIED_SINCE              "If-Unmodified-Since"
#define  ANSC_HTTP_IF_UNMODIFIED_SINCE_LENGTH       AnscSizeOfString(ANSC_HTTP_IF_UNMODIFIED_SINCE)

#define  ANSC_HTTP_LAST_MODIFIED                    "Last-Modified"
#define  ANSC_HTTP_LAST_MODIFIED_LENGTH             AnscSizeOfString(ANSC_HTTP_LAST_MODIFIED)      

#define  ANSC_HTTP_LOCATION                         "Location"
#define  ANSC_HTTP_LOCATION_LENGTH                  AnscSizeOfString(ANSC_HTTP_LOCATION)

#define  ANSC_HTTP_MAX_FORWARDS                     "Max-Forwards"
#define  ANSC_HTTP_MAX_FORWARDS_LENGTH              AnscSizeOfString(ANSC_HTTP_MAX_FORWARDS)

#define  ANSC_HTTP_PRAGMA                           "Pragma"
#define  ANSC_HTTP_PRAGMA_LENGTH                    AnscSizeOfString(ANSC_HTTP_PRAGMA)      

#define  ANSC_HTTP_PROXY_AUTHENTICATE               "Proxy-Authenticate"
#define  ANSC_HTTP_PROXY_AUTHENTICATE_LENGTH        AnscSizeOfString(ANSC_HTTP_PROXY_AUTHENTICATE)

#define  ANSC_HTTP_PROXY_AUTHORIZATION              "Proxy-Authorization"
#define  ANSC_HTTP_PROXY_AUTHORIZATION_LENGTH       AnscSizeOfString(ANSC_HTTP_PROXY_AUTHORIZATION)

#define  ANSC_HTTP_RANGE                            "Range"
#define  ANSC_HTTP_RANGE_LENGTH                     AnscSizeOfString(ANSC_HTTP_RANGE)

#define  ANSC_HTTP_REFERER                          "Referer"
#define  ANSC_HTTP_REFERER_LENGTH                   AnscSizeOfString(ANSC_HTTP_REFERER)

#define  ANSC_HTTP_RETRY_AFTER                      "Retry-After"
#define  ANSC_HTTP_RETRY_AFTER_LENGTH               AnscSizeOfString(ANSC_HTTP_RETRY_AFTER)

#define  ANSC_HTTP_SERVER                           "Server"
#define  ANSC_HTTP_SERVER_LENGTH                    AnscSizeOfString(ANSC_HTTP_SERVER)

#define  ANSC_HTTP_TE                               "TE"
#define  ANSC_HTTP_TE_LENGTH                        AnscSizeOfString(ANSC_HTTP_TE)

#define  ANSC_HTTP_TRAILER                          "Trailer"
#define  ANSC_HTTP_TRAILER_LENGTH                   AnscSizeOfString(ANSC_HTTP_TRAILER)

#define  ANSC_HTTP_TRANSFER_ENCODING                "Transfer-Encoding"
#define  ANSC_HTTP_TRANSFER_ENCODING_LENGTH         AnscSizeOfString(ANSC_HTTP_TRANSFER_ENCODING)

#define  ANSC_HTTP_UPGRADE                          "Upgrade"
#define  ANSC_HTTP_UPGRADE_LENGTH                   AnscSizeOfString(ANSC_HTTP_UPGRADE)

#define  ANSC_HTTP_USER_AGENT                       "User-Agent"
#define  ANSC_HTTP_USER_AGENT_LENGTH                AnscSizeOfString(ANSC_HTTP_USER_AGENT)

#define  ANSC_HTTP_VARY                             "Vary"
#define  ANSC_HTTP_VARY_LENGTH                      AnscSizeOfString(ANSC_HTTP_VARY)

#define  ANSC_HTTP_VIA                              "Via"
#define  ANSC_HTTP_VIA_LENGTH                       AnscSizeOfString(ANSC_HTTP_VIA)

#define  ANSC_HTTP_WARNING                          "Warning"
#define  ANSC_HTTP_WARNING_LENGTH                   AnscSizeOfString(ANSC_HTTP_WARNING)

#define  ANSC_HTTP_WWW_AUTHENTICATE                 "WWW-Authenticate"
#define  ANSC_HTTP_WWW_AUTHENTICATE_LENGTH          AnscSizeOfString(ANSC_HTTP_WWW_AUTHENTICATE)

#define  ANSC_HTTP_PROXY_CONNECTION                 "Proxy-Connection"
#define  ANSC_HTTP_PROXY_CONNECTION_LENGTH          AnscSizeOfString(ANSC_HTTP_PROXY_CONNECTION)

#define  ANSC_HTTP_COOKIE                           "Cookie"
#define  ANSC_HTTP_COOKIE_LENGTH                    AnscSizeOfString(ANSC_HTTP_COOKIE)

#define  ANSC_HTTP_SET_COOKIE                       "Set-Cookie"
#define  ANSC_HTTP_SET_COOKIE_LENGTH                AnscSizeOfString(ANSC_HTTP_SET_COOKIE)

#define  ANSC_HTTP_CHUNKED_BODY                     "Chunked-Body"
#define  ANSC_HTTP_CHUNKED_BODY_LENGTH              AnscSizeOfString(ANSC_HTTP_CHUNKED_BODY)

#define  ANSC_HTTP_MEDIA_RANGE                      "media-range"
#define  ANSC_HTTP_MEDIA_RANGE_LENGTH               AnscSizeOfString(ANSC_HTTP_MEDIA_RANGE)

#define  ANSC_HTTP_ACCEPT_PARAMS                    "accept-params"
#define  ANSC_HTTP_ACCEPT_PARAMS_LENGTH             AnscSizeOfString(ANSC_HTTP_ACCEPT_PARAMS)

#define  ANSC_HTTP_TYPE                             "type"
#define  ANSC_HTTP_TYPE_LENGTH                      AnscSizeOfString(ANSC_HTTP_TYPE)

#define  ANSC_HTTP_SUB_TYPE                         "subtype"
#define  ANSC_HTTP_SUB_TYPE_LENGTH                  AnscSizeOfString(ANSC_HTTP_SUB_TYPE)

#define  ANSC_HTTP_MEDIA_TYPE_ANY                   "*"
#define  ANSC_HTTP_MEDIA_TYPE_ANY_LENGTH            AnscSizeOfString(ANSC_HTTP_MEDIA_TYPE_ANY)

#define  ANSC_HTTP_MEDIA_SUBTYPE_ANY                "*"
#define  ANSC_HTTP_MEDIA_SUBTYPE_ANY_LENGTH         AnscSizeOfString(ANSC_HTTP_MEDIA_SUBTYPE_ANY)

#define  ANSC_HTTP_QVALUE                           "qvalue"
#define  ANSC_HTTP_QVALUE_LENGTH                    AnscSizeOfString(ANSC_HTTP_QVALUE)

#define  ANSC_HTTP_CHARSET                          "charset"
#define  ANSC_HTTP_CHARSET_LENGTH                   AnscSizeOfString(ANSC_HTTP_CHARSET)

#define  ANSC_HTTP_CODINGS                          "codings"
#define  ANSC_HTTP_CODINGS_LENGTH                   AnscSizeOfString(ANSC_HTTP_CODINGS)

#define  ANSC_HTTP_LANGUAGE_RANGE                   "language-range"
#define  ANSC_HTTP_LANGUAGE_RANGE_LENGTH            AnscSizeOfString(ANSC_HTTP_LANGUAGE_RANGE)

#define  ANSC_HTTP_ACCEPTABLE_RANGES                "acceptable-ranges"
#define  ANSC_HTTP_ACCEPTABLE_RANGES_LENGTH         AnscSizeOfString(ANSC_HTTP_ACCEPTABLE_RANGES)

#define  ANSC_HTTP_RANGE_UNIT                       "range-unit"
#define  ANSC_HTTP_RANGE_UNIT_LENGTH                AnscSizeOfString(ANSC_HTTP_RANGE_UNIT)

#define  ANSC_HTTP_ACCEPTABLE_RANGE_NONE            "none"
#define  ANSC_HTTP_ACCEPTABLE_RANGE_NONE_LENGTH     AnscSizeOfString(ANSC_HTTP_ACCEPTABLE_RANGE_NONE)

#define  ANSC_HTTP_AGE_VALUE                        "age-value"
#define  ANSC_HTTP_AGE_VALUE_LENGTH                 AnscSizeOfString(ANSC_HTTP_AGE_VALUE)

#define  ANSC_HTTP_CREDENTIALS                      "credentials"
#define  ANSC_HTTP_CREDENTIALS_LENGTH               AnscSizeOfString(ANSC_HTTP_CREDENTIALS)
        
#define  ANSC_HTTP_CACHE_DIRECTIVE                  "cache-directive"
#define  ANSC_HTTP_CACHE_DIRECTIVE_LENGTH           AnscSizeOfString(ANSC_HTTP_CACHE_DIRECTIVE)

#define  ANSC_HTTP_CONNECTION_TOKEN                 "connection-token"
#define  ANSC_HTTP_CONNECTION_TOKEN_LENGTH          AnscSizeOfString(ANSC_HTTP_CONNECTION_TOKEN)

#define  ANSC_HTTP_CONTENT_CODING                   "content-coding"
#define  ANSC_HTTP_CONTENT_CODING_LENGTH            AnscSizeOfString(ANSC_HTTP_CONTENT_CODING)

#define  ANSC_HTTP_LANGUAGE_TAG                     "language-tag"
#define  ANSC_HTTP_LANGUAGE_TAG_LENGTH              AnscSizeOfString(ANSC_HTTP_LANGUAGE_TAG)

#define  ANSC_HTTP_RELATIVE_URI                     "relativeURI"
#define  ANSC_HTTP_RELATIVE_URI_LENGTH              AnscSizeOfString(ANSC_HTTP_RELATIVE_URI)

#define  ANSC_HTTP_NET_PATH                         "net_path"
#define  ANSC_HTTP_NET_PATH_LENGTH                  AnscSizeOfString(ANSC_HTTP_NET_PATH)

#define  ANSC_HTTP_NET_LOC                          "net_loc"
#define  ANSC_HTTP_NET_LOC_LENGTH                   AnscSizeOfString(ANSC_HTTP_NET_LOC)

#define  ANSC_HTTP_MD5_DIGEST                       "md5-digest"
#define  ANSC_HTTP_MD5_DIGEST_LENGTH                AnscSizeOfString(ANSC_HTTP_MD5_DIGEST)

#define  ANSC_HTTP_FIRST_BYTE_POS                   "first-byte-pos"
#define  ANSC_HTTP_FIRST_BYTE_POS_LENGTH            AnscSizeOfString(ANSC_HTTP_FIRST_BYTE_POS)

#define  ANSC_HTTP_LAST_BYTE_POS                    "last-byte-pos"
#define  ANSC_HTTP_LAST_BYTE_POS_LENGTH             AnscSizeOfString(ANSC_HTTP_LAST_BYTE_POS)

#define  ANSC_HTTP_ENTITY_LENGTH                    "entity-length"
#define  ANSC_HTTP_ENTITY_LENGTH_LENGTH             AnscSizeOfString(ANSC_HTTP_ENTITY_LENGTH)

#define  ANSC_HTTP_PARAMETER                        "parameter"
#define  ANSC_HTTP_PARAMETER_LENGTH                 AnscSizeOfString(ANSC_HTTP_PARAMETER)

#define  ANSC_HTTP_RFC1123_DATE                     "rfc1123-date"
#define  ANSC_HTTP_RFC1123_DATE_LENGTH              AnscSizeOfString(ANSC_HTTP_RFC1123_DATE)

#define  ANSC_HTTP_RFC850_DATE                      "rfc850-date"
#define  ANSC_HTTP_RFC850_DATE_LENGTH               AnscSizeOfString(ANSC_HTTP_RFC850_DATE)

#define  ANSC_HTTP_ASCTIME_DATE                     "asctime-date"
#define  ANSC_HTTP_ASCTIME_DATE_LENGTH              AnscSizeOfString(ANSC_HTTP_ASCTIME_DATE)

#define  ANSC_HTTP_WKDAY                            "wkday"
#define  ANSC_HTTP_WKDAY_LENGTH                     AnscSizeOfString(ANSC_HTTP_WKDAY)

#define  ANSC_HTTP_DATE1                            "date1"
#define  ANSC_HTTP_DATE1_LENGTH                     AnscSizeOfString(ANSC_HTTP_DATE1)

#define  ANSC_HTTP_TIME                             "time"
#define  ANSC_HTTP_TIME_LENGTH                      AnscSizeOfString(ANSC_HTTP_TIME)

#define  ANSC_HTTP_WEEKDAY                          "weekday"
#define  ANSC_HTTP_WEEKDAY_LENGTH                   AnscSizeOfString(ANSC_HTTP_WEEKDAY)

#define  ANSC_HTTP_DATE2                            "date2"
#define  ANSC_HTTP_DATE2_LENGTH                     AnscSizeOfString(ANSC_HTTP_DATE2)

#define  ANSC_HTTP_DATE3                            "date3"
#define  ANSC_HTTP_DATE3_LENGTH                     AnscSizeOfString(ANSC_HTTP_DATE3)

#define  ANSC_HTTP_WKDAY_SUN                        "Sun"   
#define  ANSC_HTTP_WKDAY_SUN_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_SUN)

#define  ANSC_HTTP_WKDAY_MON                        "Mon"
#define  ANSC_HTTP_WKDAY_MON_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_MON)

#define  ANSC_HTTP_WKDAY_TUE                        "Tue"
#define  ANSC_HTTP_WKDAY_TUE_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_TUE)

#define  ANSC_HTTP_WKDAY_WED                        "Wed"
#define  ANSC_HTTP_WKDAY_WED_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_WED)

#define  ANSC_HTTP_WKDAY_THU                        "Thu"
#define  ANSC_HTTP_WKDAY_THU_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_THU)

#define  ANSC_HTTP_WKDAY_FRI                        "Fri"
#define  ANSC_HTTP_WKDAY_FRI_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_FRI)

#define  ANSC_HTTP_WKDAY_SAT                        "Sat"
#define  ANSC_HTTP_WKDAY_SAT_LENGTH                 AnscSizeOfString(ANSC_HTTP_WKDAY_SAT)

#define  ANSC_HTTP_WEEKDAY_SUNDAY                   "Sunday"
#define  ANSC_HTTP_WEEKDAY_SUNDAY_LENGTH            AnscSizeOfString(ANSC_HTTP_WEEKDAY_SUNDAY)

#define  ANSC_HTTP_WEEKDAY_MONDAY                   "Monday"
#define  ANSC_HTTP_WEEKDAY_MONDAY_LENGTH            AnscSizeOfString(ANSC_HTTP_WEEKDAY_MONDAY)

#define  ANSC_HTTP_WEEKDAY_TUESDAY                  "Tuesday"   
#define  ANSC_HTTP_WEEKDAY_TUESDAY_LENGTH           AnscSizeOfString(ANSC_HTTP_WEEKDAY_TUESDAY)

#define  ANSC_HTTP_WEEKDAY_WEDNESDAY                "Wednesday"
#define  ANSC_HTTP_WEEKDAY_WEDNESDAY_LENGTH         AnscSizeOfString(ANSC_HTTP_WEEKDAY_WEDNESDAY)

#define  ANSC_HTTP_WEEKDAY_THURSDAY                 "Thursday"
#define  ANSC_HTTP_WEEKDAY_THURSDAY_LENGTH          AnscSizeOfString(ANSC_HTTP_WEEKDAY_THURSDAY)

#define  ANSC_HTTP_WEEKDAY_FRIDAY                   "Friday"
#define  ANSC_HTTP_WEEKDAY_FRIDAY_LENGTH            AnscSizeOfString(ANSC_HTTP_WEEKDAY_FRIDAY)

#define  ANSC_HTTP_WEEKDAY_SATURDAY                 "Saturday"
#define  ANSC_HTTP_WEEKDAY_SATURDAY_LENGTH          AnscSizeOfString(ANSC_HTTP_WEEKDAY_SATURDAY)

#define  ANSC_HTTP_MONTH                            "month"
#define  ANSC_HTTP_MONTH_LENGTH                     AnscSizeOfString(ANSC_HTTP_MONTH)

#define  ANSC_HTTP_MONTH_JAN                        "Jan" 
#define  ANSC_HTTP_MONTH_JAN_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_JAN)

#define  ANSC_HTTP_MONTH_FEB                        "Feb"
#define  ANSC_HTTP_MONTH_FEB_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_FEB)

#define  ANSC_HTTP_MONTH_MAR                        "Mar"
#define  ANSC_HTTP_MONTH_MAR_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_MAR)

#define  ANSC_HTTP_MONTH_APR                        "Apr"
#define  ANSC_HTTP_MONTH_APR_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_APR)

#define  ANSC_HTTP_MONTH_MAY                        "May"
#define  ANSC_HTTP_MONTH_MAY_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_MAY)    

#define  ANSC_HTTP_MONTH_JUN                        "Jun"
#define  ANSC_HTTP_MONTH_JUN_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_JUN)

#define  ANSC_HTTP_MONTH_JUL                        "Jul"
#define  ANSC_HTTP_MONTH_JUL_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_JUL)    

#define  ANSC_HTTP_MONTH_AUG                        "Aug"
#define  ANSC_HTTP_MONTH_AUG_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_AUG)

#define  ANSC_HTTP_MONTH_SEP                        "Sep"
#define  ANSC_HTTP_MONTH_SEP_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_SEP)

#define  ANSC_HTTP_MONTH_OCT                        "Oct"
#define  ANSC_HTTP_MONTH_OCT_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_OCT)

#define  ANSC_HTTP_MONTH_NOV                        "Nov"
#define  ANSC_HTTP_MONTH_NOV_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_NOV)

#define  ANSC_HTTP_MONTH_DEC                        "Dec"
#define  ANSC_HTTP_MONTH_DEC_LENGTH                 AnscSizeOfString(ANSC_HTTP_MONTH_DEC)

#define  ANSC_HTTP_ENTITY_TAG                       "entity-tag"
#define  ANSC_HTTP_ENTITY_TAG_LENGTH                 AnscSizeOfString(ANSC_HTTP_ENTITY_TAG)

#define  ANSC_HTTP_WEAK                             "weak"
#define  ANSC_HTTP_WEAK_LENGTH                      AnscSizeOfString(ANSC_HTTP_WEAK)

#define  ANSC_HTTP_OPAQUE_TAG                       "opaque-tag"
#define  ANSC_HTTP_OPAQUE_TAG_LENGTH                AnscSizeOfString(ANSC_HTTP_OPAQUE_TAG)

#define  ANSC_HTTP_EXPECTATION                      "expectation"
#define  ANSC_HTTP_EXPECTATION_LENGTH               AnscSizeOfString(ANSC_HTTP_EXPECTATION)

#define  ANSC_HTTP_100_CONTINUE                     "100-continue"
#define  ANSC_HTTP_100_CONTINUE_LENGTH              AnscSizeOfString(ANSC_HTTP_100_CONTINUE)

#define  ANSC_HTTP_EXPECTATION_EXTENSION            "expectation-extension"
#define  ANSC_HTTP_EXPECTATION_EXTENSION_LENGTH     AnscSizeOfString(ANSC_HTTP_EXPECTATION_EXTENSION)

#define  ANSC_HTTP_EXPECTATION_EXTENSION_SEPARATOR  '='
#define  ANSC_HTTP_EXPECTATION_EXTENSION_SEPARATOR_LENGTH   1

#define  ANSC_HTTP_DATE                             "HTTP-date"
#define  ANSC_HTTP_DATE_LENGTH                      AnscSizeOfStringANSC_HTTP_DATE()

#define  ANSC_HTTP_LOCAL_PART                       "local-part"
#define  ANSC_HTTP_LOCAL_PART_LENGTH                AnscSizeOfString(ANSC_HTTP_LOCAL_PART)

#define  ANSC_HTTP_DOMAIN                           "domain"
#define  ANSC_HTTP_DOMAIN_LENGTH                    AnscSizeOfString(ANSC_HTTP_DOMAIN)

#define  ANSC_HTTP_IF_MATCH_ASTERISK                "*"
#define  ANSC_HTTP_IF_MATCH_ASTERISK_LENGTH         AnscSizeOfString(ANSC_HTTP_IF_MATCH_ASTERISK) 

#define  ANSC_HTTP_PRAGMA_DIRECTIVE                 "pragma-directive"
#define  ANSC_HTTP_PRAGMA_DIRECTIVE_LENGTH          AnscSizeOfString(ANSC_HTTP_PRAGMA_DIRECTIVE)

#define  ANSC_HTTP_CHALLENGE                        "challenge"
#define  ANSC_HTTP_CHALLENGE_LENGTH                 AnscSizeOfString(ANSC_HTTP_CHALLENGE)

#define  ANSC_HTTP_SUFFIX_LENGTH                    "suffix-length"
#define  ANSC_HTTP_SUFFIX_LENGTH_LENGTH             AnscSizeOfString(ANSC_HTTP_SUFFIX_LENGTH)

#define  ANSC_HTTP_DELTA_SECONDS                    "delta-seconds"
#define  ANSC_HTTP_DELTA_SECONDS_LENGTH             AnscSizeOfString(ANSC_HTTP_DELTA_SECONDS)

#define  ANSC_HTTP_PRODUCT                          "product"
#define  ANSC_HTTP_PRODUCT_LENGTH                   AnscSizeOfString(ANSC_HTTP_PRODUCT)

#define  ANSC_HTTP_HEADER_NAME_VALUE_SEPARATOR      ':'
#define  ANSC_HTTP_HEADER_NAME_VALUE_SEPARATOR_LENGTH   1

#define  ANSC_HTTP_T_CODINGS                        "t-codings"
#define  ANSC_HTTP_T_CODINGS_LENGTH                 AnscSizeOfString(ANSC_HTTP_T_CODINGS)

#define  ANSC_HTTP_TRANSFER_EXTENSION               "transfer-extension"
#define  ANSC_HTTP_TRANSFER_EXTENSION_LENGTH        AnscSizeOfString(ANSC_HTTP_TRANSFER_EXTENSION)

#define  ANSC_HTTP_TRANSFER_ENCODING_TRAILER        "trailers"
#define  ANSC_HTTP_TRANSFER_ENCODING_TRAILER_LENGTH AnscSizeOfString(ANSC_HTTP_TRANSFER_ENCODING_TRAILER)

#define  ANSC_HTTP_FIELD_NAME                       "field-name"
#define  ANSC_HTTP_FIELD_NAME_LENGTH                AnscSizeOfString(ANSC_HTTP_FIELD_NAME)

#define  ANSC_HTTP_TRANSFER_CODING                  "transfer-coding"
#define  ANSC_HTTP_TRANSFER_CODING_LENGTH           AnscSizeOfString(ANSC_HTTP_TRANSFER_CODING)

#define  ANSC_HTTP_PROTOCOL_NAME                    "protocol-name"
#define  ANSC_HTTP_PROTOCOL_NAME_LENGTH             AnscSizeOfString(ANSC_HTTP_PROTOCOL_NAME)

#define  ANSC_HTTP_PROTOCOL_VERSION                 "protocol-version"
#define  ANSC_HTTP_PROTOCOL_VERSION_LENGTH          AnscSizeOfString(ANSC_HTTP_PROTOCOL_VERSION)

#define  ANSC_HTTP_RECEIVED_BY                      "received-by"
#define  ANSC_HTTP_RECEIVED_BY_LENGTH               AnscSizeOfString(ANSC_HTTP_RECEIVED_BY)

#define  ANSC_HTTP_RECEIVED_PROTOCOL                "received-protocol"
#define  ANSC_HTTP_RECEIVED_PROTOCOL_LENGTH         AnscSizeOfString(ANSC_HTTP_RECEIVED_PROTOCOL)

#define  ANSC_HTTP_PSEUDONYM                        "pseudonym"
#define  ANSC_HTTP_PSEUDONYM_LENGTH                 AnscSizeOfString(ANSC_HTTP_PSEUDONYM)

#define  ANSC_HTTP_WARN_CODE                        "warn-code"
#define  ANSC_HTTP_WARN_CODE_LENGTH                 AnscSizeOfString(ANSC_HTTP_WARN_CODE)

#define  ANSC_HTTP_WARN_TEXT                        "warn-text"
#define  ANSC_HTTP_WARN_TEXT_LENGTH                 AnscSizeOfString(ANSC_HTTP_WARN_TEXT)

#define  ANSC_HTTP_FIELD_SEPARATOR                  ", "
#define  ANSC_HTTP_FIELD_SEPARATOR_LENGTH           AnscSizeOfString(ANSC_HTTP_FIELD_SEPARATOR)

#define  ANSC_HTTP_HEADER_SEPARATOR                 ": "
#define  ANSC_HTTP_HEADER_SEPARATOR_LENGTH          AnscSizeOfString(ANSC_HTTP_HEADER_SEPARATOR)

#define  ANSC_HTTP_NAME                             "HTTP"
#define  ANSC_HTTP_NAME_LENGTH                      AnscSizeOfString(ANSC_HTTP_NAME)

#define  ANSC_HTTP_QUALITY                          "q"
#define  ANSC_HTTP_QUALITY_LENGTH                   AnscSizeOfString(ANSC_HTTP_QUALITY)

#define  ANSC_HTTP_BYTES_UNIT                       "bytes"
#define  ANSC_HTTP_BYTES_UNIT_LENGTH                AnscSizeOfString(ANSC_HTTP_BYTES_UNIT)

#define  ANSC_HTTP_QUALITY_PARAMS                   ";q="
#define  ANSC_HTTP_QUALITY_PARAMS_LENGTH            AnscSizeOfString(ANSC_HTTP_QUALITY_PARAMS)

#define  ANSC_HTTP_WEAK_VALUE                       "W/"
#define  ANSC_HTTP_WEAK_VALUE_LENGTH                AnscSizeOfString(ANSC_HTTP_WEAK_VALUE)

#define  ANSC_HTTP_PROXY_CONNECTION_TOKEN           "proxy-connection-token"
#define  ANSC_HTTP_PROXY_CONNECTION_TOKEN_LENGTH    AnscSizeOfString(ANSC_HTTP_PROXY_CONNECTION_TOKEN)

#define  ANSC_HTTP_FIELD_VALUE                      "field-value"
#define  ANSC_HTTP_FIELD_VALUE_LENGTH               AnscSizeOfString(ANSC_HTTP_FIELD_VALUE)

#define  ANSC_HTTP_SCHEME                           "http://"
#define  ANSC_HTTP_SCHEME_LENGTH                    AnscSizeOfString(ANSC_HTTP_SCHEME)

#define  ANSC_HTTP_COOKIE_VERSION                   "cookie-version"
#define  ANSC_HTTP_COOKIE_VERSION_LENGTH            AnscSizeOfString(ANSC_HTTP_COOKIE_VERSION)

#define  ANSC_HTTP_COOKIE_VALUE                     "cookie-value"
#define  ANSC_HTTP_COOKIE_VALUE_LENGTH              AnscSizeOfString(ANSC_HTTP_COOKIE_VALUE)

#define  ANSC_HTTP_COOKIE_NAME                      "cookie-name"
#define  ANSC_HTTP_COOKIE_NAME_LENGTH               AnscSizeOfString(ANSC_HTTP_COOKIE_NAME)

#define  ANSC_HTTP_VALUE                            "value"
#define  ANSC_HTTP_VALUE_LENGTH                     AnscSizeOfString(ANSC_HTTP_VALUE)

#define  ANSC_HTTP_COOKIE_PATH                      "cookie-path"
#define  ANSC_HTTP_COOKIE_PATH_LENGTH               AnscSizeOfString(ANSC_HTTP_COOKIE_PATH)

#define  ANSC_HTTP_COOKIE_DOMAIN                    "cookie-domain"
#define  ANSC_HTTP_COOKIE_DOMAIN_LENGTH             AnscSizeOfString(ANSC_HTTP_COOKIE_DOMAIN)

#define  ANSC_HTTP_COOKIE_ENTITY                    "Cookie-Entity"
#define  ANSC_HTTP_COOKIE_ENTITY_LENGTH             AnscSizeOfString(ANSC_HTTP_COOKIE_ENTITY)

#define  ANSC_HTTP_COOKIE_AV                        "cookie-av"
#define  ANSC_HTTP_COOKIE_AV_LENGTH                 AnscSizeOfString(ANSC_HTTP_COOKIE_AV)

#define  ANSC_HTTP_COMMENT                          "comment"
#define  ANSC_HTTP_COMMENT_LENGTH                   AnscSizeOfString(ANSC_HTTP_COMMENT)

#define  ANSC_HTTP_TOKEN                            "token"
#define  ANSC_HTTP_TOKEN_LENGTH                     AnscSizeOfString(ANSC_HTTP_TOKEN)

#define  ANSC_HTTP_SET_COOKIE_COMMENT               "Comment"
#define  ANSC_HTTP_SET_COOKIE_COMMENT_LENGTH        AnscSizeOfString(ANSC_HTTP_SET_COOKIE_COMMENT)

#define  ANSC_HTTP_SET_COOKIE_DOMAIN                "Domain"
#define  ANSC_HTTP_SET_COOKIE_DOMAIN_LENGTH         AnscSizeOfString(ANSC_HTTP_SET_COOKIE_DOMAIN)

#define  ANSC_HTTP_SET_COOKIE_MAX_AGE               "Max-Age"
#define  ANSC_HTTP_SET_COOKIE_MAX_AGE_LENGTH        AnscSizeOfString(ANSC_HTTP_SET_COOKIE_MAX_AGE)

#define  ANSC_HTTP_SET_COOKIE_PATH                  "Path"
#define  ANSC_HTTP_SET_COOKIE_PATH_LENGTH           AnscSizeOfString(ANSC_HTTP_SET_COOKIE_PATH)

#define  ANSC_HTTP_SET_COOKIE_SECURE                "Secure"
#define  ANSC_HTTP_SET_COOKIE_SECURE_LENGTH         AnscSizeOfString(ANSC_HTTP_SET_COOKIE_SECURE)

#define  ANSC_HTTP_SET_COOKIE_VERSION               "Version"
#define  ANSC_HTTP_SET_COOKIE_VERSION_LENGTH        AnscSizeOfString(ANSC_HTTP_SET_COOKIE_VERSION)

#define  ANSC_HTTP_SET_COOKIE_EXPIRES               "expires"
#define  ANSC_HTTP_SET_COOKIE_EXPIRES_LENGTH        AnscSizeOfString(ANSC_HTTP_SET_COOKIE_EXPIRES)

#define  ANSC_HTTP_SET_COOKIE_ATTR_PREFIX           "$"
#define  ANSC_HTTP_SET_COOKIE_ATTR_PREFIX_LENGTH    AnscSizeOfString(ANSC_HTTP_SET_COOKIE_ATTR_PREFIX)

#define  ANSC_HTTP_ATTR_SEPARATOR                   "="
#define  ANSC_HTTP_ATTR_SEPARATOR_LENGTH            AnscSizeOfString(ANSC_HTTP_ATTR_SEPARATOR)

#define  ANSC_HTTP_BASIC_CREDENTIALS                "basic-credentials"
#define  ANSC_HTTP_BASIC_CREDENTIALS_LENGTH         AnscSizeOfString(ANSC_HTTP_BASIC_CREDENTIALS)

#define  ANSC_HTTP_BASIC_COOKIE                     "basic-cookie"
#define  ANSC_HTTP_BASIC_COOKIE_LENGTH              AnscSizeOfString(ANSC_HTTP_BASIC_COOKIE)

#define  ANSC_HTTP_REALM                            "realm"
#define  ANSC_HTTP_REALM_LENGTH                     AnscSizeOfString(ANSC_HTTP_REALM)

#define  ANSC_HTTP_REALM_VALUE                      "realm-value"
#define  ANSC_HTTP_REALM_VALUE_LENGTH               AnscSizeOfString(ANSC_HTTP_REALM_VALUE)

#define  ANSC_HTTP_AUTH_SCHEME                      "auth-scheme"
#define  ANSC_HTTP_AUTH_SCHEME_LENGTH               AnscSizeOfString(ANSC_HTTP_AUTH_SCHEME)

#define  ANSC_HTTP_CREDENTIAL_BASIC_SCHEME          "Basic"
#define  ANSC_HTTP_CREDENTIAL_BASIC_SCHEME_LENGTH   AnscSizeOfString(ANSC_HTTP_CREDENTIAL_BASIC_SCHEME)

#define  ANSC_HTTP_SPACE                            " "
#define  ANSC_HTTP_SPACE_LENGTH                     AnscSizeOfString(ANSC_HTTP_SPACE)

#define  ANSC_HTTP_COOKIE_EXPIRES_DATE              "cookie-expires-date"
#define  ANSC_HTTP_COOKIE_EXPIRES_DATE_LENGTH       AnscSizeOfString(ANSC_HTTP_COOKIE_EXPIRES_DATE)

#define  ANSC_HTTP_COOKIE_NAME_VALUE                "cookie-name-value"
#define  ANSC_HTTP_COOKIE_NAME_VALUE_LENGTH         AnscSizeOfString(ANSC_HTTP_COOKIE_NAME_VALUE)

#define  ANSC_HTTP_COOKIE_NAME_VALUE_TOKEN          "cookie-name-value-token"
#define  ANSC_HTTP_COOKIE_NAME_VALUE_TOKEN_LENGTH   AnscSizeOfString(ANSC_HTTP_COOKIE_NAME_VALUE_TOKEN)

#define  ANSC_HTTP_SET_COOKIE_DOMAIN_NODE           "set-cookie-domain"
#define  ANSC_HTTP_SET_COOKIE_DOMAIN_NODE_LENGTH    AnscSizeOfString(ANSC_HTTP_SET_COOKIE_DOMAIN_NODE)

#define  ANSC_HTTP_SET_COOKIE_PATH_NODE             "set-cookie-path"
#define  ANSC_HTTP_SET_COOKIE_PATH_NODE_LENGTH      AnscSizeOfString(ANSC_HTTP_SET_COOKIE_PATH_NODE)

#define  ANSC_HTTP_CHUNK_SIZE                       "chunk-size"
#define  ANSC_HTTP_CHUNK_SIZE_LENGTH                AnscSizeOfString(ANSC_HTTP_CHUNK_SIZE)

#define  ANSC_HTTP_CHUNK_EXT_NAME                   "chunk-ext-name"
#define  ANSC_HTTP_CHUNK_EXT_NAME_LENGTH            AnscSizeOfString(ANSC_HTTP_CHUNK_EXT_NAME)

#define  ANSC_HTTP_CHUNK_EXT_VALUE                  "chunk-ext-val"
#define  ANSC_HTTP_CHUNK_EXT_VALUE_LENGTH           AnscSizeOfString(ANSC_HTTP_CHUNK_EXT_VALUE)

#define  ANSC_HTTP_MULTIPART_BOUNDARY_DELI          "--"
#define  ANSC_HTTP_MULTIPART_BOUNDARY_DELI_LENGTH   AnscSizeOfString(ANSC_HTTP_MULTIPART_BOUNDARY_DELI)

#define  ANSC_HTTP_LINE_BREAKER                     "\r\n"
#define  ANSC_HTTP_LINE_BREAKER_LENGTH              AnscSizeOfString(ANSC_HTTP_LINE_BREAKER)

#define  ANSC_HTTP_DOUBLE_LINE_BREAKER              "\r\n\r\n"
#define  ANSC_HTTP_DOUBLE_LINE_BREAKER_LENGTH       AnscSizeOfString(ANSC_HTTP_DOUBLE_LINE_BREAKER)

#define  ANSC_HTTP_CONTENT_DISPOSITION              "content-disposition"
#define  ANSC_HTTP_CONTENT_DISPOSITION_LENGTH       AnscSizeOfString(ANSC_HTTP_CONTENT_DISPOSITION)

#define  ANSC_HTTP_MULTIPART_FIELD_NAME             "name="
#define  ANSC_HTTP_MULTIPART_FIELD_NAME_LENGTH      AnscSizeOfString(ANSC_HTTP_MULTIPART_FIELD_NAME)

/*
 * structure for HTTP-Version
 */
typedef struct _ANSC_HTTP_VERSION_CB
{
    ULONG                           MajorVersion;
    BOOL                            bMajorVersionGot;
    ULONG                           MinorVersion;
    BOOL                            bMinorVersionGot;
}
ANSC_HTTP_VERSION_CB, *PANSC_HTTP_VERSION_CB;


/*
 * structure for getting time
 */
typedef struct _ANSC_HTTP_TIME_CB
{
    PHTTP_DATE                      pDate;
    BOOL                            bHourGot;
    BOOL                            bMinuteGot;
    BOOL                            bSecondGot;
    BOOL                            bYearGot;
    BOOL                            bMonthGot;
    BOOL                            bDayGot;
}
ANSC_HTTP_TIME_CB, *PANSC_HTTP_TIME_CB;


/*
 *  structure for multipart
 */
typedef  struct _ANSC_HTTP_MULTIPART_PART
{
    SINGLE_LINK_ENTRY               Linkage;

    ULONG                           ulType;

    char                            *pHeader;
    ULONG                           ulHeaderLen;

    char                            *pName;
    ULONG                           ulNameLen;

    char                            *pValue;
    ULONG                           ulValueLen;
}
ANSC_HTTP_MULTIPART_PART, *PANSC_HTTP_MULTIPART_PART;

#define  ACCESS_HTTPMP_MULTIPART_PART_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_HTTP_MULTIPART_PART, Linkage)

#define  AnscHttpMultipartPartInit(pPart)                           \
    (pPart)->pHeader        = NULL;                                 \
    (pPart)->ulHeaderLen    = 0;                                    \
    (pPart)->pName          = NULL;                                 \
    (pPart)->ulNameLen      = 0;                                    \
    (pPart)->pValue         = NULL;                                 \
    (pPart)->ulValueLen     = 0;                                    \


#endif
