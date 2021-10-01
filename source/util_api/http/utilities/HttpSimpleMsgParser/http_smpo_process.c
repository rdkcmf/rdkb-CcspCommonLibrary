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

    module:	http_smpo_process.c

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the HTTP message parser component object.

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
    
        02/22/02

**********************************************************************/


#include "http_smpo_global.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseHeader
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP header field
        and then save it into an internal data structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                header field.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to a HTTP header object.

**********************************************************************/


ANSC_HANDLE
HttpSmpoParseHeader
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    return HttpSmpoUtilParseHeader(hThisObject, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseRequestLine
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP request
        line in given buffer and then save the information
        into an internal structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                request line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to a HTTP request line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseRequestLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    return HttpSmpoUtilParseRequestLine(hThisObject, buffer,  ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseStatusLine
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP status
        line in given buffer and then save the information
        into an internal structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                status line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to the status line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseStatusLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    return HttpSmpoUtilParseStatusLine(hThisObject, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseChunkedBody
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP status
        line in given buffer and then save the information
        into an internal structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                status line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to the status line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseChunkedLine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    return HttpSmpoUtilParseChunkedLine(hThisObject, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseQueryString
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP status
        line in given buffer and then save the information
        into an internal structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                status line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to the status line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseQueryString
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PHTTP_FORM_INPUT                pFormInput  = NULL;
    ULONG                           ulCount;
    ULONG                           ulBufSize;
    PUCHAR                          pAmp;
    PUCHAR                          pBuf, pName, pValue;
    ULONG                           ulBuf, ulName, ulValue;
    PHTTP_FORM_ELEMENT              pElement;
    ULONG                           ulNameLen;

    /* get the count of elements */
    ulCount = AnscStrBufGetCharNum(buffer, ulSize, '&');

    /*
    if (ulCount == 0)
        return NULL;
     */

    ulCount ++;

    ulBufSize   = sizeof(HTTP_FORM_INPUT) + ulCount * sizeof(HTTP_FORM_ELEMENT);
    pFormInput  = (PHTTP_FORM_INPUT)AnscAllocateMemory(ulBufSize);

    if (pFormInput)
    {
        pBuf        = buffer;
        ulBuf       = ulSize;

        ulNameLen   = 0;

        while (pBuf && ulBuf != 0)
        {
            if (pFormInput->ElementCount >= ulCount)
            {
                /* to prevent invalid input */
                break;
            }

            /* find next item */
            pAmp    = (PUCHAR)AnscMemChar(pBuf, '&', ulBuf);

            if (pAmp)
            {
                /* we got an item */
                pName   = pBuf;
                ulName  = pAmp - pBuf;

                pBuf    = pAmp + 1;
                ulBuf   -= (ulName + 1);
            }
            else
            {
                pName   = pBuf;
                ulName  = ulBuf;

                pBuf    = NULL;
                ulBuf   = 0;
            }

            if (ulName == 0)
            {
                continue;
            }

            pElement    = &pFormInput->ElementArray[pFormInput->ElementCount ++];
            HttpInitFormElement(pElement);

            /* find '=' */
            pValue  = (PUCHAR)AnscMemChar(pName, '=', ulName);
            if (!pValue)
            {
                /* only name is present */
                pElement->Name = (char *)AnscAllocateMemory(ulName + 1);

                AnscCopyMemory(pElement->Name, pName, ulName);
                pElement->Name[ulName]  = 0;

                ulNameLen   = ulName;
            }
            else
            {
                /* get name & value */
                ulNameLen   = pValue - pName;
                ulValue     = ulName - ulNameLen - 1;
                pValue  ++;

                if (ulNameLen == 0)
                {
                    /* no name specified, could the query string be malformed? */
                    pFormInput->ElementCount --;
                    continue;
                }

                if (ulNameLen != 0)
                {
                    pElement->Name = (char *)AnscAllocateMemory(ulNameLen + 1);

                    AnscCopyMemory(pElement->Name, pName, ulNameLen);
                    pElement->Name[ulNameLen]  = 0;
                }

                if (ulValue != 0)
                {
                    pElement->Value = (char *)AnscAllocateMemory(ulValue + 1);

                    AnscCopyMemory(pElement->Value, pValue, ulValue);
                    ((char *)pElement->Value)[ulValue]  = 0;

                    /* decode the string if it contains characters like "%xx" */
                    HttpSmpoDecodeUrlEncode(pElement->Value, ulValue, pElement->Value, &ulValue);

                    pElement->Size = ulValue;
                }
            }

            /* decode query name */
            HttpSmpoDecodeUrlEncode((PUCHAR)pElement->Name, ulNameLen, (PUCHAR)pElement->Name, &ulNameLen);
        }
    }

    /*CID: 53361 Dereference after null check*/
    if (pFormInput && pFormInput->ElementCount == 0)
    {
        /* the query string might be empty or malformed */
        AnscFreeMemory(pFormInput);
        pFormInput  = NULL;
    }

    return pFormInput;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoUtilParseMultipart
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pBuf,
                ULONG                       ulSize,
                char*                       pBoundary
            )

    description:

        This function is called to parse multipart messages.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pBuf
                Buffer of multipart messages.

                ULONG                       ulSize
                The length of buffer.

                char*                       pBoundary
                The boundary string used to delimit between adjacent
                multipart messages.
    
    return:     the handle to form input object created.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseMultipart
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        char*                       pBoundary
    )
{
    PHTTP_SMPO_MULTIPART_PART       pPart;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PHTTP_FORM_INPUT                pFormInput  = NULL;
    SLIST_HEADER                    PartList;
    PSINGLE_LINK_ENTRY              pEntry;
    PUCHAR                          pData;
    PUCHAR                          pLast       = ((PUCHAR)buffer) + ulSize - 1;
    ULONG                           ulDataLen;
    ULONG                           ulParts     = 0;
    ULONG                           ulIndex     = 0;
    PHTTP_FORM_ELEMENT              pFormElement;
    ULONG                           ulPartLen;
    PUCHAR                          pNextPart   = NULL;

    AnscSListInitializeHeader(&PartList);

    pData       = (PUCHAR)buffer;
    ulDataLen   = ulSize;

    while (pData <= pLast)
    {
        pPart   = (PHTTP_SMPO_MULTIPART_PART)AnscAllocateMemory(sizeof(HTTP_SMPO_MULTIPART_PART));
        if (!pPart)
        {
            break;
        }

        status  = HttpSmpoGetMultipartPart(hThisObject, pData, ulDataLen, (PUCHAR)pBoundary, pPart, &pNextPart);

        if (status != ANSC_STATUS_SUCCESS)
        {
            AnscFreeMemory(pPart);
            break;
        }

        ulPartLen   = pNextPart - pData;
        pData       = pNextPart;
        ulDataLen   -= ulPartLen;

        AnscSListPushEntry(&PartList, &pPart->Linkage);
    }

    ulParts = AnscSListQueryDepth(&PartList);

    pFormInput  = 
        (PHTTP_FORM_INPUT)AnscAllocateMemory
            (
                sizeof(HTTP_FORM_INPUT) + ulParts * sizeof(HTTP_FORM_ELEMENT)
            );

    if (!pFormInput)
    {
        return (ANSC_HANDLE)NULL;
    }

    pFormInput->ElementCount = ulParts;

    while (TRUE)
    {
        pEntry  = AnscSListPopEntry(&PartList);

        if (!pEntry)
        {
            break;
        }

        pPart   = ACCESS_HTTP_SMPO_MULTIPART_PART_OBJECT(pEntry);

        pFormElement    = &pFormInput->ElementArray[ulIndex ++];

        /* value type */
        pFormElement->ValueType = pPart->ulType;

        /* form element name */
        pFormElement->Name      = (char *)AnscAllocateMemory(pPart->ulNameLen + 1);
        if (pFormElement->Name)
        {
            AnscCopyMemory(pFormElement->Name, pPart->pName, pPart->ulNameLen);
            pFormElement->Name[pPart->ulNameLen]    = 0;
        }

        /* form element headers */
        if (pPart->pHeader && pPart->ulHeaderLen != 0)
        {
            pFormElement->Headers   = (char *)AnscAllocateMemory(pPart->ulHeaderLen + 1);
            if (pFormElement->Headers)
            {
                AnscCopyMemory(pFormElement->Headers, pPart->pHeader, pPart->ulHeaderLen);
                pFormElement->Headers[pPart->ulHeaderLen]    = 0;
            }
        }

        /* form element value */
        pFormElement->Value     = (void *)AnscAllocateMemory(pPart->ulValueLen + 1);
        if (pFormElement->Value)
        {
            PUCHAR                  pVal    = (PUCHAR)pFormElement->Value;

            AnscCopyMemory(pFormElement->Value, pPart->pValue, pPart->ulValueLen);
            pVal[pPart->ulValueLen]  = 0;
        }

        /* length of form element value */
        pFormElement->Size  = pPart->ulValueLen;

#ifdef   _DEBUG
        AnscTrace("===> Form name = <%s>, value = <%p>.\n", pFormElement->Name, pFormElement->Value);
#endif

        AnscFreeMemory(pPart);
    }

    return (ANSC_HANDLE)pFormInput;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoParseHtmlFormInput
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to first parse a HTTP status
        line in given buffer and then save the information
        into an internal structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Text stream buffer which is supposed to hold a HTTP
                status line.

                ULONG                       ulSize
                The size of text stream buffer.

    return:     the handle (pointer) to the status line object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoParseHtmlFormInput
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ULONG                       ulEncType,
        char*                       pBoundary
    )
{
    if (ulEncType == HTTP_HFP_FORM_ENCTYPE_URLENCODED)
    {
        return HttpSmpoParseQueryString(hThisObject, buffer, ulSize);
    }
    else
    if (ulEncType == HTTP_HFP_FORM_ENCTYPE_MULTIPART && pBoundary)
    {
        return HttpSmpoParseMultipart(hThisObject, buffer, ulSize, pBoundary);
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoSaveFormPartAsFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       param_name,
                char*                       file_name,
                ULONG                       ulEncType,
                char*                       pBoundary,
                ANSC_HANDLE                 hHttpMbo,
                BOOL                        bFlushBody
            );

    description:

        This function is called to save form part into given
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       param_name
                The name of form element.

                char*                       file_name
                The file to be saved.

                ULONG                       ulEncType
                Encoding type.

                char*                       pBoundary
                Boundary string.

                ANSC_HANDLE                 hHttpMbo
                HTTP message body object.

                BOOL                        bFlushBody
                Flag to indicate if HTTP message body object
                should be flushed.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
HttpSmpoSaveFormPartAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        ULONG                       ulEncType,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        BOOL                        bFlushBody
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo    = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;

    if (ulEncType != HTTP_HFP_FORM_ENCTYPE_MULTIPART)
    {
        status  = pHttpMbo->SaveBodyAsFile((ANSC_HANDLE)pHttpMbo, file_name, bFlushBody);
    }
    else
    {
        ANSC_HANDLE                 hStartBdo, hEndBdo, hStartField;
        ULONG                       ulStartOffset, ulEndOffset, ulFieldOffset;
       
        status  = 
            HttpSmpoLocateFieldData
                (
                    hHttpMbo, 
                    param_name,
                    pBoundary,
                    &hStartField,
                    &ulFieldOffset,
                    &hStartBdo,
                    &ulStartOffset,
                    &hEndBdo,
                    &ulEndOffset
                );

        if (status == ANSC_STATUS_SUCCESS)
        {
            status  =
                HttpSmpoSaveFieldData
                    (
                        hHttpMbo,
                        file_name,
                        bFlushBody,
                        hStartField,
                        ulFieldOffset,
                        hStartBdo,
                        ulStartOffset,
                        hEndBdo,
                        ulEndOffset
                    );
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpSmpoGetFormDataNoFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pBoundary,
                ANSC_HANDLE                 hHttpMbo,
                BOOL                        bFlushBody,
                PULONG                      pulFormLen
            );

    description:

        This function is called to extract form data content
        without any uploaded file content. This function can 
        only be called if multipart is used.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pBoundary
                Boundary string.

                ANSC_HANDLE                 hHttpMbo
                HTTP message body object.

                PULONG                      pulFormLen
                Contain form content length on successful return.

    return:     form content data.

**********************************************************************/

char*
HttpSmpoGetFormDataNoFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pBoundary,
        ANSC_HANDLE                 hHttpMbo,
        PULONG                      pulFormLen
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pHttpMbo        = (PHTTP_MESSAGE_BODY_OBJECT)hHttpMbo;
    ULONG                           ulBoundaryLen   = AnscSizeOfString(pBoundary);
    PANSC_BUFFER_DESCRIPTOR         pCurBdo, pNextBdo;
    ULONG                           ulCurOffset, ulNextOffset;
    BOOL                            bFound;
    char*                           pFormData       = NULL;
    ULONG                           ulFormBufSize   = 0;
    ULONG                           ulFormDataLen   = 0;
    
    pCurBdo = pNextBdo = NULL;
    ulCurOffset = ulNextOffset = 0;

    AnscAcquireLock(&pHttpMbo->BdoQueueLock);

    bFound  = 
        HttpSmpoFindNextBoundary
            (
                hHttpMbo, 
                (ANSC_HANDLE)NULL, 
                0,
                pBoundary, 
                (ANSC_HANDLE *)&pCurBdo,
                &ulCurOffset
            );

    while (bFound)
    {
        bFound  = 
            HttpSmpoFindNextBoundary
                (
                    hHttpMbo, 
                    (ANSC_HANDLE)pCurBdo, 
                    ulCurOffset + ulBoundaryLen, 
                    pBoundary, 
                    (ANSC_HANDLE *)&pNextBdo, 
                    &ulNextOffset
                );

        if (!bFound)
        {
            pNextBdo        = NULL;
            ulNextOffset    = 0;
        }

        status =
            HttpSmpoCopyFormData2
                (
                    hHttpMbo,
                    (PUCHAR*)&pFormData, 
                    &ulFormBufSize, 
                    &ulFormDataLen, 
                    pBoundary,
                    (ANSC_HANDLE)pCurBdo, 
                    ulCurOffset, 
                    (ANSC_HANDLE)pNextBdo, 
                    ulNextOffset
                );

        if (status != ANSC_STATUS_SUCCESS && status != ANSC_STATUS_DISCARD)
            break;

        pCurBdo = pNextBdo;
        ulCurOffset = ulNextOffset;
    }

    AnscReleaseLock(&pHttpMbo->BdoQueueLock);

    if (ulFormDataLen != 0)
    {
        *pulFormLen = ulFormDataLen;
    }
    else
    {
        *pulFormLen = 0;
        
        if (pFormData)
        {
            AnscFreeMemory(pFormData);
            pFormData = NULL;
        }
    }

    return pFormData;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpSmpoGetHeaderParam
            (
                ANSC_HANDLE                 hThisObject,
                char*                       parameters,
                char*                       name
            )

    description:

        This function is called to extract the value of given
        parameter from the given buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       parameters
                The buffer to parameters.

                char*                       name
                Parameter's name.

    return:     the value of the parameter. None if the parameter
                is not present.

**********************************************************************/

char*
HttpSmpoGetHeaderParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       parameters,
        char*                       name
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PUCHAR                          pLast   = (PUCHAR)parameters + AnscSizeOfString(parameters) - 1;
    PUCHAR                          pName;
    PUCHAR                          pValue;
    ULONG                           ulValue;
    PUCHAR                          pParamValue = NULL;
    PUCHAR                          pCur, pNext;
    ULONG                           ulLen;

    pCur    = (PUCHAR)parameters;

    while (pCur)
    {
        pCur    = HttpSmpoUtilLinearWhiteSpace(pCur, pLast - pCur + 1);

        pNext   = _ansc_memchr(pCur, HTTP_SMPO_CHAR_SEMICOLON, pLast - pCur + 1);

        /* check current parameter */
        if (pNext)
        {
            ulLen   = pNext - pCur;
        }
        else
        {
            ulLen   = pLast - pCur + 1;
        }

        pName   = (PUCHAR)_ansc_strstr((const char *)pCur, name);

        if (pName && pName <= pCur + ulLen)
        {
            pValue  = pName + AnscSizeOfString(name);

            if (pValue < pCur + ulLen)
            {
                if (pNext)
                {
                    ulValue = pNext - pValue;
                }
                else
                {
                    ulValue = pLast - pValue + 1;
                }

                pValue  = HttpSmpoUtilLinearWhiteSpace(pValue, ulValue);

                if (pValue < pCur + ulLen && *pValue == HTTP_SMPO_CHAR_EQUAL)
                {
                    pValue ++;

                    if (pNext)
                    {
                        ulValue = pNext - pValue;
                    }
                    else
                    {
                        ulValue = pLast - pValue + 1;
                    }

                    if (ulValue != 0 && pValue < pCur + ulLen)
                    {
                        pParamValue = (PUCHAR)AnscAllocateMemory(ulValue + 1);

                        if (pParamValue)
                        {
                            HttpSmpoUtilCopyHeaderString(pValue, ulValue,(char *) pParamValue, ulValue + 1);
                        }

                        return (char *)pParamValue;
                    }
                }
            }
        }

        /* check next parameter */

        if (pNext)
        {
            pCur    = pNext + 1;
        }
        else
        {
            pCur    = NULL;
        }
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSmpoGetHeaderSize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the header field length and
        CRLF is not included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP header

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
HttpSmpoGetHeaderSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    )
{
    /* build header and save it into header structure */
    PHTTP_SIMPLE_MSG_PARSER   pHttpMP = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;

    pHttpMP->BuildHeader(hThisObject, hHttpHfo, NULL, 0);

    return HttpSmpoUtilGetSizeHeader(hThisObject, hHttpHfo);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSmpoGetRequestLineSize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the request line length and
        CRLF is not included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP request
                line.

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
HttpSmpoGetRequestLineSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    )
{
    return HttpSmpoUtilGetSizeRequestLine(hThisObject, hHttpHfo);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSmpoGetStatusLineSize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo
            );

    description:

        This function is called to get the status line length and
        CRLF is not included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP status
                line.

    return:     the header length (>0), or error (0).

**********************************************************************/

ULONG
HttpSmpoGetStatusLineSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo
    )
{
    return HttpSmpoUtilGetSizeStatusLine(hThisObject, hHttpHfo);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoBuildHeader
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP header specified
        by hHttpHfo and save it into specified buffer. If the buffer
        size is not big enough, the constructed HTTP header will be
        trunked.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP header
                in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP header.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoBuildHeader
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return HttpSmpoUtilBuildHeader(hHttpHfo, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoBuildRequestLine
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP request line
        specified by hHttpHfo and save it into specified buffer. 
        If the buffer size is not big enough, the constructed HTTP 
        request line will be trunked.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP request
                line in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP 
                request line.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoBuildRequestLine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return HttpSmpoUtilBuildRequestLine(hHttpHfo, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoBuildStatusLine
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHttpHfo,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a HTTP status line
        specified by hHttpHfo and save it into specified buffer. 
        If the buffer size is not big enough, the constructed HTTP 
        status line will be trunked.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHttpHfo
                This handle is actually the pointer to a HTTP status
                line in an internal structure form.

                PVOID                       buffer
                Message buffer which holds the constructed HTTP 
                status line.

                ULONG                       ulSize
                The buffer size.

    return:     the status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoBuildStatusLine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpHfo,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return HttpSmpoUtilBuildStatusLine(hHttpHfo, buffer, ulSize);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpSmpoGetHfpIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the interface which can be
        used for parsing and constructing HTTP messages.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
HttpSmpoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER   pHttpMP;

    pHttpMP = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;
    
    if (!pHttpMP)
        return (ANSC_HANDLE)NULL;
    else
        return pHttpMP->hHfpIf;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoRegisterHeaderXInfo
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pName,
                ULONG                       ulHeaderId,
                PFN_SMPO_PARSE_HDR          xFunc
            )

    description:

        This function is called to register a header parsing function.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pName
                HTTP Header name.

                ULONG                       ulHeaderId
                The ID of the HTTP header.

                PFN_SMPO_PARSE_HDR          xFunc
                The parsing function for the header.
    
    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoRegisterHeaderXInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulHeaderId,
        PFN_SMPO_PARSE_HDR          xFunc
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject   = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;
    PHTTP_SMPO_HEADER_XINFO         pXinfo      = NULL;
    ULONG                           ulHashIndex = 0;

    pXinfo  = (PHTTP_SMPO_HEADER_XINFO)AnscAllocateMemory(sizeof(HTTP_SMPO_HEADER_XINFO));

    if (!pXinfo)
    {
        return ANSC_STATUS_RESOURCES;
    }

    AnscCopyString((char *)pXinfo->Name, (char *)pName);
    pXinfo->xFunc       = xFunc;
    pXinfo->HeaderId    = ulHeaderId;

    ulHashIndex = AnscHashString((char *)pName, AnscSizeOfString((const char *)pName), HTTP_SMPO_HEADER_TABLE_SIZE);

    AnscSListPushEntry(&pMyObject->HeaderXList[ulHashIndex], &pXinfo->Linkage);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        HttpSmpoRemoveXTable
            (
                ANSC_HANDLE                 hHttpSmpo
            )

    description:

        This function is called to clean up parsing function table.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoRemoveXTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject   = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;
    PHTTP_SMPO_HEADER_XINFO         pXinfo      = NULL;
    ULONG                           i;
    PSINGLE_LINK_ENTRY              pEntry;

    for (i = 0; i < HTTP_SMPO_HEADER_TABLE_SIZE; i ++)
    {
        while (TRUE)
        {
            pEntry  = AnscSListPopEntry(&pMyObject->HeaderXList[i]);
            pXinfo  = ACCESS_HTTP_SMPO_HEADER_XINFO_OBJECT(pEntry);

            if (!pXinfo)
            {
                break;
            }

            AnscFreeMemory(pXinfo);
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        BOOL
        HttpSmpoRegisterAllHeadersXInfo
            (
                ANSC_HANDLE                 hHttpSmpo
            )

    description:

        This function is called to register parsing functions
        for all HTTP headers.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSmpoRegisterAllHeadersXInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject   = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ACCEPT,
            HTTP_HEADER_ID_ACCEPT,
            HttpSmpoParseAccept
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ACCEPT_CHARSET,
            HTTP_HEADER_ID_ACCEPT_CHARSET,
            HttpSmpoParseAcceptCharset
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ACCEPT_ENCODING,
            HTTP_HEADER_ID_ACCEPT_ENCODING,
            HttpSmpoParseAcceptEncoding
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ACCEPT_LANGUAGE,
            HTTP_HEADER_ID_ACCEPT_LANGUAGE,
            HttpSmpoParseAcceptLanguage
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ACCEPT_RANGES,
            HTTP_HEADER_ID_ACCEPT_RANGES,
            HttpSmpoParseAcceptRanges
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_AGE,
            HTTP_HEADER_ID_AGE,
            HttpSmpoParseAge
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ALLOW,
            HTTP_HEADER_ID_ALLOW,
            HttpSmpoParseAllow
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_AUTHORIZATION,
            HTTP_HEADER_ID_AUTHORIZATION,
            HttpSmpoParseAuthorization
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CACHE_CONTROL,
            HTTP_HEADER_ID_CACHE_CONTROL,
            HttpSmpoParseCacheControl
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONNECTION,
            HTTP_HEADER_ID_CONNECTION,
            HttpSmpoParseConnection
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_ENCODING,
            HTTP_HEADER_ID_CONTENT_ENCODING,
            HttpSmpoParseContentEncoding
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_LANGUAGE,
            HTTP_HEADER_ID_CONTENT_LANGUAGE,
            HttpSmpoParseContentLanguage
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_LENGTH,
            HTTP_HEADER_ID_CONTENT_LENGTH,
            HttpSmpoParseContentLength
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_LOCATION,
            HTTP_HEADER_ID_CONTENT_LOCATION,
            HttpSmpoParseContentLocation
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_MD5,
            HTTP_HEADER_ID_CONTENT_MD5,
            HttpSmpoParseContentMD5
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_RANGE,
            HTTP_HEADER_ID_CONTENT_RANGE,
            HttpSmpoParseContentRange
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_CONTENT_TYPE,
            HTTP_HEADER_ID_CONTENT_TYPE,
            HttpSmpoParseContentType
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_DATE,
            HTTP_HEADER_ID_DATE,
            HttpSmpoParseDate
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_ETAG,
            HTTP_HEADER_ID_ETAG,
            HttpSmpoParseETag
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_EXPECT,
            HTTP_HEADER_ID_EXPECT,
            HttpSmpoParseExpect
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_EXPIRES,
            HTTP_HEADER_ID_EXPIRES,
            HttpSmpoParseExpires
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_FROM,
            HTTP_HEADER_ID_FROM,
            HttpSmpoParseFrom
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_HOST,
            HTTP_HEADER_ID_HOST,
            HttpSmpoParseHost
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_IF_MATCH,
            HTTP_HEADER_ID_IF_MATCH,
            HttpSmpoParseIfMatch
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_IF_MODIFIED_SINCE,
            HTTP_HEADER_ID_IF_MODIFIED_SINCE,
            HttpSmpoParseIfModifiedSince
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_IF_NONE_MATCH,
            HTTP_HEADER_ID_IF_NONE_MATCH,
            HttpSmpoParseIfNoneMatch
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_IF_RANGE,
            HTTP_HEADER_ID_IF_RANGE,
            HttpSmpoParseIfRange
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_IF_UNMODIFIED_SINCE,
            HTTP_HEADER_ID_IF_UNMODIFIED_SINCE,
            HttpSmpoParseIfUnmodifiedSince
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_LAST_MODIFIED,
            HTTP_HEADER_ID_LAST_MODIFIED,
            HttpSmpoParseLastModified
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_LOCATION,
            HTTP_HEADER_ID_LOCATION,
            HttpSmpoParseLocation
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_MAX_FORWARDS,
            HTTP_HEADER_ID_MAX_FORWARDS,
            HttpSmpoParseMaxForwards
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_PRAGMA,
            HTTP_HEADER_ID_PRAGMA,
            HttpSmpoParsePragma
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_PROXY_AUTHENTICATE,
            HTTP_HEADER_ID_PROXY_AUTHENTICATE,
            HttpSmpoParseProxyAuthenticate
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_PROXY_AUTHORIZATION,
            HTTP_HEADER_ID_PROXY_AUTHORIZATION,
            HttpSmpoParseProxyAuthorization
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_RANGE,
            HTTP_HEADER_ID_RANGE,
            HttpSmpoParseRange
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_REFERER,
            HTTP_HEADER_ID_REFERER,
            HttpSmpoParseReferer
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_RETRY_AFTER,
            HTTP_HEADER_ID_RETRY_AFTER,
            HttpSmpoParseRetryAfter
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_SERVER,
            HTTP_HEADER_ID_SERVER,
            HttpSmpoParseServer
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_TE,
            HTTP_HEADER_ID_TE,
            HttpSmpoParseTE
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_TRAILER,
            HTTP_HEADER_ID_TRAILER,
            HttpSmpoParseTrailer
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_TRANSFER_ENCODING,
            HTTP_HEADER_ID_TRANSFER_ENCODING,
            HttpSmpoParseTransferEncoding
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_UPGRADE,
            HTTP_HEADER_ID_UPGRADE,
            HttpSmpoParseUpgrade
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_USER_AGENT,
            HTTP_HEADER_ID_USER_AGENT,
            HttpSmpoParseUserAgent
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_VARY,
            HTTP_HEADER_ID_VARY,
            HttpSmpoParseVary
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_VIA,
            HTTP_HEADER_ID_VIA,
            HttpSmpoParseVia
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_WARNING,
            HTTP_HEADER_ID_WARNING,
            HttpSmpoParseWarning
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_WWW_AUTHENTICATE,
            HTTP_HEADER_ID_WWW_AUTHENTICATE,
            HttpSmpoParseWwwAuthenticate
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_PROXY_CONNECTION,
            HTTP_HEADER_ID_PROXY_CONNECTION,
            HttpSmpoParseProxyConnection
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_COOKIE,
            HTTP_HEADER_ID_COOKIE,
            HttpSmpoParseCookie
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_SET_COOKIE,
            HTTP_HEADER_ID_SET_COOKIE,
            HttpSmpoParseSetCookie
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_COOKIE2,
            HTTP_HEADER_ID_COOKIE2,
            HttpSmpoParseCookie2
        );

    pMyObject->RegisterHeaderXInfo
        (
            (ANSC_HANDLE)pMyObject,
            (PUCHAR)HTTP_HEADER_NAME_SET_COOKIE2,
            HTTP_HEADER_ID_SET_COOKIE2,
            HttpSmpoParseSetCookie2
        );

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        HttpSmpoGetHeaderXInfo
            (
                ANSC_HANDLE                 hHttpSmpo,
                PUCHAR                      pName,
                ULONG                       ulSize
            )

    description:

        This function is called to parsing info of the given header.

    argument:   ANSC_HANDLE                 hHttpSmpo
                Handle to HTTP Simple Message Parser object.

                PUCHAR                      pName
                Header name.

                ULONG                       ulSize
                The length of buffer.
    
    return:     handle to header parsing info.

**********************************************************************/

ANSC_HANDLE
HttpSmpoGetHeaderXInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName,
        ULONG                       ulSize
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject   = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;
    PHTTP_SMPO_HEADER_XINFO         pXinfo      = NULL;
    PSINGLE_LINK_ENTRY              pEntry;
    ULONG                           ulHashIndex;

    ulHashIndex = AnscHashString((char *)pName, ulSize, HTTP_SMPO_HEADER_TABLE_SIZE);
    pEntry      = AnscSListGetFirstEntry(&pMyObject->HeaderXList[ulHashIndex]);

    while (pEntry)
    {
        pXinfo  = ACCESS_HTTP_SMPO_HEADER_XINFO_OBJECT(pEntry);

        if (ulSize == AnscSizeOfString((char *)pXinfo->Name))
        {
            if (AnscEqualString2((char *)pXinfo->Name, (char *)pName, ulSize, FALSE))
            {
                return (ANSC_HANDLE)pXinfo;
            }
        }

        pEntry  = AnscSListGetNextEntry(pEntry);
    }

    return (ANSC_HANDLE)NULL;
}


