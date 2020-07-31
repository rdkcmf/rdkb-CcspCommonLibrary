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

    module:	http_bmoreq_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Bmo Req Object.

        *   HttpBmoReqGetStartLineSize
        *   HttpBmoReqPredictBodySize
        *   HttpBmoReqIsHttpVersion11
        *   HttpBmoReqGetServerName
        *   HttpBmoReqGetServerPort
        *   HttpBmoReqGetReqInfo
        *   HttpBmoReqGetMethod
        *   HttpBmoReqGetMajorVersion
        *   HttpBmoReqGetMinorVersion
        *   HttpBmoReqGetUri
        *   HttpBmoReqSetReqInfo
        *   HttpBmoReqSetMethod
        *   HttpBmoReqSetMajorVersion
        *   HttpBmoReqSetMinorVersion
        *   HttpBmoReqSetUri

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#include "http_bmoreq_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetStartLineSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE )pMyObject->hHfpIf;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    ULONG                           ulLineSize   = 0;

    if ( !pReqInfo )
    {
        return  0;
    }
    else
    {
        ulLineSize =
            pHfpIf->GetRequestLineSize
                (
                    pHfpIf->hOwnerContext,
                    (ANSC_HANDLE)pReqInfo
                );
    }

    return  ulLineSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqPredictBodySize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject               = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo                = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    PHTTP_HFO_CONTENT_LENGTH        pHttpHfoContentLength   = NULL;
    PHTTP_HFO_CONTENT_ENCODING      pHttpHfoContentEncoding = NULL;
    PHTTP_HFO_CONTENT_TYPE          pHttpHfoContentType     = NULL;
    ULONG                           ulBodySize              = 0xFFFFFFFF;

    if ( !pReqInfo )
    {
        return  0xFFFFFFFF;
    }
    else
    {
        pHttpHfoContentLength   =
            (PHTTP_HFO_CONTENT_LENGTH)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_LENGTH
                );

        pHttpHfoContentEncoding =
            (PHTTP_HFO_CONTENT_ENCODING)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_ENCODING
                );

        pHttpHfoContentType     =
            (PHTTP_HFO_CONTENT_TYPE)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_CONTENT_TYPE
                );
    }

    if ( pHttpHfoContentLength )
    {
        ulBodySize = pHttpHfoContentLength->BodySize;
    }
    else if ( !pHttpHfoContentEncoding && !pHttpHfoContentType )
    {
        if ( (pReqInfo->Method == HTTP_METHOD_POST) ||
             (pReqInfo->Method == HTTP_METHOD_PUT ) )
        {
            ulBodySize = 0xFFFFFFFF;
        }
        else
        {
            ulBodySize = 0;
        }
    }
    else
    {
        ulBodySize = 0xFFFFFFFF;
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoReqIsHttpVersion11
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check the protocol version number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoReqIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( !pReqInfo )
    {
        return  FALSE;
    }
    else if ( (pReqInfo->MajorVersion == 1) && (pReqInfo->MinorVersion == 1) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetServerName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
HttpBmoReqGetServerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI   )&pReqInfo->RequestUri;
    PHTTP_HFO_HOST                  pHttpHfoHost = NULL;

    if ( (pReqUri->Type                       == HTTP_URI_TYPE_ABSOLUTE_URI) &&
         (AnscSizeOfString(pReqUri->HostName)  > 0                         ) )
    {
        return  pReqUri->HostName;
    }
    else
    {
        pHttpHfoHost =
            (PHTTP_HFO_HOST)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_HOST
                );
    }

    if ( pHttpHfoHost )
    {
        return  pHttpHfoHost->HostName;
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpBmoReqGetServerPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

USHORT
HttpBmoReqGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI   )&pReqInfo->RequestUri;
    PHTTP_HFO_HOST                  pHttpHfoHost = NULL;

    if ( (pReqUri->Type     == HTTP_URI_TYPE_ABSOLUTE_URI) &&
         (pReqUri->HostPort != 0                         ) )
    {
        return  pReqUri->HostPort;
    }
    else
    {
        pHttpHfoHost =
            (PHTTP_HFO_HOST)pMyObject->GetHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    HTTP_HEADER_ID_HOST
                );
    }

    if ( pHttpHfoHost )
    {
        return  pHttpHfoHost->HostPort;
    }

    return  HTTP_SERVER_PORT;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqGetReqInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpBmoReqGetReqInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    
    return  (ANSC_HANDLE)pReqInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetMethod
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqGetMethod
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    
    return  pReqInfo->Method;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetMethodName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
HttpBmoReqGetMethodName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    
    return  pReqInfo->MethodName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetMajorVersion
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqGetMajorVersion
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    
    return  pReqInfo->MajorVersion;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetMinorVersion
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqGetMinorVersion
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    
    return  pReqInfo->MinorVersion;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqGetUri
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
HttpBmoReqGetUri
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI   )&pReqInfo->RequestUri;

    return  (ANSC_HANDLE)pReqUri;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetReqInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInfo
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInfo
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetReqInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    pMyObject->hReqInfo = hInfo;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetMethod
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       method
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       method
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetMethod
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       method
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( !pReqInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pReqInfo->Method = method;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetMajorVersion
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       version
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       version
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetMajorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( !pReqInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pReqInfo->MajorVersion = version;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetMinorVersion
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       version
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       version
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetMinorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( !pReqInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pReqInfo->MinorVersion = version;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqSetUri
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hUri
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hUri
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqSetUri
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUri
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO  )pMyObject->hReqInfo;

    if ( !pReqInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pReqInfo->RequestUri = *(PHTTP_REQUEST_URI)hUri;

    return  ANSC_STATUS_SUCCESS;
}
