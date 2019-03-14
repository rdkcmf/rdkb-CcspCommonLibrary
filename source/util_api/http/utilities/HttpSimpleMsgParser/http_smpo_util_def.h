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

    module:	http_smpo_util_def.h

        For HTTP Simple Message Parser (syntax parser),
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

#ifndef  _HTTP_SMPO_UTIL_DEF
#define  _HTTP_SMPO_UTIL_DEF


/*
 * Define the HTTP production names and some structures which are used
 * for collecting information from token tree node object.
 */
#define  HTTP_COOKIE_MAX_AGE_ABSENT                 (ULONG)-1


BOOL
HttpSmpoTokenIs
    (
        PUCHAR                      tokenName,
        PUCHAR                      httpToken
    );

int
HttpSmpoMemFindChar
    (
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        UCHAR                       aChar,
        BOOL                        bCaseSensitive
    );

ULONG
HttpSmpoHexString2Ulong
    (
        PUCHAR                      pHexString
    );

ANSC_STATUS
HttpSmpoDecodeUrlEncode
    (
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        PUCHAR                      pDecodedStr,
        PULONG                      pulDecodedStrLen
    );

ANSC_STATUS
HttpSmpoGetMultipartPart
    (
        ANSC_HANDLE                 hHttpSmpo,
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        PUCHAR                      pBoundary,
        ANSC_HANDLE                 hPart,
        PUCHAR                      *pNextPart
    );


#define  HTTP_SMPO_TOKEN_NAME_IS(tokenName, httpToken)      \
    HttpSmpoTokenIs(tokenName, httpToken)                   \


#define  HTTP_SMPO_DIGIT                            "DIGIT"
#define  HTTP_SMPO_DIGIT_LENGTH                     AnscSizeOfString(HTTP_SMPO_DIGIT)

#define  HTTP_SMPO_REQUEST_URI_ASTERISK             "*"
#define  HTTP_SMPO_REQUEST_URI_ASTERISK_LENGTH      AnscSizeOfString(HTTP_SMPO_REQUEST_URI_ASTERISK)

#define  HTTP_SMPO_ACCEPT                           "Accept"
#define  HTTP_SMPO_ACCEPT_LENGTH                    AnscSizeOfString(HTTP_SMPO_ACCEPT)

#define  HTTP_SMPO_ACCEPT_CHARSET                   "Accept-Charset"
#define  HTTP_SMPO_ACCEPT_CHARSET_LENGTH            AnscSizeOfString(HTTP_SMPO_ACCEPT_CHARSET)

#define  HTTP_SMPO_ACCEPT_ENCODING                  "Accept-Encoding"
#define  HTTP_SMPO_ACCEPT_ENCODING_LENGTH           AnscSizeOfString(HTTP_SMPO_ACCEPT_ENCODING)

#define  HTTP_SMPO_ACCEPT_LANGUAGE                  "Accept-Language"
#define  HTTP_SMPO_ACCEPT_LANGUAGE_LENGTH           AnscSizeOfString(HTTP_SMPO_ACCEPT_LANGUAGE)

#define  HTTP_SMPO_ACCEPT_RANGES                    "Accept-Ranges"
#define  HTTP_SMPO_ACCEPT_RANGES_LENGTH             AnscSizeOfString(HTTP_SMPO_ACCEPT_RANGES)

#define  HTTP_SMPO_AGE                              "Age"   
#define  HTTP_SMPO_AGE_LENGTH                       AnscSizeOfString(HTTP_SMPO_AGE)

#define  HTTP_SMPO_ALLOW                            "Allow"
#define  HTTP_SMPO_ALLOW_LENGTH                     AnscSizeOfString(HTTP_SMPO_ALLOW)

#define  HTTP_SMPO_AUTHORIZATION                    "Authorization"
#define  HTTP_SMPO_AUTHORIZATION_LENGTH             AnscSizeOfString(HTTP_SMPO_AUTHORIZATION)

#define  HTTP_SMPO_CACHE_CONTROL                    "Cache-Control"
#define  HTTP_SMPO_CACHE_CONTROL_LENGTH             AnscSizeOfString(HTTP_SMPO_CACHE_CONTROL)

#define  HTTP_SMPO_CONNECTION                       "Connection"
#define  HTTP_SMPO_CONNECTION_LENGTH                AnscSizeOfString(HTTP_SMPO_CONNECTION)

#define  HTTP_SMPO_CONTENT_BASE                     "Content-Base"
#define  HTTP_SMPO_CONTENT_BASE_LENGTH              AnscSizeOfString(HTTP_SMPO_CONTENT_BASE)

#define  HTTP_SMPO_CONTENT_ENCODING                 "Content-Encoding"
#define  HTTP_SMPO_CONTENT_ENCODING_LENGTH          AnscSizeOfString(HTTP_SMPO_CONTENT_ENCODING)

