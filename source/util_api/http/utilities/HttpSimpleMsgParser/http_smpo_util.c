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

    module:	http_smpo_util.c

        For HTTP Simple message parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module provides some pure interfaces which are 
        called to parse a specific HTTP header or construct
        a specific header. Also there're APIs for request
        line and status line.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

  ---------------------------------------------------------------

    last modified:

        02/27/02

**********************************************************************/

#include "http_smpo_global.h"


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoTokenIs
            (
                PUCHAR                      tokenName,
                PUCHAR                      httpToken
            )

    description:

        This function is called to determine if the given token is
        the specified HTTP token.

    argument:   PUCHAR                      tokenName
                The token name to be examined.

                PUCHAR                      httpToken
                HTTP token name to be compared with.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoTokenIs
    (
        PUCHAR                      tokenName,
        PUCHAR                      httpToken
    )
{
    return (
               tokenName                                                  &&               
               httpToken                                                  &&               
               AnscSizeOfString((char *)tokenName) == AnscSizeOfString((char *)httpToken) &&                                                             \
               strcmp((char *)tokenName, (char *)httpToken) == 0    
           );
}


/**********************************************************************

    caller:     component objects

    prototype:

        int
        HttpSmpoMemFindChar
            (
                PUCHAR                      pBuf,
                ULONG                       ulBufLen,
                UCHAR                       aChar,
                BOOL                        bCaseSensitive
            )

    description:

        This function is called to find the specified character in
        the given buffer.

    argument:   PUCHAR                      pBuf
                The buffer to be examined.

                ULONG                       ulBufLen
                The buffer length.

                UCHAR                       aChar
                The character to be searched.

                BOOL                        bCaseSensitive
                Character comparison option.
    
    return:     position of the character in buffer, -1 if no found.

**********************************************************************/

