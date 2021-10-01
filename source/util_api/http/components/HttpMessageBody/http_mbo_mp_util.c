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

    module:	http_mbo_mp_util.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Message Body Object.

        *   HttpMboProcessMPData

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        08/03/07    initial revision.

**********************************************************************/


#include "http_mbo_global.h"


#define  HTTP_MP_DATA_BACKCHECK_SIZE                (AnscSizeOfString((const char*)pMyObject->pBoundaryStr) - 1)

BOOL
HttpMboFindBoundaryPos
    (
        PUCHAR                      pData,
        ULONG                       ulDataLen,
        PUCHAR                      pBoundary,
        PULONG                      pulBoundaryPos
    )
{
    BOOL                            bFound          = FALSE;
    PUCHAR                          pOrgData        = pData;
    PUCHAR                          pEnd            = pData + ulDataLen - 1;
    ULONG                           ulBoundaryLen   = AnscSizeOfString((const char*)pBoundary);
    PUCHAR                          pNext;

    while ( pData + ulBoundaryLen <= pEnd )
    {
        pNext = _ansc_memchr(pData, *pBoundary, pEnd - pData + 1);

        /* no boundary leading character in sight or available size less than boundary string length */
        if ( !pNext || ((ULONG)(pEnd - pNext + 1)) < ulBoundaryLen )
        {
            break;
        }

        if ( AnscEqualString2((char*)pNext, (char*)pBoundary, ulBoundaryLen, TRUE) )
        {
            *pulBoundaryPos = pNext - pOrgData;
            bFound = TRUE;
            break;
        }

        pData = pNext + 1;
    }

    return bFound;
}


BOOL
HttpMboFindNextBoundary
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        ULONG                       ulStartPos,
        PULONG                      pulBoundaryPos
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject   = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBdo        = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    PUCHAR                          pData       = (PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo) + ulStartPos;
    ULONG                           ulLen       = AnscBdoGetBlockSize(pBdo) - ulStartPos - AnscBdoGetOffset(pBdo);

    return HttpMboFindBoundaryPos(pData, ulLen, pMyObject->pBoundaryStr, pulBoundaryPos);
}


BOOL
HttpMboFindNextBoundary2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLastBdo,
        ANSC_HANDLE                 hBdo,
        PBOOL                       pbFoundInLastBdo,
        PULONG                      pulOffset
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject   = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pLastBdo    = (PANSC_BUFFER_DESCRIPTOR)hLastBdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo        = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    BOOL                            bFound      = FALSE;
    PUCHAR                          pCurData    = (PUCHAR)AnscBdoGetBuffer(pLastBdo) + AnscBdoGetOffset(pLastBdo);
    ULONG                           ulCurDataLen= AnscBdoGetBlockSize(pLastBdo) - AnscBdoGetOffset(pLastBdo);
    PUCHAR                          pNewData    = (PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo);
    ULONG                           ulNewDataLen= AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo);
    ULONG                           ulBoundary  = AnscSizeOfString((const char*)pMyObject->pBoundaryStr);
    ULONG                           ulLen       = 0;
    ULONG                           ulNewFeedLen= ulBoundary - 1;

    if ( ulCurDataLen != 0 )
    {
        AnscCopyMemory(pMyObject->pMPHeadersBuf, pCurData, ulCurDataLen);
        ulLen = ulCurDataLen;
    }

    if ( ulNewDataLen < ulNewFeedLen )
    {
        ulNewFeedLen = ulNewDataLen;
    }

    AnscCopyMemory(pMyObject->pMPHeadersBuf + ulLen, pNewData, ulNewFeedLen);
    ulLen += ulNewFeedLen;

    bFound = HttpMboFindBoundaryPos(pMyObject->pMPHeadersBuf, ulLen, pMyObject->pBoundaryStr, pulOffset);

    if ( bFound && *pulOffset == 0 )
    {
        /* last BDO contains only partial of a new multipart header, we enforce doing search in current BDO */
        bFound = FALSE;
    }

    if ( bFound )
    {
        *pbFoundInLastBdo = TRUE;
        *pulOffset += AnscBdoGetOffset(pLastBdo);
    }
    else
    {
        bFound = HttpMboFindBoundaryPos(pNewData, ulNewDataLen, pMyObject->pBoundaryStr, pulOffset);

        if ( bFound )
        {
            *pbFoundInLastBdo = FALSE;
            *pulOffset += AnscBdoGetOffset(pBdo);
        }
    }

    return bFound;
}


