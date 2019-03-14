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

    module:	http_cgi_definitions.h

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

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_CGI_DEFINITIONS_
#define  _HTTP_CGI_DEFINITIONS_


/***********************************************************
     HTTP CGI ENVIRONMENT VARIABLE PARAMETERS DEFINITION
***********************************************************/

/*
 * In order to pass data about the information request from the server to the script, the server
 * uses command line arguments as well as environment variables. These environment variables are
 * set when the server executes the gateway program.
 */
#define  CGI_ENV_CODE_BASE                          0
#define  CGI_ENV_CODE_AUTH_TYPE                     CGI_ENV_CODE_BASE   + 1
#define  CGI_ENV_CODE_CONTENT_LENGTH                CGI_ENV_CODE_BASE   + 2
#define  CGI_ENV_CODE_CONTENT_TYPE                  CGI_ENV_CODE_BASE   + 3
#define  CGI_ENV_CODE_DOCUMENT_ROOT                 CGI_ENV_CODE_BASE   + 4
#define  CGI_ENV_CODE_PATH_INFO                     CGI_ENV_CODE_BASE   + 5
#define  CGI_ENV_CODE_PATH_TRANSLATED               CGI_ENV_CODE_BASE   + 6
#define  CGI_ENV_CODE_QUERY_STRING                  CGI_ENV_CODE_BASE   + 7
#define  CGI_ENV_CODE_FORM_CONTENT                  CGI_ENV_CODE_BASE   + 8
#define  CGI_ENV_CODE_REMOTE_ADDR                   CGI_ENV_CODE_BASE   + 9
#define  CGI_ENV_CODE_REMOTE_HOST                   CGI_ENV_CODE_BASE   + 10
#define  CGI_ENV_CODE_REMOTE_USER                   CGI_ENV_CODE_BASE   + 11
#define  CGI_ENV_CODE_REQUEST_METHOD                CGI_ENV_CODE_BASE   + 12
#define  CGI_ENV_CODE_SCRIPT_NAME                   CGI_ENV_CODE_BASE   + 13
#define  CGI_ENV_CODE_SERVER_NAME                   CGI_ENV_CODE_BASE   + 14
#define  CGI_ENV_CODE_SERVER_PORT                   CGI_ENV_CODE_BASE   + 15
#define  CGI_ENV_CODE_SERVER_PROTOCOL               CGI_ENV_CODE_BASE   + 16
#define  CGI_ENV_CODE_SERVER_SOFTWARE               CGI_ENV_CODE_BASE   + 17
#define  CGI_ENV_CODE_SERVER_TYPE                   CGI_ENV_CODE_BASE   + 18