#define  HTTP_SMPO_CONTENT_LANGUAGE                 "Content-Language"
#define  HTTP_SMPO_CONTENT_LANGUAGE_LENGTH          AnscSizeOfString(HTTP_SMPO_CONTENT_LANGUAGE)

#define  HTTP_SMPO_CONTENT_LENGTH                   "Content-Length"
#define  HTTP_SMPO_CONTENT_LENGTH_LENGTH            AnscSizeOfString(HTTP_SMPO_CONTENT_LENGTH)

#define  HTTP_SMPO_CONTENT_LOCATION                 "Content-Location"
#define  HTTP_SMPO_CONTENT_LOCATION_LENGTH          AnscSizeOfString(HTTP_SMPO_CONTENT_LOCATION)
    
#define  HTTP_SMPO_CONTENT_MD5                      "Content-MD5"
#define  HTTP_SMPO_CONTENT_MD5_LENGTH               AnscSizeOfString(HTTP_SMPO_CONTENT_MD5)

#define  HTTP_SMPO_CONTENT_RANGE                    "Content-Range"
#define  HTTP_SMPO_CONTENT_RANGE_LENGTH             AnscSizeOfString(HTTP_SMPO_CONTENT_RANGE)

#define  HTTP_SMPO_CONTENT_TYPE                     "Content-Type"
#define  HTTP_SMPO_CONTENT_TYPE_LENGTH              AnscSizeOfString(HTTP_SMPO_CONTENT_TYPE)

#define  HTTP_SMPO_DATE_HEADER                      "Date"
#define  HTTP_SMPO_DATE_HEADER_LENGTH               AnscSizeOfString(HTTP_SMPO_DATE_HEADER)

#define  HTTP_SMPO_ETAG                             "ETag"
#define  HTTP_SMPO_ETAG_LENGTH                      AnscSizeOfString(HTTP_SMPO_ETAG)

#define  HTTP_SMPO_EXPECT                           "Expect"
#define  HTTP_SMPO_EXPECT_LENGTH                    AnscSizeOfString(HTTP_SMPO_EXPECT)

#define  HTTP_SMPO_EXPIRES                          "Expires"
#define  HTTP_SMPO_EXPIRES_LENGTH                   AnscSizeOfString(HTTP_SMPO_EXPIRES)

#define  HTTP_SMPO_FROM                             "From"
#define  HTTP_SMPO_FROM_LENGTH                      AnscSizeOfString(HTTP_SMPO_FROM)

#define  HTTP_SMPO_HEADER_HOST                      "Host"
#define  HTTP_SMPO_HEADER_HOST_LENGTH               AnscSizeOfString(HTTP_SMPO_HEADER_HOST)

#define  HTTP_SMPO_IF_MATCH                         "If-Match"
#define  HTTP_SMPO_IF_MATCH_LENGTH                  AnscSizeOfString(HTTP_SMPO_IF_MATCH)

#define  HTTP_SMPO_IF_MODIFIED_SINCE                "If-Modified-Since"
#define  HTTP_SMPO_IF_MODIFIED_SINCE_LENGTH         AnscSizeOfString(HTTP_SMPO_IF_MODIFIED_SINCE)

#define  HTTP_SMPO_IF_NONE_MATCH                    "If-None-Match"
#define  HTTP_SMPO_IF_NONE_MATCH_LENGTH             AnscSizeOfString(HTTP_SMPO_IF_NONE_MATCH)

#define  HTTP_SMPO_IF_RANGE                         "If-Range"
#define  HTTP_SMPO_IF_RANGE_LENGTH                  AnscSizeOfString(HTTP_SMPO_IF_RANGE)

#define  HTTP_SMPO_IF_UNMODIFIED_SINCE              "If-Unmodified-Since"
#define  HTTP_SMPO_IF_UNMODIFIED_SINCE_LENGTH       AnscSizeOfString(HTTP_SMPO_IF_UNMODIFIED_SINCE)

#define  HTTP_SMPO_LAST_MODIFIED                    "Last-Modified"
#define  HTTP_SMPO_LAST_MODIFIED_LENGTH             AnscSizeOfString(HTTP_SMPO_LAST_MODIFIED)      

#define  HTTP_SMPO_LOCATION                         "Location"
#define  HTTP_SMPO_LOCATION_LENGTH                  AnscSizeOfString(HTTP_SMPO_LOCATION)

#define  HTTP_SMPO_MAX_FORWARDS                     "Max-Forwards"
#define  HTTP_SMPO_MAX_FORWARDS_LENGTH              AnscSizeOfString(HTTP_SMPO_MAX_FORWARDS)

#define  HTTP_SMPO_PRAGMA                           "Pragma"
#define  HTTP_SMPO_PRAGMA_LENGTH                    AnscSizeOfString(HTTP_SMPO_PRAGMA)      

