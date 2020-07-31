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

    module:	http_atoheader_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Ato Header Object.

        *   HttpAtoHeaderCreateCodeTable
        *   HttpAtoHeaderCreateNameTable
        *   HttpAtoHeaderGetAtomCode
        *   HttpAtoHeaderGetAtomName

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


#include "http_atoheader_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoHeaderCreateCodeTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoHeaderCreateCodeTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_HEADER_OBJECT         pMyObject    = (PHTTP_ATO_HEADER_OBJECT)hThisObject;

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ACCEPT,
            HTTP_HEADER_ID_ACCEPT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ACCEPT_CHARSET,
            HTTP_HEADER_ID_ACCEPT_CHARSET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ACCEPT_ENCODING,
            HTTP_HEADER_ID_ACCEPT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ACCEPT_LANGUAGE,
            HTTP_HEADER_ID_ACCEPT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ACCEPT_RANGES,
            HTTP_HEADER_ID_ACCEPT_RANGES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_AGE,
            HTTP_HEADER_ID_AGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ALLOW,
            HTTP_HEADER_ID_ALLOW,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_AUTHORIZATION,
            HTTP_HEADER_ID_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CACHE_CONTROL,
            HTTP_HEADER_ID_CACHE_CONTROL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONNECTION,
            HTTP_HEADER_ID_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_ENCODING,
            HTTP_HEADER_ID_CONTENT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_LANGUAGE,
            HTTP_HEADER_ID_CONTENT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_LENGTH,
            HTTP_HEADER_ID_CONTENT_LENGTH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_LOCATION,
            HTTP_HEADER_ID_CONTENT_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_MD5,
            HTTP_HEADER_ID_CONTENT_MD5,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_RANGE,
            HTTP_HEADER_ID_CONTENT_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_CONTENT_TYPE,
            HTTP_HEADER_ID_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_DATE,
            HTTP_HEADER_ID_DATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_ETAG,
            HTTP_HEADER_ID_ETAG,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_EXPECT,
            HTTP_HEADER_ID_EXPECT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_EXPIRES,
            HTTP_HEADER_ID_EXPIRES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_FROM,
            HTTP_HEADER_ID_FROM,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_HOST,
            HTTP_HEADER_ID_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_IF_MATCH,
            HTTP_HEADER_ID_IF_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_IF_MODIFIED_SINCE,
            HTTP_HEADER_ID_IF_MODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_IF_NONE_MATCH,
            HTTP_HEADER_ID_IF_NONE_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_IF_RANGE,
            HTTP_HEADER_ID_IF_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_IF_UNMODIFIED_SINCE,
            HTTP_HEADER_ID_IF_UNMODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_LAST_MODIFIED,
            HTTP_HEADER_ID_LAST_MODIFIED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_LOCATION,
            HTTP_HEADER_ID_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_MAX_FORWARDS,
            HTTP_HEADER_ID_MAX_FORWARDS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_PRAGMA,
            HTTP_HEADER_ID_PRAGMA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_PROXY_AUTHENTICATE,
            HTTP_HEADER_ID_PROXY_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_PROXY_AUTHORIZATION,
            HTTP_HEADER_ID_PROXY_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_RANGE,
            HTTP_HEADER_ID_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_REFERER,
            HTTP_HEADER_ID_REFERER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_RETRY_AFTER,
            HTTP_HEADER_ID_RETRY_AFTER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_SERVER,
            HTTP_HEADER_ID_SERVER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_TE,
            HTTP_HEADER_ID_TE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_TRAILER,
            HTTP_HEADER_ID_TRAILER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_TRANSFER_ENCODING,
            HTTP_HEADER_ID_TRANSFER_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_UPGRADE,
            HTTP_HEADER_ID_UPGRADE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_USER_AGENT,
            HTTP_HEADER_ID_USER_AGENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_VARY,
            HTTP_HEADER_ID_VARY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_VIA,
            HTTP_HEADER_ID_VIA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_WARNING,
            HTTP_HEADER_ID_WARNING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_WWW_AUTHENTICATE,
            HTTP_HEADER_ID_WWW_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_PROXY_CONNECTION,
            HTTP_HEADER_ID_PROXY_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_COOKIE,
            HTTP_HEADER_ID_COOKIE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_NAME_SET_COOKIE,
            HTTP_HEADER_ID_SET_COOKIE,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoHeaderCreateNameTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoHeaderCreateNameTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_HEADER_OBJECT         pMyObject    = (PHTTP_ATO_HEADER_OBJECT)hThisObject;

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ACCEPT,
            HTTP_HEADER_NAME_ACCEPT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ACCEPT_CHARSET,
            HTTP_HEADER_NAME_ACCEPT_CHARSET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ACCEPT_ENCODING,
            HTTP_HEADER_NAME_ACCEPT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ACCEPT_LANGUAGE,
            HTTP_HEADER_NAME_ACCEPT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ACCEPT_RANGES,
            HTTP_HEADER_NAME_ACCEPT_RANGES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_AGE,
            HTTP_HEADER_NAME_AGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ALLOW,
            HTTP_HEADER_NAME_ALLOW,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_AUTHORIZATION,
            HTTP_HEADER_NAME_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CACHE_CONTROL,
            HTTP_HEADER_NAME_CACHE_CONTROL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONNECTION,
            HTTP_HEADER_NAME_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_ENCODING,
            HTTP_HEADER_NAME_CONTENT_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_LANGUAGE,
            HTTP_HEADER_NAME_CONTENT_LANGUAGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_LENGTH,
            HTTP_HEADER_NAME_CONTENT_LENGTH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_LOCATION,
            HTTP_HEADER_NAME_CONTENT_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_MD5,
            HTTP_HEADER_NAME_CONTENT_MD5,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_RANGE,
            HTTP_HEADER_NAME_CONTENT_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_CONTENT_TYPE,
            HTTP_HEADER_NAME_CONTENT_TYPE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_DATE,
            HTTP_HEADER_NAME_DATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_ETAG,
            HTTP_HEADER_NAME_ETAG,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_EXPECT,
            HTTP_HEADER_NAME_EXPECT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_EXPIRES,
            HTTP_HEADER_NAME_EXPIRES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_FROM,
            HTTP_HEADER_NAME_FROM,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_HOST,
            HTTP_HEADER_NAME_HOST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_IF_MATCH,
            HTTP_HEADER_NAME_IF_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_IF_MODIFIED_SINCE,
            HTTP_HEADER_NAME_IF_MODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_IF_NONE_MATCH,
            HTTP_HEADER_NAME_IF_NONE_MATCH,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_IF_RANGE,
            HTTP_HEADER_NAME_IF_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_IF_UNMODIFIED_SINCE,
            HTTP_HEADER_NAME_IF_UNMODIFIED_SINCE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_LAST_MODIFIED,
            HTTP_HEADER_NAME_LAST_MODIFIED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_LOCATION,
            HTTP_HEADER_NAME_LOCATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_MAX_FORWARDS,
            HTTP_HEADER_NAME_MAX_FORWARDS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_PRAGMA,
            HTTP_HEADER_NAME_PRAGMA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_PROXY_AUTHENTICATE,
            HTTP_HEADER_NAME_PROXY_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_PROXY_AUTHORIZATION,
            HTTP_HEADER_NAME_PROXY_AUTHORIZATION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_RANGE,
            HTTP_HEADER_NAME_RANGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_REFERER,
            HTTP_HEADER_NAME_REFERER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_RETRY_AFTER,
            HTTP_HEADER_NAME_RETRY_AFTER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_SERVER,
            HTTP_HEADER_NAME_SERVER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_TE,
            HTTP_HEADER_NAME_TE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_TRAILER,
            HTTP_HEADER_NAME_TRAILER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_TRANSFER_ENCODING,
            HTTP_HEADER_NAME_TRANSFER_ENCODING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_UPGRADE,
            HTTP_HEADER_NAME_UPGRADE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_USER_AGENT,
            HTTP_HEADER_NAME_USER_AGENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_VARY,
            HTTP_HEADER_NAME_VARY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_VIA,
            HTTP_HEADER_NAME_VIA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_WARNING,
            HTTP_HEADER_NAME_WARNING,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_WWW_AUTHENTICATE,
            HTTP_HEADER_NAME_WWW_AUTHENTICATE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_PROXY_CONNECTION,
            HTTP_HEADER_NAME_PROXY_CONNECTION,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_COOKIE,
            HTTP_HEADER_NAME_COOKIE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_HEADER_ID_SET_COOKIE,
            HTTP_HEADER_NAME_SET_COOKIE,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpAtoHeaderGetAtomCode
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve the header code for
        the corresponding header name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the header name to be matched.

    return:     header code.

**********************************************************************/

ULONG
HttpAtoHeaderGetAtomCode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_ATO_HEADER_OBJECT         pMyObject       = (PHTTP_ATO_HEADER_OBJECT)hThisObject;
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
        HttpAtoHeaderGetAtomName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       code
            );

    description:

        This function is called to retrieve the header name for
        the corresponding header code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       code
                Specifies the header code to be matched.

    return:     header name.

**********************************************************************/

char*
HttpAtoHeaderGetAtomName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    )
{
    PHTTP_ATO_HEADER_OBJECT         pMyObject       = (PHTTP_ATO_HEADER_OBJECT)hThisObject;
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