#define  CGI_ENV_CODE_HTTP                          128
#define  CGI_ENV_CODE_HTTP_ACCEPT                   CGI_ENV_CODE_HTTP   + 1
#define  CGI_ENV_CODE_HTTP_ACCEPT_CHARSET           CGI_ENV_CODE_HTTP   + 2
#define  CGI_ENV_CODE_HTTP_ACCEPT_ENCODING          CGI_ENV_CODE_HTTP   + 3
#define  CGI_ENV_CODE_HTTP_ACCEPT_LANGUAGE          CGI_ENV_CODE_HTTP   + 4
#define  CGI_ENV_CODE_HTTP_ACCEPT_RANGES            CGI_ENV_CODE_HTTP   + 5
#define  CGI_ENV_CODE_HTTP_AGE                      CGI_ENV_CODE_HTTP   + 6
#define  CGI_ENV_CODE_HTTP_ALLOW                    CGI_ENV_CODE_HTTP   + 7
#define  CGI_ENV_CODE_HTTP_AUTHORIZATION            CGI_ENV_CODE_HTTP   + 8
#define  CGI_ENV_CODE_HTTP_CACHE_CONTROL            CGI_ENV_CODE_HTTP   + 9
#define  CGI_ENV_CODE_HTTP_CONNECTION               CGI_ENV_CODE_HTTP   + 10
#define  CGI_ENV_CODE_HTTP_CONTENT_ENCODING         CGI_ENV_CODE_HTTP   + 11
#define  CGI_ENV_CODE_HTTP_CONTENT_LANGUAGE         CGI_ENV_CODE_HTTP   + 12
#define  CGI_ENV_CODE_HTTP_CONTENT_LENGTH           CGI_ENV_CODE_HTTP   + 13
#define  CGI_ENV_CODE_HTTP_CONTENT_LOCATION         CGI_ENV_CODE_HTTP   + 14
#define  CGI_ENV_CODE_HTTP_CONTENT_MD5              CGI_ENV_CODE_HTTP   + 15
#define  CGI_ENV_CODE_HTTP_CONTENT_RANGE            CGI_ENV_CODE_HTTP   + 16
#define  CGI_ENV_CODE_HTTP_CONTENT_TYPE             CGI_ENV_CODE_HTTP   + 17
#define  CGI_ENV_CODE_HTTP_DATE                     CGI_ENV_CODE_HTTP   + 18
#define  CGI_ENV_CODE_HTTP_ETAG                     CGI_ENV_CODE_HTTP   + 19
#define  CGI_ENV_CODE_HTTP_EXPECT                   CGI_ENV_CODE_HTTP   + 20
#define  CGI_ENV_CODE_HTTP_EXPIRES                  CGI_ENV_CODE_HTTP   + 21
#define  CGI_ENV_CODE_HTTP_FROM                     CGI_ENV_CODE_HTTP   + 22
#define  CGI_ENV_CODE_HTTP_HOST                     CGI_ENV_CODE_HTTP   + 23
#define  CGI_ENV_CODE_HTTP_IF_MATCH                 CGI_ENV_CODE_HTTP   + 24
#define  CGI_ENV_CODE_HTTP_IF_MODIFIED_SINCE        CGI_ENV_CODE_HTTP   + 25
#define  CGI_ENV_CODE_HTTP_IF_NONE_MATCH            CGI_ENV_CODE_HTTP   + 26
#define  CGI_ENV_CODE_HTTP_IF_RANGE                 CGI_ENV_CODE_HTTP   + 27
#define  CGI_ENV_CODE_HTTP_IF_UNMODIFIED_SINCE      CGI_ENV_CODE_HTTP   + 28
#define  CGI_ENV_CODE_HTTP_LAST_MODIFIED            CGI_ENV_CODE_HTTP   + 29
#define  CGI_ENV_CODE_HTTP_LOCATION                 CGI_ENV_CODE_HTTP   + 30
#define  CGI_ENV_CODE_HTTP_MAX_FORWARDS             CGI_ENV_CODE_HTTP   + 31
#define  CGI_ENV_CODE_HTTP_PRAGMA                   CGI_ENV_CODE_HTTP   + 32
#define  CGI_ENV_CODE_HTTP_PROXY_AUTHENTICATE       CGI_ENV_CODE_HTTP   + 33
#define  CGI_ENV_CODE_HTTP_PROXY_AUTHORIZATION      CGI_ENV_CODE_HTTP   + 34
#define  CGI_ENV_CODE_HTTP_RANGE                    CGI_ENV_CODE_HTTP   + 35
#define  CGI_ENV_CODE_HTTP_REFERER                  CGI_ENV_CODE_HTTP   + 36
#define  CGI_ENV_CODE_HTTP_RETRY_AFTER              CGI_ENV_CODE_HTTP   + 37
#define  CGI_ENV_CODE_HTTP_SERVER                   CGI_ENV_CODE_HTTP   + 38
#define  CGI_ENV_CODE_HTTP_TE                       CGI_ENV_CODE_HTTP   + 39
#define  CGI_ENV_CODE_HTTP_TRAILER                  CGI_ENV_CODE_HTTP   + 40
#define  CGI_ENV_CODE_HTTP_TRANSFER_ENCODING        CGI_ENV_CODE_HTTP   + 41
#define  CGI_ENV_CODE_HTTP_UPGRADE                  CGI_ENV_CODE_HTTP   + 42
#define  CGI_ENV_CODE_HTTP_USER_AGENT               CGI_ENV_CODE_HTTP   + 43
#define  CGI_ENV_CODE_HTTP_VARY                     CGI_ENV_CODE_HTTP   + 44
#define  CGI_ENV_CODE_HTTP_VIA                      CGI_ENV_CODE_HTTP   + 45
#define  CGI_ENV_CODE_HTTP_WARNING                  CGI_ENV_CODE_HTTP   + 46
#define  CGI_ENV_CODE_HTTP_WWW_AUTHENTICATE         CGI_ENV_CODE_HTTP   + 47

