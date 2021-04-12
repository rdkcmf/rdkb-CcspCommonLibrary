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

    module:	http_mbochk_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Mbo Chunked Object.

        *   HttpMboChkExamine
        *   HttpMboChkProcess
        *   HttpMboChkCloseUp
        *   HttpMboChkRemoveCoding
        *   HttpMboChkProcessChunkData
        *   HttpMboChkProcessTrailer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.
        06/07/05    apply Kang's enhancement to enhance the chunked
                    payload processing.

**********************************************************************/


#include "http_mbochk_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboChkExamine
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to examine the received payload data
        and return the message state after VIRTUALLY integrating the
        payload into the message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the payload buffer to be examined.

                ULONG                       ulSize
                Specifies the size of the payload data buffer.

    return:     virtual message state.

**********************************************************************/

ULONG
HttpMboChkExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject           = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    ULONG                           ulTestMboState      = HTTP_MBO_STATE_EMPTY;
    
    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                /*
                 * Instead of going through the complicated size-calculation and header lookup, we
                 * always return "over-packed" testing state. However, the original purpose of per-
                 * formance optimization is therefore defeated.
                 */
                ulTestMboState = HTTP_MBO_STATE_OVER_PACKED;

                break;

        case    HTTP_MBO_STATE_COMPLETE :
        case    HTTP_MBO_STATE_OVER_PACKED :

                ulTestMboState = HTTP_MBO_STATE_OVER_PACKED;

        default :

                break;
    }

    return  ulTestMboState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkProcess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to process a received buffer descriptor
        object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject           = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf              = (PHTTP_HFP_INTERFACE     )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR )hBdo;
    PHTTP_CHUNK_INFO                pChunkInfo          = NULL;
    char*                           pRawChunkLine       = (char*)pMyObject->ScratchPad1;
    ULONG                           ulRawChunkLineSize  = 0;
    char*                           pChunkData          = NULL;
    ULONG                           ulCopySize          = 0;
    ULONG                           ulOrgLineSize       = 0;

    if ( pMyObject->SkipSize > 0 )
    {
        if ( AnscBdoGetBlockSize(pBufferDesp) <= pMyObject->SkipSize )
        {
            pMyObject->SkipSize -= AnscBdoGetBlockSize(pBufferDesp);

            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            AnscBdoShrinkRight(pBufferDesp, pMyObject->SkipSize);

            pMyObject->SkipSize = 0;
        }
    }

    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                /*
                 * The data examination procedure can be further broken down into several logical
                 * branchs, each one deals with a unique chunk-transfer-state. Note that while it
                 * seems that we should encapsulate the body-chunk processing in a separate object,
                 * it's actually not a very good idea since we have to be distinguish between the
                 * regular chunk and the last-chunk, don't even mention the trailer headers.
                 */
                if ( (pMyObject->ChunkTransferState == HTTP_MBOCHK_CTSTATE_EMPTY    ) ||
                     (pMyObject->ChunkTransferState == HTTP_MBOCHK_CTSTATE_PART_LINE) ||
                     (pMyObject->ChunkTransferState == HTTP_MBOCHK_CTSTATE_COMPLETE ) )
                {
                    ulCopySize = pMyObject->PadSize1 - pMyObject->UseSize1;
                    ulCopySize = AnscGetMin2(ulCopySize, AnscBdoGetBlockSize(pBufferDesp));

                    AnscCopyMemory
                        (
                            (char*)((ULONG)pRawChunkLine + pMyObject->UseSize1),
                            AnscBdoGetBlock(pBufferDesp),
                            ulCopySize
                        );

                    ulOrgLineSize        = pMyObject->UseSize1;
                    pMyObject->UseSize1 += ulCopySize;

                    AnscHttpFindChunkData
                        (
                            pRawChunkLine,
                            pMyObject->UseSize1,
                            pChunkData
                        );

                    if ( !pChunkData )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                        if ( pMyObject->UseSize1 == pMyObject->PadSize1 )
                        {
                            return  ANSC_STATUS_BAD_PAYLOAD;
                        }
                        else
                        {
                            pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_PART_LINE;

                            return  ANSC_STATUS_SUCCESS;
                        }
                    }
                    else
                    {
                        ulRawChunkLineSize  = (ULONG)pChunkData - (ULONG)pRawChunkLine;
                        pMyObject->UseSize1 = 0;
                        pChunkInfo          =
                            (PHTTP_CHUNK_INFO)pHfpIf->ParseChunkedLine
                                (
                                    pHfpIf->hOwnerContext,
                                    pRawChunkLine,
                                    ulRawChunkLineSize
                                );

                        if ( !pChunkInfo )
                        {
                            AnscFreeBdo(pBufferDesp);

                            return  ANSC_STATUS_BAD_PAYLOAD;
                        }
                        else
                        {
                            pMyObject->AddChunkInfo
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    (ANSC_HANDLE)pChunkInfo,
                                    pRawChunkLine,
                                    ulRawChunkLineSize
                                );
                        }

                        AnscBdoShrinkRight(pBufferDesp, ulRawChunkLineSize - ulOrgLineSize);

                        pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_PART_BODY;
                        returnStatus                  =
                            pMyObject->ProcessChunkData
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    (ANSC_HANDLE)pBufferDesp
                                );
                    }
                }
                else if ( pMyObject->ChunkTransferState == HTTP_MBOCHK_CTSTATE_PART_BODY )
                {
                    returnStatus =
                        pMyObject->ProcessChunkData
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBufferDesp
                            );
                }
                else if ( pMyObject->ChunkTransferState == HTTP_MBOCHK_CTSTATE_TRAILING )
                {
                    returnStatus =
                        pMyObject->ProcessTrailer
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBufferDesp,
                                NULL
                            );
                }

                break;

        case    HTTP_MBO_STATE_COMPLETE :
        case    HTTP_MBO_STATE_OVER_PACKED :

                pMyObject->State = HTTP_MBO_STATE_OVER_PACKED;
                returnStatus     =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_OVER_PACKED,
                            (ANSC_HANDLE)pBufferDesp
                        );
               //CID: 56133 Missing break in switch
	       break;
        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkCloseUp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to process the last buffer descriptor
        object of a particular connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject           = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR )hBdo;

    if ( pMyObject->SkipSize > 0 )
    {
        if ( AnscBdoGetBlockSize(pBufferDesp) <= pMyObject->SkipSize )
        {
            pMyObject->SkipSize -= AnscBdoGetBlockSize(pBufferDesp);

            AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

            return  ANSC_STATUS_SUCCESS;
        }
        else
        {
            AnscBdoShrinkRight(pBufferDesp, pMyObject->SkipSize);

            pMyObject->SkipSize = 0;
        }
    }

    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                pMyObject->State = HTTP_MBO_STATE_COMPLETE;
                returnStatus     =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_COMPLETED,
                            (ANSC_HANDLE)NULL
                        );

                break;

        case    HTTP_MBO_STATE_COMPLETE :
        case    HTTP_MBO_STATE_OVER_PACKED :

                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;

        default :

                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkRemoveCoding
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to remove the transfer-encoding of the
        message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkRemoveCoding
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( (pMyObject->State != HTTP_MBO_STATE_COMPLETE   ) ||
         (pMyObject->State != HTTP_MBO_STATE_OVER_PACKED) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pMyObject->ClearChunkInfos((ANSC_HANDLE)pMyObject);
    }

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( AnscBdoGetReserved1(pBodyBdo) != HTTP_MBOCHK_BTYPE_CHUNK_DATA )
        {
            AnscQueuePopEntryByLink(&pMyObject->BdoQueue, &pBodyBdo->Linkage);

            AnscFreeBdo((ANSC_HANDLE)pBodyBdo);
        }
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkProcessChunkData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to process the chunked message body
        data contained by the specified buffer descriptor which is
        guaranteed to begin with the chunk-data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkProcessChunkData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject           = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf              = (PHTTP_HFP_INTERFACE     )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pCurBodyBdo         = (PANSC_BUFFER_DESCRIPTOR )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pNewBodyBdo         = NULL;
    PHTTP_CHUNK_INFO                pChunkInfo          = NULL;
    char*                           pRawChunkLine       = (char*)pMyObject->ScratchPad1;
    ULONG                           ulRawChunkLineSize  = 0;
    char*                           pChunkData          = NULL;
    ULONG                           ulExpectedSize      = pMyObject->GetExpectedSize((ANSC_HANDLE)pMyObject);
    ULONG                           ulArrivedSize       = pMyObject->GetArrivedSize ((ANSC_HANDLE)pMyObject);
    ULONG                           ulExpectedChunkSize = ulExpectedSize - ulArrivedSize;
    ULONG                           ulCopySize          = 0;
    ULONG                           ulOrgLineSize       = 0;
    BOOL                            bNeedToNotify       = FALSE;
    BOOL                            bBccIfNotified      = FALSE;

    pChunkInfo = (PHTTP_CHUNK_INFO)pMyObject->GetCurChunkInfo((ANSC_HANDLE)pMyObject);

    if ( !pChunkInfo )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else if ( AnscBdoGetBlockSize(pCurBodyBdo) == 0 )
    {
        returnStatus =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                    (ANSC_HANDLE)pCurBodyBdo
                );

        AnscFreeBdo((ANSC_HANDLE)pCurBodyBdo);

        return  ANSC_STATUS_SUCCESS;
    }

    while ( pChunkInfo->ChunkSize > 0 )
    {
        if ( AnscBdoGetBlockSize(pCurBodyBdo) == 0 )
        {
            AnscFreeBdo((ANSC_HANDLE)pCurBodyBdo);

            return  ANSC_STATUS_SUCCESS;
        }

        if ( ulExpectedChunkSize == AnscBdoGetBlockSize(pCurBodyBdo) )
        {
            returnStatus =
                pMyObject->AppendChunkData
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pCurBodyBdo,
                        TRUE
                    );

            pMyObject->SkipSize           = 0;
            pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;
            returnStatus                  =
                pBccIf->Notify
                    (
                        pBccIf->hOwnerContext,
                        HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                        (ANSC_HANDLE)pCurBodyBdo
                    );

            return  returnStatus;
        }
        else if ( ulExpectedChunkSize > AnscBdoGetBlockSize(pCurBodyBdo) )
        {
            if ( (ulExpectedChunkSize - AnscBdoGetBlockSize(pCurBodyBdo)) == 2 )
            {
                /*
                 * Everything else has arrived except the trailing CRLF. We really don't want to
                 * define another set of states to deal with this type of situation, so we append
                 * the CRLF to the chunked data by ourselves. Note that such complexity is caused
                 * by the fact that we process each transmitted packet in "real-time" without
                 * requiring the reassembly of the whole HTTP message.
                 */
                AnscBdoAppendChar(pCurBodyBdo, HTTP_CARRIAGE_RETURN);   /* dangerous! MUST be the owner */
                AnscBdoAppendChar(pCurBodyBdo, HTTP_LINE_FEED      );   /* dangerous! MUST be the owner */

                returnStatus =
                    pMyObject->AppendChunkData
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pCurBodyBdo,
                            TRUE
                        );

                pMyObject->SkipSize           = 2;
                pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;
                returnStatus                  =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                            (ANSC_HANDLE)pCurBodyBdo
                        );

                return  returnStatus;
            }
            else if ( (ulExpectedChunkSize - AnscBdoGetBlockSize(pCurBodyBdo)) == 1 )
            {
                /*
                 * Everything else has arrived except the trailing LF. We really don't want to
                 * define another set of states to deal with this type of situation, so we append
                 * the CRLF to the chunked data by ourselves. Note that such complexity is caused
                 * by the fact that we process each transmitted packet in "real-time" without
                 * requiring the reassembly of the whole HTTP message.
                 */
                AnscBdoAppendChar(pCurBodyBdo, HTTP_LINE_FEED);         /* dangerous! MUST be the owner */

                returnStatus =
                    pMyObject->AppendChunkData
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pCurBodyBdo,
                            TRUE
                        );

                pMyObject->SkipSize           = 1;
                pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;
                returnStatus                  =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                            (ANSC_HANDLE)pCurBodyBdo
                        );

                return  returnStatus;
            }
            else
            {
                returnStatus =
                    pMyObject->AppendChunkData
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pCurBodyBdo,
                            FALSE
                        );

                pMyObject->SkipSize           = 0;
                pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_PART_BODY;
                returnStatus                  =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                            (ANSC_HANDLE)pCurBodyBdo
                        );

                return  returnStatus;
            }
        }
        else
        {
            returnStatus =
                AnscBdoBreakAt
                    (
                        (ANSC_HANDLE)pCurBodyBdo,
                        ulExpectedChunkSize,        /* this size already includes the CRLF appended to the chunk-data */
                        (PANSC_HANDLE)&pCurBodyBdo,
                        (PANSC_HANDLE)&pNewBodyBdo
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscFreeBdo((ANSC_HANDLE)pCurBodyBdo);

                return  returnStatus;
            }
            else
            {
                returnStatus =
                    pMyObject->AppendChunkData
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pCurBodyBdo,
                            TRUE
                        );

                pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;

                bNeedToNotify = TRUE;
                pCurBodyBdo   = pNewBodyBdo;
                pNewBodyBdo   = NULL;
            }

            pMyObject->UseSize1 = 0;
            ulCopySize          = pMyObject->PadSize1 - pMyObject->UseSize1;
            ulCopySize          = AnscGetMin2(ulCopySize, AnscBdoGetBlockSize(pCurBodyBdo));

            AnscCopyMemory
                (
                    (char*)((ULONG)pRawChunkLine + pMyObject->UseSize1),
                    AnscBdoGetBlock(pCurBodyBdo),
                    ulCopySize
                );

            ulOrgLineSize        = pMyObject->UseSize1;
            pMyObject->UseSize1 += ulCopySize;

            AnscHttpFindChunkData
                (
                    pRawChunkLine,
                    pMyObject->UseSize1,
                    pChunkData
                );

            if ( !pChunkData )
            {
                AnscFreeBdo(pCurBodyBdo);

                if ( pMyObject->UseSize1 == pMyObject->PadSize1 )
                {
                    return  ANSC_STATUS_BAD_PAYLOAD;
                }
                else
                {
                    if ( bNeedToNotify )
                    {
                        returnStatus =
                            pBccIf->Notify
                                (
                                    pBccIf->hOwnerContext,
                                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                                    (ANSC_HANDLE)pCurBodyBdo
                                );
                    }

                    pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_PART_LINE;

                    return  ANSC_STATUS_SUCCESS;
                }
            }
            else
            {
                ulRawChunkLineSize  = (ULONG)pChunkData - (ULONG)pRawChunkLine;
                pMyObject->UseSize1 = 0;
                pChunkInfo          =
                    (PHTTP_CHUNK_INFO)pHfpIf->ParseChunkedLine
                        (
                            pHfpIf->hOwnerContext,
                            pRawChunkLine,
                            ulRawChunkLineSize
                        );

                if ( !pChunkInfo )
                {
                    AnscFreeBdo(pCurBodyBdo);

                    return  ANSC_STATUS_BAD_PAYLOAD;
                }
                else
                {
                    pMyObject->AddChunkInfo
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pChunkInfo,
                            pRawChunkLine,
                            ulRawChunkLineSize
                        );
                }

                AnscBdoShrinkRight(pCurBodyBdo, ulRawChunkLineSize - ulOrgLineSize);

                pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_PART_BODY;
                ulExpectedChunkSize           = pChunkInfo->ChunkSize + 2;
            }
        }
    }

    /*
     * The last chunk needs special handling: it does NOT contain any real data except a single
     * pair of CRLF. In this case, we need to notify BCC with event HTTP_BCC_EVENT_COMPLETED and
     * end the processing right away.
     */
    if ( AnscBdoGetBlockSize(pCurBodyBdo) >= 2 )
    {
        char*                       pBodyData = (char*)AnscBdoGetBlock(pCurBodyBdo);

        if ( (*(pBodyData + 0) == '\r') &&
             (*(pBodyData + 1) == '\n') )
        {
            /*
             * This is the last chunk and we've got all the chunk data...
             */
            returnStatus =
                pMyObject->AppendChunkData
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pCurBodyBdo,
                        TRUE
                    );

            pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;
            returnStatus                  =
                pBccIf->Notify
                    (
                        pBccIf->hOwnerContext,
                        HTTP_BCC_EVENT_COMPLETED,
                        (ANSC_HANDLE)pCurBodyBdo
                    );

            return  returnStatus;
        }
    }

    /*
     * Don't forget the trailing CRLF at the end of each chunk. We do our regular stuff: break the
     * buffer descriptor into two pieces - one for the trailing CRLF, one for the rest body. Note
     * that we're making the same assumptions here as we did above:
     *
     *      (1) The current buffer descriptor owns the data buffer, or it's the youngest
     *          child (rightmost) of the family tree.
     *      (2) We can append several bytes at the end of the buffer without causing any
     *          memory overflow problem, which is satisfied by the descriptor-allocation
     *          mechanism (always reserves extra 16 bytes at the end).
     */
    if ( AnscBdoGetBlockSize(pCurBodyBdo) == 0 )
    {
        AnscBdoAppendChar(pCurBodyBdo, HTTP_CARRIAGE_RETURN);   /* dangerous! MUST be the owner */
        AnscBdoAppendChar(pCurBodyBdo, HTTP_LINE_FEED      );   /* dangerous! MUST be the owner */

        returnStatus =
            pMyObject->AppendChunkData
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pCurBodyBdo,
                    TRUE
                );

        pMyObject->SkipSize           = 2;
        pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;
        returnStatus                  =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                    (ANSC_HANDLE)pCurBodyBdo
                );

        return  returnStatus;
    }
    else if ( AnscBdoGetBlockSize(pCurBodyBdo) == 1 )
    {
        AnscBdoAppendChar(pCurBodyBdo, HTTP_LINE_FEED);         /* dangerous! MUST be the owner */

        returnStatus =
            pMyObject->AppendChunkData
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pCurBodyBdo,
                    TRUE
                );

        pMyObject->SkipSize           = 1;
        pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;
        returnStatus                  =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                    (ANSC_HANDLE)pCurBodyBdo
                );

        return  returnStatus;
    }
    else if ( AnscBdoGetBlockSize(pCurBodyBdo) == 2 )
    {
        returnStatus =
            pMyObject->AppendChunkData
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pCurBodyBdo,
                    TRUE
                );

        pMyObject->SkipSize           = 0;
        pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;
        returnStatus                  =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                    (ANSC_HANDLE)pCurBodyBdo
                );

        return  returnStatus;
    }
    else
    {
        returnStatus =
            AnscBdoBreakAt
                (
                    (ANSC_HANDLE)pCurBodyBdo,
                    2,
                    (PANSC_HANDLE)&pCurBodyBdo,
                    (PANSC_HANDLE)&pNewBodyBdo
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscFreeBdo((ANSC_HANDLE)pCurBodyBdo);

            return  returnStatus;
        }
        else
        {
            returnStatus =
                pMyObject->AppendChunkData
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pCurBodyBdo,
                        TRUE
                    );

            pMyObject->SkipSize           = 0;
            pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;

            bNeedToNotify = TRUE;
            pCurBodyBdo   = pNewBodyBdo;
            pNewBodyBdo   = NULL;
        }
    }

    /*
     * We have completed the processing for all the body chunks, and we have received the last
     * chunk line along with at least one octet following the last chunk (otherwise the chunk_data
     * wouldn't be considered as valid). Now we have to recognize the trailer and signal the end
     * of it.
     */
    pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;

    if ( AnscBdoGetBlockSize(pCurBodyBdo) == 0 )
    {
        AnscFreeBdo((ANSC_HANDLE)pCurBodyBdo);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        returnStatus =
            pMyObject->ProcessTrailer
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pCurBodyBdo,
                    &bBccIfNotified
                );

        if ( bBccIfNotified )
        {
            bNeedToNotify = FALSE;
        }
    }

    if ( bNeedToNotify )
    {
        returnStatus =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                    (ANSC_HANDLE)pCurBodyBdo
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkProcessTrailer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo,
                PBOOL                       pbBccIfNotified
            );

    description:

        This function is called to process the chunked message body
        data contained by the specified buffer descriptor which is
        guaranteed to contain trailer data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the buffer descriptor to be processed.

                PBOOL                       pbBccIfNotified
                On return, it indicates whether a BCC IF event has
                been signaled by this function.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkProcessTrailer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        PBOOL                       pbBccIfNotified
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject        = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf           = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = (PANSC_BUFFER_DESCRIPTOR )hBdo;
    char*                           pRawTrailer      = (char*)pMyObject->ScratchPad3;
    ULONG                           ulRawTrailerSize = 0;
    ULONG                           ulCopySize       = 0;
    ULONG                           ulOrgLineSize    = 0;
    BOOL                            bTrailerComplete = FALSE;

    if ( pbBccIfNotified )
    {
        *pbBccIfNotified = FALSE;
    }

    ulCopySize = pMyObject->PadSize3 - pMyObject->UseSize3;
    ulCopySize = AnscGetMin2(ulCopySize, AnscBdoGetBlockSize(pBufferDesp));

    AnscCopyMemory
        (
            (char*)((ULONG)pRawTrailer + pMyObject->UseSize3),
            AnscBdoGetBlock(pBufferDesp),
            ulCopySize
        );

    ulOrgLineSize        = pMyObject->UseSize3;
    pMyObject->UseSize3 += ulCopySize;

    AnscHttpIsTrailerComplete
        (
            pRawTrailer,
            pMyObject->UseSize3,
            bTrailerComplete
        );

    if ( !bTrailerComplete )
    {
        AnscFreeBdo(pBufferDesp);

        if ( pMyObject->UseSize3 == pMyObject->PadSize3 )
        {
            return  ANSC_STATUS_BAD_PAYLOAD;
        }
        else
        {
            pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_TRAILING;

            return  ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        AnscHttpGetTrailerSize
            (
                pRawTrailer,
                pMyObject->UseSize3,
                ulRawTrailerSize
            );

        pMyObject->UseSize3 = 0;

        AnscBdoShrinkRight(pBufferDesp, ulRawTrailerSize - ulOrgLineSize);

        pMyObject->ChunkTransferState = HTTP_MBOCHK_CTSTATE_COMPLETE;
        returnStatus                  =
            pMyObject->AppendTrailer
                (
                    (ANSC_HANDLE)pMyObject,
                    pRawTrailer,
                    ulRawTrailerSize
                );
    }

    if ( AnscBdoGetBlockSize(pBufferDesp) == 0 )
    {
        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

        pMyObject->State = HTTP_MBO_STATE_COMPLETE;
        returnStatus     =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_COMPLETED,
                    (ANSC_HANDLE)NULL
                );
    }
    else
    {
        pMyObject->State = HTTP_MBO_STATE_OVER_PACKED;
        returnStatus     =
            pBccIf->Notify
                (
                    pBccIf->hOwnerContext,
                    HTTP_BCC_EVENT_OVER_PACKED,
                    (ANSC_HANDLE)pBufferDesp
                );
    }

    if ( pbBccIfNotified )
    {
        *pbBccIfNotified = TRUE;
    }

    return  returnStatus;
}