int
HttpSmpoMemFindChar
    (
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        UCHAR                       aChar,
        BOOL                        bCaseSensitive
    )
{
    int                             i;
    UCHAR                           ch;


    for (i = 0; i < (int)ulBufLen; i ++)
    {
        ch  = pBuf[i];

        if (bCaseSensitive)
        {
            if (ch >= 'a' && ch <= 'z')
                ch  = ch - 'a' + 'A';
        }

        if (ch == aChar)
            return i;
    }

    return -1;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoHexString2Ulong
            (
                PUCHAR                      pHexString
            )

    description:

        This function is called to convert a hex string to ULONG.

    argument:   PUCHAR                      pHexString
                Hex string to be converted.

    return:     the value.

**********************************************************************/

ULONG
HttpSmpoHexString2Ulong
    (
        PUCHAR                      pHexString
    )
{
    ULONG                           ulValue = 0;
    ULONG                           ulLen, i, val;
    UCHAR                           uc;

    if (pHexString)
    {
        ulLen   = AnscSizeOfString((const char *)pHexString);

        for (i = 0; i < ulLen; i ++)
        {
            uc      = pHexString[i];

            if (uc >= '0' && uc <= '9')
            {
                val = (ULONG)(uc - '0');
            }
            else
            if (uc >= 'a' && uc <= 'z')
            {
                val = (ULONG)(uc - 'a') + 10;
            }
            else
            if (uc >= 'A' && uc <= 'Z')
            {
                val = (ULONG)(uc - 'A') + 10;
            }
            else
            {
                break;
            }

            ulValue <<= 4;
            ulValue += val;
        }
    }

    return ulValue;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoDecodeUrlGetHex
            (
                UCHAR                       uc,
                PUCHAR                      pHex
            )

    description:

        This function is called to the given character into hex
        value.

    argument:   UCHAR                       uc
                The character to be converted.

                PUCHAR                      pHex
                The buffer for result.
    
    return:     status of operation.

**********************************************************************/

BOOL
HttpSmpoDecodeUrlGetHex
    (
        UCHAR                       uc, 
        PUCHAR                      pHex
    )
{
    UCHAR                           ucHex;

    if (uc >= '0' && uc <= '9')
        ucHex = uc - '0';
    else
        if (uc >= 'a' && uc <= 'f')
            ucHex = uc - 'a' + 10;
        else
            if (uc >= 'A' && uc <= 'F')
                ucHex = uc - 'A' + 10;
            else
            {
                return FALSE;
            }

    *pHex   = ucHex;

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoMultipartFindBoundary
            (
                PUCHAR                      pBuf,
                ULONG                       ulBufLen,
                PUCHAR                      pBoundary,
                PBOOL                       pbEndBoundary
            )

    description:

        This function is called to find boundary of the next
        multipart message.

    argument:   PUCHAR                      pBuf
                Buffer of multipart messages.

                ULONG                       ulBufLen
                The length of buffer.

                PUCHAR                      pBoundary
                Boundary string.

                PBOOL                       pbEndBoundary
                If end of boundary found.
    
    return:     the starting position of next boundary.

**********************************************************************/

static
PUCHAR
HttpSmpoMultipartFindBoundary
    (
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        PUCHAR                      pBoundary,
        PBOOL                       pbEndBoundary
    )
{
    PUCHAR                          pData   = pBuf;
    PUCHAR                          pLast   = pData + ulBufLen - 1;
    PUCHAR                          pStr    = pBuf;
    PUCHAR                          pFirstH = NULL;

    *pbEndBoundary  = FALSE;

    while (pData < pLast)
    {
        pStr    = _ansc_memchr(pData, HTTP_SMPO_CHAR_HYPHEN, pLast - pData);

        if (!pStr)
        {
            return NULL;
        }
        else
        {
            pFirstH = pStr;

            if (pStr < pLast && *(pStr + 1) == HTTP_SMPO_CHAR_HYPHEN)
            {
                pStr    += HTTP_SMPO_MULTIPART_BOUNDARY_DELI_LENGTH;

                if (AnscEqualString2((char *)pStr, (char *)pBoundary, AnscSizeOfString((const char *)pBoundary), TRUE))
                {
                    PUCHAR              pPostfix    = pStr + AnscSizeOfString((const char *)pBoundary);

                    if (AnscEqualMemory(pPostfix, HTTP_SMPO_MULTIPART_BOUNDARY_DELI, HTTP_SMPO_MULTIPART_BOUNDARY_DELI_LENGTH))
                    {
                        *pbEndBoundary  = TRUE;

                        return pStr;
                    }
                    else
                    {
                        *pbEndBoundary  = FALSE;

                        return pStr;
                    }
                }
            }

            pData   = pFirstH + 1;
        }
    }

    return NULL;
}


/**********************************************************************

    caller:     component objects

    prototype:

        PUCHAR
        HttpSmpoMultipartFiledName
            (
                PUCHAR                      pHeaders,
                ULONG                       ulHeaderLen,
                PULONG                      pulNameLen
            )

    description:

        This function is called to get field name of current 
        multipart message.

    argument:   PUCHAR                      pHeaders
                The buffer of headers of current multipart message.

                ULONG                       ulHeaderLen
                The length of buffer.

                PULONG                      pulNameLen
                The length of field name.
    
    return:     the field name.

**********************************************************************/

static
PUCHAR
HttpSmpoMultipartFieldName
    (
        PUCHAR                      pHeaders,
        ULONG                       ulHeaderLen,
        PULONG                      pulNameLen
    )
{
    UNREFERENCED_PARAMETER(ulHeaderLen);
    PUCHAR                          pStr        = NULL, pStrEnd = NULL;
    PUCHAR                          pLineEnd    = NULL;

    pStr    = (PUCHAR)_ansc_strstr((const char *)pHeaders, (const char *)HTTP_SMPO_CONTENT_DISPOSITION);

    if (!pStr)
    {
        return NULL;
    }
    else
    {
        pStr        += HTTP_SMPO_CONTENT_DISPOSITION_LENGTH;
        pLineEnd    = (PUCHAR)_ansc_strstr((const char *)pStr, (const char *)HTTP_SMPO_LINE_BREAKER);

        pStr        = (PUCHAR)_ansc_strstr((const char *)pStr, (const char *)HTTP_SMPO_MULTIPART_FIELD_NAME);

        if (pStr >= pLineEnd)
        {
            return NULL;
        }
        else
        {
            pStr    += HTTP_SMPO_MULTIPART_FIELD_NAME_LENGTH;

            while (pStr <= pLineEnd && *pStr != '"')
            {
                pStr ++;
            }

            if (pStr >= pLineEnd)
            {
                return NULL;
            }

            pStrEnd = ++pStr;

            while (pStrEnd <= pLineEnd && *pStrEnd != '"')
            {
                pStrEnd ++;
            }

            if (pStrEnd >= pLineEnd)
            {
                return NULL;
            }

            *pulNameLen = pStrEnd - pStr;

            return pStr;
        }
    }
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoAnalyzeMultipartContentType
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pHeaders,
                ULONG                       ulHeaderLen,
                PULONG                      pulContentType
            )

    description:

        This function is called to get content type of the message.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pHeaders
                Buffer to headers of this multipart message.

                ULONG                       ulHeaderLen
                The size of headers.

                PULONG                      pulContentType
                The content type.
    
    return:     status of operation.

**********************************************************************/

static
BOOL
AnscAnalyzeMultipartContentType
    (
        ANSC_HANDLE                 hHttpSmpo,
        PUCHAR                      pHeaders,
        ULONG                       ulHeaderLen,
        PULONG                      pulContentType
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pStr;
    ULONG                           ulContentType   = HTTP_FORM_DATA_TYPE_text;

    pStr    = (PUCHAR)_ansc_strstr((const char *)pHeaders, (const char *)HTTP_SMPO_CONTENT_TYPE);

    if (!pStr)
    {
        ulContentType   = HTTP_FORM_DATA_TYPE_text;
    }
    else
    {
        if (pStr >= pHeaders + ulHeaderLen)
        {
            ulContentType   = HTTP_FORM_DATA_TYPE_text;
        }
        else
        {
            ulContentType   = HTTP_FORM_DATA_TYPE_binary;
        }
    }

    if (pulContentType)
    {
        *pulContentType = ulContentType;
    }

    return TRUE;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ULONG
        HttpSmpoMultipartContentType
            (
                ANSC_HANDLE                 hHttpSmpo,
                ANSC_HANDLE                 hPart
            )

    description:

        This function is called to get content type of given part.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                ANSC_HANDLE                 hPart
                Handle to a multipart message.

    return:     the content type of given part.

**********************************************************************/

static
ULONG
HttpSmpoMultipartContentType
    (
        ANSC_HANDLE                 hHttpSmpo,
        ANSC_HANDLE                 hPart
    )
{
    PHTTP_SMPO_MULTIPART_PART       pPart       = (PHTTP_SMPO_MULTIPART_PART)hPart;
    PUCHAR                          pContent    = pPart->pValue;
    ULONG                           ulContentLen= pPart->ulValueLen;
    PUCHAR                          pZero;
    BOOL                            bSure       = FALSE;
    ULONG                           ContentType;

    bSure   = 
        AnscAnalyzeMultipartContentType
            (
                hHttpSmpo,
                pPart->pHeader,
                pPart->ulHeaderLen,
                &ContentType
            );

    if (!bSure)
    {
        pZero   = _ansc_memchr(pContent, 0, ulContentLen);

        if (!pZero)
        {
            ContentType = HTTP_FORM_DATA_TYPE_text;
        }
        else
        {
            ContentType = HTTP_FORM_DATA_TYPE_binary;
        }
    }

    return ContentType;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoGetMultipartPart
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pBuf,
                ULONG                       ulBufLen,
                PUCHAR                      pBoundary,
                ANSC_HANDLE                 hPart,
                PUCHAR                      *pNextPart
            )

    description:

        This function is called to get a multipart message.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pBuf
                Buffer of multipart messages.

                ULONG                       ulBufLen
                The length of buffer.

                PUCHAR                      pBoundary
                Boundary string.

                ANSC_HANDLE                 hPart
                The multipart part object.

                PUCHAR                      *pNextPart
                Starting position to the next part.
    
    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoGetMultipartPart
    (
        ANSC_HANDLE                 hHttpSmpo,
        PUCHAR                      pBuf,
        ULONG                       ulBufLen,
        PUCHAR                      pBoundary,
        ANSC_HANDLE                 hPart,
        PUCHAR                      *pNextPart
    )
{
    PHTTP_SMPO_MULTIPART_PART       pPart   = (PHTTP_SMPO_MULTIPART_PART)hPart;
    ANSC_STATUS                     status  = ANSC_STATUS_SUCCESS;
    PUCHAR                          pLast   = pBuf + ulBufLen - 1;
    PUCHAR                          pStr;
    BOOL                            bEndBoundary;

    *pNextPart  = NULL;

    /* find next boundary */
    pStr    = HttpSmpoMultipartFindBoundary(pBuf, ulBufLen, pBoundary, &bEndBoundary);

    if (bEndBoundary)
    {
        return ANSC_STATUS_FAILURE;
    }
    
    pStr    += HTTP_SMPO_MULTIPART_BOUNDARY_DELI_LENGTH + AnscSizeOfString((const char *)pBoundary);
    /*
    pStr    += HTTP_SMPO_LINE_BREAKER_LENGTH;
    */

    if (pStr > pLast)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* got start of headers of current part */
    pPart->pHeader  = pStr;

    /* find content of this part */
    pStr    = (PUCHAR)_ansc_strstr((const char *)pStr, HTTP_SMPO_DOUBLE_LINE_BREAKER);
    if (!pStr)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* got header length */
    pPart->ulHeaderLen  = pStr - pPart->pHeader + HTTP_SMPO_LINE_BREAKER_LENGTH;

    pStr    += HTTP_SMPO_DOUBLE_LINE_BREAKER_LENGTH;

    pPart->pValue   = pStr;

    /* find end of content of this part */
    pStr    = HttpSmpoMultipartFindBoundary(pStr, pLast - pStr + 1, pBoundary, &bEndBoundary);

    if (!pStr)
    {
        return ANSC_STATUS_FAILURE;
    }

    pStr -= HTTP_SMPO_MULTIPART_BOUNDARY_DELI_LENGTH;

    *pNextPart  = pStr;

    pStr -= HTTP_SMPO_LINE_BREAKER_LENGTH;

    /* got content length */
    pPart->ulValueLen   = pStr - pPart->pValue;

    /* got form field name */
    pPart->pName = HttpSmpoMultipartFieldName(pPart->pHeader, pPart->ulHeaderLen, &pPart->ulNameLen);

    if (!pPart->pName || pPart->ulNameLen == 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    pPart->ulType = HttpSmpoMultipartContentType(hHttpSmpo, (ANSC_HANDLE)pPart);

    return status;
}


ANSC_STATUS
HttpSmpoLocateFieldData
    (
        ANSC_HANDLE                 hHttpMbo, 
        char*                       param_name,
        char*                       pBoundary,
        ANSC_HANDLE                 *phStartField,
        PULONG                      pulFieldOffset,
        ANSC_HANDLE                 *phStartBdo,
        PULONG                      pulStartOffset,
        ANSC_HANDLE                 *phEndBdo,
        PULONG                      pulEndOffset
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo        = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;
    PANSC_BUFFER_DESCRIPTOR         pStartBdo, pEndBdo;
    ULONG                           ulOffset1, ulOffset;
    PUCHAR                          pBuf            = NULL;
    BOOL                            bFound;
    ANSC_HANDLE                     hBdo;
    ULONG                           ulDataOffset    = 0;

    pStartBdo   = pEndBdo   = NULL;
    ulOffset1   = 0;

    AnscAcquireLock(&pHttpMbo->BdoQueueLock);

    while (TRUE)
    {
        bFound  = 
            HttpSmpoFindNextBoundary
                (
                    hHttpMbo, 
                    (ANSC_HANDLE)pStartBdo, 
                    ulOffset1, 
                    pBoundary, 
                    &hBdo, 
                    &ulOffset
                );

        if (!bFound)
        {
            status  = ANSC_STATUS_CANT_FIND;
            break;
        }
        else
        {
            if (phStartField)
            {
                *phStartField   = (ANSC_HANDLE)hBdo;
            }

            if (pulFieldOffset)
            {
                *pulFieldOffset = ulOffset;
            }

            bFound  = 
                HttpSmpoGetFormField
                    (
                        hHttpMbo,
                        param_name,
                        hBdo,
                        ulOffset,
                        pBoundary,
                        &hBdo,
                        &ulDataOffset
                    );

            if (!bFound)
            {
                pStartBdo   = (PANSC_BUFFER_DESCRIPTOR)hBdo;
                ulOffset1   = ulOffset + 1;
            }
            else
            {
                status      = ANSC_STATUS_SUCCESS;

                if (phStartBdo)
                {
                    *phStartBdo = (ANSC_HANDLE)hBdo;
                }

                if (pulStartOffset)
                {
                    *pulStartOffset = ulDataOffset;
                }

                bFound  = 
                    HttpSmpoFindNextBoundary
                        (
                            hHttpMbo, 
                            (ANSC_HANDLE)hBdo, 
                            ulOffset + 1, 
                            pBoundary, 
                            &hBdo, 
                            &ulOffset
                        );

                if (bFound)
                {
                    if (phEndBdo)
                    {
                        *phEndBdo   = hBdo;
                    }

                    if (pulEndOffset)
                    {
                        *pulEndOffset   = ulOffset;
                    }
                }
                else
                {
                    if (phEndBdo)
                    {
                        *phEndBdo       = (ANSC_HANDLE)NULL;
                    }

                    if (pulEndOffset)
                    {
                        *pulEndOffset   = 0;
                    }
                }

                break;
            }
        }
    }

    AnscReleaseLock(&pHttpMbo->BdoQueueLock);
    
    if (pBuf)
    {
        AnscFreeMemory(pBuf);
    }

    return status;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoSaveFieldData
            (
                ANSC_HANDLE                 hHttpMbo,
                char*                       file_name,
                BOOL                        bFlushBody,
                ANSC_HANDLE                 hStartField,
                ULONG                       ulFieldOffset,
                ANSC_HANDLE                 hStartBdo,
                ULONG                       ulStartOffset,
                ANSC_HANDLE                 hEndBdo,
                ULONG                       ulEndOffset
            )

    description:

        This function is called to save uploaded file into the
        given "disk" file.

    argument:   ANSC_HANDLE                 hHttpMbo
                The handle to Simple HTTP message parser object.

                char*                       file_name
                The file name to save the content to.

                BOOL                        bFlushBody
                Indicate if BDOs that contains the uploaded file
                should be removed from the chain.

                ANSC_HANDLE                 hStartField
                The starting field containing uploaded file content.

                ULONG                       ulFieldOffset
                Where in starting BDO the content begins.

                ANSC_HANDLE                 hStartBdo
                The starting BDO containing uploaded file content.

                ULONG                       ulStartOffset
                The offset in starting BDO where content begins.

                ANSC_HANDLE                 hEndBdo
                The last BDO where file content ends.

                ULONG                       ulEndOffset
                The last byte of content in last BDO.
    
    return:     operation of status.

**********************************************************************/

ANSC_STATUS
HttpSmpoSaveFieldData
    (
        ANSC_HANDLE                 hHttpMbo,
        char*                       file_name,
        BOOL                        bFlushBody,
        ANSC_HANDLE                 hStartField,
        ULONG                       ulFieldOffset,
        ANSC_HANDLE                 hStartBdo,
        ULONG                       ulStartOffset,
        ANSC_HANDLE                 hEndBdo,
        ULONG                       ulEndOffset
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo        = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo;
    PANSC_BUFFER_DESCRIPTOR         pStartBdo       = (PANSC_BUFFER_DESCRIPTOR)hStartBdo;
    PANSC_BUFFER_DESCRIPTOR         pEndBdo         = (PANSC_BUFFER_DESCRIPTOR)hEndBdo;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulBlockSize     = 0;
    ANSC_HANDLE                     hTargetFile     = (ANSC_HANDLE              )NULL;
    ULONG                           ulOffset;
    PUCHAR                          pData;
    ULONG                           ulDataLen;
    BOOL                            bRemove         = FALSE;

    hTargetFile =
        AnscOpenFile
            (
                file_name,
                ANSC_FILE_MODE_WRITE | ANSC_FILE_MODE_CREATE | ANSC_FILE_MODE_TRUNC,
                ANSC_FILE_TYPE_WRITE
            );

    if ( !hTargetFile )
    {
        return ANSC_STATUS_FAILURE;
    }

    AnscAcquireLock(&pHttpMbo->BdoQueueLock);

    pSLinkEntry     = &pStartBdo->Linkage;

    while (pSLinkEntry)
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        ulDataLen   = ulBlockSize = AnscBdoGetBlockSize(pBodyBdo);
        pData       = (PUCHAR)AnscBdoGetBlock(pBodyBdo);

        if ((ANSC_HANDLE)pBodyBdo == hStartBdo)
        {
            pData       += ulStartOffset;
            if (ulDataLen >= ulStartOffset)
            {
                ulDataLen   -= ulStartOffset;
            }
            else
            {
                ulDataLen   = 0;
            }
        }

        if ((ANSC_HANDLE)pBodyBdo == hEndBdo)
        {
            if (ulDataLen >= (ulBlockSize - ulEndOffset))
            {
                ulDataLen   -= (ulBlockSize - ulEndOffset);
                ulDataLen   -= 2;   /* CRLF at the end */
            }
        }

        if (ulDataLen > 0 && ulDataLen <= ulBlockSize)
        {
            status =
                AnscWriteFile
                    (
                        hTargetFile,
                        pData,
                        &ulDataLen
                    );
        }

        /* we need to remove BDOs immediately after we save the content into file */
        if (bFlushBody)
        {
            ulBlockSize = AnscBdoGetBlockSize(pBodyBdo);
            ulOffset    = AnscBdoGetOffset(pBodyBdo);

            /* modify the first and the end BDOs, and pop out all BDOs in between */
            if ((ANSC_HANDLE)pBodyBdo == hStartField && pBodyBdo == pEndBdo)
            {
                bRemove = TRUE;

                if (ulFieldOffset == 0)
                {
                    AnscBdoSetOffset(pBodyBdo, ulOffset + ulEndOffset);
                    AnscBdoSetBlockSize(pBodyBdo, ulBlockSize - ulEndOffset);
                }
                else
                {
                    PANSC_BUFFER_DESCRIPTOR     pBufferDesp = NULL;
                    ULONG                       ulSize;
                    PUCHAR                      pData;

                    AnscBdoSetBlockSize(pBodyBdo, ulFieldOffset);

                    ulSize  = ulBlockSize - ulEndOffset;
                    if (ulSize != 0 && ulBlockSize > ulEndOffset)
                    {
                        /* create a new one */
                        pBufferDesp = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(ulSize, 0, 0);
                        if (pBufferDesp)
                        {
                            AnscBdoSetBlockSize(pBufferDesp, ulSize);
        
                            pData   = (PUCHAR)AnscBdoGetBuffer(pBufferDesp);
                            AnscCopyMemory
                                (
                                    pData, 
                                    ((PUCHAR)AnscBdoGetBuffer(pBodyBdo)) + ulOffset + ulEndOffset, 
                                    ulSize
                                );

                            if (pBufferDesp)
                            {
                                AnscQueuePushEntryAfter
                                    (
                                        &pHttpMbo->BdoQueue, 
                                        &pBufferDesp->Linkage,
                                        &pBodyBdo->Linkage
                                    );
                            }
                        }
                    }
                }

                /* the file content is in one BDO */
                break;
            }
            else if ((ANSC_HANDLE)pBodyBdo == hStartField)
            {
                bRemove = TRUE;

                if (ulFieldOffset == 0)
                {
                    AnscQueuePopEntryByLink(&pHttpMbo->BdoQueue, &pBodyBdo->Linkage);
                    AnscFreeBdo(pBodyBdo);
                }
                else
                {
                    AnscBdoSetBlockSize(pBodyBdo, ulFieldOffset);
                }
            } 
            else if (pBodyBdo == pEndBdo)
            {
                if (ulOffset + ulEndOffset >= ulBlockSize)
                {
                    AnscQueuePopEntryByLink(&pHttpMbo->BdoQueue, &pBodyBdo->Linkage);
                    AnscFreeBdo(pBodyBdo);
                }
                else
                {
                    AnscBdoSetOffset(pBodyBdo, ulOffset + ulEndOffset);
                    AnscBdoSetBlockSize(pBodyBdo, ulBlockSize - ulEndOffset);
                }

                /* exit when last chunk has been written into file */
                break;
            }
            else if (bRemove)
            {
                /* remove BDOs in middle of BDO chain */
                AnscQueuePopEntryByLink(&pHttpMbo->BdoQueue, &pBodyBdo->Linkage);
                AnscFreeBdo(pBodyBdo);
            }
        }
    }

    if ( hTargetFile )
    {
        AnscCloseFile(hTargetFile);
    }

    AnscReleaseLock(&pHttpMbo->BdoQueueLock);

    return status;
}


BOOL
HttpSmpoFindNextBoundary
    (
        ANSC_HANDLE                 hHttpMbo,
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        char*                       pBoundary,
        ANSC_HANDLE                 *phBdo,
        PULONG                      pulOffset
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo        = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;
    ULONG                           ulBoundaryLen   = AnscSizeOfString(pBoundary);
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo        = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulBlockSize     = 0;
    PANSC_BUFFER_DESCRIPTOR         pStartBdo;
    ULONG                           ulTotalSize     = 0;
    PUCHAR                          pPos;
    PUCHAR                          pBuf            = NULL;
    ULONG                           ulBufSize       = 0;
    BOOL                            bFound          = FALSE;
    ULONG                           ulSize          = 0;
    PANSC_BUFFER_DESCRIPTOR         pBdo            = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    ULONG                           ulOrgTotalSize  = 0;
    ULONG                           ulTotalOffset   = 0;

    pStartBdo   = (PANSC_BUFFER_DESCRIPTOR)hBdo;

    pSLinkEntry = AnscQueueGetFirstEntry(&pHttpMbo->BdoQueue);

    if (pBdo)
    {
        pSLinkEntry = &pBdo->Linkage;
    }

    ulBufSize   = HTTP_SMPO_BODY_MSG_MAX_SIZE;
    pBuf        = (PUCHAR)AnscAllocateMemory(ulBufSize);

    if (!pBuf)
    {
        return FALSE;
    }

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        ulBlockSize = AnscBdoGetBlockSize(pBodyBdo);

        if (!pStartBdo)
        {
            pStartBdo   = pBodyBdo;
        }

        if ((ANSC_HANDLE)pBodyBdo == hBdo && ulOffset != 0)
        {
            if (ulBlockSize > ulOffset)
            {
                pBuf    = 
                    HttpSmpoCopyFormData
                        (
                            pBuf, 
                            &ulBufSize, 
                            &ulTotalSize, 
                            ((PUCHAR)AnscBdoGetBuffer(pBodyBdo)) + AnscBdoGetOffset(pBodyBdo) + ulOffset,
                            ulBlockSize - ulOffset
                        );
            }
        }
        else
        {
            pBuf    = 
                HttpSmpoCopyFormData
                    (
                        pBuf, 
                        &ulBufSize, 
                        &ulTotalSize, 
                        ((PUCHAR)AnscBdoGetBuffer(pBodyBdo)) + AnscBdoGetOffset(pBodyBdo),
                        ulBlockSize
                    );
        }

        if (ulTotalSize >= ulBoundaryLen + 4)
        {
            /* try to find boundary string */
            pPos    = 
                HttpSmpoMultipartFindBoundary
                    (
                        pBuf,
                        ulTotalSize,
                        (PUCHAR)pBoundary,
                        &bFound
                    );  

            if (!pPos)
            {
                ulOrgTotalSize  = ulTotalSize;

                /* try to shift starting BDO */
                while (pStartBdo != pBodyBdo)
                {
                    ulSize  = AnscBdoGetBlockSize(pStartBdo);

                    if ((ANSC_HANDLE)pStartBdo == hBdo)
                    {
                        if (ulSize > ulOffset)
                        {
                            ulSize  -= ulOffset;
                        }
                        else
                        {
                            ulSize  = 0;
                        }
                    }

                    if (ulTotalSize - ulSize >= ulBoundaryLen + 4)
                    {
                        ulTotalSize -= ulSize;
                        pStartBdo   = ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetNextEntry(&pStartBdo->Linkage));
                    }
                    else
                    {
                        break;
                    }
                }

                if (ulTotalSize != ulOrgTotalSize)
                {
                    ulSize  = ulOrgTotalSize - ulTotalSize;

                    _ansc_memmove(pBuf, pBuf + ulSize, ulTotalSize);
                }
            }
            else
            {
                bFound          = TRUE;
                ulTotalOffset   = pPos - pBuf - 2;

                while (TRUE)
                {
                    ulSize  = AnscBdoGetBlockSize(pStartBdo);

                    if ((ANSC_HANDLE)pStartBdo == hBdo)
                    {
                        if (ulSize > ulOffset)
                        {
                            ulSize  -= ulOffset;
                        }
                        else
                        {
                            ulSize  = 0;
                        }
                    }

                    if (ulTotalOffset >= ulSize)
                    {
                        ulTotalOffset   -= ulSize;
                        pStartBdo   = ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetNextEntry(&pStartBdo->Linkage));

                        if (ulTotalOffset == 0)
                        {
                            *phBdo      = (ANSC_HANDLE)pStartBdo;
                            *pulOffset  = 0;
                            break;
                        }
                    }
                    else
                    {
                        if ((ANSC_HANDLE)pStartBdo == hBdo)
                        {
                            ulTotalOffset   += ulOffset;
                        }

                        if (pulOffset)
                        {
                            *pulOffset  = ulTotalOffset;
                        }

                        if (phBdo)
                        {
                            *phBdo      = (ANSC_HANDLE)pStartBdo;
                        }

                        break;
                    }

                    if (pStartBdo == pBodyBdo)
                    {
                        if (ulTotalOffset != 0)
                        {
                            if (pulOffset)
                            {
                                *pulOffset  = ulTotalOffset;
                            }

                            if (phBdo)
                            {
                                *phBdo      = (ANSC_HANDLE)pBodyBdo;
                            }
                        }
                        else
                        {
                            /* must be the next BDO with offset 0 */
                            if (pulOffset)
                            {
                                *pulOffset  = 0;
                            }

                            if (phBdo)
                            {
                                *phBdo      = 
                                    (ANSC_HANDLE)ACCESS_ANSC_BUFFER_DESCRIPTOR
                                        (
                                            AnscQueueGetNextEntry(&pStartBdo->Linkage)
                                        );
                            }
                        }

                        break;
                    }
                }

                break;
            }
        }
    }

    if (pBuf)
    {
        AnscFreeMemory(pBuf);
    }

    return bFound;
}


BOOL
HttpSmpoGetFormField
    (
        ANSC_HANDLE                 hHttpMbo,
        char*                       pFieldName,
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        char*                       pBoundary,
        ANSC_HANDLE                 *phBdo,
        PULONG                      pulOffset
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo        = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;
    ULONG                           ulBoundaryLen   = AnscSizeOfString(pBoundary);
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo        = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulBlockSize     = 0;
    PANSC_BUFFER_DESCRIPTOR         pStartBdo;
    ULONG                           ulTotalSize     = 0;
    PUCHAR                          pPos;
    PUCHAR                          pBuf            = NULL;
    ULONG                           ulBufSize       = 0;
    BOOL                            bFound          = FALSE;
    ULONG                           ulSize          = 0;
    PANSC_BUFFER_DESCRIPTOR         pBdo            = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    ULONG                           ulTotalOffset   = 0;
    PUCHAR                          pHeaders        = NULL;
    BOOL                            bHeadersFound   = FALSE;
    ULONG                           ulHeadersLen    = 0;
    PUCHAR                          pName;
    ULONG                           ulNameLen;

    pStartBdo   = (PANSC_BUFFER_DESCRIPTOR)hBdo;

    pSLinkEntry = AnscQueueGetFirstEntry(&pHttpMbo->BdoQueue);

    if (pBdo)
    {
        pSLinkEntry = &pBdo->Linkage;
    }

    ulBufSize   = HTTP_SMPO_BODY_MSG_MAX_SIZE;
    pBuf        = (PUCHAR)AnscAllocateMemory(ulBufSize);

    if (!pBuf)
    {
        return FALSE;
    }

    while ( pSLinkEntry && !bFound )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        ulBlockSize = AnscBdoGetBlockSize(pBodyBdo);

        if (!pStartBdo)
        {
            pStartBdo   = pBodyBdo;
        }

        if ((ANSC_HANDLE)pBodyBdo == hBdo && ulOffset != 0)
        {
            pBuf    = 
                HttpSmpoCopyFormData
                    (
                        pBuf, 
                        &ulBufSize, 
                        &ulTotalSize, 
                        ((PUCHAR)AnscBdoGetBuffer(pBodyBdo)) + AnscBdoGetOffset(pBodyBdo) + ulOffset,
                        ulBlockSize - ulOffset
                    );
        }
        else
        {
            pBuf    = 
                HttpSmpoCopyFormData
                    (
                        pBuf, 
                        &ulBufSize, 
                        &ulTotalSize, 
                        ((PUCHAR)AnscBdoGetBuffer(pBodyBdo)) + AnscBdoGetOffset(pBodyBdo),
                        ulBlockSize
                    );
        }

        if (ulTotalSize - ulOffset > ulBoundaryLen + 4)
        {
            ULONG                   ulStart = 0;

            bHeadersFound   = FALSE;

            /* find the end of headers */
            while (TRUE)
            {
                pPos    = _ansc_memchr(pBuf + ulStart, HTTP_SMPO_CHAR_CR, ulTotalSize - ulStart);
                if (pPos)
                {
                    if (pPos - pBuf >= 4)
                    {
                        bHeadersFound   = 
                            (
                                *(pPos + 1) == HTTP_SMPO_CHAR_LF    &&
                                *(pPos + 2) == HTTP_SMPO_CHAR_CR    &&
                                *(pPos + 3) == HTTP_SMPO_CHAR_LF
                            );
                    }
                }

                if (bHeadersFound)
                {
                    break;
                }
                else
                {
                    if (!pPos)
                    {
                        break;
                    }
                    else
                    {
                        ulStart = pPos - pBuf + 1;
                    }
                }
            }

            if (bHeadersFound)
            {
                pHeaders        = pBuf + ulBoundaryLen + 4;
                ulHeadersLen    = pPos + 4 - pHeaders;

                pName           = HttpSmpoMultipartFieldName(pHeaders, ulHeadersLen, &ulNameLen);
                if (!pName)
                {
                    bFound  = FALSE;

                    break;
                }

                if (ulNameLen == AnscSizeOfString(pFieldName) && 
                    AnscEqualString2(pFieldName, (char *)pName, ulNameLen, FALSE))
                {
                    bFound  = TRUE;
                }
                else
                {
                    break;
                }

                ulTotalOffset   = pPos + 4 - pBuf;

                while (TRUE)
                {
                    ulSize  = AnscBdoGetBlockSize(pStartBdo);

                    if ((ANSC_HANDLE)pStartBdo == hBdo)
                    {
                        ulSize  -= ulOffset;
                    }

                    if (ulTotalOffset > ulSize && ulTotalOffset >= ulBoundaryLen + 4)
                    {
                        ulTotalOffset   -= ulSize;
                        pStartBdo       = ACCESS_ANSC_BUFFER_DESCRIPTOR(AnscQueueGetNextEntry(&pStartBdo->Linkage));
                    }
                    else
                    {
                        if ((ANSC_HANDLE)pStartBdo == hBdo)
                        {
                            ulTotalOffset   += ulOffset;
                        }

                        if (pulOffset)
                        {
                            *pulOffset  = ulTotalOffset;
                        }

                        if (phBdo)
                        {
                            *phBdo      = (ANSC_HANDLE)pStartBdo;
                        }

                        break;
                    }

                    if (pStartBdo == pBodyBdo)
                    {
                        /* must be the next BDO with offset 0 */
                        if (pulOffset)
                        {
                            *pulOffset  = 0;
                        }

                        if (phBdo)
                        {
                            *phBdo      = 
                                (ANSC_HANDLE)ACCESS_ANSC_BUFFER_DESCRIPTOR
                                    (
                                        AnscQueueGetNextEntry(&pStartBdo->Linkage)
                                    );
                        }

                        break;
                    }
                }
            }
        }
    }

    if (pBuf)
    {
        AnscFreeMemory(pBuf);
    }

    return bFound;
}


PUCHAR
HttpSmpoCopyFormData
    (
        PUCHAR                      pBuf,
        PULONG                      pulBufSize,
        PULONG                      pulDataLen,
        PUCHAR                      pData,
        ULONG                       ulDataLen
    )
{
    PUCHAR                          pBuffer         = pBuf;
    ULONG                           ulBufSize       = *pulBufSize;
    ULONG                           ulBufDataLen    = *pulDataLen;

    if (ulBufDataLen + ulDataLen > ulBufSize)
    {
        ulBufSize   = ulBufDataLen + ulDataLen;
        pBuffer     = (PUCHAR)AnscAllocateMemory(ulBufSize);

        if (!pBuffer)
        {
            return pBuffer;
        }

        *pulBufSize = ulBufSize;
        AnscCopyMemory(pBuffer, pBuf, ulBufDataLen);
        AnscFreeMemory(pBuf);
    }

    AnscCopyMemory(pBuffer + ulBufDataLen, pData, ulDataLen);
    ulBufDataLen    += ulDataLen;
    *pulDataLen     = ulBufDataLen;

    return pBuffer;
}



ANSC_STATUS
HttpSmpoCopyFormData2
    (
        ANSC_HANDLE                 hHttpMbo,
        PUCHAR*                     ppBuf,
        PULONG                      pulBufSize,
        PULONG                      pulDataLen,
        char*                       pBoundary,
        ANSC_HANDLE                 hStartBdo,
        ULONG                       ulStartOffset,
        ANSC_HANDLE                 hEndBdo,
        ULONG                       ulEndOffset
    )
{
    UNREFERENCED_PARAMETER(hHttpMbo);
    UNREFERENCED_PARAMETER(pBoundary);
    /* This function only copies non-file form data into the given buffer from ulStartOffset
     * in hStartBdo to (ulEndOffset-1) in hEndBdo. If hEndBdo is NULL, copies the rest of
     * the data from the position in hStartBdo.
     */
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PUCHAR                          pBuf            = *ppBuf;
    ULONG                           ulBufSize       = *pulBufSize;
    ULONG                           ulDataLen       = *pulDataLen;
    PANSC_BUFFER_DESCRIPTOR         pStartBdo       = (PANSC_BUFFER_DESCRIPTOR)hStartBdo;
    PANSC_BUFFER_DESCRIPTOR         pEndBdo         = (PANSC_BUFFER_DESCRIPTOR)hEndBdo;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulBlockSize     = 0;
    PANSC_BUFFER_DESCRIPTOR         pBdo;
    ULONG                           ulPartSize      = 0;
    ULONG                           ulNewSize       = 0;
    PUCHAR                          pNewBuf         = NULL;
    
    pSLinkEntry = &pStartBdo->Linkage;    
    while (pSLinkEntry)
    {
        pBdo = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        ulBlockSize = AnscBdoGetBlockSize(pBdo);

        if (pBdo == pStartBdo)
        {
            if (pBdo == pEndBdo)
            {
                ulPartSize += (ulEndOffset - ulStartOffset);
            }
            else
            {
                ulPartSize += (ulBlockSize - ulStartOffset);
            }
        }
        else if (pBdo == pEndBdo)
        {
            ulPartSize += ulEndOffset;
            break;
        }
        else
        {
            ulPartSize += ulBlockSize;
        }
    }

    if (ulPartSize >= HTTP_SMPO_BODY_MSG_MAX_SIZE)
    {
        return ANSC_STATUS_DISCARD;
    }
   
    if (pBuf)
    {
        if (ulBufSize - ulDataLen <= ulPartSize)
        {
            ulNewSize += HTTP_SMPO_BODY_MSG_MAX_SIZE;
        }
    }
    else
    {
        ulNewSize   = HTTP_SMPO_BODY_MSG_MAX_SIZE;
    }

    if (ulNewSize != 0)
    {
        pNewBuf = AnscAllocateMemory(ulNewSize);
        if (!pNewBuf)
        {
            return ANSC_STATUS_SUCCESS;
        }

        if (pBuf && ulDataLen != 0)
        {
            AnscCopyMemory(pNewBuf, pBuf, ulDataLen);
            AnscFreeMemory(pBuf);
        }

        pBuf = pNewBuf;
        ulBufSize = ulNewSize;

        *ppBuf = pNewBuf;
        *pulBufSize = ulBufSize;
    }

    pSLinkEntry = &pStartBdo->Linkage;    
    while (pSLinkEntry)
    {
        pBdo = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        ulBlockSize = AnscBdoGetBlockSize(pBdo);

        if (pBdo == pStartBdo)
        {
            if (pBdo == pEndBdo)
            {
                AnscCopyMemory(pBuf + ulDataLen, ((PUCHAR)AnscBdoGetBuffer(pBdo)) + ulStartOffset, ulEndOffset - ulStartOffset);
                ulDataLen += (ulEndOffset - ulStartOffset);
                break;
            }
            else
            {
                AnscCopyMemory(pBuf + ulDataLen, ((PUCHAR)AnscBdoGetBuffer(pBdo)) + ulStartOffset, ulBlockSize - ulStartOffset);
                ulDataLen += (ulBlockSize - ulStartOffset);
            }
        }
        else if (pBdo == pEndBdo)
        {
            if (ulEndOffset != 0)
            {
                AnscCopyMemory(pBuf + ulDataLen, ((PUCHAR)AnscBdoGetBuffer(pBdo)), ulEndOffset);
                ulDataLen += ulEndOffset;
            }
            break;
        }
        else
        {
            AnscCopyMemory(pBuf + ulDataLen, ((PUCHAR)AnscBdoGetBuffer(pBdo)), ulBlockSize);
            ulPartSize += ulBlockSize;
        }
    }

    pBuf[ulDataLen] = 0;

    if (TRUE)
    {
        PUCHAR                      pHeaders    = pBuf + ulDataLen - ulPartSize;
        PUCHAR                      pHeadersEnd = NULL;
        PUCHAR                      pContentType;

        pHeaders = (PUCHAR)_ansc_strstr((const char *)pHeaders, "\r\n");

        if (!pHeaders)
        {
            status = ANSC_STATUS_INTERNAL_ERROR;
            goto EXIT;
        }

        pHeaders += 2;
        pHeadersEnd = (PUCHAR)_ansc_strstr((const char *)pHeaders, "\r\n\r\n");

        if (!pHeadersEnd)
        {
            status = ANSC_STATUS_INTERNAL_ERROR;
            goto EXIT;
        }
    
        pContentType = (PUCHAR)_ansc_strstr((const char *)pHeaders, (const char *)HTTP_SMPO_CONTENT_TYPE);
        if (pContentType)
        {
            if (pContentType[-1] != '\n' || pContentType[-2] != '\r' || pContentType >= pHeadersEnd)
            {
                goto EXIT;
            }
        }
    }

    goto SUCC;

EXIT:

    ulDataLen -= ulPartSize;

SUCC:

    *pulDataLen = ulDataLen;

    return status;
}


