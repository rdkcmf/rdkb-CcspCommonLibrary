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

    module:	http_definitions.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the Http element parameters that can be
        applied to the Http Systems.

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


#ifndef  _HTTP_DEFINITIONS_
#define  _HTTP_DEFINITIONS_


/*
 * An HTTP server is often used as a gateway to a legacy information system; for example, an
 * existing body of documents or an existing database application. The Common Gateway Interface is
 * an agreement between HTTP server implementors about how to integrate such gateway scripts and
 * programs. It is typically used in conjunction with HTML forms to build database applications.
 */
#include  "http_cgi_definitions.h"


/***********************************************************
        HTTP PROTOCOL COMPONENT PARAMETERS DEFINITION
***********************************************************/

/*
 * The set of common methods for HTTP/1.1 is defined below. Although this set can be expanded,
 * additional methods cannot be assumed to share the same semantics for separately extended clients
 * and servers. The Host request-header field MUST accompany all HTTP/1.1 requests.
 */
#define  HTTP_METHOD_RESERVED                       HTTP_METHOD_CODE_RESERVED
#define  HTTP_METHOD_OPTIONS                        HTTP_METHOD_CODE_OPTIONS
#define  HTTP_METHOD_GET                            HTTP_METHOD_CODE_GET
#define  HTTP_METHOD_HEAD                           HTTP_METHOD_CODE_HEAD
#define  HTTP_METHOD_POST                           HTTP_METHOD_CODE_POST
#define  HTTP_METHOD_PUT                            HTTP_METHOD_CODE_PUT
#define  HTTP_METHOD_DELETE                         HTTP_METHOD_CODE_DELETE
#define  HTTP_METHOD_TRACE                          HTTP_METHOD_CODE_TRACE
#define  HTTP_METHOD_CONNECT                        HTTP_METHOD_CODE_CONNECT

#define  HTTP_METHOD_NOTIFY                         HTTP_METHOD_CODE_NOTIFY
#define  HTTP_METHOD_SEARCH                         HTTP_METHOD_CODE_SEARCH
#define  HTTP_METHOD_M_SEARCH                       HTTP_METHOD_CODE_M_SEARCH
#define  HTTP_METHOD_M_POST                         HTTP_METHOD_CODE_M_POST
#define  HTTP_METHOD_SUBSCRIBE                      HTTP_METHOD_CODE_SUBSCRIBE
#define  HTTP_METHOD_UNSUBSCRIBE                    HTTP_METHOD_CODE_UNSUBSCRIBE

#define  HTTP_METHOD_CODE_RESERVED                  0
#define  HTTP_METHOD_CODE_OPTIONS                   1
#define  HTTP_METHOD_CODE_GET                       2
#define  HTTP_METHOD_CODE_HEAD                      3
#define  HTTP_METHOD_CODE_POST                      4
#define  HTTP_METHOD_CODE_PUT                       5
#define  HTTP_METHOD_CODE_DELETE                    6
#define  HTTP_METHOD_CODE_TRACE                     7
#define  HTTP_METHOD_CODE_CONNECT                   8

#define  HTTP_METHOD_CODE_NOTIFY                    101
#define  HTTP_METHOD_CODE_SEARCH                    102
#define  HTTP_METHOD_CODE_M_SEARCH                  103
#define  HTTP_METHOD_CODE_M_POST                    104
#define  HTTP_METHOD_CODE_SUBSCRIBE                 105
#define  HTTP_METHOD_CODE_UNSUBSCRIBE               106

#define  HTTP_METHOD_NAME_OPTIONS                   "OPTIONS"
#define  HTTP_METHOD_NAME_GET                       "GET"
#define  HTTP_METHOD_NAME_HEAD                      "HEAD"
#define  HTTP_METHOD_NAME_POST                      "POST"
#define  HTTP_METHOD_NAME_PUT                       "PUT"
#define  HTTP_METHOD_NAME_DELETE                    "DELETE"
#define  HTTP_METHOD_NAME_TRACE                     "TRACE"
#define  HTTP_METHOD_NAME_CONNECT                   "CONNECT"

#define  HTTP_METHOD_NAME_NOTIFY                    "NOTIFY"
#define  HTTP_METHOD_NAME_SEARCH                    "SEARCH"
#define  HTTP_METHOD_NAME_M_SEARCH                  "M-SEARCH"
#define  HTTP_METHOD_NAME_M_POST                    "M-POST"
#define  HTTP_METHOD_NAME_SUBSCRIBE                 "SUBSCRIBE"
#define  HTTP_METHOD_NAME_UNSUBSCRIBE               "UNSUBSCRIBE"


#define  HTTP_STATUS_INFORMATIONAL                  100
#define  HTTP_STATUS_SUCCESS                        200
#define  HTTP_STATUS_REDIRECTION                    300
#define  HTTP_STATUS_CLIENT_ERROR                   400
#define  HTTP_STATUS_SERVER_ERROR                   500

#define  HTTP_STATUS_CONTINUE                       100
#define  HTTP_STATUS_SWITCHING_PROTOCOL             101

#define  HTTP_STATUS_OK                             200
#define  HTTP_STATUS_CREATED                        201
#define  HTTP_STATUS_ACCEPTED                       202
#define  HTTP_STATUS_NON_AUTHORITATIVE              203
#define  HTTP_STATUS_NO_CONTENT                     204
#define  HTTP_STATUS_RESET_CONTENT                  205
#define  HTTP_STATUS_PARTIAL_CONTENT                206

#define  HTTP_STATUS_MULTIPLE_CHOICES               300
#define  HTTP_STATUS_MOVED_PERMANENTLY              301
#define  HTTP_STATUS_FOUND                          302
#define  HTTP_STATUS_SEE_OTHER                      303
#define  HTTP_STATUS_NOT_MODIFIED                   304
#define  HTTP_STATUS_USE_PROXY                      305
#define  HTTP_STATUS_TEMP_REDIRECT                  307

#define  HTTP_STATUS_BAD_REQUEST                    400
#define  HTTP_STATUS_UNAUTHORIZED                   401
#define  HTTP_STATUS_PAYMENT_REQUIRED               402
#define  HTTP_STATUS_FORBIDDEN                      403
#define  HTTP_STATUS_NOT_FOUND                      404
#define  HTTP_STATUS_METHOD_NOT_ALLOWED             405
#define  HTTP_STATUS_NOT_ACCEPTABLE                 406
#define  HTTP_STATUS_PROXY_AUTH_REQUIRED            407
#define  HTTP_STATUS_REQUEST_TIMEOUT                408
#define  HTTP_STATUS_CONFLICT                       409
#define  HTTP_STATUS_GONE                           410
#define  HTTP_STATUS_LENGTH_REQUIRED                411
#define  HTTP_STATUS_PRECON_FAILED                  412
#define  HTTP_STATUS_ENTITY_TOO_LARGE               413
#define  HTTP_STATUS_URI_TOO_LONG                   414
#define  HTTP_STATUS_UNSUPPORTED_MEDIA              415
#define  HTTP_STATUS_RANGE_NOT_SATISFIABLE          416
#define  HTTP_STATUS_EXPECTATION_FAILED             417

#define  HTTP_STATUS_INTERNAL_SERVER_ERROR          500
#define  HTTP_STATUS_NOT_IMPLEMENTED                501
#define  HTTP_STATUS_BAD_GATEWAY                    502
#define  HTTP_STATUS_SERVICE_UNAVAILABLE            503
#define  HTTP_STATUS_GATEWAY_TIMEOUT                504
#define  HTTP_STATUS_VERSION_NOT_SUPPORTED          505

#define  HTTP_PHRASE_INFORMATIONAL                  "Continue"
#define  HTTP_PHRASE_SUCCESS                        "OK"
#define  HTTP_PHRASE_REDIRECTION                    "Multiple Choices"
#define  HTTP_PHRASE_CLIENT_ERROR                   "Bad Request"
#define  HTTP_PHRASE_SERVER_ERROR                   "Internal Server Error"

#define  HTTP_PHRASE_CONTINUE                       "Continue"
#define  HTTP_PHRASE_SWITCHING_PROTOCOL             "Switching Protocols"

#define  HTTP_PHRASE_OK                             "OK"
#define  HTTP_PHRASE_CREATED                        "Created"
#define  HTTP_PHRASE_ACCEPTED                       "Accepted"
#define  HTTP_PHRASE_NON_AUTHORITATIVE              "Non-Authoritative Information"
#define  HTTP_PHRASE_NO_CONTENT                     "No Content"
#define  HTTP_PHRASE_RESET_CONTENT                  "Reset Content"
#define  HTTP_PHRASE_PARTIAL_CONTENT                "Partial Content"

#define  HTTP_PHRASE_MULTIPLE_CHOICES               "Multiple Choices"
#define  HTTP_PHRASE_MOVED_PERMANENTLY              "Moved Permanently"
#define  HTTP_PHRASE_FOUND                          "Found"
#define  HTTP_PHRASE_SEE_OTHER                      "See Other"
#define  HTTP_PHRASE_NOT_MODIFIED                   "Not Modified"
#define  HTTP_PHRASE_USE_PROXY                      "Use Proxy"
#define  HTTP_PHRASE_TEMP_REDIRECT                  "Temporary Redirect"

#define  HTTP_PHRASE_BAD_REQUEST                    "Bad Request"
#define  HTTP_PHRASE_UNAUTHORIZED                   "Unauthorized"
#define  HTTP_PHRASE_PAYMENT_REQUIRED               "Payment Required"
#define  HTTP_PHRASE_FORBIDDEN                      "Forbidden"
#define  HTTP_PHRASE_NOT_FOUND                      "Not Found"
#define  HTTP_PHRASE_METHOD_NOT_ALLOWED             "Method Not Allowed"
#define  HTTP_PHRASE_NOT_ACCEPTABLE                 "Not Acceptable"
#define  HTTP_PHRASE_PROXY_AUTH_REQUIRED            "Proxy Authentication Required"
#define  HTTP_PHRASE_REQUEST_TIMEOUT                "Request Time-out"
#define  HTTP_PHRASE_CONFLICT                       "Conflict"
#define  HTTP_PHRASE_GONE                           "Gone"
#define  HTTP_PHRASE_LENGTH_REQUIRED                "Length Required"
#define  HTTP_PHRASE_PRECON_FAILED                  "Precondition Failed"
#define  HTTP_PHRASE_ENTITY_TOO_LARGE               "Request Entity Too Large"
#define  HTTP_PHRASE_URI_TOO_LONG                   "Request-URI Too Large"
#define  HTTP_PHRASE_UNSUPPORTED_MEDIA              "Unsupported Media Type"
#define  HTTP_PHRASE_RANGE_NOT_SATISFIABLE          "Requested Range Not Satisfiable"
#define  HTTP_PHRASE_EXPECTATION_FAILED             "Expectation Failed"

#define  HTTP_PHRASE_INTERNAL_SERVER_ERROR          "Internal Server Error"
#define  HTTP_PHRASE_NOT_IMPLEMENTED                "Not Implemented"
#define  HTTP_PHRASE_BAD_GATEWAY                    "Bad Gateway"
#define  HTTP_PHRASE_SERVICE_UNAVAILABLE            "Service Unavailable"
#define  HTTP_PHRASE_GATEWAY_TIMEOUT                "Gateway Time-out"
#define  HTTP_PHRASE_VERSION_NOT_SUPPORTED          "HTTP Version Not Supported"

/*
 * HTTP header fields, which include general-header, request-header, response header, and entity-
 * header fields, follow the same generic format as that give in Section 3.1 of RFC 822. Each
 * header field consists of a name followed by a colon (":") and the field value. Field names are
 * case-insensitive. The message-body (if any) of an HTTP message is used to carry the entity-body
 * associated with the request or response.
 */
#define  HTTP_HEADER_ID_RESERVED                    0
#define  HTTP_HEADER_ID_ACCEPT                      1
#define  HTTP_HEADER_ID_ACCEPT_CHARSET              2
#define  HTTP_HEADER_ID_ACCEPT_ENCODING             3
#define  HTTP_HEADER_ID_ACCEPT_LANGUAGE             4
#define  HTTP_HEADER_ID_ACCEPT_RANGES               5
#define  HTTP_HEADER_ID_AGE                         6
#define  HTTP_HEADER_ID_ALLOW                       7
#define  HTTP_HEADER_ID_AUTHORIZATION               8
#define  HTTP_HEADER_ID_CACHE_CONTROL               9
#define  HTTP_HEADER_ID_CONNECTION                  10
#define  HTTP_HEADER_ID_CONTENT_ENCODING            11
#define  HTTP_HEADER_ID_CONTENT_LANGUAGE            12
#define  HTTP_HEADER_ID_CONTENT_LENGTH              13
#define  HTTP_HEADER_ID_CONTENT_LOCATION            14
#define  HTTP_HEADER_ID_CONTENT_MD5                 15
#define  HTTP_HEADER_ID_CONTENT_RANGE               16
#define  HTTP_HEADER_ID_CONTENT_TYPE                17
#define  HTTP_HEADER_ID_DATE                        18
#define  HTTP_HEADER_ID_ETAG                        19
#define  HTTP_HEADER_ID_EXPECT                      20
#define  HTTP_HEADER_ID_EXPIRES                     21
#define  HTTP_HEADER_ID_FROM                        22
#define  HTTP_HEADER_ID_HOST                        23
#define  HTTP_HEADER_ID_IF_MATCH                    24
#define  HTTP_HEADER_ID_IF_MODIFIED_SINCE           25
#define  HTTP_HEADER_ID_IF_NONE_MATCH               26
#define  HTTP_HEADER_ID_IF_RANGE                    27
#define  HTTP_HEADER_ID_IF_UNMODIFIED_SINCE         28
#define  HTTP_HEADER_ID_LAST_MODIFIED               29
#define  HTTP_HEADER_ID_LOCATION                    30
#define  HTTP_HEADER_ID_MAX_FORWARDS                31
#define  HTTP_HEADER_ID_PRAGMA                      32
#define  HTTP_HEADER_ID_PROXY_AUTHENTICATE          33
#define  HTTP_HEADER_ID_PROXY_AUTHORIZATION         34
#define  HTTP_HEADER_ID_RANGE                       35
#define  HTTP_HEADER_ID_REFERER                     36
#define  HTTP_HEADER_ID_RETRY_AFTER                 37
#define  HTTP_HEADER_ID_SERVER                      38
#define  HTTP_HEADER_ID_TE                          39
#define  HTTP_HEADER_ID_TRAILER                     40
#define  HTTP_HEADER_ID_TRANSFER_ENCODING           41
#define  HTTP_HEADER_ID_UPGRADE                     42
#define  HTTP_HEADER_ID_USER_AGENT                  43
#define  HTTP_HEADER_ID_VARY                        44
#define  HTTP_HEADER_ID_VIA                         45
#define  HTTP_HEADER_ID_WARNING                     46
#define  HTTP_HEADER_ID_WWW_AUTHENTICATE            47

