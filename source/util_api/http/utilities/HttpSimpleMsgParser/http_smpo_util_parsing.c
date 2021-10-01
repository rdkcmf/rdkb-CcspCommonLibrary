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

    module:	http_smpo_util_parsing.c

        For HTTP message parser (syntax parser),
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

        ANSC_HANDLE
        HttpSmpoUtilParseRequestLine
            (
                ANSC_HANDLE         hHttpSmpo,
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse request line.

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of request line to be parsed.
    
    return:     handle to request line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoUtilParseRequestLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ULONG                           i;
    PHTTP_REQUEST_INFO              pRequestInfo = NULL;
    PUCHAR                          pMethod, pUri, pVersion;
    ULONG                           ulMethod, ulUri, ulVersion;
    PUCHAR                          pBuf = NULL;
    PUCHAR                          pMsg = NULL;

    pMethod             = NULL;
    pUri                = NULL;
    pVersion            = NULL;

    if(buffer)
    {
        pBuf                = (PUCHAR)buffer;
    }
    else
    {
        return (ANSC_HANDLE)NULL;
    }

    /* skip possible leading spaces and tab */
    for (i = 0; i < ulSize; i ++)
    {
        if (pBuf[i] != ' ')
            break;
    }

    pMethod = &pBuf[i];
    if (!pMethod) /*RDKB-6241, CID-24122, If method is null return*/
    {
        return (ANSC_HANDLE)NULL;
    }

    pMsg    = (PUCHAR)_ansc_strchr((const char *)pMethod, ' ');

    if (!pMsg)
    {
        return (ANSC_HANDLE)NULL;
    }

    ulMethod        = (ULONG)(pMsg - pMethod);
    
    /* skip possible leading spaces and tab */
    while (*pMsg == ' ')
    {
        pMsg ++;
    }

    pUri    = pMsg;
    if (!pMethod) /*RDKB-6241, CID-24394, If method is null return*/
    {
        return (ANSC_HANDLE)NULL;
    }

    pMsg    = (PUCHAR)_ansc_strchr((const char *)pUri, ' ');

    if (!pMsg)
        return (ANSC_HANDLE)NULL;

    ulUri           = (ULONG)(pMsg - pUri);
    pMsg ++;

    pVersion        = pMsg;
    ulVersion       = ((PUCHAR)buffer + ulSize - pMsg);

    /*removing pUri check CID: 54262 Dereference before null check*/
    if (pMethod && pVersion)
    {
        pRequestInfo    = (PHTTP_REQUEST_INFO)AnscAllocateMemory(sizeof(HTTP_REQUEST_INFO));
        
        /* parse method */
        pRequestInfo->Method    = HttpSmpoUtilGetMethodId(pMethod, ulMethod);

        if (pMethod && ulMethod != 0)
        {
            ULONG                   ulNameLen   = ulMethod;

            if (ulNameLen >= HTTP_MAX_METHOD_STR_SIZE)
            {
                ulNameLen   = HTTP_MAX_METHOD_STR_SIZE - 1;
            }

            AnscCopyMemory(pRequestInfo->MethodName, pMethod, ulNameLen);
            pRequestInfo->MethodName[ulNameLen] = '\0';
        }

        pRequestInfo->RequestUri.PathLevel = 1;

        pRequestInfo->MajorVersion  = 1;
        pRequestInfo->MinorVersion  = 0;

        /* parse version */
        HttpSmpoUtilParseRequestLineVersion
            (
                pVersion, 
                ulVersion, 
                &pRequestInfo->MajorVersion, 
                &pRequestInfo->MinorVersion
            );

        pRequestInfo->RequestUri.HostPort   = HTTP_SERVER_PORT;

        /* parse URI */
        HttpSmpoUtilParseRquestLineUri(hHttpSmpo, (ANSC_HANDLE)&pRequestInfo->RequestUri, pUri, ulUri);

        if (pRequestInfo->RequestUri.Type == HTTP_URI_TYPE_ASTERISK || 
            pRequestInfo->RequestUri.Type == HTTP_URI_TYPE_RESERVED)
        {
            pRequestInfo->RequestUri.PathLevel = 0;
        }

        HttpSmpoUtilCopyHeaderString((PUCHAR)buffer, ulSize, pRequestInfo->StartLine, HTTP_MAX_START_LINE_SIZE);
    }

    return (ANSC_HANDLE)pRequestInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoUtilParseStatusLine
            (
                ANSC_HANDLE         hHttpSmpo,
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse status line.

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of status line to be parsed.
    
    return:     handle to status line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoUtilParseStatusLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PUCHAR                          pLast           = ((PUCHAR)buffer) + ulSize - 1;
    PHTTP_RESPONSE_INFO             pResponseInfo   = NULL;
    ULONG                           ulMajor, ulMinor;
    PUCHAR                          pNext, pBuf;
    ULONG                           ulStatusCode;
    PUCHAR                          pReason;
    ULONG                           ulReason;
    ULONG                           ulCopySize;

    /* version */
    pBuf    = (PUCHAR)buffer;
    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SPACE, ulSize);

    if (!pNext)
    {
        return NULL;
    }

    HttpSmpoUtilParseRequestLineVersion
        (
            pBuf, 
            pNext - pBuf, 
            &ulMajor, 
            &ulMinor
        );

    /* status code */
    pBuf    = pNext + 1;
    ulStatusCode    = _ansc_atoi((const char *)pBuf);

    pNext   = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SPACE, pLast - pBuf + 1);
    if (!pNext)
    {
        return NULL;
    }

    /* reason phrase */
    pReason     = pNext + 1;
    ulReason    = pLast - pReason + 1;

    pResponseInfo   = (PHTTP_RESPONSE_INFO)AnscAllocateMemory(sizeof(HTTP_RESPONSE_INFO));

    if (pResponseInfo)
    {
        ulCopySize  = ulSize;
        if (ulCopySize >= HTTP_MAX_START_LINE_SIZE)
        {
            ulCopySize  = HTTP_MAX_START_LINE_SIZE - 1;
        }
        
        AnscCopyMemory(pResponseInfo->StartLine, buffer, ulCopySize);
        pResponseInfo->StartLine[ulCopySize] = 0;

        pResponseInfo->MajorVersion = ulMajor;
        pResponseInfo->MinorVersion = ulMinor;
        pResponseInfo->StatusCode   = ulStatusCode;
        
        ulCopySize  = ulReason;
        if (ulCopySize >= HTTP_MAX_REASON_PHRASE_SIZE)
        {
            ulCopySize  = HTTP_MAX_REASON_PHRASE_SIZE - 1;
        }

        AnscCopyMemory(pResponseInfo->ReasonPhrase, pReason, ulCopySize);
        pResponseInfo->ReasonPhrase[ulCopySize] = 0;
    }

    return (ANSC_HANDLE)pResponseInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoUtilParseHeader
            (
                ANSC_HANDLE         hHttpSmpo,
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to parse header field.

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of header field to be parsed.
    
    return:     handle to header field object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoUtilParseHeader
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject   = (PHTTP_SIMPLE_MSG_PARSER)hHttpSmpo;
    PUCHAR                          pLast       = ((PUCHAR)buffer) + ulSize - 1;
    ANSC_HANDLE                     hHeader     = (ANSC_HANDLE)NULL;
    PUCHAR                          pHeader;
    ULONG                           ulHeaderSize;
    PUCHAR                          pValue;
    ULONG                           ulValueSize;
    PHTTP_SMPO_HEADER_XINFO         pXinfo;
    BOOL                            bUnknown    = FALSE;

    /* get header name and call corresponding function to parse it */
    pHeader = (PUCHAR)buffer;

    pValue  = _ansc_memchr(pHeader, HTTP_SMPO_CHAR_COLON, ulSize);

    if (!pValue)
    {
#ifdef   _DEBUG
        ULONG                       i;
        AnscTrace("Failed to parse header:\n");
        for (i = 0; i < ulSize; i ++)
        {
            AnscTrace("%s", ((char *)buffer) + i);
        }

        AnscTrace("\n\n");
#endif

        return  (ANSC_HANDLE)NULL;
    }

    ulHeaderSize    = pValue - pHeader;

    pValue ++;
    pValue          = HttpSmpoUtilLinearWhiteSpace(pValue, pLast - pValue + 1);
    ulValueSize     = pLast - pValue + 1;
    
    pXinfo          = pMyObject->GetHeaderXInfo((ANSC_HANDLE)pMyObject, pHeader, ulHeaderSize);

    if (pXinfo)
    {
        if ( !pValue || ulValueSize == 0 )
        {
            hHeader = (ANSC_HANDLE)NULL;
        }
        else
        {
            hHeader = (pXinfo->xFunc)(hHttpSmpo, (PVOID)pValue, ulValueSize);
        }
    }

    if (!hHeader)
    {
        /* extension header */
        PHTTP_HEADER_FIELD          pHeader;

        pHeader     = (PHTTP_HEADER_FIELD)AnscAllocateMemory(sizeof(HTTP_HEADER_FIELD));

        hHeader     = (ANSC_HANDLE)pHeader;
        bUnknown    = TRUE;
    }

    if (hHeader)
    {
        PHTTP_HEADER_FIELD          pHeaderField    = (PHTTP_HEADER_FIELD)hHeader;

        HttpSmpoUtilCopyHeaderString((PUCHAR)buffer, ulSize, pHeaderField->HeaderLine, HTTP_HEADER_LINE_SIZE);
        pHeaderField->Flags  |= HTTP_FIELD_FLAG_LINE_PRESENT;

        if ( bUnknown )
        {
            pHeaderField->HeaderId      = HTTP_HEADER_ID_EXTENSION;
        }
        else
        {
            pHeaderField->HeaderId      = pXinfo?pXinfo->HeaderId:HTTP_HEADER_ID_EXTENSION;
        }
        pHeaderField->Linkage.Next  = NULL;
        pHeaderField->HashIndex     = 0;
    }

    return hHeader;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoUtilParseChunkedLine
            (
                ANSC_HANDLE         hHttpSmpo,
                PVOID               buffer,
                ULONG               ulSize
            )

    description:

        This function is called to chunked line.

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of chunked line to be parsed.
    
    return:     handle to chunked line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoUtilParseChunkedLine
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hHttpSmpo);
    PHTTP_CHUNK_INFO                pChunkInfo  = NULL;
    PUCHAR                          pBuf        = (PUCHAR)buffer;
    PUCHAR                          pLast       = pBuf + ulSize - 1;
    PUCHAR                          pExt        = NULL;
    PUCHAR                          pNext = NULL, pCRLF = NULL;
    ULONG                           ulChunkSize = 0;
    UCHAR                           uc;
    ULONG                           ulValue;

    /*  chunk          = chunk-size [ chunk-extension ] CRLF
                        chunk-data CRLF
        chunk-size     = 1*HEX
        chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
     */

    pChunkInfo  = (PHTTP_CHUNK_INFO)AnscAllocateMemory(sizeof(HTTP_CHUNK_INFO));

    if (!pChunkInfo)
    {
        return (ANSC_HANDLE)NULL;
    }

    pNext   = pBuf;

    while (pNext < pLast)
    {
        pCRLF   = _ansc_memchr(pNext, HTTP_SMPO_CHAR_CR, pLast - pNext);

        if (!pCRLF || pCRLF == pLast)
        {
            AnscFreeMemory(pChunkInfo);/*RDKB-6241, CID-24413, free resource befor return*/
            return (ANSC_HANDLE)NULL;
        }

        if (*(pCRLF + 1) == HTTP_SMPO_CHAR_LF)
        {
            break;
        }

        pNext = pCRLF + 1;
    }

    pExt    = _ansc_memchr(pBuf, HTTP_SMPO_CHAR_SEMICOLON, pCRLF - pBuf);

    pNext   = pBuf;
    while (pNext)
    {
        uc  = *pNext;

        if (uc >= '0' && uc <= '9')
        {
            ulValue = uc - '0';
        }
        else if ((uc >= 'a' && uc <= 'f') || (uc >= 'A' && uc <= 'F'))
        {
            ulValue = uc - 'a' + 10;
        }
        else
        {
            break;
        }

        ulChunkSize = (ulChunkSize << 4) + ulValue;

        pNext ++;
    }

    pChunkInfo->ChunkSize   = ulChunkSize;

    if (!pExt)
    {
        pChunkInfo->ExtCount    = 0;
    }
    else
    {
        PUCHAR                      pExtValue;
        ULONG                       ulExtTokenSize;
        ULONG                       ulExtCount  = 0;

        while (pExt < pCRLF)
        {
            if (ulExtCount >= HTTP_MAX_CHUNK_EXT_NUMBER)
            {
                break;
            }
            /*CID 62548 Dereference after null check*/
	    /* Incrementing null pointer pExt */
	    if ( pExt )
                pExt ++;
            pExt    = HttpSmpoUtilLinearWhiteSpace(pExt, pCRLF - pExt);

            pNext   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_SEMICOLON, pCRLF - pExt);

            if (pNext)
            {
                pExtValue   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_EQUAL, pNext - pExt);
            }
            else
            {
                pExtValue   = _ansc_memchr(pExt, HTTP_SMPO_CHAR_EQUAL, pCRLF - pExt);
            }

            ulExtTokenSize  = pExtValue ? pExtValue - pExt : (pNext ? pNext - pExt : pCRLF - pExt);

            if (ulExtTokenSize >= HTTP_MAX_CHUNK_TOKEN_SIZE)
            {
                ulExtTokenSize  = HTTP_MAX_CHUNK_TOKEN_SIZE - 1;
            }

            AnscCopyMemory(pChunkInfo->ExtArray[ulExtCount], pExt, ulExtTokenSize);
            pChunkInfo->ExtArray[ulExtCount][ulExtTokenSize]    = 0;

            ulExtCount ++;

            pExt    = pNext;
        }

        pChunkInfo->ExtCount    = ulExtCount;
    }

    return (ANSC_HANDLE)pChunkInfo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoParseUrl
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            )

    description:

        This function is called to parse URL.

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of chunked line to be parsed.

                ULONG               ulSize
                The size of URL.
    
    return:     handle to HTTP Request URL.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseUrl
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_REQUEST_URI               pRequestUri;
    BOOL                            bSucc;

    if (!buffer || ulSize == 0)
    {
        return (ANSC_HANDLE)NULL;
    }

    pRequestUri = (PHTTP_REQUEST_URI)AnscAllocateMemory(sizeof(HTTP_REQUEST_URI));

    if (!pRequestUri)
    {
        return (ANSC_HANDLE)NULL;
    }

    bSucc = 
        HttpSmpoUtilParseRquestLineUri
            (
                hHttpSmpo, 
                (ANSC_HANDLE)pRequestUri, 
                (PUCHAR)buffer, 
                ulSize
            );

    if (!bSucc)
    {
        AnscFreeMemory(pRequestUri);
        pRequestUri = NULL;
    }

    return (ANSC_HANDLE)pRequestUri;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoParseUrlList
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                PULONG                      pulUrlCount,
                PANSC_HANDLE                phUrlArray
            )

    description:

        This function is called to parse URL list in the
        following format.

        urllist = <url1><url2>...<urlN>

    argument:   ANSC_HANDLE         hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PVOID               buffer
                Buffer of chunked line to be parsed.

                ULONG               ulSize
                Size of URL.

                PULONG              pulUrlCount
                On return it contains the count of URLs in the list.

                PANSC_HANDLE        phUrlArray
                Pointer to the HTTP Request URL array.
    
    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoParseUrlList
    (
        ANSC_HANDLE                 hHttpSmpo,
        PVOID                       buffer,
        ULONG                       ulSize,
        PULONG                      pulUrlCount,
        PANSC_HANDLE                phUrlArray
    )
{
    PHTTP_REQUEST_URI               *pUrlArray  = (PHTTP_REQUEST_URI *)phUrlArray;
    PHTTP_REQUEST_URI               pRequestUri;
    PUCHAR                          pUri        = buffer;
    PUCHAR                          pLast       = pUri + ulSize - 1;
    PUCHAR                          pNext;
    ULONG                           ulUriLen;
    ULONG                           ulUrlCount  = 0;
    ULONG                           ulMaxCount  = pulUrlCount ? *pulUrlCount : 0;
    BOOL                            bSucc;

    if (!buffer || ulSize == 0)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    while (pUri && pUri < pLast && ulUrlCount < ulMaxCount)
    {
        pUri = _ansc_memchr(pUri, HTTP_SMPO_CHAR_LESS, pLast - pUri + 1);

        if (pUri)
        {
            pUri ++;
            pNext = _ansc_memchr(pUri, HTTP_SMPO_CHAR_LARGE, pLast - pUri + 1);

            if (!pNext)
            {
                break;
            }

            ulUriLen    = pNext - pUri;

            if (ulUriLen != 0)
            {
                pRequestUri = (PHTTP_REQUEST_URI)AnscAllocateMemory(sizeof(HTTP_REQUEST_URI));

                if (!pRequestUri)
                {
                    break;
                }

                bSucc = 
                    HttpSmpoUtilParseRquestLineUri
                        (
                            hHttpSmpo, 
                            (ANSC_HANDLE)pRequestUri, 
                            pUri, 
                            ulUriLen
                        );

                if (!bSucc)
                {
                    AnscFreeMemory(pRequestUri);
                }
                else
                {
                    pUrlArray[ulUrlCount ++] = pRequestUri;
                }
            }

            pUri = pNext + 1;
        }
    }

    if (pulUrlCount)
    {
        *pulUrlCount    = ulUrlCount;
    }

    return ANSC_STATUS_SUCCESS;
}