#define  CGI_ENV_CODE_HTTP_PROXY_CONNECTION         CGI_ENV_CODE_HTTP   + 1 + 128
#define  CGI_ENV_CODE_HTTP_COOKIE                   CGI_ENV_CODE_HTTP   + 2 + 128
#define  CGI_ENV_CODE_HTTP_SET_COOKIE               CGI_ENV_CODE_HTTP   + 3 + 128

#define  CGI_ENV_NAME_AUTH_TYPE                     "AUTH_TYPE"
#define  CGI_ENV_NAME_CONTENT_LENGTH                "CONTENT_LENGTH"
#define  CGI_ENV_NAME_CONTENT_TYPE                  "CONTENT_TYPE"
#define  CGI_ENV_NAME_DOCUMENT_ROOT                 "DOCUMENT_ROOT"
#define  CGI_ENV_NAME_PATH_INFO                     "PATH_INFO"
#define  CGI_ENV_NAME_PATH_TRANSLATED               "PATH_TRANSLATED"
#define  CGI_ENV_NAME_QUERY_STRING                  "QUERY_STRING"
#define  CGI_ENV_NAME_FORM_CONTENT                  "FORM_CONTENT"
#define  CGI_ENV_NAME_REMOTE_ADDR                   "REMOTE_ADDR"
#define  CGI_ENV_NAME_REMOTE_HOST                   "REMOTE_HOST"
#define  CGI_ENV_NAME_REMOTE_USER                   "REMOTE_USER"
#define  CGI_ENV_NAME_REQUEST_METHOD                "REQUEST_METHOD"
#define  CGI_ENV_NAME_SCRIPT_NAME                   "SCRIPT_NAME"
#define  CGI_ENV_NAME_SERVER_NAME                   "SERVER_NAME"
#define  CGI_ENV_NAME_SERVER_PORT                   "SERVER_PORT"
#define  CGI_ENV_NAME_SERVER_PROTOCOL               "SERVER_PROTOCOL"
#define  CGI_ENV_NAME_SERVER_SOFTWARE               "SERVER_SOFTWARE"
#define  CGI_ENV_NAME_SERVER_TYPE                   "SERVER_TYPE"

