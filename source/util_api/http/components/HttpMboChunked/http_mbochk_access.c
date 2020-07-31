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

    module:	http_mbochk_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Mbo Chunked Object.

        *   HttpMboChkGetEntitySize
        *   HttpMboChkGetArrivedSize
        *   HttpMboChkAppendBodyBdo
        *   HttpMboChkGetExpectedSize
        *   HttpMboChkAppendChunkData
        *   HttpMboChkCopyBodyFrom

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/02/02    initial revision.

**********************************************************************/


#include "http_mbochk_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboChkGetEntitySize
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
HttpMboChkGetEntitySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;

    if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        ulBodySize = pMyObject->EntitySize;
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_RELAY )
    {
        ulBodySize = pMyObject->EntitySize;
    }
    else
    {
        AnscAcquireLock(&pMyObject->BdoQueueLock);

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( AnscBdoGetReserved1(pBodyBdo) == HTTP_MBOCHK_BTYPE_CHUNK_DATA )
            {
                ulBodySize += AnscBdoGetBlockSize(pBodyBdo);
            }
        }

        AnscReleaseLock(&pMyObject->BdoQueueLock);
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboChkGetArrivedSize
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
HttpMboChkGetArrivedSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject     = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulArrivedSize = 0;

    if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        ulArrivedSize = pMyObject->ArrivedSize;
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_RELAY )
    {
        ulArrivedSize = pMyObject->ArrivedSize;
    }
    else
    {
        AnscAcquireLock(&pMyObject->BdoQueueLock);

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( (AnscBdoGetReserved1(pBodyBdo) == HTTP_MBOCHK_BTYPE_CHUNK_DATA) ||
                 (AnscBdoGetReserved1(pBodyBdo) == HTTP_MBOCHK_BTYPE_CHUNK_CRLF) )
            {
                ulArrivedSize += AnscBdoGetBlockSize(pBodyBdo);
            }
        }

        AnscReleaseLock(&pMyObject->BdoQueueLock);
    }

    return  ulArrivedSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkAppendBodyBdo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to append a body-part buffer descriptor
        to the current message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the body buffer descriptor to be appended.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR )hBdo;

    pMyObject->TransferSize += AnscBdoGetBlockSize(pBodyBdo);

    if ( AnscBdoGetReserved1(pBodyBdo) == HTTP_MBOCHK_BTYPE_CHUNK_DATA )
    {
        pMyObject->EntitySize  += AnscBdoGetBlockSize(pBodyBdo);
        pMyObject->ArrivedSize += AnscBdoGetBlockSize(pBodyBdo);
    }
    else if ( AnscBdoGetReserved1(pBodyBdo) == HTTP_MBOCHK_BTYPE_CHUNK_CRLF )
    {
        pMyObject->ArrivedSize += AnscBdoGetBlockSize(pBodyBdo);
    }

    if ( pMyObject->Mode == HTTP_MBO_MODE_STORE )
    {
        AnscAcquireLock   (&pMyObject->BdoQueueLock);
        AnscQueuePushEntry(&pMyObject->BdoQueue, &pBodyBdo->Linkage);
        AnscReleaseLock   (&pMyObject->BdoQueueLock);
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        AnscFreeBdo((ANSC_HANDLE)pBodyBdo);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboChkGetExpectedSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the expected body size
        so far by adding all the chunk size.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     expected body size.

**********************************************************************/

ULONG
HttpMboChkGetExpectedSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject      = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_CHUNK_INFO                pChunkInfo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    ULONG                           ulExpectedSize = 0;

    AnscAcquireLock(&pMyObject->ChunkInfoSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->ChunkInfoSList);

    while ( pSLinkEntry )
    {
        pChunkInfo  = ACCESS_HTTP_CHUNK_INFO(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        ulExpectedSize += pChunkInfo->ChunkSize + 2;    /* the chunk-sizes doesn't include the CRLF following chunk-data */
    }

    AnscReleaseLock(&pMyObject->ChunkInfoSListLock);

    return  ulExpectedSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkAppendChunkData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo,
                BOOL                        bHasCrlf
            );

    description:

        This function is called to append a body-part buffer descriptor
        to the current message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the body buffer descriptor to be appended.

                BOOL                        bHasCrlf
                Specifies whether the chunk data contains CRLF.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkAppendChunkData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        BOOL                        bHasCrlf
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pCrlfBdo     = NULL;

    if ( !bHasCrlf )
    {
        AnscBdoSetReserved1(pBodyBdo, HTTP_MBOCHK_BTYPE_CHUNK_DATA);

        returnStatus =
            pMyObject->AppendBodyBdo
                (
                    (ANSC_HANDLE)pMyObject,
                    pBodyBdo
                );
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_RELAY )
    {
        pMyObject->TransferSize += AnscBdoGetBlockSize(pBodyBdo);
        pMyObject->EntitySize   += AnscBdoGetBlockSize(pBodyBdo) - 2;
        pMyObject->ArrivedSize  += AnscBdoGetBlockSize(pBodyBdo);

        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else if ( AnscBdoGetBlockSize(pBodyBdo) == 2 )      /* only contains CRLF - the last chunk */
    {
        AnscBdoSetReserved1(pBodyBdo, HTTP_MBOCHK_BTYPE_CHUNK_CRLF);

        returnStatus =
            pMyObject->AppendBodyBdo
                (
                    (ANSC_HANDLE)pMyObject,
                    pBodyBdo
                );
    }
    else
    {
        returnStatus =
            AnscBdoBreakAt
                (
                    (ANSC_HANDLE)pBodyBdo,
                    AnscBdoGetBlockSize(pBodyBdo) - 2,
                    (PANSC_HANDLE)&pBodyBdo,
                    (PANSC_HANDLE)&pCrlfBdo
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscBdoSetReserved1(pBodyBdo, HTTP_MBOCHK_BTYPE_CHUNK_DATA);

            returnStatus =
                pMyObject->AppendBodyBdo
                    (
                        (ANSC_HANDLE)pMyObject,
                        pBodyBdo
                    );
        }
        else
        {
            AnscBdoSetReserved1(pBodyBdo, HTTP_MBOCHK_BTYPE_CHUNK_DATA);
            AnscBdoSetReserved1(pCrlfBdo, HTTP_MBOCHK_BTYPE_CHUNK_CRLF);

            returnStatus =
                pMyObject->AppendBodyBdo
                    (
                        (ANSC_HANDLE)pMyObject,
                        pBodyBdo
                    );

            returnStatus =
                pMyObject->AppendBodyBdo
                    (
                        (ANSC_HANDLE)pMyObject,
                        pCrlfBdo
                    );
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkCopyBodyFrom
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the message body from the
        internal buffer into an external buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the body to be
                copied to.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied body data as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;
    ULONG                           ulCopySize   = 0;
    ULONG                           ulLeftSize   = *pulSize;

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( AnscBdoGetReserved1(pBodyBdo) != HTTP_MBOCHK_BTYPE_CHUNK_DATA )
        {
            continue;
        }

        if ( ulLeftSize >= AnscBdoGetBlockSize(pBodyBdo) )
        {
            ulCopySize = AnscBdoGetBlockSize(pBodyBdo);
        }
        else
        {
            ulCopySize = ulLeftSize;
        }

        if ( ulCopySize > 0 )
        {
            AnscCopyMemory
                (
                    (PVOID)((ULONG)buffer + ulBodySize),
                    AnscBdoGetBlock(pBodyBdo),
                    ulCopySize
                );
        }

        ulBodySize += ulCopySize;
        ulLeftSize -= ulCopySize;

        if ( ulLeftSize == 0 )
        {
            break;
        }
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    *pulSize = ulBodySize;

    return  ANSC_STATUS_SUCCESS;
}
