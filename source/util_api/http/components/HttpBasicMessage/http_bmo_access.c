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

    module:	http_bmo_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Basic Message Object.

        *   HttpBmoGetStartLineSize
        *   HttpBmoGetHeadersSize
        *   HttpBmoPredictBodySize
        *   HttpBmoGetBodySize
        *   HttpBmoGetBodySize2
        *   HttpBmoGetMessageSize
        *   HttpBmoGetPtmForHeaders
        *   HttpBmoGetPtmForBody
        *   HttpBmoGetHeaderBdo
        *   HttpBmoSetHeaderBdo
        *   HttpBmoIsProxyKeepAlive
        *   HttpBmoIsCloseConnection
        *   HttpBmoIsConnKeepAlive
        *   HttpBmoIsChunkedCoding
        *   HttpBmoIsHttpVersion11

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/26/02    initial revision.

**********************************************************************/


#include "http_bmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoGetStartLineSize
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
HttpBmoGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoGetHeadersSize
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
HttpBmoGetHeadersSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject     = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo    = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;
    PHTTP_HEADER_FIELD              pHttpHfo      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulHeadersSize = 0;
    ULONG                           ulHfLineSize  = 0;
    ULONG                           i             = 0;

    if ( pHeaderBdo )
    {
        return  AnscBdoGetBlockSize(pHeaderBdo);
    }

    AnscAcquireLock(&pMyObject->HfoTableLock);

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[i]);

        while ( pSLinkEntry )
        {
            pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pHttpHfo->Flags & HTTP_FIELD_FLAG_LINE_PRESENT )
            {
                ulHfLineSize = AnscSizeOfString(pHttpHfo->HeaderLine);
            }
            else
            {
                ulHfLineSize =
                    pHfpIf->GetHeaderSize
                        (
                            pHfpIf->hOwnerContext,
                            (ANSC_HANDLE)pHttpHfo
                        );
            }

            if ( ulHfLineSize > 0 )
            {
                ulHeadersSize += ulHfLineSize;
                ulHeadersSize += 2;
            }
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    ulHeadersSize += 2;
    ulHeadersSize += pMyObject->GetStartLineSize((ANSC_HANDLE)pMyObject);
    ulHeadersSize += 2;

    return  ulHeadersSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoPredictBodySize
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
HttpBmoPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  0xFFFFFFFF;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoGetBodySize
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
HttpBmoGetBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    ULONG                           ulBodySize   = 0;

    if ( pMessageBody )
    {
        ulBodySize = pMessageBody->GetBodySize((ANSC_HANDLE)pMessageBody);
    }
    else
    {
        ulBodySize = 0;
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoGetBodySize2
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
HttpBmoGetBodySize2
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    ULONG                           ulBodySize   = 0;

    if ( pMessageBody )
    {
        ulBodySize = pMessageBody->GetBodySize2((ANSC_HANDLE)pMessageBody);
    }
    else
    {
        ulBodySize = 0;
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoGetMessageSize
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
HttpBmoGetMessageSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject     = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    ULONG                           ulHeadersSize = 0;
    ULONG                           ulBodySize    = 0;
    ULONG                           ulMessageSize = 0;

    ulHeadersSize = pMyObject->GetHeadersSize((ANSC_HANDLE)pMyObject);
    ulBodySize    = pMyObject->GetBodySize   ((ANSC_HANDLE)pMyObject);
    ulMessageSize = ulHeadersSize + ulBodySize;

    return  ulMessageSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoGetPtmForHeaders
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get permission modify header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoGetPtmForHeaders
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo   = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;

    if ( pHeaderBdo )
    {
        AnscFreeBdo((ANSC_HANDLE)pHeaderBdo);

        pMyObject->hHeaderBdo = (ANSC_HANDLE)NULL;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoGetPtmForBody
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get permission modify message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoGetPtmForBody
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( !pMessageBody )
    {
        return  FALSE;
    }
    else if ( (pMessageBody->GetState((ANSC_HANDLE)pMessageBody) != HTTP_MBO_STATE_COMPLETE   ) ||
              (pMessageBody->GetState((ANSC_HANDLE)pMessageBody) != HTTP_MBO_STATE_OVER_PACKED) )
    {
        return  FALSE;
    }

    pMessageBody->RemoveCoding((ANSC_HANDLE)pMessageBody);

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoGetHeaderBdo
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
HttpBmoGetHeaderBdo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo   = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;

    return  (ANSC_HANDLE)pHeaderBdo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSetHeaderBdo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the header buffer descriptor to be set.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSetHeaderBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    
    pMyObject->hHeaderBdo = hBdo;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoIsProxyKeepAlive
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the "Keep-Alive" token
        is included in the "Proxy-Connection" header.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoIsProxyKeepAlive
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject               = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFO_PROXY_CONNECTION      pHttpHfoProxyConnection = NULL;
    ULONG                           i                       = 0;

    pHttpHfoProxyConnection =
        (PHTTP_HFO_PROXY_CONNECTION)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_PROXY_CONNECTION
            );

    if ( pHttpHfoProxyConnection )
    {
        for ( i = 0; i < pHttpHfoProxyConnection->TokenCount; i++ )
        {
            if ( pHttpHfoProxyConnection->TokenArray[i].OptionType == IANA_CONNECTION_CODE_KEEP_ALIVE )
            {
                return  TRUE;
            }
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoIsCloseConnection
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the "close" token
        is included in the "Connection" header.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoIsCloseConnection
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject          = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFO_CONNECTION            pHttpHfoConnection = NULL;
    ULONG                           i                  = 0;

    pHttpHfoConnection =
        (PHTTP_HFO_CONNECTION)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_CONNECTION
            );

    if ( pHttpHfoConnection )
    {
        for ( i = 0; i < pHttpHfoConnection->TokenCount; i++ )
        {
            if ( pHttpHfoConnection->TokenArray[i].OptionType == IANA_CONNECTION_CODE_CLOSE )
            {
                return  TRUE;
            }
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoIsConnKeepAlive
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the "Keep-Alive" token
        is included in the "Connection" header.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoIsConnKeepAlive
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject          = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFO_CONNECTION            pHttpHfoConnection = NULL;
    ULONG                           i                  = 0;

    pHttpHfoConnection =
        (PHTTP_HFO_CONNECTION)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_CONNECTION
            );

    if ( pHttpHfoConnection )
    {
        for ( i = 0; i < pHttpHfoConnection->TokenCount; i++ )
        {
            if ( pHttpHfoConnection->TokenArray[i].OptionType == IANA_CONNECTION_CODE_KEEP_ALIVE )
            {
                return  TRUE;
            }
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoIsChunkedCoding
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the "chunked" transfer
        coding is enabled.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpBmoIsChunkedCoding
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject                = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFO_TRANSFER_ENCODING     pHttpHfoTransferEncoding = NULL;
    ULONG                           ulIndex                  = 0;

    pHttpHfoTransferEncoding =
        (PHTTP_HFO_TRANSFER_ENCODING)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_TRANSFER_ENCODING
            );

    if ( pHttpHfoTransferEncoding && (pHttpHfoTransferEncoding->EncodingCount > 0) )
    {
        ulIndex = pHttpHfoTransferEncoding->EncodingCount - 1;

        if ( pHttpHfoTransferEncoding->EncodingArray[ulIndex].Type == IANA_TCODING_CODE_CHUNKED )
        {
            return  TRUE;
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpBmoIsHttpVersion11
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
HttpBmoIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  FALSE;
}