ANSC_STATUS
HttpMboParsePartHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        PULONG                      pulHeadersLen,
        PBOOL                       pbPartEndFound,
        PULONG                      pulPartEndOffset,
        PUCHAR*                     ppFieldName,
        PUCHAR*                     ppFileName,
        PBOOL                       pbFileData
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject   = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBdo        = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    PANSC_BUFFER_DESCRIPTOR         pLastBdo    = (PANSC_BUFFER_DESCRIPTOR)pMyObject->hLastMPBdo;
    ULONG                           ulCurSize   = pLastBdo ? AnscBdoGetBlockSize(pLastBdo) - AnscBdoGetOffset(pLastBdo) : 0;
    ULONG                           ulNewSize   = AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo);

    *pulHeadersLen      = 0;
    *pbPartEndFound     = FALSE;
    *pulPartEndOffset   = 0;
    *ppFieldName        = NULL;
    *ppFileName         = NULL;
    *pbFileData         = FALSE;

    if ( ulCurSize + ulNewSize < HTTP_MBO_MP_PART_HEADER_MIN_SIZE )
    {
        status = ANSC_STATUS_MORE_DATA;
    }
    else
    {
        PUCHAR                      pHdrBuf         = pMyObject->pMPHeadersBuf;
        ULONG                       ulHdrSize       = 0;
        ULONG                       ulBoundarySize  = AnscSizeOfString((const char*)pMyObject->pBoundaryStr);
        PUCHAR                      pData, pCrlf;
        ULONG                       ulDataLen;

        if ( !pMyObject->pMPHeadersBuf )
        {
            pMyObject->pMPHeadersBuf = (PUCHAR)AnscAllocateMemory(HTTP_MBO_MP_PART_HEADER_SIZE);
            pHdrBuf = pMyObject->pMPHeadersBuf;

            if ( !pHdrBuf )
            {
                return ANSC_STATUS_RESOURCES;
            }
        }

        if ( ulCurSize > ulBoundarySize + 2 )
        {
            pData = (PUCHAR)AnscBdoGetBuffer(pLastBdo) + AnscBdoGetOffset(pLastBdo);
            ulDataLen = AnscBdoGetBlockSize(pLastBdo) - AnscBdoGetOffset(pLastBdo);

            ulHdrSize = ulDataLen - ulBoundarySize - 2;

            AnscCopyMemory
                (
                    pHdrBuf,
                    pData + ulBoundarySize + 2,
                    ulHdrSize
                );
        }

        pData = (PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo);
        ulDataLen = AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo);

        if ( ulDataLen + ulHdrSize >= HTTP_MBO_MP_PART_HEADER_SIZE )
        {
            ulDataLen = HTTP_MBO_MP_PART_HEADER_SIZE - ulHdrSize - 1;
        }

        if ( ulCurSize == 0 )
        {
            ulDataLen -= ulBoundarySize + 2;
            pData += ulBoundarySize + 2;
        }

        AnscCopyMemory(pHdrBuf + ulHdrSize, pData, ulDataLen);
        ulHdrSize += ulDataLen;
        pHdrBuf[ulHdrSize] = 0;

        /* make sure we got all headers */
        pCrlf = (PUCHAR)_ansc_strstr((const char*)pData, "\r\n\r\n");
        if ( !pCrlf )
        {
            status = ANSC_STATUS_MORE_DATA;
            goto EXIT;
        }

        /* parse headers */
        pData = pHdrBuf;
        while ( pData && pData < pHdrBuf + ulHdrSize )
        {
            pCrlf = (PUCHAR)_ansc_strstr((const char*)pData, "\r\n");

            if ( pCrlf == pData )
            {
                /* end of headers */

                *pulHeadersLen = pCrlf - pHdrBuf + 2 + ulBoundarySize + 2;

                break;
            }

            if ( !pCrlf )
            {
                status = ANSC_STATUS_MORE_DATA;
                break;
            }
            else
            {
                PUCHAR                  pToken;
                ULONG                   ulLen;
                PCHAR                   pContentDisp = "Content-Disposition";
                PCHAR                   pContentType = "Content-Type";
                PCHAR                   pFieldName   = "name=";
                PCHAR                   pFileName    = "filename=";

                pToken = _ansc_memchr(pData, ':', pCrlf - pData);

                if ( !pToken )
                {
                    status = ANSC_STATUS_BAD_PARAMETER;
                    break;
                }

                *pCrlf = 0;

                /* check header name */
                ulLen = pToken - pData;

                if ( AnscSizeOfString(pContentType) == ulLen && AnscEqualString2((char*)pData, pContentType, ulLen, FALSE) )
                {
                    *pbFileData = TRUE;
                }
                else if ( AnscSizeOfString(pContentDisp) == ulLen && AnscEqualString2((char*)pData, pContentDisp, ulLen, FALSE) )
                {
                    PUCHAR          pNext = NULL;

                    pToken = (PUCHAR)_ansc_strstr((const char*)(pToken + 1), (const char*)pFieldName);

                    if ( pToken )
                    {
                        pToken += AnscSizeOfString((const char*)pFieldName);

                        pToken = (PUCHAR)_ansc_strchr((const char*)pToken, '"');
                        if ( pToken )
                        {
                            pToken ++;

                            pNext = (PUCHAR)_ansc_strchr((const char*)pToken, '"');

                            if ( pNext )
                            {
                                ulLen = pNext - pToken;
                                *ppFieldName = (PUCHAR)AnscAllocateMemory(ulLen + 1);
                                if ( *ppFieldName )
                                {
                                    AnscCopyMemory(*ppFieldName, pToken, ulLen);
                                    (*ppFieldName)[ulLen] = 0;
                                }
                            }
                        }
                    }
                    /*CID: 61811 Dereference after null check*/
		    if ( pToken )
                        pToken = (PUCHAR)_ansc_strstr((const char*)(pToken + 1), (const char*)pFileName);

                    if ( pToken )
                    {
                        pToken += AnscSizeOfString((const char*)pFileName);

                        pToken = (PUCHAR)_ansc_strchr((const char*)pToken, '"');
                        if ( pToken )
                        {
                            pToken ++;

                            pNext = (PUCHAR)_ansc_strchr((const char*)pToken, '"');

                            if ( pNext )
                            {
                                ulLen = pNext - pToken;
                                *ppFileName = (PUCHAR)AnscAllocateMemory(ulLen + 2);
                                if ( *ppFileName )
                                {
                                    AnscCopyMemory(*ppFileName, pToken, ulLen);
                                    (*ppFileName)[ulLen] = 0;
                                }
                            }
                        }
                    }
                }

                pData = pCrlf + 2;
            }
        }

        if ( status == ANSC_STATUS_SUCCESS )
        {
            ULONG                   ulHdrSize   = *pulHeadersLen;
            ULONG                   ulStartPos  = ulHdrSize;

            if ( pLastBdo )
            {
                ulStartPos -= ulCurSize;
            }

            *pbPartEndFound =
                HttpMboFindNextBoundary
                    (
                        hThisObject,
                        (ANSC_HANDLE)pBdo,
                        ulStartPos,
                        pulPartEndOffset
                    );

            if ( *pbPartEndFound )
            {
                *pulPartEndOffset += AnscBdoGetOffset(pBdo) + ulHdrSize - ulCurSize;
            }
        }
    }

