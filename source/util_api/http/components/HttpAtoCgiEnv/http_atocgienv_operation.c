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

    module:	http_atocgienv_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Ato CgiEnv Object.

        *   HttpAtoCgiEnvCreateCodeTable
        *   HttpAtoCgiEnvCreateNameTable
        *   HttpAtoCgiEnvGetAtomCode
        *   HttpAtoCgiEnvGetAtomName

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


#include "http_atocgienv_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoCgiEnvCreateCodeTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP CGI environment
        variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoCgiEnvCreateCodeTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_CGIENV_OBJECT         pMyObject    = (PHTTP_ATO_CGIENV_OBJECT)hThisObject;

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_AUTH_TYPE,
            CGI_ENV_CODE_AUTH_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_CONTENT_LENGTH,
            CGI_ENV_CODE_CONTENT_LENGTH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_CONTENT_TYPE,
            CGI_ENV_CODE_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_DOCUMENT_ROOT,
            CGI_ENV_CODE_DOCUMENT_ROOT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_PATH_INFO,
            CGI_ENV_CODE_PATH_INFO,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_PATH_TRANSLATED,
            CGI_ENV_CODE_PATH_TRANSLATED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_QUERY_STRING,
            CGI_ENV_CODE_QUERY_STRING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_REMOTE_ADDR,
            CGI_ENV_CODE_REMOTE_ADDR,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_REMOTE_HOST,
            CGI_ENV_CODE_REMOTE_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_REMOTE_USER,
            CGI_ENV_CODE_REMOTE_USER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_REQUEST_METHOD,
            CGI_ENV_CODE_REQUEST_METHOD,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_SERVER_NAME,
            CGI_ENV_CODE_SERVER_NAME,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_SERVER_PORT,
            CGI_ENV_CODE_SERVER_PORT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_SERVER_PROTOCOL,
            CGI_ENV_CODE_SERVER_PROTOCOL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_SERVER_SOFTWARE,
            CGI_ENV_CODE_SERVER_SOFTWARE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_SERVER_TYPE,
            CGI_ENV_CODE_SERVER_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ACCEPT,
            CGI_ENV_CODE_HTTP_ACCEPT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ACCEPT_CHARSET,
            CGI_ENV_CODE_HTTP_ACCEPT_CHARSET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ACCEPT_ENCODING,
            CGI_ENV_CODE_HTTP_ACCEPT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ACCEPT_LANGUAGE,
            CGI_ENV_CODE_HTTP_ACCEPT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ACCEPT_RANGES,
            CGI_ENV_CODE_HTTP_ACCEPT_RANGES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_AGE,
            CGI_ENV_CODE_HTTP_AGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ALLOW,
            CGI_ENV_CODE_HTTP_ALLOW,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_AUTHORIZATION,
            CGI_ENV_CODE_HTTP_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CACHE_CONTROL,
            CGI_ENV_CODE_HTTP_CACHE_CONTROL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONNECTION,
            CGI_ENV_CODE_HTTP_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_ENCODING,
            CGI_ENV_CODE_HTTP_CONTENT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_LANGUAGE,
            CGI_ENV_CODE_HTTP_CONTENT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_LOCATION,
            CGI_ENV_CODE_HTTP_CONTENT_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_MD5,
            CGI_ENV_CODE_HTTP_CONTENT_MD5,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_RANGE,
            CGI_ENV_CODE_HTTP_CONTENT_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_CONTENT_TYPE,
            CGI_ENV_CODE_HTTP_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_DATE,
            CGI_ENV_CODE_HTTP_DATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_ETAG,
            CGI_ENV_CODE_HTTP_ETAG,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_EXPECT,
            CGI_ENV_CODE_HTTP_EXPECT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_EXPIRES,
            CGI_ENV_CODE_HTTP_EXPIRES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_FROM,
            CGI_ENV_CODE_HTTP_FROM,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_HOST,
            CGI_ENV_CODE_HTTP_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_IF_MATCH,
            CGI_ENV_CODE_HTTP_IF_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_IF_MODIFIED_SINCE,
            CGI_ENV_CODE_HTTP_IF_MODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_IF_NONE_MATCH,
            CGI_ENV_CODE_HTTP_IF_NONE_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_IF_RANGE,
            CGI_ENV_CODE_HTTP_IF_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_IF_UNMODIFIED_SINCE,
            CGI_ENV_CODE_HTTP_IF_UNMODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_LAST_MODIFIED,
            CGI_ENV_CODE_HTTP_LAST_MODIFIED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_LOCATION,
            CGI_ENV_CODE_HTTP_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_MAX_FORWARDS,
            CGI_ENV_CODE_HTTP_MAX_FORWARDS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_PRAGMA,
            CGI_ENV_CODE_HTTP_PRAGMA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_PROXY_AUTHENTICATE,
            CGI_ENV_CODE_HTTP_PROXY_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_PROXY_AUTHORIZATION,
            CGI_ENV_CODE_HTTP_PROXY_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_RANGE,
            CGI_ENV_CODE_HTTP_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_REFERER,
            CGI_ENV_CODE_HTTP_REFERER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_RETRY_AFTER,
            CGI_ENV_CODE_HTTP_RETRY_AFTER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_SERVER,
            CGI_ENV_CODE_HTTP_SERVER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_TE,
            CGI_ENV_CODE_HTTP_TE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_TRAILER,
            CGI_ENV_CODE_HTTP_TRAILER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_TRANSFER_ENCODING,
            CGI_ENV_CODE_HTTP_TRANSFER_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_UPGRADE,
            CGI_ENV_CODE_HTTP_UPGRADE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_USER_AGENT,
            CGI_ENV_CODE_HTTP_USER_AGENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_VARY,
            CGI_ENV_CODE_HTTP_VARY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_VIA,
            CGI_ENV_CODE_HTTP_VIA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_WARNING,
            CGI_ENV_CODE_HTTP_WARNING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_WWW_AUTHENTICATE,
            CGI_ENV_CODE_HTTP_WWW_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_PROXY_CONNECTION,
            CGI_ENV_CODE_HTTP_PROXY_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_COOKIE,
            CGI_ENV_CODE_HTTP_COOKIE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_NAME_HTTP_SET_COOKIE,
            CGI_ENV_CODE_HTTP_SET_COOKIE,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoCgiEnvCreateNameTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP CGI environment
        variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoCgiEnvCreateNameTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_CGIENV_OBJECT         pMyObject    = (PHTTP_ATO_CGIENV_OBJECT)hThisObject;

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_AUTH_TYPE,
            CGI_ENV_NAME_AUTH_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_CONTENT_LENGTH,
            CGI_ENV_NAME_CONTENT_LENGTH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_CONTENT_TYPE,
            CGI_ENV_NAME_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_DOCUMENT_ROOT,
            CGI_ENV_NAME_DOCUMENT_ROOT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_PATH_INFO,
            CGI_ENV_NAME_PATH_INFO,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_PATH_TRANSLATED,
            CGI_ENV_NAME_PATH_TRANSLATED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_QUERY_STRING,
            CGI_ENV_NAME_QUERY_STRING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_REMOTE_ADDR,
            CGI_ENV_NAME_REMOTE_ADDR,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_REMOTE_HOST,
            CGI_ENV_NAME_REMOTE_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_REMOTE_USER,
            CGI_ENV_NAME_REMOTE_USER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_REQUEST_METHOD,
            CGI_ENV_NAME_REQUEST_METHOD,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_SERVER_NAME,
            CGI_ENV_NAME_SERVER_NAME,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_SERVER_PORT,
            CGI_ENV_NAME_SERVER_PORT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_SERVER_PROTOCOL,
            CGI_ENV_NAME_SERVER_PROTOCOL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_SERVER_SOFTWARE,
            CGI_ENV_NAME_SERVER_SOFTWARE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_SERVER_TYPE,
            CGI_ENV_NAME_SERVER_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ACCEPT,
            CGI_ENV_NAME_HTTP_ACCEPT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ACCEPT_CHARSET,
            CGI_ENV_NAME_HTTP_ACCEPT_CHARSET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ACCEPT_ENCODING,
            CGI_ENV_NAME_HTTP_ACCEPT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ACCEPT_LANGUAGE,
            CGI_ENV_NAME_HTTP_ACCEPT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ACCEPT_RANGES,
            CGI_ENV_NAME_HTTP_ACCEPT_RANGES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_AGE,
            CGI_ENV_NAME_HTTP_AGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ALLOW,
            CGI_ENV_NAME_HTTP_ALLOW,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_AUTHORIZATION,
            CGI_ENV_NAME_HTTP_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CACHE_CONTROL,
            CGI_ENV_NAME_HTTP_CACHE_CONTROL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONNECTION,
            CGI_ENV_NAME_HTTP_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_ENCODING,
            CGI_ENV_NAME_HTTP_CONTENT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_LANGUAGE,
            CGI_ENV_NAME_HTTP_CONTENT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_LOCATION,
            CGI_ENV_NAME_HTTP_CONTENT_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_MD5,
            CGI_ENV_NAME_HTTP_CONTENT_MD5,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_RANGE,
            CGI_ENV_NAME_HTTP_CONTENT_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_CONTENT_TYPE,
            CGI_ENV_NAME_HTTP_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_DATE,
            CGI_ENV_NAME_HTTP_DATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_ETAG,
            CGI_ENV_NAME_HTTP_ETAG,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_EXPECT,
            CGI_ENV_NAME_HTTP_EXPECT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_EXPIRES,
            CGI_ENV_NAME_HTTP_EXPIRES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_FROM,
            CGI_ENV_NAME_HTTP_FROM,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_HOST,
            CGI_ENV_NAME_HTTP_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_IF_MATCH,
            CGI_ENV_NAME_HTTP_IF_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_IF_MODIFIED_SINCE,
            CGI_ENV_NAME_HTTP_IF_MODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_IF_NONE_MATCH,
            CGI_ENV_NAME_HTTP_IF_NONE_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_IF_RANGE,
            CGI_ENV_NAME_HTTP_IF_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_IF_UNMODIFIED_SINCE,
            CGI_ENV_NAME_HTTP_IF_UNMODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_LAST_MODIFIED,
            CGI_ENV_NAME_HTTP_LAST_MODIFIED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_LOCATION,
            CGI_ENV_NAME_HTTP_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_MAX_FORWARDS,
            CGI_ENV_NAME_HTTP_MAX_FORWARDS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_PRAGMA,
            CGI_ENV_NAME_HTTP_PRAGMA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_PROXY_AUTHENTICATE,
            CGI_ENV_NAME_HTTP_PROXY_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_PROXY_AUTHORIZATION,
            CGI_ENV_NAME_HTTP_PROXY_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_RANGE,
            CGI_ENV_NAME_HTTP_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_REFERER,
            CGI_ENV_NAME_HTTP_REFERER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_RETRY_AFTER,
            CGI_ENV_NAME_HTTP_RETRY_AFTER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_SERVER,
            CGI_ENV_NAME_HTTP_SERVER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_TE,
            CGI_ENV_NAME_HTTP_TE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_TRAILER,
            CGI_ENV_NAME_HTTP_TRAILER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_TRANSFER_ENCODING,
            CGI_ENV_NAME_HTTP_TRANSFER_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_UPGRADE,
            CGI_ENV_NAME_HTTP_UPGRADE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_USER_AGENT,
            CGI_ENV_NAME_HTTP_USER_AGENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_VARY,
            CGI_ENV_NAME_HTTP_VARY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_VIA,
            CGI_ENV_NAME_HTTP_VIA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_WARNING,
            CGI_ENV_NAME_HTTP_WARNING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_WWW_AUTHENTICATE,
            CGI_ENV_NAME_HTTP_WWW_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_PROXY_CONNECTION,
            CGI_ENV_NAME_HTTP_PROXY_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_COOKIE,
            CGI_ENV_NAME_HTTP_COOKIE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            CGI_ENV_CODE_HTTP_SET_COOKIE,
            CGI_ENV_NAME_HTTP_SET_COOKIE,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpAtoCgiEnvGetAtomCode
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve the cgienv code for
        the corresponding cgienv name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the cgienv name to be matched.

    return:     cgienv code.

**********************************************************************/

ULONG
HttpAtoCgiEnvGetAtomCode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_ATO_CGIENV_OBJECT         pMyObject       = (PHTTP_ATO_CGIENV_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomByName
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( !pAtomDescriptor )
    {
        return  0;
    }

    return  pAtomDescriptor->AtomId;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpAtoCgiEnvGetAtomName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       code
            );

    description:

        This function is called to retrieve the cgienv name for
        the corresponding cgienv code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       code
                Specifies the cgienv code to be matched.

    return:     cgienv name.

**********************************************************************/

char*
HttpAtoCgiEnvGetAtomName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    )
{
    PHTTP_ATO_CGIENV_OBJECT         pMyObject       = (PHTTP_ATO_CGIENV_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomById
            (
                (ANSC_HANDLE)pMyObject,
                code
            );

    if ( !pAtomDescriptor )
    {
        return  NULL;
    }

    return  pAtomDescriptor->AtomName;
}