#define  HTTP_SMPO_PROXY_AUTHENTICATE               "Proxy-Authenticate"
#define  HTTP_SMPO_PROXY_AUTHENTICATE_LENGTH        AnscSizeOfString(HTTP_SMPO_PROXY_AUTHENTICATE)

#define  HTTP_SMPO_PROXY_AUTHORIZATION              "Proxy-Authorization"
#define  HTTP_SMPO_PROXY_AUTHORIZATION_LENGTH       AnscSizeOfString(HTTP_SMPO_PROXY_AUTHORIZATION)

#define  HTTP_SMPO_RANGE                            "Range"
#define  HTTP_SMPO_RANGE_LENGTH                     AnscSizeOfString(HTTP_SMPO_RANGE)

#define  HTTP_SMPO_REFERER                          "Referer"
#define  HTTP_SMPO_REFERER_LENGTH                   AnscSizeOfString(HTTP_SMPO_REFERER)

#define  HTTP_SMPO_RETRY_AFTER                      "Retry-After"
#define  HTTP_SMPO_RETRY_AFTER_LENGTH               AnscSizeOfString(HTTP_SMPO_RETRY_AFTER)

#define  HTTP_SMPO_SERVER                           "Server"
#define  HTTP_SMPO_SERVER_LENGTH                    AnscSizeOfString(HTTP_SMPO_SERVER)

#define  HTTP_SMPO_TE                               "TE"
#define  HTTP_SMPO_TE_LENGTH                        AnscSizeOfString(HTTP_SMPO_TE)

#define  HTTP_SMPO_TRAILER                          "Trailer"
#define  HTTP_SMPO_TRAILER_LENGTH                   AnscSizeOfString(HTTP_SMPO_TRAILER)

#define  HTTP_SMPO_TRANSFER_ENCODING                "Transfer-Encoding"
#define  HTTP_SMPO_TRANSFER_ENCODING_LENGTH         AnscSizeOfString(HTTP_SMPO_TRANSFER_ENCODING)

#define  HTTP_SMPO_UPGRADE                          "Upgrade"
#define  HTTP_SMPO_UPGRADE_LENGTH                   AnscSizeOfString(HTTP_SMPO_UPGRADE)

#define  HTTP_SMPO_USER_AGENT                       "User-Agent"
#define  HTTP_SMPO_USER_AGENT_LENGTH                AnscSizeOfString(HTTP_SMPO_USER_AGENT)

#define  HTTP_SMPO_VARY                             "Vary"
#define  HTTP_SMPO_VARY_LENGTH                      AnscSizeOfString(HTTP_SMPO_VARY)

#define  HTTP_SMPO_VIA                              "Via"
#define  HTTP_SMPO_VIA_LENGTH                       AnscSizeOfString(HTTP_SMPO_VIA)

#define  HTTP_SMPO_WARNING                          "Warning"
#define  HTTP_SMPO_WARNING_LENGTH                   AnscSizeOfString(HTTP_SMPO_WARNING)

#define  HTTP_SMPO_WWW_AUTHENTICATE                 "WWW-Authenticate"
#define  HTTP_SMPO_WWW_AUTHENTICATE_LENGTH          AnscSizeOfString(HTTP_SMPO_WWW_AUTHENTICATE)

#define  HTTP_SMPO_PROXY_CONNECTION                 "Proxy-Connection"
#define  HTTP_SMPO_PROXY_CONNECTION_LENGTH          AnscSizeOfString(HTTP_SMPO_PROXY_CONNECTION)

#define  HTTP_SMPO_COOKIE                           "Cookie"
#define  HTTP_SMPO_COOKIE_LENGTH                    AnscSizeOfString(HTTP_SMPO_COOKIE)

#define  HTTP_SMPO_SET_COOKIE                       "Set-Cookie"
#define  HTTP_SMPO_SET_COOKIE_LENGTH                AnscSizeOfString(HTTP_SMPO_SET_COOKIE)

#define  HTTP_SMPO_COOKIE2                          "Cookie2"
#define  HTTP_SMPO_COOKIE2_LENGTH                   AnscSizeOfString(HTTP_SMPO_COOKIE2)

#define  HTTP_SMPO_SET_COOKIE2                      "Set-Cookie2"
#define  HTTP_SMPO_SET_COOKIE2_LENGTH               AnscSizeOfString(HTTP_SMPO_SET_COOKIE2)

#define  HTTP_SMPO_CHUNKED_BODY                     "Chunked-Body"
#define  HTTP_SMPO_CHUNKED_BODY_LENGTH              AnscSizeOfString(HTTP_SMPO_CHUNKED_BODY)

#define  HTTP_SMPO_MEDIA_TYPE_ANY                   "*"
#define  HTTP_SMPO_MEDIA_TYPE_ANY_LENGTH            AnscSizeOfString(HTTP_SMPO_MEDIA_TYPE_ANY)

