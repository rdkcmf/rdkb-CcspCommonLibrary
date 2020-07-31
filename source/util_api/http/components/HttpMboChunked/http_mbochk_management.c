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

    module:	http_mbochk_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Mbo Chunked Object.

        *   HttpMboChkClearChunkInfos
        *   HttpMboChkGetCurChunkInfo
        *   HttpMboChkAddChunkInfo

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#include "http_mbochk_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkClearChunkInfos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean the chunk info list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkClearChunkInfos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_CHUNK_INFO                pChunkInfo   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->ChunkInfoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->ChunkInfoSList);

    while ( pSLinkEntry )
    {
        pChunkInfo  = ACCESS_HTTP_CHUNK_INFO(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->ChunkInfoSList);

        AnscFreeMemory((ANSC_HANDLE)pChunkInfo);
    }

    AnscReleaseLock(&pMyObject->ChunkInfoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpMboChkGetCurChunkInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the first chunk info.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     matched chunk info.

**********************************************************************/

ANSC_HANDLE
HttpMboChkGetCurChunkInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_CHUNK_INFO                pChunkInfo   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->ChunkInfoSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->ChunkInfoSList);
    AnscReleaseLock(&pMyObject->ChunkInfoSListLock);

    if ( pSLinkEntry )
    {
        pChunkInfo = ACCESS_HTTP_CHUNK_INFO(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pChunkInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkAddChunkInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChunkInfo,
                char*                       pRawChunkLine,
                ULONG                       ulLineSize
            );

    description:

        This function is called to add a new chunk info.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChunkInfo
                Specifies the chunk info to be added.

                char*                       pRawChunkLine
                Specifies the text buffer holding the raw chunk line.

                ULONG                       ulLineSize
                Specifies the size of the raw chunk line.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkAddChunkInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChunkInfo,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_CHUNK_INFO                pChunkInfo   = (PHTTP_CHUNK_INFO        )hChunkInfo;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = NULL;

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                ulLineSize * 2,
                0,
                0
            );

    if ( !pBufferDesp )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        AnscCopyMemory
            (
                pBufferDesp->Buffer,
                pRawChunkLine,
                ulLineSize
            );

        AnscBdoSetBlockSize(pBufferDesp, ulLineSize                  );
        AnscBdoSetReserved1(pBufferDesp, HTTP_MBOCHK_BTYPE_CHUNK_LINE);
    }

    pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pBufferDesp);

    AnscAcquireLock   (&pMyObject->ChunkInfoSListLock);
    AnscSListPushEntry(&pMyObject->ChunkInfoSList, &pChunkInfo->Linkage);
    AnscReleaseLock   (&pMyObject->ChunkInfoSListLock);

    /*
    returnStatus =
        pBccIf->Notify
            (
                pBccIf->hOwnerContext,
                HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                (ANSC_HANDLE)pBufferDesp
            );
    */

    return  returnStatus;
}