#define  HTTP_HEADER_ID_EXTENSION                   128
#define  HTTP_HEADER_ID_PROXY_CONNECTION            HTTP_HEADER_ID_EXTENSION + 1
#define  HTTP_HEADER_ID_COOKIE                      HTTP_HEADER_ID_EXTENSION + 2
#define  HTTP_HEADER_ID_SET_COOKIE                  HTTP_HEADER_ID_EXTENSION + 3
#define  HTTP_HEADER_ID_COOKIE2                     HTTP_HEADER_ID_EXTENSION + 4
#define  HTTP_HEADER_ID_SET_COOKIE2                 HTTP_HEADER_ID_EXTENSION + 5
#define  HTTP_HEADER_ID_WILDCARD                    0xFFFFFFFF

#define  HTTP_HEADER_NAME_ACCEPT                    "Accept"
#define  HTTP_HEADER_NAME_ACCEPT_CHARSET            "Accept-Charset"
#define  HTTP_HEADER_NAME_ACCEPT_ENCODING           "Accept-Encoding"
#define  HTTP_HEADER_NAME_ACCEPT_LANGUAGE           "Accept-Language"
#define  HTTP_HEADER_NAME_ACCEPT_RANGES             "Accept-Ranges"
#define  HTTP_HEADER_NAME_AGE                       "Age"
#define  HTTP_HEADER_NAME_ALLOW                     "Allow"
#define  HTTP_HEADER_NAME_AUTHORIZATION             "Authorization"
#define  HTTP_HEADER_NAME_CACHE_CONTROL             "Cache-Control"
#define  HTTP_HEADER_NAME_CONNECTION                "Connection"
#define  HTTP_HEADER_NAME_CONTENT_ENCODING          "Content-Encoding"
#define  HTTP_HEADER_NAME_CONTENT_LANGUAGE          "Content-Language"
#define  HTTP_HEADER_NAME_CONTENT_LENGTH            "Content-Length"
#define  HTTP_HEADER_NAME_CONTENT_LOCATION          "Content-Location"
#define  HTTP_HEADER_NAME_CONTENT_MD5               "Content-MD5"
#define  HTTP_HEADER_NAME_CONTENT_RANGE             "Content-Range"
#define  HTTP_HEADER_NAME_CONTENT_TYPE              "Content-Type"
#define  HTTP_HEADER_NAME_DATE                      "Date"
#define  HTTP_HEADER_NAME_ETAG                      "ETag"
#define  HTTP_HEADER_NAME_EXPECT                    "Expect"
#define  HTTP_HEADER_NAME_EXPIRES                   "Expires"
#define  HTTP_HEADER_NAME_FROM                      "From"
#define  HTTP_HEADER_NAME_HOST                      "Host"
#define  HTTP_HEADER_NAME_IF_MATCH                  "If-Match"
#define  HTTP_HEADER_NAME_IF_MODIFIED_SINCE         "If-Modified-Since"
#define  HTTP_HEADER_NAME_IF_NONE_MATCH             "If-None-Match"
#define  HTTP_HEADER_NAME_IF_RANGE                  "If-Range"
#define  HTTP_HEADER_NAME_IF_UNMODIFIED_SINCE       "If-Unmodified-Since"
#define  HTTP_HEADER_NAME_LAST_MODIFIED             "Last-Modified"
#define  HTTP_HEADER_NAME_LOCATION                  "Location"
#define  HTTP_HEADER_NAME_MAX_FORWARDS              "Max-Forwards"
#define  HTTP_HEADER_NAME_PRAGMA                    "Pragma"
#define  HTTP_HEADER_NAME_PROXY_AUTHENTICATE        "Proxy-Authenticate"
#define  HTTP_HEADER_NAME_PROXY_AUTHORIZATION       "Proxy-Authorization"
#define  HTTP_HEADER_NAME_RANGE                     "Range"
#define  HTTP_HEADER_NAME_REFERER                   "Referer"
#define  HTTP_HEADER_NAME_RETRY_AFTER               "Retry-After"
#define  HTTP_HEADER_NAME_SERVER                    "Server"
#define  HTTP_HEADER_NAME_TE                        "TE"
#define  HTTP_HEADER_NAME_TRAILER                   "Trailer"
#define  HTTP_HEADER_NAME_TRANSFER_ENCODING         "Transfer-Encoding"
#define  HTTP_HEADER_NAME_UPGRADE                   "Upgrade"
#define  HTTP_HEADER_NAME_USER_AGENT                "User-Agent"
#define  HTTP_HEADER_NAME_VARY                      "Vary"
#define  HTTP_HEADER_NAME_VIA                       "Via"
#define  HTTP_HEADER_NAME_WARNING                   "Warning"
#define  HTTP_HEADER_NAME_WWW_AUTHENTICATE          "WWW-Authenticate"

#define  HTTP_HEADER_NAME_PROXY_CONNECTION          "Proxy-Connection"
#define  HTTP_HEADER_NAME_COOKIE                    "Cookie"
#define  HTTP_HEADER_NAME_SET_COOKIE                "Set-Cookie"
#define  HTTP_HEADER_NAME_COOKIE2                   "Cookie2"
#define  HTTP_HEADER_NAME_SET_COOKIE2               "Set-Cookie2"
#define  HTTP_HEADER_NAME_WILDCARD                  "*"

/*
 * HTTP/1.1 allows a client to request that only part (a range of) the response entity be included
 * within the response. HTTP/1.1 uses range units in the Range and Content-Range header fields. An
 * entity can be broken down into subrange according to various structural units.
 */
#define  HTTP_RANGE_UNIT_TEXT_NONE                  "none"
#define  HTTP_RANGE_UNIT_TEXT_BYTES                 "bytes"

#define  HTTP_RANGE_UNIT_CODE_RESERVED              0
#define  HTTP_RANGE_UNIT_CODE_NONE                  1
#define  HTTP_RANGE_UNIT_CODE_BYTES                 2

/*
 * The Cache-Control general-header field is used to specify directives that MUST be obeyed by all
 * caching mechanisms along the request/response chain. The directives specify behavior intended to
 * prevent caches from adversely interfering with the request or response. These directives
 * typically override the default caching algorithms. Cache directives are unidirectional in that
 * the presence of a directive in a request does not imply that the same directive is to be given
 * in the response.
 */
#define  HTTP_CACHE_DIR_TEXT_NO_CACHE               "no-cache"
#define  HTTP_CACHE_DIR_TEXT_NO_STORE               "no-store"
#define  HTTP_CACHE_DIR_TEXT_MAX_AGE                "max-age"
#define  HTTP_CACHE_DIR_TEXT_MAX_STALE              "max-stale"
#define  HTTP_CACHE_DIR_TEXT_MIN_FRESH              "min-fresh"
#define  HTTP_CACHE_DIR_TEXT_NO_TRANSFORM           "no-transform"
#define  HTTP_CACHE_DIR_TEXT_ONLY_IF_CACHED         "only-if-cached"
#define  HTTP_CACHE_DIR_TEXT_PUBLIC                 "public"
#define  HTTP_CACHE_DIR_TEXT_PRIVATE                "private"
#define  HTTP_CACHE_DIR_TEXT_MUST_REVALIDATE        "must-revalidate"
#define  HTTP_CACHE_DIR_TEXT_PROXY_REVALIDATE       "proxy-revalidate"
#define  HTTP_CACHE_DIR_TEXT_S_MAXAGE               "s-maxage"

/*
 * The Warning general-header field is used to carry additional information about the status or
 * transformation of a message which might not be reflected in the message. This information is
 * typically used to warn about a possible lack of semantic transparency from caching operations or
 * transformations applied to the entity body of the message.
 */
#define  HTTP_WARNING_STALE_RESPONSE                110
#define  HTTP_WARNING_REVALIDATION_FAILED           111
#define  HTTP_WARNING_DISCONNECTED_OPERATION        112
#define  HTTP_WARNING_HEURISTIC_EXPIRATION          113
#define  HTTP_WARNING_MISCELLANEOUS                 114
#define  HTTP_WARNING_TRANSFORMATION_APPLIED        214
#define  HTTP_WARNING_MISCELLANEOUS_PERSISTENT      299

/*
 * HTTP provides a simple challenge-response authentication mechanism that MAY be used by a server
 * to challenge a client request and by a client to provide authentication information. It uses an
 * extensible, case-insensitive token to identify the authentication scheme, followed by a comma-
 * separated list of attribute-value pairs which carry the parameters necessary for achieving
 * authentication via that scheme.
 *
 *      auth-scheme    = token
 *      auth-param     = token "=" ( token | quoted-string )
 *
 * The 401 (Unauthorized) response message is used by an origin server to challenge the authori-
 * zation of a user agent. This response MUST include a WWW-Authenticate header field containing at
 * least one challenge applicable to the requested resource. The 407 (Proxy Authentication Required)
 * response message is used by a proxy to challenge the authorization of a client and MUST include
 * a Proxy-Authenticate header field containing at least one challenge applicable to the proxy for
 * the requested resource.
 */
#define  HTTP_AUTH_TYPE_RESERVED                    0
#define  HTTP_AUTH_TYPE_NONE                        1
#define  HTTP_AUTH_TYPE_BASIC                       2
#define  HTTP_AUTH_TYPE_DIGEST                      3

#define  HTTP_AUTH_TEXT_RESERVED                    "reserved"
#define  HTTP_AUTH_TEXT_NONE                        "none"
#define  HTTP_AUTH_TEXT_BASIC                       "basic"
#define  HTTP_AUTH_TEXT_DIGEST                      "digest"

/*
 * HTTP is a text-based protocol, which has the advantages of being flexible and extensible while
 * inherently slower than binary protocols. We define some text strings that are regularly used
 * by different HTTP components along with some other constant values.
 */
#define  HTTP_CURRENT_VERSION                       "HTTP/1.1"
#define  HTTP_CURRENT_MAJOR_VERSION                 1
#define  HTTP_CURRENT_MINOR_VERSION                 1
#define  HTTP_SERVER_PRODUCT_NAME                   "BroadWay Web Manager/2.1"
#define  HTTP_ROOT_URI_PATH                         "/"
#define  HTTP_DEFAULT_MIME_TYPE                     "text/html"
#define  HTTP_CURRENT_COOKIE_VERSION                1


/***********************************************************
            HTTP REQUEST URI PARAMETERS DEFINITION
***********************************************************/

/*
 * The Request-URI is a Uniform Resource Identifier and identifies the resource upon which to apply
 * the request.
 *
 *      Request-URI = "*" | absoluteURI | abs_path | authority
 *
 * The four options for Request-URI are dependent on the nature of the request. The asterisk "*"
 * means that the request does not apply to a particular resource, but to the server itself, and
 * is only allowed when the method used does not necessarily apply to a resource.
 */
#define  HTTP_URI_TYPE_RESERVED                     0
#define  HTTP_URI_TYPE_ASTERISK                     1
#define  HTTP_URI_TYPE_ABSOLUTE_URI                 2
#define  HTTP_URI_TYPE_RELATIVE_URI                 3
#define  HTTP_URI_TYPE_ABS_PATH                     4
#define  HTTP_URI_TYPE_AUTHORITY                    5

#define  HTTP_URI_FLAG_HTTPS                        0x00000001

/*
 * As illustrated above, not every URI form contains the host name, host port (Tcp port 80 by
 * default), or the query parameters. The message parser MUST always indicate the number of path
 * levels by setting the path_level member. Note that its value MUST be greater or equal to 1,
 * which indicates the root "/". The query parameters will be interpreted by a separeate parser.
 * If the URI form is absolute_uri, the port number MUST be set even it's not present in the
 * host part (80 in this case).
 */
#define  HTTP_MAX_SERVER_PREFIX_LEN                 16
#define  HTTP_MAX_PATH_LEVEL_NUMBER                 16
#define  HTTP_MAX_PATH_NAME_SIZE                    128
#define  HTTP_MAX_URI_QUERY_SIZE                    4096   
#define  HTTP_MAX_HEADER_FIELD_SIZE                 8192
#define  HTTP_MAX_QUERY_STRING_SIZE                 524288      /* as the message body of a POST method */

typedef  struct
_HTTP_REQUEST_URI
{
    ULONG                           Flag;
    ULONG                           Type;
    char                            HostName   [ANSC_DOMAIN_NAME_SIZE     ];
    USHORT                          HostPort;
    ULONG                           PathLevel;
    char                            PathArray  [HTTP_MAX_PATH_LEVEL_NUMBER][HTTP_MAX_PATH_NAME_SIZE];
    char                            QueryParams[HTTP_MAX_URI_QUERY_SIZE   ];
}
HTTP_REQUEST_URI,  *PHTTP_REQUEST_URI;


/***********************************************************
           HTTP FORM ELEMENT PARAMETERS DEFINITION
***********************************************************/

/*
 * HTML forms let you create a variety of user interface controls to collect input on a web page.
 * Each of the controls typically has a name and a value, where the name is specified in the HTML
 * and the value comes either from the HTML or by means of user input. The entire form is asso-
 * ciated with the URL of a program that will process the data, and when the user submits the form
 * (usually by pressing a button), the names and values of the controls are sent to the designated
 * URL as a string of the form:
 *
 *      Name1=Value1&Name2=Value2...NameN=ValueN
 *
 * This string can be sent to the designated program in one of two ways. The first, which uses the
 * HTTP GET method, appends the string to the end of the specified URL, after a question mark. The
 * second way data can be sent is by the HTTP POST method. Here, the POST request line, the HTTP
 * request headers, and a blank line are first sent the server, and then the data string is sent
 * on the following line.
 */
#define  HTTP_MAX_QUERY_NAME_SIZE                   128
#define  HTTP_MAX_QUERY_VALUE_SIZE                  256

#define  HTTP_FORM_DATA_TYPE_text                   1
#define  HTTP_FORM_DATA_TYPE_binary                 2
#define  HTTP_FORM_DATA_TYPE_base64                 3

typedef  struct
_HTTP_FORM_ELEMENT
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           FormIndex;
    ULONG                           HashIndex;
    ULONG                           ValueType;
    char*                           Name;
    char*                           Headers;        /* only meaningful if encoded as "multipart/form-data" */
    void*                           Value;          /* can either be an ascii string or a binary string    */
    ULONG                           Size;           /* specifies the size of the above 'Value' data stream */
}
HTTP_FORM_ELEMENT,  *PHTTP_FORM_ELEMENT;