#define  HTTP_SMPO_MEDIA_SUBTYPE_ANY                "*"
#define  HTTP_SMPO_MEDIA_SUBTYPE_ANY_LENGTH         AnscSizeOfString(HTTP_SMPO_MEDIA_SUBTYPE_ANY)

#define  HTTP_SMPO_WKDAY_SUN                        "Sun"   
#define  HTTP_SMPO_WKDAY_SUN_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_SUN)

#define  HTTP_SMPO_WKDAY_MON                        "Mon"
#define  HTTP_SMPO_WKDAY_MON_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_MON)

#define  HTTP_SMPO_WKDAY_TUE                        "Tue"
#define  HTTP_SMPO_WKDAY_TUE_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_TUE)

#define  HTTP_SMPO_WKDAY_WED                        "Wed"
#define  HTTP_SMPO_WKDAY_WED_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_WED)

#define  HTTP_SMPO_WKDAY_THU                        "Thu"
#define  HTTP_SMPO_WKDAY_THU_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_THU)

#define  HTTP_SMPO_WKDAY_FRI                        "Fri"
#define  HTTP_SMPO_WKDAY_FRI_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_FRI)

#define  HTTP_SMPO_WKDAY_SAT                        "Sat"
#define  HTTP_SMPO_WKDAY_SAT_LENGTH                 AnscSizeOfString(HTTP_SMPO_WKDAY_SAT)

#define  HTTP_SMPO_WEEKDAY_SUNDAY                   "Sunday"
#define  HTTP_SMPO_WEEKDAY_SUNDAY_LENGTH            AnscSizeOfString(HTTP_SMPO_WEEKDAY_SUNDAY)

#define  HTTP_SMPO_WEEKDAY_MONDAY                   "Monday"
#define  HTTP_SMPO_WEEKDAY_MONDAY_LENGTH            AnscSizeOfString(HTTP_SMPO_WEEKDAY_MONDAY)

#define  HTTP_SMPO_WEEKDAY_TUESDAY                  "Tuesday"   
#define  HTTP_SMPO_WEEKDAY_TUESDAY_LENGTH           AnscSizeOfString(HTTP_SMPO_WEEKDAY_TUESDAY)

#define  HTTP_SMPO_WEEKDAY_WEDNESDAY                "Wednesday"
#define  HTTP_SMPO_WEEKDAY_WEDNESDAY_LENGTH         AnscSizeOfString(HTTP_SMPO_WEEKDAY_WEDNESDAY)

#define  HTTP_SMPO_WEEKDAY_THURSDAY                 "Thursday"
#define  HTTP_SMPO_WEEKDAY_THURSDAY_LENGTH          AnscSizeOfString(HTTP_SMPO_WEEKDAY_THURSDAY)

#define  HTTP_SMPO_WEEKDAY_FRIDAY                   "Friday"
#define  HTTP_SMPO_WEEKDAY_FRIDAY_LENGTH            AnscSizeOfString(HTTP_SMPO_WEEKDAY_FRIDAY)

#define  HTTP_SMPO_WEEKDAY_SATURDAY                 "Saturday"
#define  HTTP_SMPO_WEEKDAY_SATURDAY_LENGTH          AnscSizeOfString(HTTP_SMPO_WEEKDAY_SATURDAY)

#define  HTTP_SMPO_MONTH_JAN                        "Jan" 
#define  HTTP_SMPO_MONTH_JAN_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_JAN)

#define  HTTP_SMPO_MONTH_FEB                        "Feb"
#define  HTTP_SMPO_MONTH_FEB_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_FEB)

#define  HTTP_SMPO_MONTH_MAR                        "Mar"
#define  HTTP_SMPO_MONTH_MAR_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_MAR)

#define  HTTP_SMPO_MONTH_APR                        "Apr"
#define  HTTP_SMPO_MONTH_APR_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_APR)

#define  HTTP_SMPO_MONTH_MAY                        "May"
#define  HTTP_SMPO_MONTH_MAY_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_MAY)    

#define  HTTP_SMPO_MONTH_JUN                        "Jun"
#define  HTTP_SMPO_MONTH_JUN_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_JUN)

#define  HTTP_SMPO_MONTH_JUL                        "Jul"
#define  HTTP_SMPO_MONTH_JUL_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_JUL)    

#define  HTTP_SMPO_MONTH_AUG                        "Aug"
#define  HTTP_SMPO_MONTH_AUG_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_AUG)

#define  HTTP_SMPO_MONTH_SEP                        "Sep"
#define  HTTP_SMPO_MONTH_SEP_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_SEP)

#define  HTTP_SMPO_MONTH_OCT                        "Oct"
#define  HTTP_SMPO_MONTH_OCT_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_OCT)