#define  CGI_ENV_NAME_HTTP_ACCEPT                   "HTTP_ACCEPT"
#define  CGI_ENV_NAME_HTTP_ACCEPT_CHARSET           "HTTP_ACCEPT_CHARSET"
#define  CGI_ENV_NAME_HTTP_ACCEPT_ENCODING          "HTTP_ACCEPT_ENCODING"
#define  CGI_ENV_NAME_HTTP_ACCEPT_LANGUAGE          "HTTP_ACCEPT_LANGUAGE"
#define  CGI_ENV_NAME_HTTP_ACCEPT_RANGES            "HTTP_ACCEPT_RANGES"
#define  CGI_ENV_NAME_HTTP_AGE                      "HTTP_AGE"
#define  CGI_ENV_NAME_HTTP_ALLOW                    "HTTP_ALLOW"
#define  CGI_ENV_NAME_HTTP_AUTHORIZATION            "HTTP_AUTHORIZATION"
#define  CGI_ENV_NAME_HTTP_CACHE_CONTROL            "HTTP_CACHE_CONTROL"
#define  CGI_ENV_NAME_HTTP_CONNECTION               "HTTP_CONNECTION"
#define  CGI_ENV_NAME_HTTP_CONTENT_ENCODING         "HTTP_CONTENT_ENCODING"
#define  CGI_ENV_NAME_HTTP_CONTENT_LANGUAGE         "HTTP_CONTENT_LANGUAGE"
#define  CGI_ENV_NAME_HTTP_CONTENT_LENGTH           "HTTP_CONTENT_LENGTH"
#define  CGI_ENV_NAME_HTTP_CONTENT_LOCATION         "HTTP_CONTENT_LOCATION"
#define  CGI_ENV_NAME_HTTP_CONTENT_MD5              "HTTP_CONTENT_MD5"
#define  CGI_ENV_NAME_HTTP_CONTENT_RANGE            "HTTP_CONTENT_RANGE"
#define  CGI_ENV_NAME_HTTP_CONTENT_TYPE             "HTTP_CONTENT_TYPE"
#define  CGI_ENV_NAME_HTTP_DATE                     "HTTP_DATE"
#define  CGI_ENV_NAME_HTTP_ETAG                     "HTTP_ETAG"
#define  CGI_ENV_NAME_HTTP_EXPECT                   "HTTP_EXPECT"
#define  CGI_ENV_NAME_HTTP_EXPIRES                  "HTTP_EXPIRES"
#define  CGI_ENV_NAME_HTTP_FROM                     "HTTP_FROM"
#define  CGI_ENV_NAME_HTTP_HOST                     "HTTP_HOST"
#define  CGI_ENV_NAME_HTTP_IF_MATCH                 "HTTP_IF_MATCH"
#define  CGI_ENV_NAME_HTTP_IF_MODIFIED_SINCE        "HTTP_IF_MODIFIED_SINCE"
#define  CGI_ENV_NAME_HTTP_IF_NONE_MATCH            "HTTP_IF_NONE_MATCH"
#define  CGI_ENV_NAME_HTTP_IF_RANGE                 "HTTP_IF_RANGE"
#define  CGI_ENV_NAME_HTTP_IF_UNMODIFIED_SINCE      "HTTP_IF_UNMODIFIED_SINCE"
#define  CGI_ENV_NAME_HTTP_LAST_MODIFIED            "HTTP_LAST_MODIFIED"
#define  CGI_ENV_NAME_HTTP_LOCATION                 "HTTP_LOCATION"
#define  CGI_ENV_NAME_HTTP_MAX_FORWARDS             "HTTP_MAX_FORWARDS"
#define  CGI_ENV_NAME_HTTP_PRAGMA                   "HTTP_PRAGMA"
#define  CGI_ENV_NAME_HTTP_PROXY_AUTHENTICATE       "HTTP_PROXY_AUTHENTICATE"
#define  CGI_ENV_NAME_HTTP_PROXY_AUTHORIZATION      "HTTP_PROXY_AUTHORIZATION"
#define  CGI_ENV_NAME_HTTP_RANGE                    "HTTP_RANGE"
#define  CGI_ENV_NAME_HTTP_REFERER                  "HTTP_REFERER"
#define  CGI_ENV_NAME_HTTP_RETRY_AFTER              "HTTP_RETRY_AFTER"
#define  CGI_ENV_NAME_HTTP_SERVER                   "HTTP_SERVER"
#define  CGI_ENV_NAME_HTTP_TE                       "HTTP_TE"
#define  CGI_ENV_NAME_HTTP_TRAILER                  "HTTP_TRAILER"
#define  CGI_ENV_NAME_HTTP_TRANSFER_ENCODING        "HTTP_TRANSFER_ENCODING"
#define  CGI_ENV_NAME_HTTP_UPGRADE                  "HTTP_UPGRADE"
#define  CGI_ENV_NAME_HTTP_USER_AGENT               "HTTP_USER_AGENT"
#define  CGI_ENV_NAME_HTTP_VARY                     "HTTP_VARY"
#define  CGI_ENV_NAME_HTTP_VIA                      "HTTP_VIA"
#define  CGI_ENV_NAME_HTTP_WARNING                  "HTTP_WARNING"
#define  CGI_ENV_NAME_HTTP_WWW_AUTHENTICATE         "HTTP_WWW_AUTHENTICATE"

#define  CGI_ENV_NAME_HTTP_PROXY_CONNECTION         "HTTP_PROXY_CONNECTION"
#define  CGI_ENV_NAME_HTTP_COOKIE                   "HTTP_COOKIE"
#define  CGI_ENV_NAME_HTTP_SET_COOKIE               "HTTP_SET_COOKIE"


#endif