#define  ACCESS_HTTP_FORM_ELEMENT(p)                \
         ACCESS_CONTAINER(p, HTTP_FORM_ELEMENT, Linkage)

#define  HttpInitFormElement(form_element)                                                  \
         {                                                                                  \
            form_element->FormIndex = 0;                                                    \
            form_element->HashIndex = 0;                                                    \
            form_element->ValueType = HTTP_FORM_DATA_TYPE_text;                             \
            form_element->Name      = NULL;                                                 \
            form_element->Headers   = NULL;                                                 \
            form_element->Value     = NULL;                                                 \
            form_element->Size      = 0;                                                    \
         }

#define  HttpCleanFormElement(form_element)                                                 \
         {                                                                                  \
            if ( form_element->Name )                                                       \
            {                                                                               \
                AnscFreeMemory(form_element->Name);                                         \
                                                                                            \
                form_element->Name = NULL;                                                  \
            }                                                                               \
                                                                                            \
            if ( form_element->Headers )                                                    \
            {                                                                               \
                AnscFreeMemory(form_element->Headers);                                      \
                                                                                            \
                form_element->Headers = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( form_element->Value )                                                      \
            {                                                                               \
                AnscFreeMemory(form_element->Value);                                        \
                                                                                            \
                form_element->Value = NULL;                                                 \
            }                                                                               \
         }

#define  HttpFreeFormElement(form_element)                                                  \
         {                                                                                  \
            HttpCleanFormElement(form_element);                                             \
            AnscFreeMemory      (form_element);                                             \
         }

typedef  struct
_HTTP_FORM_INPUT
{
    ULONG                           ElementCount;
    HTTP_FORM_ELEMENT               ElementArray[ANSC_ZERO_ARRAY_SIZE];
}
HTTP_FORM_INPUT,  *PHTTP_FORM_INPUT;

#define  HttpCleanFormInput(form_input)                                                     \
         {                                                                                  \
            ULONG                   ii = 0;                                                 \
                                                                                            \
            for ( ii = 0; ii < form_input->ElementCount; ii++ )                             \
            {                                                                               \
                HttpCleanFormElement((&form_input->ElementArray[ii]));                      \
                HttpInitFormElement ((&form_input->ElementArray[ii]));                      \
            }                                                                               \
         }

#define  HttpFreeFormInput(form_input)                                                      \
         {                                                                                  \
            HttpCleanFormInput(form_input);                                                 \
            AnscFreeMemory    (form_input);                                                 \
         }


/***********************************************************
            HTTP REQUEST INFO PARAMETERS DEFINITION
***********************************************************/

/*
 * The Request-Line begins with a method token, followed by the Request-URI and the protocol
 * version, and ending with CRLF. The elements are separated by SP characters. No CR or LF is
 * allowed except in the final CRLF sequence.
 *
 *      Request-Line = Method SP Request-URI SP HTTP-Version CRLF
 */
#define  HTTP_MAX_START_LINE_SIZE                   2048
#define  HTTP_MAX_METHOD_STR_SIZE                   32

typedef  struct
_HTTP_REQUEST_INFO
{
    char                            StartLine [HTTP_MAX_START_LINE_SIZE];
    char                            MethodName[HTTP_MAX_METHOD_STR_SIZE];
    ULONG                           Method;
    ULONG                           MajorVersion;
    ULONG                           MinorVersion;
    HTTP_REQUEST_URI                RequestUri;
}
HTTP_REQUEST_INFO,  *PHTTP_REQUEST_INFO;


/***********************************************************
           HTTP RESPONSE INFO PARAMETERS DEFINITION
***********************************************************/

/*
 * After receiving and interpreting a request message, a server responds with an HTTP response
 * message.
 *
 *      Response = Status-Line
 *                 *(( general-header | response-header | entity-header ) CRLF)
 *                 CRLF
 *                 [ message-body ]
 *
 * The first line of a Response message is the Status-Line, consisting of the protocol version
 * followed by a numeric status code and its associated textual phrase, with each element separated
 * by SP characters. No CR or LF is allowed except in the final CRLF sequence.
 *
 *      Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
 */
#define  HTTP_MAX_REASON_PHRASE_SIZE                64

typedef  struct
_HTTP_RESPONSE_INFO
{
    char                            StartLine[HTTP_MAX_START_LINE_SIZE];
    ULONG                           MajorVersion;
    ULONG                           MinorVersion;
    ULONG                           StatusCode;
    char                            ReasonPhrase[HTTP_MAX_REASON_PHRASE_SIZE];
}
HTTP_RESPONSE_INFO,  *PHTTP_RESPONSE_INFO;


/***********************************************************
          HTTP CHUNKED TRANSFER ENCODING DEFINITION
***********************************************************/

/*
 * The chunked encoding modifies the body of a message in order to transfer it as a series of
 * chunks, each with its own size indicator, followed by an OPTIONAL trailer containing entity
 * -header fields. This allows dynamically produced content to be transferred along with the
 * information necessary for the recipient to verify that it has received the full message.
 *
 *      Chunked-Body    = *chunk
 *                        last-chunk
 *                        trailer
 *                        CRLF
 *
 *      chunk           = chunk-size [ chunk-extension ] CRLF
 *                        chunk-data CRLF
 *      chunk-size      = 1*HEX
 *      last-chunk      = 1*("0") [ chunk-extension ] CRLF
 *
 *      chunk-extension = *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
 *      chunk-ext-name  = token
 *      chunk-ext-val   = token | quoted-string
 *      chunk-data      = chunk-size(OCTET)
 *      trailer         = *(entity-header CRLF)
 *
 * The chunk-size field is a string of hex digits indicating the size of the chunk. The chunked
 * encoding is ended by any chunk whose size is zero, followed by the trailer, which is terminated
 * by an empty line.
 *
 * The trailer allows the sender to include additional HTTP header fields at the end of the message.
 * The Trailer header field can be used to indicate which header fields are included in a trailer.
 */
#define  HTTP_MAX_CHUNK_EXT_NUMBER                  4
#define  HTTP_MAX_CHUNK_TOKEN_SIZE                  64

typedef  struct
_HTTP_CHUNK_INFO
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           ChunkSize;
    ULONG                           ExtCount;
    char                            ExtArray[HTTP_MAX_CHUNK_EXT_NUMBER][HTTP_MAX_CHUNK_TOKEN_SIZE];
}
HTTP_CHUNK_INFO,  *PHTTP_CHUNK_INFO;

#define  ACCESS_HTTP_CHUNK_INFO(p)                  \
         ACCESS_CONTAINER(p, HTTP_CHUNK_INFO, Linkage)


/***********************************************************
             HTTP DATE/TIME PARAMETERS DEFINITION
***********************************************************/

/*
 * HTTP applications have historically allowed three different formats for the representation of
 * date/time stamp:
 *
 *      Sun, 06 Nov 1994 08:49:37 GMT ; RFC 822, updated by RFC 1123
 *      Sunday, 06-Nov-94 08:49:37 GMT; RFC 850, obsoleted by RFC 1036
 *      Sun Nov 6 08:49: 37 1994      ; ANSC C's asctime() format
 *
 * The first format is preferred as an Internet standard and represents a fixed-length subset of
 * that defined by RFC 1123.
 */
typedef  struct
_HTTP_DATE
{
    USHORT                          Year;
    USHORT                          Month;
    USHORT                          DayOfMonth;
    USHORT                          DayOfWeek;
    USHORT                          Hour;
    USHORT                          Minute;
    USHORT                          Second;
    USHORT                          MilliSecond;
}
HTTP_DATE,  *PHTTP_DATE;


/***********************************************************
           HTTP AUTHENTICATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The "basic" authentication scheme is based on the model that the client must authenticate itself
 * with a user-ID and a password for each realm.  The realm value should be considered an opaque
 * string which can only be compared for equality with other realms on that server. The server will
 * service the request only if it can validate the user-ID and password for the protection space of
 * the Request-URI. There are no optional authentication parameters.
 *
 * For Basic, the framework above is utilized as follows:
 *
 *      challenge   = "Basic" realm
 *      credentials = "Basic" basic-credentials
 */
#define  HTTP_MAX_USERNAME_SIZE                     1024
#define  HTTP_MAX_PASSWORD_SIZE                     128
#define  HTTP_MAX_REALM_SIZE                        1024

typedef  struct
_HTTP_CREDENTIAL_BASIC
{
    char                            UserName[HTTP_MAX_USERNAME_SIZE];
    char                            Password[HTTP_MAX_PASSWORD_SIZE];
}
HTTP_CREDENTIAL_BASIC,  *PHTTP_CREDENTIAL_BASIC;

#define  ACCESS_HTTP_CREDENTIAL_BASIC(p)            \
         ACCESS_CONTAINER(p, HTTP_CREDENTIAL_BASIC, Linkage)

typedef  struct
_HTTP_CHALLENGE_BASIC
{
    char                            Realm[HTTP_MAX_REALM_SIZE];
}
HTTP_CHALLENGE_BASIC,  *PHTTP_CHALLENGE_BASIC;

#define  ACCESS_HTTP_CHALLENGE_BASIC(p)             \
         ACCESS_CONTAINER(p, HTTP_CHALLENGE_BASIC, Linkage)

/*
 * The Digest Access Authentication scheme is conceptually similar to the Basic scheme. The formats
 * of the modified WWW-Authenticate header line and the Authorization header line are specified
 * below. In addition, a new header, Authentication-Info, is specified.
 *
 * If a server receives a request for an access-protected object, and an acceptable Authorization
 * header is not sent, the server responds with a "401 Unauthorized" status code, and a WWW-
 * Authenticate header as per the framework defined above, which for the digest scheme is utilized
 * as follows:
 *
 *      challenge         =  "Digest" digest-challenge
 *      digest-challenge  = 1#( realm | [ domain ] | nonce |
 *                          [ opaque ] |[ stale ] | [ algorithm ] |
 *                          [ qop-options ] | [auth-param] )
 *      domain            = "domain" "=" <"> URI ( 1*SP URI ) <">
 *      URI               = absoluteURI | abs_path
 *      nonce             = "nonce" "=" nonce-value
 *      nonce-value       = quoted-string
 *      opaque            = "opaque" "=" quoted-string
 *      stale             = "stale" "=" ( "true" | "false" )
 *      algorithm         = "algorithm" "=" ( "MD5" | "MD5-sess" |
 *                           token )
 *      qop-options       = "qop" "=" <"> 1#qop-value <">
 *      qop-value         = "auth" | "auth-int" | token
 */
typedef  struct
_HTTP_CREDENTIAL_DIGEST
{
    char                            UserName[HTTP_MAX_USERNAME_SIZE];
}
HTTP_CREDENTIAL_DIGEST,  *PHTTP_CREDENTIAL_DIGEST;

#define  ACCESS_HTTP_CREDENTIAL_DIGEST(p)           \
         ACCESS_CONTAINER(p, HTTP_CREDENTIAL_DIGEST, Linkage)

typedef  struct
_HTTP_CHALLENGE_DIGEST
{
    char                            Realm[HTTP_MAX_REALM_SIZE];
}
HTTP_CHALLENGE_DIGEST,  *PHTTP_CHALLENGE_DIGEST;

#define  ACCESS_HTTP_CHALLENGE_DIGEST(p)            \
         ACCESS_CONTAINER(p, HTTP_CHALLENGE_DIGEST, Linkage)

/*
 * HTTP provides a simple challenge-response authentication mechanism that MAY be used by a server
 * to challenge a client request and by a client to provide authentication information. It uses an
 * extensible, case-insensitive token to identify the authentication scheme, followed by a comma-
 * separated list of attribute-value pairs which carry the parameters necessary for achieving
 * authentication via that scheme.
 *
 *      auth-scheme    = token
 *      auth-param     = token "=" ( token | quoted-string )
 *
 * The 401 (Unauthorized) response message is used by an origin server to challenge the authori-
 * zation of a user agent. This response MUST include a WWW-Authenticate header field containing
 * at least one challenge applicable to the requested resource. The 407 (Proxy Authentication
 * Required) response message is used by a proxy to challenge the authorization of a client and
 * MUST include a Proxy-Authenticate header field containing at least one challenge applicable to
 * the proxy for the requested resource.
 */
#define  HTTP_MAX_CHALLENGE_NUMBER                  4

typedef  struct
_HTTP_AUTH_CREDENTIAL
{
    ULONG                           AuthType;
    union
    {
        HTTP_CREDENTIAL_BASIC       Basic;
        HTTP_CREDENTIAL_DIGEST      Digest;
    }Credential;
}
HTTP_AUTH_CREDENTIAL,  *PHTTP_AUTH_CREDENTIAL;

typedef  struct
_HTTP_AUTH_CHALLENGE
{
    ULONG                           AuthType;
    union
    {
        HTTP_CHALLENGE_BASIC        Basic;
        HTTP_CHALLENGE_DIGEST       Digest;
    }Challenge;
}
HTTP_AUTH_CHALLENGE,  *PHTTP_AUTH_CHALLENGE;


/***********************************************************
           HTTP HEADER FIELD PARAMETERS DEFINITION
***********************************************************/

/*
 * HTTP header fields, which include general-header, request-header, response header, and entity-
 * header fields, follow the same generic format as that give in Section 3.1 of RFC 822. Each
 * header field consists of a name followed by a colon (":") and the field value. Field names are
 * case-insensitive. The message-body (if any) of an HTTP message is used to carry the entity-body
 * associated with the request or response.
 */
#define  HTTP_HEADER_LINE_SIZE                      2048
#define  HTTP_FIELD_NAME_SIZE                       32
#define  HTTP_FIELD_VALUE_SIZE                      2000
#define  HTTP_MAX_HEADER_PARAM_SIZE                 512

#define  HTTP_FIELD_FLAG_NAME_PRESENT               0x00000001
#define  HTTP_FIELD_FLAG_VALUE_PRESENT              0x00000002
#define  HTTP_FIELD_FLAG_LINE_PRESENT               0x00000004

#define  HTTP_HEADER_FIELD_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ULONG                           HashIndex;                                              \
    ULONG                           Flags;                                                  \
    char                            HeaderLine[HTTP_HEADER_LINE_SIZE];                      \
    ULONG                           HeaderId;                                               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HEADER_FIELD
{
    HTTP_HEADER_FIELD_CONTENT
}
HTTP_HEADER_FIELD,  *PHTTP_HEADER_FIELD;

#define  ACCESS_HTTP_HEADER_FIELD(p)                \
         ACCESS_CONTAINER(p, HTTP_HEADER_FIELD, Linkage)