#define  HTTP_SMPO_MONTH_NOV                        "Nov"
#define  HTTP_SMPO_MONTH_NOV_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_NOV)

#define  HTTP_SMPO_MONTH_DEC                        "Dec"
#define  HTTP_SMPO_MONTH_DEC_LENGTH                 AnscSizeOfString(HTTP_SMPO_MONTH_DEC)

#define  HTTP_SMPO_WEAK                             "weak"
#define  HTTP_SMPO_WEAK_LENGTH                      AnscSizeOfString(HTTP_SMPO_WEAK)

#define  HTTP_SMPO_100_CONTINUE                     "100-continue"
#define  HTTP_SMPO_100_CONTINUE_LENGTH              AnscSizeOfString(HTTP_SMPO_100_CONTINUE)

#define  HTTP_SMPO_EXPECTATION_EXTENSION            "expectation-extension"
#define  HTTP_SMPO_EXPECTATION_EXTENSION_LENGTH     AnscSizeOfString(HTTP_SMPO_EXPECTATION_EXTENSION)

#define  HTTP_SMPO_EXPECTATION_EXTENSION_SEPARATOR  '='
#define  HTTP_SMPO_EXPECTATION_EXTENSION_SEPARATOR_LENGTH   1

#define  HTTP_SMPO_IF_MATCH_ASTERISK                "*"
#define  HTTP_SMPO_IF_MATCH_ASTERISK_LENGTH         AnscSizeOfString(HTTP_SMPO_IF_MATCH_ASTERISK) 


#define  HTTP_SMPO_HEADER_NAME_VALUE_SEPARATOR      ':'
#define  HTTP_SMPO_HEADER_NAME_VALUE_SEPARATOR_LENGTH   1

#define  HTTP_SMPO_FIELD_SEPARATOR                  ", "
#define  HTTP_SMPO_FIELD_SEPARATOR_LENGTH           AnscSizeOfString(HTTP_SMPO_FIELD_SEPARATOR)

#define  HTTP_SMPO_HEADER_SEPARATOR                 ": "
#define  HTTP_SMPO_HEADER_SEPARATOR_LENGTH          AnscSizeOfString(HTTP_SMPO_HEADER_SEPARATOR)

#define  HTTP_SMPO_NAME                             "HTTP"
#define  HTTP_SMPO_NAME_LENGTH                      AnscSizeOfString(HTTP_SMPO_NAME)

#define  HTTP_SMPO_QUALITY                          "q"
#define  HTTP_SMPO_QUALITY_LENGTH                   AnscSizeOfString(HTTP_SMPO_QUALITY)

#define  HTTP_SMPO_BYTES_UNIT                       "bytes"
#define  HTTP_SMPO_BYTES_UNIT_LENGTH                AnscSizeOfString(HTTP_SMPO_BYTES_UNIT)

#define  HTTP_SMPO_QUALITY_PARAMS                   ";q="
#define  HTTP_SMPO_QUALITY_PARAMS_LENGTH            AnscSizeOfString(HTTP_SMPO_QUALITY_PARAMS)

#define  HTTP_SMPO_WEAK_VALUE                       "W/"
#define  HTTP_SMPO_WEAK_VALUE_LENGTH                AnscSizeOfString(HTTP_SMPO_WEAK_VALUE)

#define  HTTP_SMPO_PROXY_CONNECTION_TOKEN           "proxy-connection-token"
#define  HTTP_SMPO_PROXY_CONNECTION_TOKEN_LENGTH    AnscSizeOfString(HTTP_SMPO_PROXY_CONNECTION_TOKEN)

#define  HTTP_SMPO_FIELD_VALUE                      "field-value"
#define  HTTP_SMPO_FIELD_VALUE_LENGTH               AnscSizeOfString(HTTP_SMPO_FIELD_VALUE)

#define  HTTP_SMPO_SCHEME                           "http://"
#define  HTTP_SMPO_SCHEME_LENGTH                    AnscSizeOfString(HTTP_SMPO_SCHEME)

#define  HTTP_SMPO_SCHEME_HTTPS                     "https://"
#define  HTTP_SMPO_SCHEME_HTTPS_LENGTH              AnscSizeOfString(HTTP_SMPO_SCHEME_HTTPS)

#define  HTTP_SMPO_PROTOCOL_HTTPS                   "https"
#define  HTTP_SMPO_PROTOCOL_HTTPS_LENGTH            AnscSizeOfString(HTTP_SMPO_PROTOCOL_HTTPS)

#define  HTTP_SMPO_COOKIE_VERSION                   "cookie-version"
#define  HTTP_SMPO_COOKIE_VERSION_LENGTH            AnscSizeOfString(HTTP_SMPO_COOKIE_VERSION)