EXIT:

    return status;
}


ANSC_STATUS
HttpMboSaveNonFileData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        BOOL                        bEndFound,
        ULONG                       ulEndOffset,
        BOOL                        bHoldBoundary
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject   = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBdo        = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    PANSC_BUFFER_DESCRIPTOR         pSavedBdo;
    ULONG                           ulDataLen   = 0;

    if ( bEndFound )
    {
        ulDataLen = ulEndOffset - AnscBdoGetOffset(pBdo);
    }
    else
    {
        ulDataLen = AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo);
        if ( bHoldBoundary )
        {
            if ( ulDataLen >= HTTP_MP_DATA_BACKCHECK_SIZE )
            {
                ulDataLen -= HTTP_MP_DATA_BACKCHECK_SIZE;
            }
            else
            {
                ulDataLen = 0;
            }
        }
    }

    pSLinkEntry = AnscQueueGetLastEntry(&pMyObject->BdoQueue);

    if ( pSLinkEntry )
    {
        pSavedBdo   = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
    }
    else
    {
        pSavedBdo   = AnscAllocateBdo(HTTP_MBO_MP_PART_BDO_SIZE, 0, 0);
        if ( !pSavedBdo )
        {
            return ANSC_STATUS_RESOURCES;
        }
        AnscQueuePushEntry(&pMyObject->BdoQueue, &pSavedBdo->Linkage);
    }

    if ( AnscBdoGetBufferSize(pSavedBdo) - AnscBdoGetBlockSize(pSavedBdo) < ulDataLen )
    {
        ULONG                       ulNewBdoSize = ulDataLen;

        if ( ulDataLen < HTTP_MBO_MP_PART_BDO_SIZE )
        {
            ulNewBdoSize = HTTP_MBO_MP_PART_BDO_SIZE;
        }

        pSavedBdo   = AnscAllocateBdo(ulNewBdoSize, 0, 0);
        if ( !pSavedBdo )
        {
            return ANSC_STATUS_RESOURCES;
        }
        AnscQueuePushEntry(&pMyObject->BdoQueue, &pSavedBdo->Linkage);
    }