/***********************************************************
             HTTP HEADER FIELD ACCEPT DEFINITION
***********************************************************/

/*
 * The Accept request-header field can be used to specify certain media types which are acceptable
 * for the response. Accept headers can be used to indicate that the request is specifically
 * limited to a small set of desired types, as in the case of a request for an in-line image.
 */
#define  HTTP_MAX_MEDIA_RANGE_NUMBER                16

typedef  struct
_HTTP_MEDIA_RANGE
{
    ULONG                           MediaType;                              /* as defined in ansc_iana_number.h     */
    ULONG                           SubType;                                /* as defined in ansc_iana_number.h     */
    ULONG                           Quality;                                /* q x 1000; e.g., q=0.5 -> 500         */
    char                            Parameters[HTTP_MAX_HEADER_PARAM_SIZE]; /* media type parameters; e.g., level=1 */
}
HTTP_MEDIA_RANGE,  *PHTTP_MEDIA_RANGE;

#define  HTTP_HFO_ACCEPT_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           RangeCount;                                             \
    HTTP_MEDIA_RANGE                RangeArray[HTTP_MAX_MEDIA_RANGE_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ACCEPT
{
    HTTP_HFO_ACCEPT_CONTENT
}
HTTP_HFO_ACCEPT,  *PHTTP_HFO_ACCEPT;

#define  ACCESS_HTTP_HFO_ACCEPT(p)                  \
         ACCESS_CONTAINER(p, HTTP_HFO_ACCEPT, Linkage)


/***********************************************************
          HTTP HEADER FIELD ACCEPT-CHARSET DEFINITION
***********************************************************/

/*
 * The Accept-Charset request-header field can be used to indicate what character sets are
 * acceptable for the response. This field allows clients capable of understanding more
 * comprehensive or special-purpose character sets to signal that capability to a server which is
 * capable of representing documents in those character sets.
 */
#define  HTTP_MAX_CHARSET_NUMBER                    16

typedef  struct
_HTTP_CHARSET
{
    ULONG                           Type;                                   /* as defined in ansc_iana_number.h */
    ULONG                           Quality;                                /* q x 1000; e.g., q=0.5 -> 500     */
}
HTTP_CHARSET,  *PHTTP_CHARSET;

#define  HTTP_HFO_ACCEPT_CHARSET_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           CharsetCount;                                           \
    HTTP_CHARSET                    CharsetArray[HTTP_MAX_CHARSET_NUMBER];                  \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ACCEPT_CHARSET
{
    HTTP_HFO_ACCEPT_CHARSET_CONTENT
}
HTTP_HFO_ACCEPT_CHARSET,  *PHTTP_HFO_ACCEPT_CHARSET;

#define  ACCESS_HTTP_HFO_ACCEPT_CHARSET(p)          \
         ACCESS_CONTAINER(p, HTTP_HFO_ACCEPT_CHARSET, Linkage)


/***********************************************************
        HTTP HEADER FIELD ACCEPT-ENCODING DEFINITION
***********************************************************/

/*
 * The Accept-Encoding request-header field is similar to Accept, but restricts the content-codings
 * that are acceptable in the response.
 */
#define  HTTP_MAX_ENCODING_NUMBER                   4
#define  HTTP_ENCODING_QUALITY_BEST                 1000

typedef  struct
_HTTP_ENCODING
{
    ULONG                           Type;                                   /* as defined in ansc_iana_number.h */
    ULONG                           Quality;                                /* q x 1000; e.g., q=0.5 -> 500     */
}
HTTP_ENCODING,  *PHTTP_ENCODING;

#define  HTTP_HFO_ACCEPT_ENCODING_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           EncodingCount;                                          \
    HTTP_ENCODING                   EncodingArray[HTTP_MAX_ENCODING_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ACCEPT_ENCODING
{
    HTTP_HFO_ACCEPT_ENCODING_CONTENT
}
HTTP_HFO_ACCEPT_ENCODING,  *PHTTP_HFO_ACCEPT_ENCODING;

#define  ACCESS_HTTP_HFO_ACCEPT_ENCODING(p)         \
         ACCESS_CONTAINER(p, HTTP_HFO_ACCEPT_ENCODING, Linkage)


/***********************************************************
        HTTP HEADER FIELD ACCEPT-LANGUAGE DEFINITION
***********************************************************/

/*
 * The Accept-Language request-header field is similar to Accept, but restricts the set of natural
 * languages that are preferred as a response to the request.
 */
#define  HTTP_MAX_LANGUAGE_NUMBER                   4
#define  HTTP_MAX_LANGUAGE_TAG_SIZE                 32

typedef  struct
_HTTP_LANGUAGE
{
    char                            Tag[HTTP_MAX_LANGUAGE_TAG_SIZE];        /* identifies a natural language */
    ULONG                           Quality;                                /* q x 1000; e.g., q=0.5 -> 500  */
}
HTTP_LANGUAGE,  *PHTTP_LANGUAGE;

#define  HTTP_HFO_ACCEPT_LANGUAGE_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           LanguageCount;                                          \
    HTTP_LANGUAGE                   LanguageArray[HTTP_MAX_LANGUAGE_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ACCEPT_LANGUAGE
{
    HTTP_HFO_ACCEPT_LANGUAGE_CONTENT
}
HTTP_HFO_ACCEPT_LANGUAGE,  *PHTTP_HFO_ACCEPT_LANGUAGE;

#define  ACCESS_HTTP_HFO_ACCEPT_LANGUAGE(p)         \
         ACCESS_CONTAINER(p, HTTP_HFO_ACCEPT_LANGUAGE, Linkage)


/***********************************************************
         HTTP HEADER FIELD ACCEPT-RANGES DEFINITION
***********************************************************/

/*
 * The Accept-Ranges response-header field allows the server to indicate its acceptance of range
 * requests for a resource:
 *
 *      Accept-Ranges     = "Accept-Ranges" ":" acceptable-ranges
 *      acceptable-ranges = 1#range-unit | "none"
 */
#define  HTTP_MAX_RANGE_NUMBER                      2
#define  HTTP_MAX_RANGE_UNIT_SIZE                   16

typedef  struct
_HTTP_RANGE
{
    char                            Unit[HTTP_MAX_RANGE_UNIT_SIZE];         /* identifies a natural language */
}
HTTP_RANGE,  *PHTTP_RANGE;

#define  HTTP_HFO_ACCEPT_RANGES_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           RangeCount;                                             \
    HTTP_RANGE                      RangeArray[HTTP_MAX_RANGE_NUMBER];                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ACCEPT_RANGES
{
    HTTP_HFO_ACCEPT_RANGES_CONTENT
}
HTTP_HFO_ACCEPT_RANGES,  *PHTTP_HFO_ACCEPT_RANGES;

#define  ACCESS_HTTP_HFO_ACCEPT_RANGES(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_ACCEPT_RANGES, Linkage)


/***********************************************************
               HTTP HEADER FIELD AGE DEFINITION
***********************************************************/

/*
 * The Age response-header field conveys the sender's estimate of the amount of time since the
 * response (or its revalidation) was generated at the origin server. A cached response is "fresh"
 * if its age does not exceed its freshness lifetime.
 */
#define  HTTP_HFO_AGE_CONTENT                                                               \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           DeltaSeconds;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_AGE
{
    HTTP_HFO_AGE_CONTENT
}
HTTP_HFO_AGE,  *PHTTP_HFO_AGE;

#define  ACCESS_HTTP_HFO_AGE(p)                     \
         ACCESS_CONTAINER(p, HTTP_HFO_AGE, Linkage)


/***********************************************************
              HTTP HEADER FIELD ALLOW DEFINITION
***********************************************************/

/*
 * The Allow entity-header field lists the set of methods supported by the resource identified by
 * the Request-URI. The purpose of this field is strictly to inform the recipient of valid methods
 * associated with the resource. An Allow header field MUST be present in a 405 (Method Not
 * Allowed) response.
 *
 *      Allow   = "Allow" ":" #Method
 *
 * Example of use:
 *
 *      Allow: GET, HEAD, PUT
 */
#define  HTTP_MAX_METHOD_NUMBER                     16

#define  HTTP_HFO_ALLOW_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           MethodCount;                                            \
    ULONG                           MethodArray[HTTP_MAX_METHOD_NUMBER];                    \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ALLOW
{
    HTTP_HFO_ALLOW_CONTENT
}
HTTP_HFO_ALLOW,  *PHTTP_HFO_ALLOW;

#define  ACCESS_HTTP_HFO_ALLOW(p)                   \
         ACCESS_CONTAINER(p, HTTP_HFO_ALLOW, Linkage)


/***********************************************************
          HTTP HEADER FIELD AUTHORIZATION DEFINITION
***********************************************************/

/*
 * A user agent that wishes to authenticate itself with a server-- usually, but not necessarily,
 * after receiving a 401 response--does so by including an Authorization request-header field with
 * the request. The Authorization field value consists of credentials containing the authentication
 * information of the user agent for the realm of the resource being requested.
 */
#define  HTTP_HFO_AUTHORIZATION_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_AUTH_CREDENTIAL            Credential;                                             \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_AUTHORIZATION
{
    HTTP_HFO_AUTHORIZATION_CONTENT
}
HTTP_HFO_AUTHORIZATION,  *PHTTP_HFO_AUTHORIZATION;

#define  ACCESS_HTTP_HFO_AUTHORIZATION(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_AUTHORIZATION, Linkage)


/***********************************************************
          HTTP HEADER FIELD CACHE-CONTROL DEFINITION
***********************************************************/

/*
 * The Cache-Control general-header field is used to specify directives that MUST be obeyed by all
 * caching mechanisms along the request/response chain. The directives specify behavior intended to
 * prevent caches from adversely interfering with the request or response. These directives
 * typically override the default caching algorithms. Cache directives are unidirectional in that
 * the presence of a directive in a request does not imply that the same directive is to be given
 * in the response.
 */
#define  HTTP_MAX_CACHE_DIRECTIVE_SIZE              128

#define  HTTP_HFO_CACHE_CONTROL_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            Directives[HTTP_MAX_CACHE_DIRECTIVE_SIZE];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CACHE_CONTROL
{
    HTTP_HFO_CACHE_CONTROL_CONTENT
}
HTTP_HFO_CACHE_CONTROL,  *PHTTP_HFO_CACHE_CONTROL;

#define  ACCESS_HTTP_HFO_CACHE_CONTROL(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_CACHE_CONTROL, Linkage)


/***********************************************************
            HTTP HEADER FIELD CONNECTION DEFINITION
***********************************************************/

/*
 * The Connection general-header field allows the sender to specify options that are desired for
 * that particular connection and MUST NOT be communicated by proxies over further connections. The
 * Connection header has the following grammar:
 *
 *      Connection        = "Connection" ":" 1#(connection-token)
 *      connection-token  = token
 *
 * HTTP/1.1 proxies MUST parse the Connection header field before a message is forwarded and, for
 * each connection-token in this field, remove any header field(s) from the message with the same
 * name as the connection-token. Connection options are signaled by the presence of a connection-
 * token in the Connection header field, not by any corresponding additional header field(s), since
 * the additional header field may not be sent if there are no parameters associated with that
 * connection option.
 */
#define  HTTP_MAX_CONNECTION_TOKEN_NUMBER           16
#define  HTTP_MAX_CONNECTION_TOKEN_SIZE             64

typedef  struct
_HTTP_CONNECTION_TOKEN
{
    ULONG                           OptionType;                 /* defined in ansc_iana_number.h */
    char                            Token[HTTP_MAX_CONNECTION_TOKEN_SIZE];
}
HTTP_CONNECTION_TOKEN,  *PHTTP_CONNECTION_TOKEN;

#define  HTTP_HFO_CONNECTION_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           TokenCount;                                             \
    HTTP_CONNECTION_TOKEN           TokenArray[HTTP_MAX_CONNECTION_TOKEN_NUMBER];           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONNECTION
{
    HTTP_HFO_CONNECTION_CONTENT
}
HTTP_HFO_CONNECTION,  *PHTTP_HFO_CONNECTION;

#define  ACCESS_HTTP_HFO_CONNECTION(p)              \
         ACCESS_CONTAINER(p, HTTP_HFO_CONNECTION, Linkage)


/***********************************************************
        HTTP HEADER FIELD CONTENT-ENCODING DEFINITION
***********************************************************/

/*
 * The Content-Encoding entity-header field is used as a modifier to the media-type. When present,
 * its value indicates what additional content codings have been applied to the entity-body, and
 * thus what decoding mechanisms must be applied in order to obtain the media-type referenced by
 * the Content-Type header field. Content-Encoding is primarily used to allow a document to be
 * compressed without losing the identity of its underlying media type.
 */
#define  HTTP_HFO_CONTENT_ENCODING_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           EncodingCount;                                          \
    HTTP_ENCODING                   EncodingArray[HTTP_MAX_ENCODING_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_ENCODING
{
    HTTP_HFO_CONTENT_ENCODING_CONTENT
}
HTTP_HFO_CONTENT_ENCODING,  *PHTTP_HFO_CONTENT_ENCODING;

#define  ACCESS_HTTP_HFO_CONTENT_ENCODING(p)        \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_ENCODING, Linkage)


/***********************************************************
        HTTP HEADER FIELD CONTENT-LANGUAGE DEFINITION
***********************************************************/

/*
 * The Content-Language entity-header field describes the natural language(s) of the intended
 * audience for the enclosed entity. Note that this might not be equivalent to all the languages
 * used within the entity-body.
 */
#define  HTTP_HFO_CONTENT_LANGUAGE_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           LanguageCount;                                          \
    HTTP_LANGUAGE                   LanguageArray[HTTP_MAX_LANGUAGE_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_LANGUAGE
{
    HTTP_HFO_CONTENT_LANGUAGE_CONTENT
}
HTTP_HFO_CONTENT_LANGUAGE,  *PHTTP_HFO_CONTENT_LANGUAGE;

#define  ACCESS_HTTP_HFO_CONTENT_LANGUAGE(p)        \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_LANGUAGE, Linkage)


/***********************************************************
         HTTP HEADER FIELD CONTENT-LENGTH DEFINITION
***********************************************************/

/*
 * The Content-Length entity-header field indicates the size of the entity-body, in decimal number
 * of OCTETs, sent to the recipient or, in the case of the HEAD method, the size of the entity-body
 * that would have been sent had the request been a GET.
 *
 *      Content-Length    = "Content-Length" ":" 1*DIGIT
 *
 * An example is
 *
 *      Content-Length: 3495
 *
 * Applications SHOULD use this field to indicate the transfer-length of the message-body, unless
 * this is prohibited by the rules.
 */
#define  HTTP_HFO_CONTENT_LENGTH_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           BodySize;                                               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_LENGTH
{
    HTTP_HFO_CONTENT_LENGTH_CONTENT
}
HTTP_HFO_CONTENT_LENGTH,  *PHTTP_HFO_CONTENT_LENGTH;

#define  ACCESS_HTTP_HFO_CONTENT_LENGTH(p)          \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_LENGTH, Linkage)


/***********************************************************
        HTTP HEADER FIELD CONTENT-LOCATION DEFINITION
***********************************************************/

/*
 * The Content-Location entity-header field MAY be used to supply the resource location for the
 * entity enclosed in the message when that entity is accessible from a location separate from the
 * requested resource's URI. A server SHOULD provide a Content-Location for the variant corres-
 * ponding to the response entity; especially in the case where a resource has multiple entities
 * associated with it, and those entities actually have separate locations by which they might be
 * individually accessed, the server SHOULD provide a Content-Location for the particular variant
 * which is returned.
 *
 *      Content-Location = "Content-Location" ":" ( absoluteURI | relativeURI )
 *
 * The value of Content-Location also defines the base URI for the entity.
 */
#define  HTTP_HFO_CONTENT_LOCATION_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_REQUEST_URI                LocationUri;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_LOCATION
{
    HTTP_HFO_CONTENT_LOCATION_CONTENT
}
HTTP_HFO_CONTENT_LOCATION,  *PHTTP_HFO_CONTENT_LOCATION;

#define  ACCESS_HTTP_HFO_CONTENT_LOCATION(p)        \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_LOCATION, Linkage)


/***********************************************************
          HTTP HEADER FIELD CONTENT-MD5 DEFINITION
***********************************************************/

/*
 * The Content-MD5 entity-header field, as defined in RFC 1864, is an MD5 digest of the entity-body
 * for the purpose of providing an end-to-end message integrity check (MIC) of the entity-body.
 * (Note: a MIC is good for detecting accidental modification of the entity-body in transit, but is
 * not proof against malicious attacks.)
 *
 *      Content-MD5  = "Content-MD5" ":" md5-digest
 *      md5-digest   = <base64 of 128 bit MD5 digest as per RFC 1864>
 *
 * The Content-MD5 header field MAY be generated by an origin server or client to function as an
 * integrity check of the entity-body. Only origin servers or clients MAY generate the Content-MD5
 * header field; proxies and gateways MUST NOT generate it, as this would defeat its value as an
 * end-to-end integrity check. Any recipient of the entity-body, including gateways and proxies,
 * MAY check that the digest value in this header field matches that of the entity-body as received.
 */
#define  HTTP_HFO_CONTENT_MD5_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    UCHAR                           Digest[ANSC_MD5_OUTPUT_SIZE];                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_MD5
{
    HTTP_HFO_CONTENT_MD5_CONTENT
}
HTTP_HFO_CONTENT_MD5,  *PHTTP_HFO_CONTENT_MD5;

#define  ACCESS_HTTP_HFO_CONTENT_MD5(p)             \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_MD5, Linkage)


/***********************************************************
          HTTP HEADER FIELD CONTENT-RANGE DEFINITION
***********************************************************/

/*
 * The Content-Range entity-header is sent with a partial entity-body to specify where in the full
 * entity-body the partial body should be applied. Range units are defined in section 3.12.
 *
 *      Content-Range           = "Content-Range" ":" content-range-spec
 *      content-range-spec      = byte-content-range-spec
 *      byte-content-range-spec = bytes-unit SP byte-range-resp-spec "/" ( instance-length | "*" )
 *      byte-range-resp-spec    = (first-byte-pos "-" last-byte-pos) | "*"
 *      instance-length         = 1*DIGIT
 *
 * The header SHOULD indicate the total length of the full entity-body, unless this length is
 * unknown or difficult to determine. The asterisk "*" character means that the instance-length is
 * unknown at the time when the response was generated.
 */
#define  HTTP_UNKNOWN_INSTANCE_LENGTH               0xFFFFFFFF

#define  HTTP_HFO_CONTENT_RANGE_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           FirstBytePos;                                           \
    ULONG                           LastBytePos;                                            \
    ULONG                           InstaceLength;                                          \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_RANGE
{
    HTTP_HFO_CONTENT_RANGE_CONTENT
}
HTTP_HFO_CONTENT_RANGE,  *PHTTP_HFO_CONTENT_RANGE;

#define  ACCESS_HTTP_HFO_CONTENT_RANGE(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_RANGE, Linkage)


/***********************************************************
          HTTP HEADER FIELD CONTENT-TYPE DEFINITION
***********************************************************/

/*
 * The Content-Type entity-header field indicates the media type of the entity-body sent to the
 * recipient or, in the case of the HEAD method, the media type that would have been sent had the
 * request been a GET.
 *
 *      Content-Type   = "Content-Type" ":" media-type
 */
#define  HTTP_HFO_CONTENT_TYPE_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           MediaType;                                              \
    ULONG                           SubType;                                                \
    char                            Parameters[HTTP_MAX_HEADER_PARAM_SIZE];                 \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_CONTENT_TYPE
{
    HTTP_HFO_CONTENT_TYPE_CONTENT
}
HTTP_HFO_CONTENT_TYPE,  *PHTTP_HFO_CONTENT_TYPE;

#define  ACCESS_HTTP_HFO_CONTENT_TYPE(p)            \
         ACCESS_CONTAINER(p, HTTP_HFO_CONTENT_TYPE, Linkage)


/***********************************************************
               HTTP HEADER FIELD DATE DEFINITION
***********************************************************/

/*
 * The Date general-header field represents the date and time at which the message was originated,
 * having the same semantics as orig-date in RFC 822. The field value is an HTTP-date, it MUST be
 * sent in RFC 1123 date format.
 */
#define  HTTP_HFO_DATE_CONTENT                                                              \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_DATE
{
    HTTP_HFO_DATE_CONTENT
}
HTTP_HFO_DATE,  *PHTTP_HFO_DATE;

#define  ACCESS_HTTP_HFO_DATE(p)                    \
         ACCESS_CONTAINER(p, HTTP_HFO_DATE, Linkage)


/***********************************************************
               HTTP HEADER FIELD ETAG DEFINITION
***********************************************************/

/*
 * The Date general-header field represents the date and time at which the message was originated,
 * having the same semantics as orig-date in RFC 822. The field value is an HTTP-date, it MUST be
 * sent in RFC 1123 date format.
 */
#define  HTTP_MAX_ETAG_SIZE                         32

#define  HTTP_HFO_ETAG_CONTENT                                                              \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            Tag[HTTP_MAX_ETAG_SIZE];                                \
    BOOL                            bWeak;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_ETAG
{
    HTTP_HFO_ETAG_CONTENT
}
HTTP_HFO_ETAG,  *PHTTP_HFO_ETAG;

#define  ACCESS_HTTP_HFO_ETAG(p)                    \
         ACCESS_CONTAINER(p, HTTP_HFO_ETAG, Linkage)


/***********************************************************
              HTTP HEADER FIELD EXPECT DEFINITION
***********************************************************/

/*
 * The Expect request-header field is used to indicate that particular server behaviors are
 * required by the client.
 *
 *      Expect                =  "Expect" ":" 1#expectation
 *      expectation           =  "100-continue" | expectation-extension
 *      expectation-extension =  token [ "=" ( token | quoted-string ) *expect-params ]
 *      expect-params         =  ";" token [ "=" ( token | quoted-string ) ]
 *
 * A server that does not understand or is unable to comply with any of the expectation values in
 * the Expect field of a request MUST respond with appropriate error status. The server MUST
 * respond with a 417 (Expectation Failed) status if any of the expectations cannot be met or, if
 * there are other problems with the request, some other 4xx status.
 */
#define  HTTP_MAX_EXPECT_TOKEN_NUMBER               16
#define  HTTP_MAX_EXPECT_TOKEN_SIZE                 64

typedef  struct
_HTTP_EXPECT_TOKEN
{
    char                            Token     [HTTP_MAX_EXPECT_TOKEN_SIZE];
    char                            Parameters[HTTP_MAX_HEADER_PARAM_SIZE];
}
HTTP_EXPECT_TOKEN,  *PHTTP_EXPECT_TOKEN;

#define  HTTP_HFO_EXPECT_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           TokenCount;                                             \
    HTTP_EXPECT_TOKEN               TokenArray[HTTP_MAX_EXPECT_TOKEN_NUMBER];               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_EXPECT
{
    HTTP_HFO_EXPECT_CONTENT
}
HTTP_HFO_EXPECT,  *PHTTP_HFO_EXPECT;

#define  ACCESS_HTTP_HFO_EXPECT(p)                  \
         ACCESS_CONTAINER(p, HTTP_HFO_EXPECT, Linkage)


/***********************************************************
             HTTP HEADER FIELD EXPIRES DEFINITION
***********************************************************/

/*
 * The Expires entity-header field gives the date/time after which the response is considered stale.
 * A stale cache entry may not normally be returned by a cache (either a proxy cache or a user
 * agent cache) unless it is first validated with the origin server (or with an intermediate cache
 * that has a fresh copy of the entity). The presence of an Expires field does not imply that the
 * original resource will change or cease to exist at, before, or after that time.
 *
 * The format is an absolute date and time as defined by HTTP-date; it MUST be in RFC 1123 date
 * format:
 *
 *      Expires = "Expires" ":" HTTP-date
 */
#define  HTTP_HFO_EXPIRES_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_EXPIRES
{
    HTTP_HFO_EXPIRES_CONTENT
}
HTTP_HFO_EXPIRES,  *PHTTP_HFO_EXPIRES;

#define  ACCESS_HTTP_HFO_EXPIRES(p)                 \
         ACCESS_CONTAINER(p, HTTP_HFO_EXPIRES, Linkage)


/***********************************************************
               HTTP HEADER FIELD FROM DEFINITION
***********************************************************/

/*
 * The From request-header field, if given, SHOULD contain an Internet e-mail address for the human
 * user who controls the requesting user agent. The address SHOULD be machine-usable, as defined by
 * "mailbox" in RFC 822 as updated by RFC 1123:
 *
 *      From   = "From" ":" mailbox
 *
 * An example is:
 *
 *      From: webmaster@w3.org
 *
 * This header field MAY be used for logging purposes and as a means for identifying the source of
 * invalid or unwanted requests. It SHOULD NOT be used as an insecure form of access protection.
 * The interpretation of this field is that the request is being performed on behalf of the person
 * given, who accepts responsibility for the method performed. In particular, robot agents SHOULD
 * include this header so that the person responsible for running the robot can be contacted if
 * problems occur on the receiving end.
 */
#define  HTTP_HFO_FROM_CONTENT                                                              \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ANSC_RFC822_MAILBOX             MailBox;                                                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_FROM
{
    HTTP_HFO_FROM_CONTENT
}
HTTP_HFO_FROM,  *PHTTP_HFO_FROM;

#define  ACCESS_HTTP_HFO_FROM(p)                    \
         ACCESS_CONTAINER(p, HTTP_HFO_FROM, Linkage)


/***********************************************************
               HTTP HEADER FIELD HOST DEFINITION
***********************************************************/

/*
 * The Host request-header field specifies the Internet host and port number of the resource being
 * requested, as obtained from the original URI given by the user or referring resource (generally
 * an HTTP URL). The Host field value MUST represent the naming authority of the origin server or
 * gateway given by the original URL. This allows the origin server or gateway to differentiate be-
 * tween internally-ambiguous URLs, such as the root "/" URL of a server for multiple host names on
 * a single IP address.
 *
 *      Host = "Host" ":" host [ ":" port ] ;
 *
 * A "host" without any trailing port information implies the default port for the service
 * requested (e.g., "80" for an HTTP URL). For example, a request on the origin server for
 *
 *      <http://www.w3.org/pub/WWW/>
 *
 * would properly include:
 *
 *      GET /pub/WWW/ HTTP/1.1
 *      Host: www.w3.org
 *
 * A client MUST include a Host header field in all HTTP/1.1 request messages. If the requested URI
 * does not include an Internet host name for the service being requested, then the Host header
 * field MUST be given with an empty value. An HTTP/1.1 proxy MUST ensure that any request message
 * it forwards does contain an appropriate Host header field that identifies the service being
 * requested by the proxy. All Internet-based HTTP/1.1 servers MUST respond with a 400 (Bad Request)
 * status code to any HTTP/1.1 request message which lacks a Host header field.
 */
#define  HTTP_HFO_HOST_CONTENT                                                              \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            HostName[ANSC_DOMAIN_NAME_SIZE];                        \
    USHORT                          HostPort;                                               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_HOST
{
    HTTP_HFO_HOST_CONTENT
}
HTTP_HFO_HOST,  *PHTTP_HFO_HOST;

#define  ACCESS_HTTP_HFO_HOST(p)                    \
         ACCESS_CONTAINER(p, HTTP_HFO_HOST, Linkage)


/***********************************************************
             HTTP HEADER FIELD IF-MATCH DEFINITION
***********************************************************/

/*
 * The If-Match request-header field is used with a method to make it conditional. A client that
 * has one or more entities previously obtained from the resource can verify that one of those
 * entities is current by including a list of their associated entity tags in the If-Match header
 * field. Entity tags are defined in section 3.11. The purpose of this feature is to allow effi-
 * cient updates of cached information with a minimum amount of transaction overhead. It is also
 * used, on updating requests, to prevent inadvertent modification of the wrong version of a re-
 * source. As a special case, the value "*" matches any current entity of the resource.
 *
 *      If-Match = "If-Match" ":" ( "*" | 1#entity-tag )
 *
 * If any of the entity tags match the entity tag of the entity that would have been returned in
 * the response to a similar GET request (without the If-Match header) on that resource, or if "*"
 * is given and any current entity exists for that resource, then the server MAY perform the re-
 * quested method as if the If-Match header field did not exist. A server MUST use the strong
 * comparison function to compare the entity tags in If-Match.
 */
#define  HTTP_MAX_ETAG_NUMBER                       16

typedef  struct
_HTTP_ENTITY_TAG
{
    char                            Tag[HTTP_MAX_ETAG_SIZE];
    BOOL                            bWeak;
}
HTTP_ENTITY_TAG,  *PHTTP_ENTITY_TAG;

#define  HTTP_HFO_IF_MATCH_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           TagCount;                                               \
    HTTP_ENTITY_TAG                 TagArray[HTTP_MAX_ETAG_NUMBER];                         \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_IF_MATCH
{
    HTTP_HFO_IF_MATCH_CONTENT
}
HTTP_HFO_IF_MATCH,  *PHTTP_HFO_IF_MATCH;

#define  ACCESS_HTTP_HFO_IF_MATCH(p)                \
         ACCESS_CONTAINER(p, HTTP_HFO_IF_MATCH, Linkage)


/***********************************************************
       HTTP HEADER FIELD IF-MODIFIED-SINCE DEFINITION
***********************************************************/

/*
 * The If-Modified-Since request-header field is used with a method to make it conditional: if the
 * requested variant has not been modified since the time specified in this field, an entity will
 * not be returned from the server; instead, a 304 (not modified) response will be returned without
 * any message-body.
 *
 *      If-Modified-Since = "If-Modified-Since" ":" HTTP-date
 *
 * An example of the field is:
 *
 *      If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT
 */
#define  HTTP_HFO_IF_MODIFIED_SINCE_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_IF_MODIFIED_SINCE
{
    HTTP_HFO_IF_MODIFIED_SINCE_CONTENT
}
HTTP_HFO_IF_MODIFIED_SINCE,  *PHTTP_HFO_IF_MODIFIED_SINCE;

#define  ACCESS_HTTP_HFO_IF_MODIFIED_SINCE(p)       \
         ACCESS_CONTAINER(p, HTTP_HFO_IF_MODIFIED_SINCE, Linkage)


/***********************************************************
          HTTP HEADER FIELD IF-NONE-MATCH DEFINITION
***********************************************************/

/*
 * The If-None-Match request-header field is used with a method to make it conditional. A client
 * that has one or more entities previously obtained from the resource can verify that none of
 * those entities is current by including a list of their associated entity tags in the If-None-
 * Match header field. The purpose of this feature is to allow efficient updates of cached infor-
 * mation with a minimum amount of transaction overhead. It is also used to prevent a method (e.g.
 * PUT) from inadvertently modifying an existing resource when the client believes that the re-
 * source does not exist.
 *
 * As a special case, the value "*" matches any current entity of the resource.
 *
 *      If-None-Match = "If-None-Match" ":" ( "*" | 1#entity-tag )
 *
 * If any of the entity tags match the entity tag of the entity that would have been returned in
 * the response to a similar GET request (without the If-None-Match header) on that resource, or
 * if "*" is given and any current entity exists for that resource, then the server MUST NOT
 * perform the requested method, unless required to do so because the resource's modification date
 * fails to match that supplied in an If-Modified-Since header field in the request. Instead, if
 * the request method was GET or HEAD, the server SHOULD respond with a 304 (Not Modified) response,
 * including the cache-related header fields (particularly ETag) of one of the entities that
 * matched. For all other request methods, the server MUST respond with a status of 412
 * (Precondition Failed).
 */
#define  HTTP_HFO_IF_NONE_MATCH_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           TagCount;                                               \
    HTTP_ENTITY_TAG                 TagArray[HTTP_MAX_ETAG_NUMBER];                         \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_IF_NONE_MATCH
{
    HTTP_HFO_IF_NONE_MATCH_CONTENT
}
HTTP_HFO_IF_NONE_MATCH,  *PHTTP_HFO_IF_NONE_MATCH;

#define  ACCESS_HTTP_HFO_IF_NONE_MATCH(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_IF_NONE_MATCH, Linkage)


/***********************************************************
             HTTP HEADER FIELD IF-RANGE DEFINITION
***********************************************************/

/*
 * If a client has a partial copy of an entity in its cache, and wishes to have an up-to-date copy
 * of the entire entity in its cache, it could use the Range request-header with a conditional GET
 * (using either or both of If-Unmodified-Since and If-Match.) However, if the condition fails
 * because the entity has been modified, the client would then have to make a second request to
 * obtain the entire current entity-body.
 *
 * The If-Range header allows a client to "short-circuit" the second request. Informally, its
 * meaning is `if the entity is unchanged, send me the part(s) that I am missing; otherwise, send
 * me the entire new entity'.
 *
 *      If-Range = "If-Range" ":" ( entity-tag | HTTP-date )
 *
 * If the client has no entity tag for an entity, but does have a Last-Modified date, it MAY use
 * that date in an If-Range header. (The server can distinguish between a valid HTTP-date and any
 * form of entity-tag by examining no more than two characters.) The If-Range header SHOULD only be
 * used together with a Range header, and MUST be ignored if the request does not include a Range
 * header, or if the server does not support the sub-range operation.
 */
#define  HTTP_HFO_IF_RANGE_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    BOOL                            bTagUsed;                                               \
    HTTP_ENTITY_TAG                 ETag;                                                   \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_IF_RANGE
{
    HTTP_HFO_IF_RANGE_CONTENT
}
HTTP_HFO_IF_RANGE,  *PHTTP_HFO_IF_RANGE;

#define  ACCESS_HTTP_HFO_IF_RANGE(p)                \
         ACCESS_CONTAINER(p, HTTP_HFO_IF_RANGE, Linkage)


/***********************************************************
      HTTP HEADER FIELD IF-UNMODIFIED-SINCE DEFINITION
***********************************************************/

/*
 * The If-Unmodified-Since request-header field is used with a method to make it conditional. If
 * the requested resource has not been modified since the time specified in this field, the server
 * SHOULD perform the requested operation as if the If-Unmodified-Since header were not present.
 *
 * If the requested variant has been modified since the specified time, the server MUST NOT perform
 * the requested operation, and MUST return a 412 (Precondition Failed).
 *
 *      If-Unmodified-Since = "If-Unmodified-Since" ":" HTTP-date
 *
 * An example of the field is:
 *
 *      If-Unmodified-Since: Sat, 29 Oct 1994 19:43:31 GMT
 *
 * If the request normally (i.e., without the If-Unmodified-Since header) would result in anything
 * other than a 2xx or 412 status, the If-Unmodified-Since header SHOULD be ignored. If the speci-
 * fied date is invalid, the header is ignored.
 */
#define  HTTP_HFO_IF_UNMODIFIED_SINCE_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_IF_UNMODIFIED_SINCE
{
    HTTP_HFO_IF_UNMODIFIED_SINCE_CONTENT
}
HTTP_HFO_IF_UNMODIFIED_SINCE,  *PHTTP_HFO_IF_UNMODIFIED_SINCE;

#define  ACCESS_HTTP_HFO_IF_UNMODIFIED_SINCE(p)     \
         ACCESS_CONTAINER(p, HTTP_HFO_IF_UNMODIFIED_SINCE, Linkage)


/***********************************************************
         HTTP HEADER FIELD LAST-MODIFIED DEFINITION
***********************************************************/

/*
 * The Last-Modified entity-header field indicates the date and time at which the origin server
 * believes the variant was last modified.
 *
 *      Last-Modified  = "Last-Modified" ":" HTTP-date
 *
 * An example of its use is
 *
 *      Last-Modified: Tue, 15 Nov 1994 12:45:26 GMT
 *
 * The exact meaning of this header field depends on the implementation of the origin server and
 * the nature of the original resource. For files, it may be just the file system last-modified
 * time. For entities with dynamically included parts, it may be the most recent of the set of
 * last-modify times for its component parts. For database gateways, it may be the last-update time
 * stamp of the record. For virtual objects, it may be the last time the internal state changed.
 */
#define  HTTP_HFO_LAST_MODIFIED_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_DATE                       Date;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_LAST_MODIFIED
{
    HTTP_HFO_LAST_MODIFIED_CONTENT
}
HTTP_HFO_LAST_MODIFIED,  *PHTTP_HFO_LAST_MODIFIED;

#define  ACCESS_HTTP_HFO_LAST_MODIFIED(p)           \
         ACCESS_CONTAINER(p, HTTP_HFO_LAST_MODIFIED, Linkage)


/***********************************************************
            HTTP HEADER FIELD LOCATION DEFINITION
***********************************************************/

/*
 * The Location response-header field is used to redirect the recipient to a location other than
 * the Request-URI for completion of the request or identification of a new resource. For 201
 * (Created) responses, the Location is that of the new resource which was created by the request.
 * For 3xx responses, the location SHOULD indicate the server's preferred URI for automatic re-
 * direction to the resource. The field value consists of a single absolute URI.
 *
 *      Location = "Location" ":" absoluteURI
 *
 * An example is:
 *
 *      Location: http://www.w3.org/pub/WWW/People.html
 *
 * Note: The Content-Location header field differs from Location in that the Content-Location
 * identifies the original location of the entity enclosed in the request. It is therefore possible
 * for a response to contain header fields for both Location and Content-Location.
 */
#define  HTTP_HFO_LOCATION_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_REQUEST_URI                PreferredUri;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_LOCATION
{
    HTTP_HFO_LOCATION_CONTENT
}
HTTP_HFO_LOCATION,  *PHTTP_HFO_LOCATION;

#define  ACCESS_HTTP_HFO_LOCATION(p)                \
         ACCESS_CONTAINER(p, HTTP_HFO_LOCATION, Linkage)


/***********************************************************
           HTTP HEADER FIELD MAX-FORWARDS DEFINITION
***********************************************************/

/*
 * The Max-Forwards request-header field provides a mechanism with the TRACE and OPTIONS methods to
 * limit the number of proxies or gateways that can forward the request to the next inbound server.
 * This can be useful when the client is attempting to trace a request chain which appears to be
 * failing or looping in mid-chain.
 *
 *      Max-Forwards   = "Max-Forwards" ":" 1*DIGIT
 *
 * The Max-Forwards value is a decimal integer indicating the remaining number of times this
 * request message may be forwarded.
 */
#define  HTTP_HFO_MAX_FORWARDS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           HopCount;                                               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_MAX_FORWARDS
{
    HTTP_HFO_MAX_FORWARDS_CONTENT
}
HTTP_HFO_MAX_FORWARDS,  *PHTTP_HFO_MAX_FORWARDS;

#define  ACCESS_HTTP_HFO_MAX_FORWARDS(p)            \
         ACCESS_CONTAINER(p, HTTP_HFO_MAX_FORWARDS, Linkage)


/***********************************************************
             HTTP HEADER FIELD PRAGMA DEFINITION
***********************************************************/

/*
 * The Pragma general-header field is used to include implementation-specific directives that might
 * apply to any recipient along the request/response chain. All pragma directives specify optional
 * behavior from the viewpoint of the protocol; however, some systems MAY require that behavior be
 * consistent with the directives.
 *
 *      Pragma            = "Pragma" ":" 1#pragma-directive
 *      pragma-directive  = "no-cache" | extension-pragma
 *      extension-pragma  = token [ "=" ( token | quoted-string ) ]
 *
 * When the no-cache directive is present in a request message, an application SHOULD forward the
 * request toward the origin server even if it has a cached copy of what is being requested. This
 * pragma directive has the same semantics as the no-cache cache-directive and is defined here for
 * backward compatibility with HTTP/1.0. Clients SHOULD include both header fields when a no-cache
 * request is sent to a server not known to be HTTP/1.1 compliant.
 */
#define  HTTP_MAX_PRAGMA_DIRECTIVE_SIZE             32

#define  HTTP_HFO_PRAGMA_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            Directives[HTTP_MAX_PRAGMA_DIRECTIVE_SIZE];             \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_PRAGMA
{
    HTTP_HFO_PRAGMA_CONTENT
}
HTTP_HFO_PRAGMA,  *PHTTP_HFO_PRAGMA;

#define  ACCESS_HTTP_HFO_PRAGMA(p)                  \
         ACCESS_CONTAINER(p, HTTP_HFO_PRAGMA, Linkage)


/***********************************************************
       HTTP HEADER FIELD PROXY-AUTHENTICATE DEFINITION
***********************************************************/

/*
 * The Proxy-Authenticate response-header field MUST be included as part of a 407 (Proxy Authen-
 * tication Required) response. The field value consists of a challenge that indicates the authen-
 * tication scheme and parameters applicable to the proxy for this Request-URI.
 *
 *      Proxy-Authenticate  = "Proxy-Authenticate" ":" 1#challenge
 *
 * The HTTP access authentication process is described in "HTTP Authentication: Basic and Digest
 * Access Authentication". Unlike WWW-Authenticate, the Proxy-Authenticate header field applies
 * only to the current connection and SHOULD NOT be passed on to downstream clients. However, an
 * intermediate proxy might need to obtain its own credentials by requesting them from the down-
 * stream client, which in some circumstances will appear as if the proxy is forwarding the Proxy-
 * Authenticate header field.
 */
#define  HTTP_HFO_PROXY_AUTHENTICATE_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           ChallengeCount;                                         \
    HTTP_AUTH_CHALLENGE             ChallengeArray[HTTP_MAX_CHALLENGE_NUMBER];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_PROXY_AUTHENTICATE
{
    HTTP_HFO_PROXY_AUTHENTICATE_CONTENT
}
HTTP_HFO_PROXY_AUTHENTICATE,  *PHTTP_HFO_PROXY_AUTHENTICATE;

#define  ACCESS_HTTP_HFO_PROXY_AUTHENTICATE(p)      \
         ACCESS_CONTAINER(p, HTTP_HFO_PROXY_AUTHENTICATE, Linkage)


/***********************************************************
       HTTP HEADER FIELD PROXY-AUTHORIZATION DEFINITION
***********************************************************/

/*
 * The Proxy-Authorization request-header field allows the client to identify itself (or its user)
 * to a proxy which requires authentication. The Proxy-Authorization field value consists of cre-
 * dentials containing the authentication information of the user agent for the proxy and/or realm
 * of the resource being requested.
 *
 *      Proxy-Authorization  = "Proxy-Authorization" ":" credentials
 *
 * The HTTP access authentication process is described in "HTTP Authentication: Basic and Digest
 * Access Authentication". Unlike Authorization, the Proxy-Authorization header field applies only
 * to the next outbound proxy that demanded authentication using the Proxy-Authenticate field. When
 * multiple proxies are used in a chain, the Proxy-Authorization header field is consumed by the
 * first outbound proxy that was expecting to receive credentials. A proxy MAY relay the cre-
 * dentials from the client request to the next proxy if that is the mechanism by which the proxies
 * cooperatively authenticate a given request.
 */
#define  HTTP_HFO_PROXY_AUTHORIZATION_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_AUTH_CREDENTIAL            Credential;                                             \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_PROXY_AUTHORIZATION
{
    HTTP_HFO_PROXY_AUTHORIZATION_CONTENT
}
HTTP_HFO_PROXY_AUTHORIZATION,  *PHTTP_HFO_PROXY_AUTHORIZATION;

#define  ACCESS_HTTP_HFO_PROXY_AUTHORIZATION(p)     \
         ACCESS_CONTAINER(p, HTTP_HFO_PROXY_AUTHORIZATION, Linkage)


/***********************************************************
               HTTP HEADER FIELD RANGE DEFINITION
***********************************************************/

/*
 * A byte range operation MAY specify a single range of bytes, or a set of ranges within a single
 * entity.
 *
 *      ranges-specifier      = byte-ranges-specifier
 *      byte-ranges-specifier = bytes-unit "=" byte-range-set
 *      byte-range-set        = 1#( byte-range-spec | suffix-byte-range-spec )
 *      byte-range-spec       = first-byte-pos "-" [last-byte-pos]
 *      first-byte-pos        = 1*DIGIT
 *      last-byte-pos         = 1*DIGIT
 *
 * The first-byte-pos value in a byte-range-spec gives the byte-offset of the first byte in a range.
 * The last-byte-pos value gives the byte-offset of the last byte in the range; that is, the byte
 * positions specified are inclusive. Byte offsets start at zero.
 *
 * If the last-byte-pos value is present, it MUST be greater than or equal to the first-byte-pos in
 * that byte-range-spec, or the byte-range-spec is syntactically invalid. The recipient of a byte-
 * range-set that includes one or more syntactically invalid byte-range-spec values MUST ignore the
 * header field that includes that byte-range-set.
 *
 * If the last-byte-pos value is absent, or if the value is greater than or equal to the current
 * length of the entity-body, last-byte-pos is taken to be equal to one less than the current
 * length of the entity-body in bytes.
 *
 * By its choice of last-byte-pos, a client can limit the number of bytes retrieved without knowing
 * the size of the entity.
 *
 *      suffix-byte-range-spec = "-" suffix-length
 *      suffix-length          = 1*DIGIT
 *
 * A suffix-byte-range-spec is used to specify the suffix of the entity-body, of a length given by
 * the suffix-length value. (That is, this form specifies the last N bytes of an entity-body.) If
 * the entity is shorter than the specified suffix-length, the entire entity-body is used.
 */
#define  HTTP_HFO_RANGE_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    BOOL                            bFbpPresent;                                            \
    BOOL                            bLbpPresent;                                            \
    ULONG                           FirstBytePos;                                           \
    ULONG                           LastBytePos;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_RANGE
{
    HTTP_HFO_RANGE_CONTENT
}
HTTP_HFO_RANGE,  *PHTTP_HFO_RANGE;

#define  ACCESS_HTTP_HFO_RANGE(p)                   \
         ACCESS_CONTAINER(p, HTTP_HFO_RANGE, Linkage)


/***********************************************************
             HTTP HEADER FIELD REFERER DEFINITION
***********************************************************/

/*
 * The Referer request-header field allows the client to specify, for the server's benefit, the
 * address (URI) of the resource from which the Request-URI was obtained (the "referrer", although
 * the header field is misspelled.) The Referer request-header allows a server to generate lists
 * of back-links to resources for interest, logging, optimized caching, etc. It also allows obso-
 * lete or mistyped links to be traced for maintenance. The Referer field MUST NOT be sent if the
 * Request-URI was obtained from a source that does not have its own URI, such as input from the
 * user keyboard.
 *
 *      Referer = "Referer" ":" ( absoluteURI | relativeURI )
 *
 * Example:
 *
 *      Referer: http://www.w3.org/hypertext/DataSources/Overview.html
 *
 * If the field value is a relative URI, it SHOULD be interpreted relative to the Request-URI. The
 * URI MUST NOT include a fragment.
 */
#define  HTTP_HFO_REFERER_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    HTTP_REQUEST_URI                ReferrerUri;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_REFERER
{
    HTTP_HFO_REFERER_CONTENT
}
HTTP_HFO_REFERER,  *PHTTP_HFO_REFERER;

#define  ACCESS_HTTP_HFO_REFERER(p)                 \
         ACCESS_CONTAINER(p, HTTP_HFO_REFERER, Linkage)


/***********************************************************
            HTTP HEADER FIELD RETRY-AFTER DEFINITION
***********************************************************/

/*
 * The Retry-After response-header field can be used with a 503 (Service Unavailable) response to
 * indicate how long the service is expected to be unavailable to the requesting client. This field
 * MAY also be used with any 3xx (Redirection) response to indicate the minimum time the user-agent
 * is asked wait before issuing the redirected request. The value of this field can be either an
 * HTTP-date or an integer number of seconds (in decimal) after the time of the response.
 *
 *      Retry-After = "Retry-After" ":" ( HTTP-date | delta-seconds )
 *
 * Two examples of its use are
 *
 *      Retry-After: Fri, 31 Dec 1999 23:59:59 GMT
 *      Retry-After: 120
 *
 * In the latter example, the delay is 2 minutes.
 */
#define  HTTP_HFO_RETRY_AFTER_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    BOOL                            bDateUsed;                                              \
    HTTP_DATE                       Date;                                                   \
    ULONG                           DeltaSeconds;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_RETRY_AFTER
{
    HTTP_HFO_RETRY_AFTER_CONTENT
}
HTTP_HFO_RETRY_AFTER,  *PHTTP_HFO_RETRY_AFTER;

#define  ACCESS_HTTP_HFO_RETRY_AFTER(p)             \
         ACCESS_CONTAINER(p, HTTP_HFO_RETRY_AFTER, Linkage)


/***********************************************************
              HTTP HEADER FIELD SERVER DEFINITION
***********************************************************/

/*
 * The Server response-header field contains information about the software used by the origin
 * server to handle the request. The field can contain multiple product tokens and comments iden-
 * tifying the server and any significant subproducts. The product tokens are listed in order of
 * their significance for identifying the application.
 *
 *      Server = "Server" ":" 1*( product | comment )
 *
 * Example:
 *
 *      Server: CERN/3.0 libwww/2.17
 *
 * If the response is being forwarded through a proxy, the proxy application MUST NOT modify the
 * Server response-header. Instead, it SHOULD include a Via field.
 */
#define  HTTP_MAX_PRODUCT_TOKEN_SIZE                128

#define  HTTP_HFO_SERVER_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            ProductToken[HTTP_MAX_PRODUCT_TOKEN_SIZE];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_SERVER
{
    HTTP_HFO_SERVER_CONTENT
}
HTTP_HFO_SERVER,  *PHTTP_HFO_SERVER;

#define  ACCESS_HTTP_HFO_SERVER(p)                  \
         ACCESS_CONTAINER(p, HTTP_HFO_SERVER, Linkage)


/***********************************************************
                 HTTP HEADER FIELD TE DEFINITION
***********************************************************/

/*
 * The TE request-header field indicates what extension transfer-codings it is willing to accept in
 * the response and whether or not it is willing to accept trailer fields in a chunked transfer-
 * coding. Its value may consist of the keyword "trailers" and/or a comma-separated list of exten-
 * sion transfer-coding names with optional accept parameters.
 *
 *      TE        = "TE" ":" #( t-codings )
 *      t-codings = "trailers" | ( transfer-extension [ accept-params ] )
 *
 * The presence of the keyword "trailers" indicates that the client is willing to accept trailer
 * fields in a chunked transfer-coding. This keyword is reserved for use with transfer-coding
 * values even though it does not itself represent a transfer-coding.
 *
 * Examples of its use are:
 *
 *      TE: deflate
 *      TE:
 *      TE: trailers, deflate;q=0.5
 *
 * The TE header field only applies to the immediate connection. Therefore, the keyword MUST be
 * supplied within a Connection header field whenever TE is present in an HTTP/1.1 message.
 */
#define  HTTP_HFO_TE_CONTENT                                                                \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           EncodingCount;                                          \
    HTTP_ENCODING                   EncodingArray[HTTP_MAX_ENCODING_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_TE
{
    HTTP_HFO_TE_CONTENT
}
HTTP_HFO_TE,  *PHTTP_HFO_TE;

#define  ACCESS_HTTP_HFO_TE(p)                      \
         ACCESS_CONTAINER(p, HTTP_HFO_TE, Linkage)


/***********************************************************
             HTTP HEADER FIELD TRAILER DEFINITION
***********************************************************/

/*
 * The Trailer general field value indicates that the given set of header fields is present in the
 * trailer of a message encoded with chunked transfer-coding.
 *
 *      Trailer  = "Trailer" ":" 1#field-name
 *
 * An HTTP/1.1 message SHOULD include a Trailer header field in a message using chunked transfer-
 * coding with a non-empty trailer. Doing so allows the recipient to know which header fields to
 * expect in the trailer.
 */
#define  HTTP_MAX_FIELD_NUMBER                      16

#define  HTTP_HFO_TRAILER_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           FieldCount;                                             \
    ULONG                           FieldArray[HTTP_MAX_FIELD_NUMBER];                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_TRAILER
{
    HTTP_HFO_TRAILER_CONTENT
}
HTTP_HFO_TRAILER,  *PHTTP_HFO_TRAILER;

#define  ACCESS_HTTP_HFO_TRAILER(p)                 \
         ACCESS_CONTAINER(p, HTTP_HFO_TRAILER, Linkage)


/***********************************************************
        HTTP HEADER FIELD TRANSFER-ENCODING DEFINITION
***********************************************************/

/*
 * The Transfer-Encoding general-header field indicates what (if any) type of transformation has
 * been applied to the message body in order to safely transfer it between the sender and the
 * recipient. This differs from the content-coding in that the transfer-coding is a property of the
 * message, not of the entity.
 *
 *      Transfer-Encoding = "Transfer-Encoding" ":" 1#transfer-coding
 *
 * An example is:
 *
 *      Transfer-Encoding: chunked
 *
 * If multiple encodings have been applied to an entity, the transfer-codings MUST be listed in the
 * order in which they were applied. Additional information about the encoding parameters MAY be
 * provided by other entity-header fields not defined by this specification.
 */
#define  HTTP_HFO_TRANSFER_ENCODING_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           EncodingCount;                                          \
    HTTP_ENCODING                   EncodingArray[HTTP_MAX_ENCODING_NUMBER];                \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_TRANSFER_ENCODING
{
    HTTP_HFO_TRANSFER_ENCODING_CONTENT
}
HTTP_HFO_TRANSFER_ENCODING,  *PHTTP_HFO_TRANSFER_ENCODING;

#define  ACCESS_HTTP_HFO_TRANSFER_ENCODING(p)       \
         ACCESS_CONTAINER(p, HTTP_HFO_TRANSFER_ENCODING, Linkage)


/***********************************************************
             HTTP HEADER FIELD UPGRADE DEFINITION
***********************************************************/

/*
 * The Upgrade general-header allows the client to specify what additional communication protocols
 * it supports and would like to use if the server finds it appropriate to switch protocols. The
 * server MUST use the Upgrade header field within a 101 (Switching Protocols) response to indicate
 * which protocol(s) are being switched.
 *
 *      Upgrade = "Upgrade" ":" 1#product
 *
 * For example,
 *
 *      Upgrade: HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11
 *
 * The Upgrade header field is intended to provide a simple mechanism for transition from HTTP/1.1
 * to some other, incompatible protocol. It does so by allowing the client to advertise its desire
 * to use another protocol, such as a later version of HTTP with a higher major version number,
 * even though the current request has been made using HTTP/1.1. This eases the difficult transi-
 * tion between incompatible protocols by allowing the client to initiate a request in the more
 * commonly supported protocol while indicating to the server that it would like to use a "better"
 * protocol if available (where "better" is determined by the server, possibly according to the
 * nature of the method and/or resource being requested).
 */
#define  HTTP_HFO_UPGRADE_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            ProductToken[HTTP_MAX_PRODUCT_TOKEN_SIZE];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_UPGRADE
{
    HTTP_HFO_UPGRADE_CONTENT
}
HTTP_HFO_UPGRADE,  *PHTTP_HFO_UPGRADE;

#define  ACCESS_HTTP_HFO_UPGRADE(p)                 \
         ACCESS_CONTAINER(p, HTTP_HFO_UPGRADE, Linkage)


/***********************************************************
            HTTP HEADER FIELD USER-AGENT DEFINITION
***********************************************************/

/*
 * The User-Agent request-header field contains information about the user agent originating the
 * request. This is for statistical purposes, the tracing of protocol violations, and automated
 * recognition of user agents for the sake of tailoring responses to avoid particular user agent
 * limitations. User agents SHOULD include this field with requests. The field can contain multiple
 * product tokens and comments identifying the agent and any subproducts which form a significant
 * part of the user agent. By convention, the product tokens are listed in order of their signifi-
 * cance for identifying the application.
 *
 *      User-Agent = "User-Agent" ":" 1*( product | comment )
 *
 * Example:
 *
 *      User-Agent: CERN-LineMode/2.15 libwww/2.17b3
 */
#define  HTTP_HFO_USER_AGENT_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    char                            ProductToken[HTTP_MAX_PRODUCT_TOKEN_SIZE];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_USER_AGENT
{
    HTTP_HFO_USER_AGENT_CONTENT
}
HTTP_HFO_USER_AGENT,  *PHTTP_HFO_USER_AGENT;

#define  ACCESS_HTTP_HFO_USER_AGENT(p)              \
         ACCESS_CONTAINER(p, HTTP_HFO_USER_AGENT, Linkage)


/***********************************************************
              HTTP HEADER FIELD VARY DEFINITION
***********************************************************/

/*
 * The Vary field value indicates the set of request-header fields that fully determines, while the
 * response is fresh, whether a cache is permitted to use the response to reply to a subsequent
 * request without revalidation. For uncacheable or stale responses, the Vary field value advises
 * the user agent about the criteria that were used to select the representation. A Vary field
 * value of "*" implies that a cache cannot determine from the request headers of a subsequent
 * request whether this response is the appropriate representation.
 *
 *      Vary  = "Vary" ":" ( "*" | 1#field-name )
 *
 * An HTTP/1.1 server SHOULD include a Vary header field with any cacheable response that is
 * subject to server-driven negotiation. Doing so allows a cache to properly interpret future
 * requests on that resource and informs the user agent about the presence of negotiation on that
 * resource. A server MAY include a Vary header field with a non-cacheable response that is subject
 * to server-driven negotiation, since this might provide the user agent with useful information
 * about the dimensions over which the response varies at the time of the response.
 */
#define  HTTP_HFO_VARY_CONTENT                                                              \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           FieldCount;                                             \
    ULONG                           FieldArray[HTTP_MAX_FIELD_NUMBER];                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_VARY
{
    HTTP_HFO_VARY_CONTENT
}
HTTP_HFO_VARY,  *PHTTP_HFO_VARY;

#define  ACCESS_HTTP_HFO_VARY(p)                    \
         ACCESS_CONTAINER(p, HTTP_HFO_VARY, Linkage)


/***********************************************************
               HTTP HEADER FIELD VIA DEFINITION
***********************************************************/

/*
 * The Via general-header field MUST be used by gateways and proxies to indicate the intermediate
 * protocols and recipients between the user agent and the server on requests, and between the
 * origin server and the client on responses. It is analogous to the "Received" field of RFC 822
 * and is intended to be used for tracking message forwards, avoiding request loops, and identi-
 * fying the protocol capabilities of all senders along the request/response chain.
 *
 *      Via               =  "Via" ":" 1#( received-protocol received-by [ comment ] )
 *      received-protocol = [ protocol-name "/" ] protocol-version
 *      protocol-name     = token
 *      protocol-version  = token
 *      received-by       = ( host [ ":" port ] ) | pseudonym
 *      pseudonym         = token
 *
 * The received-protocol indicates the protocol version of the message received by the server or
 * client along each segment of the request/response chain. The received-protocol version is app-
 * ended to the Via field value when the message is forwarded so that information about the pro-
 * tocol capabilities of upstream applications remains visible to all recipients.
 */
#define  HTTP_MAX_VIA_HOP_NUMBER                    16
#define  HTTP_MAX_VIA_TOKEN_SIZE                    64

typedef  struct
_HTTP_VIA_HOP
{
    char                            Protocol[HTTP_MAX_VIA_TOKEN_SIZE];
    char                            Version [HTTP_MAX_VIA_TOKEN_SIZE];
    char                            HostName[HTTP_MAX_VIA_TOKEN_SIZE];
    USHORT                          HostPort;
}
HTTP_VIA_HOP,  *PHTTP_VIA_HOP;

#define  HTTP_HFO_VIA_CONTENT                                                               \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           ViaHopCount;                                            \
    HTTP_VIA_HOP                    ViaHopArray[HTTP_MAX_VIA_HOP_NUMBER];                   \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_VIA
{
    HTTP_HFO_VIA_CONTENT
}
HTTP_HFO_VIA,  *PHTTP_HFO_VIA;

#define  ACCESS_HTTP_HFO_VIA(p)                     \
         ACCESS_CONTAINER(p, HTTP_HFO_VIA, Linkage)


/***********************************************************
             HTTP HEADER FIELD WARNING DEFINITION
***********************************************************/

/*
 * The Warning general-header field is used to carry additional information about the status or
 * transformation of a message which might not be reflected in the message. This information is
 * typically used to warn about a possible lack of semantic transparency from caching operations or
 * transformations applied to the entity body of the message. Warning headers are sent with
 * responses using:
 *
 *      Warning       = "Warning" ":" 1#warning-value
 *      warning-value = warn-code SP warn-agent SP warn-text [SP warn-date]
 *      warn-code     = 3DIGIT
 *      warn-agent    = ( host [ ":" port ] ) | pseudonym
 *                          ; the name or pseudonym of the server adding
 *                          ; the Warning header, for use in debugging
 *      warn-text     = quoted-string
 *      warn-date     = <"> HTTP-date <">
 *
 * A response MAY carry more than one Warning header. The warn-text SHOULD be in a natural language
 * and character set that is most likely to be intelligible to the human user receiving the res-
 * ponse. This decision MAY be based on any available knowledge, such as the location of the cache
 * or user, the Accept-Language field in a request, the Content-Language field in a response, etc.
 * The default language is English and the default character set is ISO-8859-1.
 */
#define  HTTP_MAX_WARNING_VALUE_NUMBER              16
#define  HTTP_MAX_WARNING_TEXT_SIZE                 128

typedef  struct
_HTTP_WARNING_VALUE
{
    ULONG                           Code;
    char                            Text    [HTTP_MAX_WARNING_TEXT_SIZE];
    char                            HostName[ANSC_DOMAIN_NAME_SIZE     ];
    USHORT                          HostPort;
}
HTTP_WARNING_VALUE,  *PHTTP_WARNING_VALUE;

#define  HTTP_HFO_WARNING_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           WarningCount;                                           \
    HTTP_WARNING_VALUE              WarningArray[HTTP_MAX_WARNING_VALUE_NUMBER];            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_WARNING
{
    HTTP_HFO_WARNING_CONTENT
}
HTTP_HFO_WARNING,  *PHTTP_HFO_WARNING;

#define  ACCESS_HTTP_HFO_WARNING(p)                 \
         ACCESS_CONTAINER(p, HTTP_HFO_WARNING, Linkage)


/***********************************************************
        HTTP HEADER FIELD WWW-AUTHENTICATE DEFINITION
***********************************************************/

/*
 * The WWW-Authenticate response-header field MUST be included in 401 (Unauthorized) response
 * messages. The field value consists of at least one challenge that indicates the authentication
 * scheme(s) and parameters applicable to the Request-URI.
 *
 *      WWW-Authenticate  = "WWW-Authenticate" ":" 1#challenge
 *
 * The HTTP access authentication process is described in "HTTP Authentication: Basic and Digest
 * Access Authentication". User agents are advised to take special care in parsing the WWW-
 * Authenticate field value as it might contain more than one challenge, or if more than one WWW-
 * Authenticate header field is provided, the contents of a challenge itself can contain a comma-
 * separated list of authentication parameters.
 */
#define  HTTP_HFO_WWW_AUTHENTICATE_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           ChallengeCount;                                         \
    HTTP_AUTH_CHALLENGE             ChallengeArray[HTTP_MAX_CHALLENGE_NUMBER];              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_WWW_AUTHENTICATE
{
    HTTP_HFO_WWW_AUTHENTICATE_CONTENT
}
HTTP_HFO_WWW_AUTHENTICATE,  *PHTTP_HFO_WWW_AUTHENTICATE;

#define  ACCESS_HTTP_HFO_WWW_AUTHENTICATE(p)        \
         ACCESS_CONTAINER(p, HTTP_HFO_WWW_AUTHENTICATE, Linkage)


/***********************************************************
        HTTP HEADER FIELD PROXY-CONNECTION DEFINITION
***********************************************************/

/*
 * The Proxy-Connection general-header field is not defined in any of the IETF RFCs, but it's found
 * contained in the HTTP request message sent from IE browsers to HTTP proxies. From observation,
 * The Proxy-Connection header has the following grammar:
 *
 *      Proxy-Connection        = "Proxy-Connection" ":" 1#(proxy-connection-token)
 *      proxy-connection-token  = token
 */
#define  HTTP_HFO_PROXY_CONNECTION_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           TokenCount;                                             \
    HTTP_CONNECTION_TOKEN           TokenArray[HTTP_MAX_CONNECTION_TOKEN_NUMBER];           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_PROXY_CONNECTION
{
    HTTP_HFO_PROXY_CONNECTION_CONTENT
}
HTTP_HFO_PROXY_CONNECTION,  *PHTTP_HFO_PROXY_CONNECTION;

#define  ACCESS_HTTP_HFO_PROXY_CONNECTION(p)        \
         ACCESS_CONTAINER(p, HTTP_HFO_PROXY_CONNECTION, Linkage)


/***********************************************************
          HTTP HEADER FIELD PROXY COOKIE DEFINITION
***********************************************************/

/*
 * When it sends a request to an origin server, the user agent sends a Cookie request header to the
 * origin server if it has cookies that are applicable to the request, based on
 *
 *      - the request-host
 *      - the request-URI
 *      - the cookie's age
 *
 * The syntax for the header is (defined by RFC 2109):
 *
 *      cookie          =       "Cookie:" cookie-version
 *                              1*((";" | ",") cookie-value)
 *      cookie-value    =       NAME "=" VALUE [";" path] [";" domain]
 *      cookie-version  =       "$Version" "=" value
 *      NAME            =       attr
 *      VALUE           =       value
 *      path            =       "$Path" "=" value
 *      domain          =       "$Domain" "=" value
 *
 * RFC 2965 obsoletes RFC 2109 and redefines it.
 *
 *      cookie          =  "Cookie:" cookie-version 1*((";" | ",") cookie-value)
 *      cookie-value    =  NAME "=" VALUE [";" path] [";" domain] [";" port]
 *      cookie-version  =  "$Version" "=" value
 *      NAME            =  attr
 *      VALUE           =  value
 *      path            =  "$Path" "=" value
 *      domain          =  "$Domain" "=" value
 *      port            =  "$Port" [ "=" <"> value <"> ]
 *
 * The value of the cookie-version attribute must be the value from the Version attribute, if any,
 * of the corresponding Set-Cookie response header. Otherwise the value for cookie-version is 0.
 * The value for the path attribute must be the value from the Path attribute, if any, of the cor-
 * responding Set-Cookie response header. Otherwise the attribute should be omitted from the Cookie
 * request header. The value for the domain attribute must be the value from the Domain attribute,
 * if any, of the corresponding Set-Cookie response header. Otherwise the attribute should be
 * omitted from the Cookie request header.
 */
#define  HTTP_MAX_COOKIE_COMMENT_SIZE               256
#define  HTTP_MAX_COOKIE_NAME_SIZE                  1024
#define  HTTP_MAX_COOKIE_VALUE_SIZE                 4096
#define  HTTP_MAX_COOKIE_PORT_SIZE                  256

typedef  struct
_HTTP_COOKIE_CONTENT
{
    BOOL                            bSecure;
    BOOL                            bPermanent;
    ULONG                           Version;
    ULONG                           MaxAgeInSeconds;
    ANSC_UNIVERSAL_TIME             Expires;
    BOOL                            bDiscard;

    char                            Name      [HTTP_MAX_COOKIE_NAME_SIZE   ];
    char                            Value     [HTTP_MAX_COOKIE_VALUE_SIZE  ];
    char                            Comment   [HTTP_MAX_COOKIE_COMMENT_SIZE];
    char                            CommentUrl[ANSC_URI_STRING_SIZE        ];
    char                            Path      [HTTP_MAX_PATH_NAME_SIZE     ];
    char                            Domain    [ANSC_DOMAIN_NAME_SIZE       ];
    char                            Port      [HTTP_MAX_COOKIE_PORT_SIZE   ];
}
HTTP_COOKIE_CONTENT,  *PHTTP_COOKIE_CONTENT;

#define  HTTP_HFO_COOKIE_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           CookieCount;                                            \
    HTTP_COOKIE_CONTENT             CookieArray[ANSC_ZERO_ARRAY_SIZE];                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_COOKIE
{
    HTTP_HFO_COOKIE_CONTENT
}
HTTP_HFO_COOKIE,  *PHTTP_HFO_COOKIE;

#define  ACCESS_HTTP_HFO_COOKIE(p)                  \
         ACCESS_CONTAINER(p, HTTP_HFO_COOKIE, Linkage)


/***********************************************************
        HTTP HEADER FIELD PROXY SET-COOKIE DEFINITION
***********************************************************/

/*
 * The origin server initiates a session, if it so desires. (Note that "session" here does not
 * refer to a persistent network connection but to a logical session created from HTTP requests and
 * responses. The presence or absence of a persistent connection should have no effect on the use
 * of cookie-derived sessions). To initiate a session, the origin server returns an extra response
 * header to the client, Set-Cookie. The syntax for the Set-Cookie response header is
 *
 *      set-cookie      =       "Set-Cookie:" cookies
 *      cookies         =       1#cookie
 *      cookie          =       NAME "=" VALUE *(";" cookie-av)
 *      NAME            =       attr
 *      VALUE           =       value
 *      cookie-av       =       "Comment" "=" value
 *                      |       "Domain" "=" value
 *                      |       "Max-Age" "=" value
 *                      |       "Path" "=" value
 *                      |       "Secure"
 *                      |       "Version" "=" 1*DIGIT
 *
 * Informally, the Set-Cookie response header comprises the token Set-Cookie:, followed by a comma-
 * separated list of one or more cookies. Each cookie begins with a NAME=VALUE pair, followed by
 * zero or more semi-colon-separated attribute-value pairs. The syntax for attribute-value pairs
 * was shown earlier. The specific attributes and the semantics of their values follows. The NAME=
 * VALUE attribute-value pair must come first in each cookie. The others, if present, can occur in
 * any order. If an attribute appears more than once in a cookie, the behavior is undefined.
 */
#define  HTTP_HFO_SET_COOKIE_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           CookieCount;                                            \
    HTTP_COOKIE_CONTENT             CookieArray[ANSC_ZERO_ARRAY_SIZE];                      \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_SET_COOKIE
{
    HTTP_HFO_SET_COOKIE_CONTENT
}
HTTP_HFO_SET_COOKIE,  *PHTTP_HFO_SET_COOKIE;

#define  ACCESS_HTTP_HFO_SET_COOKIE(p)              \
         ACCESS_CONTAINER(p, HTTP_HFO_SET_COOKIE, Linkage)


/***********************************************************
        HTTP HEADER FIELD PROXY COOKIE2 DEFINITION
***********************************************************/

/*
 * Cookie2 is defined by RFC 2965.
 *
 *  Cookie2: $Version="1"
 *
 */
#define  HTTP_HFO_COOKIE2_CONTENT                                                           \
    /* duplication of the base object class content */                                      \
    HTTP_HEADER_FIELD_CONTENT                                                               \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HFO_COOKIE2
{
    HTTP_HFO_COOKIE2_CONTENT
}
HTTP_HFO_COOKIE2,  *PHTTP_HFO_COOKIE2;

#define  ACCESS_HTTP_HFO_COOKIE2(p)              \
         ACCESS_CONTAINER(p, HTTP_HFO_COOKIE2, Linkage)


/***********************************************************
        HTTP HEADER FIELD PROXY SET-COOKIE2 DEFINITION
***********************************************************/

/*
 * Set-Cookie2 is defined by RFC 2965, and it obsoletes RFC 2109.
 *
 *  set-cookie      =       "Set-Cookie2:" cookies
 *  cookies         =       1#cookie
 *  cookie          =       NAME "=" VALUE *(";" set-cookie-av)
 *  NAME            =       attr
 *  VALUE           =       value
 *  set-cookie-av   =       "Comment" "=" value
 *                  |       "CommentURL" "=" <"> http_URL <">
 *                  |       "Discard"
 *                  |       "Domain" "=" value
 *                  |       "Max-Age" "=" value
 *                  |       "Path" "=" value
 *                  |       "Port" [ "=" <"> portlist <"> ]
 *                  |       "Secure"
 *                  |       "Version" "=" 1*DIGIT
 *  portlist        =       1#portnum
 *  portnum         =       1*DIGIT
 *
 */

typedef  HTTP_HFO_SET_COOKIE                        HTTP_HFO_SET_COOKIE2,  *PHTTP_HFO_SET_COOKIE2;

#define  ACCESS_HTTP_HFO_SET_COOKIE2(p)              \
         ACCESS_CONTAINER(p, HTTP_HFO_SET_COOKIE2, Linkage)


#endif