#define  HTTP_SMPO_COOKIE_VALUE                     "cookie-value"
#define  HTTP_SMPO_COOKIE_VALUE_LENGTH              AnscSizeOfString(HTTP_SMPO_COOKIE_VALUE)

#define  HTTP_SMPO_COOKIE_NAME                      "cookie-name"
#define  HTTP_SMPO_COOKIE_NAME_LENGTH               AnscSizeOfString(HTTP_SMPO_COOKIE_NAME)

#define  HTTP_SMPO_VALUE                            "value"
#define  HTTP_SMPO_VALUE_LENGTH                     AnscSizeOfString(HTTP_SMPO_VALUE)

#define  HTTP_SMPO_COOKIE_PATH                      "cookie-path"
#define  HTTP_SMPO_COOKIE_PATH_LENGTH               AnscSizeOfString(HTTP_SMPO_COOKIE_PATH)

#define  HTTP_SMPO_COOKIE_DOMAIN                    "cookie-domain"
#define  HTTP_SMPO_COOKIE_DOMAIN_LENGTH             AnscSizeOfString(HTTP_SMPO_COOKIE_DOMAIN)

#define  HTTP_SMPO_COOKIE_ENTITY                    "Cookie-Entity"
#define  HTTP_SMPO_COOKIE_ENTITY_LENGTH             AnscSizeOfString(HTTP_SMPO_COOKIE_ENTITY)

#define  HTTP_SMPO_COOKIE_AV                        "cookie-av"
#define  HTTP_SMPO_COOKIE_AV_LENGTH                 AnscSizeOfString(HTTP_SMPO_COOKIE_AV)

#define  HTTP_SMPO_COMMENT                          "comment"
#define  HTTP_SMPO_COMMENT_LENGTH                   AnscSizeOfString(HTTP_SMPO_COMMENT)

#define  HTTP_SMPO_TOKEN                            "token"
#define  HTTP_SMPO_TOKEN_LENGTH                     AnscSizeOfString(HTTP_SMPO_TOKEN)

#define  HTTP_SMPO_SET_COOKIE_COMMENT               "Comment"
#define  HTTP_SMPO_SET_COOKIE_COMMENT_LENGTH        AnscSizeOfString(HTTP_SMPO_SET_COOKIE_COMMENT)

#define  HTTP_SMPO_SET_COOKIE_COMMENT_URL           "CommentURL"
#define  HTTP_SMPO_SET_COOKIE_COMMENT_URL_LENGTH    AnscSizeOfString(HTTP_SMPO_SET_COOKIE_COMMENT_URL)

#define  HTTP_SMPO_SET_COOKIE_DOMAIN                "Domain"
#define  HTTP_SMPO_SET_COOKIE_DOMAIN_LENGTH         AnscSizeOfString(HTTP_SMPO_SET_COOKIE_DOMAIN)

#define  HTTP_SMPO_SET_COOKIE_MAX_AGE               "Max-Age"
#define  HTTP_SMPO_SET_COOKIE_MAX_AGE_LENGTH        AnscSizeOfString(HTTP_SMPO_SET_COOKIE_MAX_AGE)

#define  HTTP_SMPO_SET_COOKIE_PATH                  "Path"
#define  HTTP_SMPO_SET_COOKIE_PATH_LENGTH           AnscSizeOfString(HTTP_SMPO_SET_COOKIE_PATH)

#define  HTTP_SMPO_SET_COOKIE_SECURE                "Secure"
#define  HTTP_SMPO_SET_COOKIE_SECURE_LENGTH         AnscSizeOfString(HTTP_SMPO_SET_COOKIE_SECURE)

#define  HTTP_SMPO_SET_COOKIE_DISCARD               "Discard"
#define  HTTP_SMPO_SET_COOKIE_DISCARD_LENGTH        AnscSizeOfString(HTTP_SMPO_SET_COOKIE_DISCARD)

#define  HTTP_SMPO_SET_COOKIE_VERSION               "Version"
#define  HTTP_SMPO_SET_COOKIE_VERSION_LENGTH        AnscSizeOfString(HTTP_SMPO_SET_COOKIE_VERSION)

#define  HTTP_SMPO_SET_COOKIE_EXPIRES               "expires"
#define  HTTP_SMPO_SET_COOKIE_EXPIRES_LENGTH        AnscSizeOfString(HTTP_SMPO_SET_COOKIE_EXPIRES)

#define  HTTP_SMPO_SET_COOKIE_PORT                  "Port"
#define  HTTP_SMPO_SET_COOKIE_PORT_LENGTH           AnscSizeOfString(HTTP_SMPO_SET_COOKIE_PORT)

#define  HTTP_SMPO_COOKIE2_CONTENT                  "$Version=\"1\""
#define  HTTP_SMPO_COOKIE2_CONTENT_LENGTH           AnscSizeOfString(HTTP_SMPO_COOKIE2_CONTENT)