#ifdef   _DEBUG /* kang debug */
    AnscTrace("Non file form data saved %lu bytes.\n", ulDataLen);
    AnscPrintBinaryMessage((PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo), ulDataLen);
#endif

    AnscBdoAppendData(pSavedBdo, (PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo), ulDataLen);

    return status;
}


ANSC_STATUS
HttpMboSaveFileData
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulReqBodySize,
        ANSC_HANDLE                 hBdo,
        BOOL                        bEndFound,
        ULONG                       ulEndOffset,
        BOOL                        bHoldBoundary
    )
{
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject   = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_FUM_INTERFACE             pFumIf      = (PHTTP_FUM_INTERFACE      )pMyObject->hFumIf;
    PHTTP_MDH_INTERFACE             pMdhIf      = NULL;
    PANSC_BUFFER_DESCRIPTOR         pBdo        = (PANSC_BUFFER_DESCRIPTOR  )hBdo;
    ULONG                           ulDataLen   = 0;
    PHTTP_BMO_REQ_OBJECT            pBmoRequest = (PHTTP_BMO_REQ_OBJECT     )pMyObject->hOwnerContext;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pBmoRequest->hRcpIf;

    PUCHAR                          pReqUri     = (PUCHAR)pRcpIf->GetPathInfo(pRcpIf->hOwnerContext, (ANSC_HANDLE)pBmoRequest);

    pMdhIf = (PHTTP_MDH_INTERFACE)pFumIf->GetMdhIf(pFumIf->hOwnerContext, pReqUri);

    if ( !pMdhIf )
    {
        /* save it in memory if MDH interface is not specified */
        return HttpMboSaveNonFileData(hThisObject, hBdo, bEndFound, ulEndOffset, bHoldBoundary);
    }

    if ( bEndFound )
    {
        ulDataLen = ulEndOffset - AnscBdoGetOffset(pBdo) - 2;
    }
    else
    {
        ulDataLen = AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo);

        if ( ulDataLen == 0 )
        {
            return ANSC_STATUS_SUCCESS; /* no data actually, it happens if data comes in with another packet */
        }

        if ( bHoldBoundary )
        {
            if ( ulDataLen >= HTTP_MP_DATA_BACKCHECK_SIZE )
            {
                ulDataLen -= HTTP_MP_DATA_BACKCHECK_SIZE;
            }
            else
            {
                return ANSC_STATUS_SUCCESS;
            }
        }
    }

    status =
        pMdhIf->DataReceived
            (
                pMdhIf->hOwnerContext,
                pBmoRequest->GetWebSessionId((ANSC_HANDLE)pBmoRequest),
                pReqUri,
                ulReqBodySize,
                pMyObject->pMPFieldName,
                pMyObject->pMPFileName,
                (PUCHAR)AnscBdoGetBuffer(pBdo) + AnscBdoGetOffset(pBdo),
                ulDataLen
            );

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboProcessMPData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to process Multipart data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the body buffer descriptor to be appended.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboProcessMPData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     status              = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject           = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE      )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBdo                = (PANSC_BUFFER_DESCRIPTOR  )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pLastBdo            = (PANSC_BUFFER_DESCRIPTOR  )pMyObject->hLastMPBdo;
    ULONG                           ulHdrSize           = 0;
    ULONG                           ulEndOffset         = 0;
    BOOL                            bEndFound           = FALSE;
    BOOL                            bFileData           = FALSE;
    QUEUE_HEADER                    BdoQueue;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    ULONG                           ulPredictedBodySize = pBccIf->PredictBodySize(pBccIf->hOwnerContext);
    BOOL                            bFoundInLastBdo     = FALSE;
    PUCHAR                          pFieldName          = NULL;
    PUCHAR                          pFileName           = NULL;

    AnscQueueInitializeHeader(&BdoQueue);
    AnscQueuePushEntry(&BdoQueue, &pBdo->Linkage);

    while ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&BdoQueue);
        pBdo = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);

        if ( !pBdo )
        {
            break;
        }

        pLastBdo = (PANSC_BUFFER_DESCRIPTOR)pMyObject->hLastMPBdo;

        switch ( pMyObject->MPState )
        {
            case    HTTP_MBO_MP_STATE_INIT:

                    /* we must start at the beginning of boundary string, and try to find
                     * a part that may end in the same BDO
                     *
                     * ulHdrSize - the size of headers of this part on successful parsing
                     * bEndFound - TRUE means we find the end of this part at ulEndOffset (absolute to BDO buffer)
                     * ulEndOffset - absolute offset from the beginning of the BDO buffer.
                     */
                    status =
                        HttpMboParsePartHeaders
                            (
                                hThisObject,
                                hBdo,
                                &ulHdrSize,
                                &bEndFound,
                                &ulEndOffset,
                                &pFieldName,
                                &pFileName,
                                &bFileData
                            );

                    if ( status == ANSC_STATUS_MORE_DATA )
                    {

                        if ( !pLastBdo )
                        {
                            pMyObject->hLastMPBdo = (ANSC_HANDLE)pBdo;
                        }
                        else
                        {
                            AnscBdoAppendData(pLastBdo, AnscBdoGetBuffer(pBdo), AnscBdoGetBlockSize(pBdo));
                            AnscFreeBdo(hBdo);
                        }

                        status = ANSC_STATUS_SUCCESS;

                        return status;
                    }
                    else if ( status != ANSC_STATUS_SUCCESS )
                    {
                        if(pFieldName) /*RDKB-6237, CID-24315, free resource before return*/
                        {
                            AnscFreeMemory(pFieldName);
                        }

                        if(pFileName) /*RDKB-6237, CID-24272, free resource before return*/
                        {
                            AnscFreeMemory(pFileName);
                        }

                        AnscFreeBdo(hBdo);

                        return status;
                    }
                    else
                    {
                        if ( !pMyObject->pMPFieldName || pFieldName )
                        {
                            if ( pMyObject->pMPFieldName )
                            {
                                AnscFreeMemory(pMyObject->pMPFieldName);
                            }

                            if ( pMyObject->pMPFileName )
                            {
                                AnscFreeMemory(pMyObject->pMPFileName);
                            }

                            pMyObject->pMPFieldName = pFieldName;
                            pMyObject->pMPFileName  = pFileName;
                            pMyObject->bMPFileData  = bFileData;
                        }

                        if ( !pMyObject->bMPFileData )
                        {
                            if ( pMyObject->hLastMPBdo )
                            {
                                status =
                                    HttpMboSaveNonFileData
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pLastBdo,
                                            FALSE,
                                            0,
                                            FALSE
                                        );
                                AnscFreeBdo(pMyObject->hLastMPBdo);
                                pMyObject->hLastMPBdo = NULL;
                            }

                            status =
                                HttpMboSaveNonFileData
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pBdo,
                                        bEndFound,
                                        ulEndOffset,
                                        !bEndFound
                                    );
                        }
                        else
                        {
                            ULONG   ulDataOffset = ulHdrSize;

                            if ( pMyObject->hLastMPBdo )
                            {
                                ulDataOffset -= AnscBdoGetBlockSize(pLastBdo) - AnscBdoGetOffset(pLastBdo);
                                AnscFreeBdo(pMyObject->hLastMPBdo);
                                pMyObject->hLastMPBdo = NULL;
                            }

                            ulDataOffset += AnscBdoGetOffset(pBdo);
                            AnscBdoSetOffset(pBdo, ulDataOffset);

                            status =
                                HttpMboSaveFileData
                                    (
                                        hThisObject,
                                        ulPredictedBodySize,
                                        (ANSC_HANDLE)pBdo,
                                        bEndFound,
                                        ulEndOffset,
                                        !bEndFound
                                    );
                        }

                        if ( !bEndFound )
                        {
                            if ( AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo) != 0 )
                            {
                                if ( AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo) > HTTP_MP_DATA_BACKCHECK_SIZE )
                                {
                                    AnscBdoSetOffset(pBdo, AnscBdoGetBlockSize(pBdo) - HTTP_MP_DATA_BACKCHECK_SIZE);
                                }
                                pMyObject->hLastMPBdo = (ANSC_HANDLE)pBdo;
                            }
                            else
                            {
                                AnscFreeMemory(pBdo);
                                pBdo = NULL;
                                pMyObject->hLastMPBdo = (ANSC_HANDLE)NULL;
                            }

                            pMyObject->MPState = HTTP_MBO_MP_STATE_PART_DATA;
                        }
                        else if ( bEndFound && ulEndOffset < AnscBdoGetBlockSize(pBdo) )
                        {
                            AnscBdoSetOffset(pBdo, ulEndOffset);
                            AnscQueuePushEntry(&BdoQueue, &pBdo->Linkage);
                        }
                        else
                        {
                            AnscFreeMemory(pBdo);
                            pBdo = NULL;
                        }

                        if ( status != ANSC_STATUS_SUCCESS )
                        {
                            goto EXIT;
                        }
                    }

                    break;

            case    HTTP_MBO_MP_STATE_PART_DATA:

                    bFoundInLastBdo = FALSE;

                    if ( pMyObject->hLastMPBdo )
                    {
                        bEndFound = HttpMboFindNextBoundary2(hThisObject, pMyObject->hLastMPBdo, hBdo, &bFoundInLastBdo, &ulEndOffset);
                    }
                    else
                    {
                        bEndFound = HttpMboFindNextBoundary(hThisObject, (ANSC_HANDLE)pBdo, 0, &ulEndOffset);
                    }

                    if ( bEndFound )
                    {
                        if ( bFoundInLastBdo )
                        {
                            /* we found the next boundary in last BDO */
                            if ( !pMyObject->bMPFileData )
                            {
                                status =
                                    HttpMboSaveNonFileData
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pLastBdo,
                                            TRUE,
                                            ulEndOffset,
                                            FALSE
                                        );
                            }
                            else
                            {
                                status =
                                    HttpMboSaveFileData
                                        (
                                            hThisObject,
                                            ulPredictedBodySize,
                                            (ANSC_HANDLE)pLastBdo,
                                            TRUE,
                                            ulEndOffset,
                                            FALSE
                                        );
                            }

                            AnscQueuePushEntry(&BdoQueue, &pBdo->Linkage);
                            AnscBdoSetOffset(pLastBdo, ulEndOffset);

                            if ( status != ANSC_STATUS_SUCCESS )
                            {
                                goto EXIT;
                            }
                        }
                        else
                        {
                            /* we found the next boundary in current BDO */
                            if ( !pMyObject->bMPFileData )
                            {
                                if ( pMyObject->hLastMPBdo )
                                {
                                    status =
                                        HttpMboSaveNonFileData
                                            (
                                                hThisObject,
                                                pMyObject->hLastMPBdo,
                                                FALSE,
                                                0,
                                                FALSE
                                            );
                                }

                                status =
                                    HttpMboSaveNonFileData
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pBdo,
                                            bEndFound,
                                            ulEndOffset,
                                            !bEndFound
                                        );
                            }
                            else
                            {
                                if ( pMyObject->hLastMPBdo )
                                {
                                    status =
                                        HttpMboSaveFileData
                                            (
                                                hThisObject,
                                                ulPredictedBodySize,
                                                pMyObject->hLastMPBdo,
                                                FALSE,
                                                0,
                                                FALSE
                                            );

                                }
                                status =
                                    HttpMboSaveFileData
                                        (
                                            hThisObject,
                                            ulPredictedBodySize,
                                            (ANSC_HANDLE)pBdo,
                                            bEndFound,
                                            ulEndOffset,
                                            !bEndFound
                                        );
                            }

                            if ( pLastBdo )
                            {
                                AnscFreeBdo(pLastBdo);
                                pMyObject->hLastMPBdo = NULL;
                            }

                            AnscBdoSetOffset(pBdo, ulEndOffset);
                            AnscQueuePushEntry(&BdoQueue, &pBdo->Linkage);

                            if ( status != ANSC_STATUS_SUCCESS )
                            {
                                goto EXIT;
                            }

                            pMyObject->MPState      = HTTP_MBO_MP_STATE_INIT;
                        }

                        if ( pMyObject->pMPFieldName )
                        {
                            AnscFreeMemory(pMyObject->pMPFieldName);
                            pMyObject->pMPFieldName = NULL;
                        }
                    }
                    else
                    {
                        /* we cannot find boundary */
                        if ( !pMyObject->bMPFileData )
                        {
                            if ( pMyObject->hLastMPBdo )
                            {
                                status =
                                    HttpMboSaveNonFileData
                                        (
                                            hThisObject,
                                            pMyObject->hLastMPBdo,
                                            FALSE,
                                            0,
                                            FALSE
                                        );
                                AnscFreeBdo(pMyObject->hLastMPBdo);
                                pMyObject->hLastMPBdo = NULL;
                            }

                            status =
                                HttpMboSaveNonFileData
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pBdo,
                                        bEndFound,
                                        ulEndOffset,
                                        !bEndFound
                                    );
                        }
                        else
                        {
                            if ( pMyObject->hLastMPBdo )
                            {
                                status =
                                    HttpMboSaveFileData
                                        (
                                            hThisObject,
                                            ulPredictedBodySize,
                                            pMyObject->hLastMPBdo,
                                            FALSE,
                                            0,
                                            FALSE
                                        );
                                AnscFreeBdo(pMyObject->hLastMPBdo);
                                pMyObject->hLastMPBdo = NULL;
                            }

                            status =
                                HttpMboSaveFileData
                                    (
                                        hThisObject,
                                        ulPredictedBodySize,
                                        (ANSC_HANDLE)pBdo,
                                        bEndFound,
                                        ulEndOffset,
                                        !bEndFound
                                    );
                        }

                        if ( AnscBdoGetBlockSize(pBdo) - AnscBdoGetOffset(pBdo) > HTTP_MP_DATA_BACKCHECK_SIZE )
                        {
                            AnscBdoSetOffset(pBdo, AnscBdoGetBlockSize(pBdo) - HTTP_MP_DATA_BACKCHECK_SIZE);
                        }
                        pMyObject->hLastMPBdo = (ANSC_HANDLE)pBdo;

                        if ( status != ANSC_STATUS_SUCCESS )
                        {
                            goto EXIT;
                        }
                    }

                    break;
        }
    }

    if ( pMyObject->hLastMPBdo && ulPredictedBodySize == pMyObject->ArrivedSize )
    {
        pLastBdo = (PANSC_BUFFER_DESCRIPTOR  )pMyObject->hLastMPBdo;

        AnscFreeBdo(pLastBdo);
        pMyObject->hLastMPBdo = NULL;

        pMyObject->Mode = HTTP_MBO_MODE_STORE;
    }

EXIT:

    if ( status != ANSC_STATUS_SUCCESS )
    {
        pLastBdo = (PANSC_BUFFER_DESCRIPTOR  )pMyObject->hLastMPBdo;
        if ( pLastBdo )
        {
            AnscFreeBdo(pLastBdo);
            pMyObject->hLastMPBdo = NULL;
        }

        while ( TRUE )
        {
            pSLinkEntry = AnscQueuePopEntry(&BdoQueue);
            pBdo = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);

            if ( pBdo )
            {
                AnscFreeBdo(pBdo);
            }
            else
            {
                break;
            }
     	}
    }
    /*CID: 137764 Resource leak*/
    if(pFileName)
    {
       AnscFreeMemory(pFileName);
    }

    return status;
}