#define  HTTP_SMPO_SET_COOKIE_ATTR_PREFIX           "$"
#define  HTTP_SMPO_SET_COOKIE_ATTR_PREFIX_LENGTH    AnscSizeOfString(HTTP_SMPO_SET_COOKIE_ATTR_PREFIX)

#define  HTTP_SMPO_ATTR_SEPARATOR                   "="
#define  HTTP_SMPO_ATTR_SEPARATOR_LENGTH            AnscSizeOfString(HTTP_SMPO_ATTR_SEPARATOR)

#define  HTTP_SMPO_BASIC_CREDENTIALS                "basic-credentials"
#define  HTTP_SMPO_BASIC_CREDENTIALS_LENGTH         AnscSizeOfString(HTTP_SMPO_BASIC_CREDENTIALS)

#define  HTTP_SMPO_BASIC_COOKIE                     "basic-cookie"
#define  HTTP_SMPO_BASIC_COOKIE_LENGTH              AnscSizeOfString(HTTP_SMPO_BASIC_COOKIE)

#define  HTTP_SMPO_REALM                            "realm"
#define  HTTP_SMPO_REALM_LENGTH                     AnscSizeOfString(HTTP_SMPO_REALM)

#define  HTTP_SMPO_REALM_VALUE                      "realm-value"
#define  HTTP_SMPO_REALM_VALUE_LENGTH               AnscSizeOfString(HTTP_SMPO_REALM_VALUE)

#define  HTTP_SMPO_AUTH_SCHEME                      "auth-scheme"
#define  HTTP_SMPO_AUTH_SCHEME_LENGTH               AnscSizeOfString(HTTP_SMPO_AUTH_SCHEME)

#define  HTTP_SMPO_CREDENTIAL_BASIC_SCHEME          "Basic"
#define  HTTP_SMPO_CREDENTIAL_BASIC_SCHEME_LENGTH   AnscSizeOfString(HTTP_SMPO_CREDENTIAL_BASIC_SCHEME)

#define  HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME         "Digest"
#define  HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME_LENGTH  AnscSizeOfString(HTTP_SMPO_CREDENTIAL_DIGEST_SCHEME)

#define  HTTP_SMPO_SPACE                            " "
#define  HTTP_SMPO_SPACE_LENGTH                     AnscSizeOfString(HTTP_SMPO_SPACE)

#define  HTTP_SMPO_COOKIE_EXPIRES_DATE              "cookie-expires-date"
#define  HTTP_SMPO_COOKIE_EXPIRES_DATE_LENGTH       AnscSizeOfString(HTTP_SMPO_COOKIE_EXPIRES_DATE)

#define  HTTP_SMPO_COOKIE_NAME_VALUE                "cookie-name-value"
#define  HTTP_SMPO_COOKIE_NAME_VALUE_LENGTH         AnscSizeOfString(HTTP_SMPO_COOKIE_NAME_VALUE)

#define  HTTP_SMPO_COOKIE_NAME_VALUE_TOKEN          "cookie-name-value-token"
#define  HTTP_SMPO_COOKIE_NAME_VALUE_TOKEN_LENGTH   AnscSizeOfString(HTTP_SMPO_COOKIE_NAME_VALUE_TOKEN)

#define  HTTP_SMPO_SET_COOKIE_DOMAIN_NODE           "set-cookie-domain"
#define  HTTP_SMPO_SET_COOKIE_DOMAIN_NODE_LENGTH    AnscSizeOfString(HTTP_SMPO_SET_COOKIE_DOMAIN_NODE)

#define  HTTP_SMPO_SET_COOKIE_PATH_NODE             "set-cookie-path"
#define  HTTP_SMPO_SET_COOKIE_PATH_NODE_LENGTH      AnscSizeOfString(HTTP_SMPO_SET_COOKIE_PATH_NODE)

#define  HTTP_SMPO_CHUNK_SIZE                       "chunk-size"
#define  HTTP_SMPO_CHUNK_SIZE_LENGTH                AnscSizeOfString(HTTP_SMPO_CHUNK_SIZE)

#define  HTTP_SMPO_CHUNK_EXT_NAME                   "chunk-ext-name"
#define  HTTP_SMPO_CHUNK_EXT_NAME_LENGTH            AnscSizeOfString(HTTP_SMPO_CHUNK_EXT_NAME)

#define  HTTP_SMPO_CHUNK_EXT_VALUE                  "chunk-ext-val"
#define  HTTP_SMPO_CHUNK_EXT_VALUE_LENGTH           AnscSizeOfString(HTTP_SMPO_CHUNK_EXT_VALUE)

#define  HTTP_SMPO_MULTIPART_BOUNDARY_DELI          "--"
#define  HTTP_SMPO_MULTIPART_BOUNDARY_DELI_LENGTH   AnscSizeOfString(HTTP_SMPO_MULTIPART_BOUNDARY_DELI)

#define  HTTP_SMPO_LINE_BREAKER                     "\r\n"
#define  HTTP_SMPO_LINE_BREAKER_LENGTH              AnscSizeOfString(HTTP_SMPO_LINE_BREAKER)

#define  HTTP_SMPO_DOUBLE_LINE_BREAKER              "\r\n\r\n"
#define  HTTP_SMPO_DOUBLE_LINE_BREAKER_LENGTH       AnscSizeOfString(HTTP_SMPO_DOUBLE_LINE_BREAKER)

#define  HTTP_SMPO_CONTENT_DISPOSITION              "Content-Disposition"
#define  HTTP_SMPO_CONTENT_DISPOSITION_LENGTH       AnscSizeOfString(HTTP_SMPO_CONTENT_DISPOSITION)

#define  HTTP_SMPO_MULTIPART_FIELD_NAME             "name="
#define  HTTP_SMPO_MULTIPART_FIELD_NAME_LENGTH      AnscSizeOfString(HTTP_SMPO_MULTIPART_FIELD_NAME)

#define  HTTP_SMPO_CHAR_SPACE                       ' '
#define  HTTP_SMPO_CHAR_HTAB                        0x09
#define  HTTP_SMPO_CHAR_CR                          '\r'
#define  HTTP_SMPO_CHAR_LF                          '\n'
#define  HTTP_SMPO_CHAR_COLON                       ':'
#define  HTTP_SMPO_CHAR_COMMA                       ','
#define  HTTP_SMPO_CHAR_HYPHEN                      '-'
#define  HTTP_SMPO_CHAR_SLASH                       '/'
#define  HTTP_SMPO_CHAR_ASTERISK                    '*'
#define  HTTP_SMPO_CHAR_SEMICOLON                   ';'
#define  HTTP_SMPO_CHAR_DOT                         '.'
#define  HTTP_SMPO_CHAR_QUESTION                    '?'
#define  HTTP_SMPO_CHAR_DQUOTE                      '"'
#define  HTTP_SMPO_CHAR_EQUAL                       '='
#define  HTTP_SMPO_CHAR_LESS                        '<'
#define  HTTP_SMPO_CHAR_LARGE                       '>'
#define  HTTP_SMPO_CHAR_AT                          '@'

#define  HTTP_SMPO_STRING_QUALITY                   "q="
#define  HTTP_SMPO_STRING_COOKIE_VERSION            "$Version"
#define  HTTP_SMPO_STRING_COOKIE_PATH               "$Path"
#define  HTTP_SMPO_STRING_COOKIE_DOMAIN             "$Domain"
#define  HTTP_SMPO_STRING_COOKIE_PORT               "$Port"

#define  HTTP_SMPO_STRING_SCOOKIE_COMMENT           "Comment"
#define  HTTP_SMPO_STRING_SCOOKIE_DOMAIN            "Domain"
#define  HTTP_SMPO_STRING_SCOOKIE_MAX_AGE           "Max-Age"
#define  HTTP_SMPO_STRING_SCOOKIE_PATH              "Path"
#define  HTTP_SMPO_STRING_SCOOKIE_SECURE            "Secure"
#define  HTTP_SMPO_STRING_SCOOKIE_VERSION           "Version"
#define  HTTP_SMPO_STRING_SCOOKIE_EXPIRES           "expires"

#define  HTTP_SMPO_STRING_SCOOKIE2_DISCARD          "Discard"
#define  HTTP_SMPO_STRING_SCOOKIE2_COMMENT_URL      "CommentURL"
#define  HTTP_SMPO_STRING_SCOOKIE2_PORT             "Port"

/*
 *  structure for multipart
 */
typedef  struct _HTTP_SMPO_MULTIPART_PART
{
    SINGLE_LINK_ENTRY               Linkage;

    ULONG                           ulType;

    PUCHAR                          pHeader;
    ULONG                           ulHeaderLen;

    PUCHAR                          pName;
    ULONG                           ulNameLen;

    PUCHAR                          pValue;
    ULONG                           ulValueLen;
}
HTTP_SMPO_MULTIPART_PART, *PHTTP_SMPO_MULTIPART_PART;

#define  ACCESS_HTTP_SMPO_MULTIPART_PART_OBJECT(p)            \
         ACCESS_CONTAINER(p, HTTP_SMPO_MULTIPART_PART, Linkage)

#define  HttpSmpoMultipartPartInit(pPart)                           \
    (pPart)->pHeader        = NULL;                                 \
    (pPart)->ulHeaderLen    = 0;                                    \
    (pPart)->pName          = NULL;                                 \
    (pPart)->ulNameLen      = 0;                                    \
    (pPart)->pValue         = NULL;                                 \
    (pPart)->ulValueLen     = 0;                                    \


#endif
