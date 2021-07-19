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

    module:	http_bmo_header.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Basic Message Object.

        *   HttpBmoGetHeaderValueById
        *   HttpBmoGetHeaderValueByName
        *   HttpBmoSetHeaderValueById
        *   HttpBmoSetHeaderValueByName
        *   HttpBmoDelStartLine
        *   HttpBmoParseStartLine
        *   HttpBmoClearHeaders
        *   HttpBmoParseHeaders
        *   HttpBmoCopyStartLineFrom
        *   HttpBmoCopyHeadersFrom
        *   HttpBmoGetHeaderField
        *   HttpBmoAddHeaderField
        *   HttpBmoDelHeaderField
        *   HttpBmoDelAllHfos

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoGetHeaderValueById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header field id to be matched.

    return:     header field value.

**********************************************************************/

char*
HttpBmoGetHeaderValueById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
#endif
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHttpHfValue = NULL;

    pHttpHfo =
        (PHTTP_HEADER_FIELD)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                ulHeaderId
            );

    if ( !pHttpHfo )
    {
        return  NULL;
    }
    else if ( !(pHttpHfo->Flags & HTTP_FIELD_FLAG_LINE_PRESENT) )
    {
        pHfpIf->BuildHeader
                (
                    pHfpIf->hOwnerContext,
                    (ANSC_HANDLE)pHttpHfo,
                    NULL,
                    0
                );
    }

    AnscHttpGetHfValue(pHttpHfo->HeaderLine, AnscSizeOfString(pHttpHfo->HeaderLine), pHttpHfValue);

    return  pHttpHfValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoGetHeaderValueById2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header field id to be matched.

                ULONG                       ulIndex
                The index to the multiple headers.

    return:     header field value.

**********************************************************************/

char*
HttpBmoGetHeaderValueById2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
#endif
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHttpHfValue = NULL;

    pHttpHfo =
        (PHTTP_HEADER_FIELD)pMyObject->GetHeaderField2
            (
                (ANSC_HANDLE)pMyObject,
                ulHeaderId,
                ulIndex
            );

    if ( !pHttpHfo )
    {
        return  NULL;
    }
    else if ( !(pHttpHfo->Flags & HTTP_FIELD_FLAG_LINE_PRESENT) )
    {
        pHfpIf->BuildHeader
                (
                    pHfpIf->hOwnerContext,
                    (ANSC_HANDLE)pHttpHfo,
                    NULL,
                    0
                );
    }

    AnscHttpGetHfValue(pHttpHfo->HeaderLine, AnscSizeOfString(pHttpHfo->HeaderLine), pHttpHfValue);

    return  pHttpHfValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoGetHeaderValueByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the header name to be matched.

    return:     header field value.

**********************************************************************/

char*
HttpBmoGetHeaderValueByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
#endif
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHttpHfValue = NULL;
    char*                           pHttpHfColon = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHeaderId   = HTTP_HEADER_ID_RESERVED;
    ULONG                           i            = 0;;

    ulHeaderId =
#ifdef _CCSP_CWMP_TCP_CONNREQ_HANDLER
        AnscHttpGetHeaderCode(name);
#else
        pHttpHco->GetHeaderId
            (
                (ANSC_HANDLE)pHttpHco,
                name
            );
#endif

    if ( ulHeaderId != HTTP_HEADER_ID_RESERVED )
    {
        return  pMyObject->GetHeaderValueById((ANSC_HANDLE)pMyObject, ulHeaderId);
    }

    AnscAcquireLock(&pMyObject->HfoTableLock);

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[i]);

        while ( pSLinkEntry )
        {
            pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( !(pHttpHfo->Flags & HTTP_FIELD_FLAG_LINE_PRESENT) )
            {
                continue;
            }
            else if ( AnscSizeOfString(pHttpHfo->HeaderLine) < AnscSizeOfString(name) )
            {
                continue;
            }
            else
            {
                pHttpHfColon = _ansc_strchr(pHttpHfo->HeaderLine, ':');
            }

            if ( !pHttpHfColon )
            {
                continue;
            }
            else if ( ((ULONG)pHttpHfColon - (ULONG)pHttpHfo->HeaderLine) != AnscSizeOfString(name) )
            {
                continue;
            }
            else if ( AnscEqualString2
                        (
                            pHttpHfo->HeaderLine,
                            name,
                            AnscSizeOfString(name),
                            FALSE
                        ) )
            {
                AnscReleaseLock(&pMyObject->HfoTableLock);

                AnscHttpGetHfValue(pHttpHfo->HeaderLine, AnscSizeOfString(pHttpHfo->HeaderLine), pHttpHfValue);

                return  pHttpHfValue;
            }
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoGetHeaderValueByName2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the header name to be matched.

                ULONG                       ulIndex

    return:     header field value.

**********************************************************************/

char*
HttpBmoGetHeaderValueByName2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulIndex
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
#endif
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHttpHfValue = NULL;
    char*                           pHttpHfColon = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHeaderId   = HTTP_HEADER_ID_RESERVED;
    ULONG                           i            = 0;
    ULONG                           ulCount      = 0;

    ulHeaderId =
#ifdef _CCSP_CWMP_TCP_CONNREQ_HANDLER
        AnscHttpGetHeaderCode(name);
#else
        pHttpHco->GetHeaderId
            (
                (ANSC_HANDLE)pHttpHco,
                name
            );
#endif

    if ( ulHeaderId != HTTP_HEADER_ID_RESERVED )
    {
        return  pMyObject->GetHeaderValueById2((ANSC_HANDLE)pMyObject, ulHeaderId, ulIndex);
    }

    AnscAcquireLock(&pMyObject->HfoTableLock);

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[i]);

        while ( pSLinkEntry )
        {
            pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( !(pHttpHfo->Flags & HTTP_FIELD_FLAG_LINE_PRESENT) )
            {
                continue;
            }
            else if ( AnscSizeOfString(pHttpHfo->HeaderLine) < AnscSizeOfString(name) )
            {
                continue;
            }
            else
            {
                pHttpHfColon = _ansc_strchr(pHttpHfo->HeaderLine, ':');
            }

            if ( !pHttpHfColon )
            {
                continue;
            }
            else if ( ((ULONG)pHttpHfColon - (ULONG)pHttpHfo->HeaderLine) != AnscSizeOfString(name) )
            {
                continue;
            }
            else if ( AnscEqualString2
                        (
                            pHttpHfo->HeaderLine,
                            name,
                            AnscSizeOfString(name),
                            FALSE
                        ) )
            {
                if ( ulCount != ulIndex )
                {
                    ulCount ++;
                    continue;
                }

                AnscReleaseLock(&pMyObject->HfoTableLock);

                AnscHttpGetHfValue(pHttpHfo->HeaderLine, AnscSizeOfString(pHttpHfo->HeaderLine), pHttpHfValue);

                return  pHttpHfValue;
            }
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    return  NULL;
}

#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSetHeaderValueById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId,
                char*                       value
            );

    description:

        This function is called to configure a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header field id to be added.

                char*                       value
                Specifies the header value to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSetHeaderValueById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        char*                       value
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHeaderName  = NULL;

    pHeaderName =
        pHttpHco->GetHeaderName
            (
                (ANSC_HANDLE)pHttpHco,
                ulHeaderId
            );

    if ( !pHeaderName )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pMyObject->SetHeaderValueByName
                (
                    (ANSC_HANDLE)pMyObject,
                    pHeaderName,
                    value
                );
    }

    return  returnStatus;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoSetHeaderValueByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       value
            );

    description:

        This function is called to configure a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the header name to be added.

                char*                       value
                Specifies the header value to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoSetHeaderValueByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       value
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT   )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
#endif
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    char*                           pHfLine      = (char*)pMyObject->ScratchPad1;
    ULONG                           ulLineSize   = 0;
    errno_t                         rc           = -1;

    if ( (AnscSizeOfString(name) + AnscSizeOfString(value) + 4) >= pMyObject->PadSize1 )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        rc = strcpy_s(pHfLine, sizeof(pMyObject->ScratchPad1), name);
        ERR_CHK(rc);

        ulLineSize  = AnscSizeOfString(name);
        pHfLine[ulLineSize + 0] = HTTP_COLON;
        pHfLine[ulLineSize + 1] = HTTP_SPACE;
        ulLineSize += 2;

        rc = strcpy_s(pHfLine + ulLineSize, (sizeof(pMyObject->ScratchPad1) - ulLineSize), value);
        ERR_CHK(rc);

        ulLineSize += AnscSizeOfString(value);
        pHfLine[ulLineSize + 0] = HTTP_CARRIAGE_RETURN;
        pHfLine[ulLineSize + 1] = HTTP_LINE_FEED;
    }

    pHttpHfo =
        (PHTTP_HEADER_FIELD)pHfpIf->ParseHeader
            (
                pHfpIf->hOwnerContext,
                pHfLine,
                ulLineSize
            );

    if ( !pHttpHfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pMyObject->AddHeaderField
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pHttpHfo
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoDelStartLine
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean start line information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoDelStartLine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoParseStartLine
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to parse start line information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoParseStartLine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoClearHeaders
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean the header buffer descriptor
        along with all the header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoClearHeaders
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

    pMyObject->DelStartLine((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllHfos  ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoParseHeaders
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to parse the HTTP header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoParseHeaders
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject     = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo    = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;
    PHTTP_HEADER_FIELD              pHttpHfo      = NULL;
    PVOID                           pHeaderBuffer = NULL;
    ULONG                           ulBufferSize  = 0;
    char*                           pHfStart      = NULL;
    ULONG                           ulSkipSize    = 0;
    char*                           pRawHfLine    = NULL;
    char*                           pStdHfLine    = (char*)pMyObject->ScratchPad1;
    ULONG                           ulRawLineSize = 0;
    ULONG                           ulStdLineSize = 0;

    pMyObject->DelStartLine((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllHfos  ((ANSC_HANDLE)pMyObject);

    returnStatus = pMyObject->ParseStartLine((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else if ( !pHeaderBdo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pHeaderBuffer = AnscBdoGetBlock    (pHeaderBdo);
        ulBufferSize  = AnscBdoGetBlockSize(pHeaderBdo);
    }

    AnscHttpFindHfStart(pHeaderBuffer, ulBufferSize, pHfStart);

    if ( !pHfStart )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulSkipSize    = (ULONG)pHfStart - (ULONG)pHeaderBuffer;
        ulBufferSize -= ulSkipSize;
        pRawHfLine    = pHfStart;
    }

    /*
     * Skip the first line, which is the start line: request-line in client message and status-line
     * in server message.
     */
    AnscHttpGetHfLineSize(pRawHfLine, ulBufferSize, ulRawLineSize);

    pRawHfLine   += ulRawLineSize;
    ulBufferSize -= ulRawLineSize;

    /*
     * We don't have to verify the completeness of the header fields, since the caller SHOULD have
     * done so already. We create a separate HTTP Header Field Object for each header line and add
     * it into the distributed hash table. The end of header fields is signalled by the presece of
     * a CRLF pair.
     */
    while ( (ulBufferSize > 0) && pRawHfLine && !AnscHttpIsCr(*pRawHfLine) && !AnscHttpIsLf(*pRawHfLine) )
    {
        AnscHttpGetHfLineSize(pRawHfLine, ulBufferSize, ulRawLineSize);

        if ( ulRawLineSize <= pMyObject->PadSize1 )
        {
            AnscHttpPrepareHeader(pRawHfLine, ulRawLineSize, pStdHfLine, ulStdLineSize);

            pStdHfLine[ulStdLineSize + 0] = HTTP_CARRIAGE_RETURN;
            pStdHfLine[ulStdLineSize + 1] = HTTP_LINE_FEED;

            pHttpHfo =
                (PHTTP_HEADER_FIELD)pHfpIf->ParseHeader
                    (
                        pHfpIf->hOwnerContext,
                        pStdHfLine,
                        ulStdLineSize
                    );

            if ( pHttpHfo )
            {
                returnStatus =
                    pMyObject->AddHeaderField
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pHttpHfo
                        );
            }
        }

        pRawHfLine   += ulRawLineSize;
        ulBufferSize -= ulRawLineSize;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoCopyStartLineFrom
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the start line from the
        internal buffer into an external buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the data to be
                copied to.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied payload data as
                the outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoCopyStartLineFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(buffer);
    ULONG                           ulStartLineSize = 0;
    
    *pulSize = ulStartLineSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoCopyHeadersFrom
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the start line from the
        internal buffer into an external buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the data to be
                copied to.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied payload data as
                the outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoCopyHeadersFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject     = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo    = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;
    PHTTP_HEADER_FIELD              pHttpHfo      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulHeadersSize = 0;
    ULONG                           ulCopySize    = 0;
    ULONG                           ulLeftSize    = *pulSize;
    char*                           pHfStream     = (char*)buffer;
    ULONG                           i             = 0;

    if ( *pulSize < pMyObject->GetHeadersSize((ANSC_HANDLE)pMyObject) )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }
    else if ( pHeaderBdo )
    {
        AnscCopyMemory
            (
                buffer,
                AnscBdoGetBlock    (pHeaderBdo),
                AnscBdoGetBlockSize(pHeaderBdo)
            );

        *pulSize = AnscBdoGetBlockSize(pHeaderBdo);

        return  ANSC_STATUS_SUCCESS;
    }

    ulCopySize   = ulLeftSize;
    pHfStream    = (char*)((ULONG)buffer + ulHeadersSize);
    returnStatus =
        pMyObject->CopyStartLineFrom
            (
                (ANSC_HANDLE)pMyObject,
                (PVOID)pHfStream,
                &ulCopySize
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pHfStream     += ulCopySize;
        ulHeadersSize += ulCopySize;
        ulLeftSize    -= ulCopySize;

        pHfStream[0] = HTTP_CARRIAGE_RETURN;
        pHfStream[1] = HTTP_LINE_FEED;

        pHfStream     += 2;
        ulHeadersSize += 2;
        ulLeftSize    -= 2;
    }

    AnscAcquireLock(&pMyObject->HfoTableLock);

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[i]);

        while ( pSLinkEntry )
        {
            pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            ulCopySize  = pHfpIf->GetHeaderSize(pHfpIf->hOwnerContext, (ANSC_HANDLE)pHttpHfo);

            if ( ulCopySize > 0 )
            {
                returnStatus =
                    pHfpIf->BuildHeader
                        (
                            pHfpIf->hOwnerContext,
                            (ANSC_HANDLE)pHttpHfo,
                            (PVOID)pHfStream,
                            ulCopySize
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
		    /*CID: 137590 Missing unlock*/
		    AnscReleaseLock(&pMyObject->HfoTableLock);
                    return  returnStatus;
                }
                else
                {
                    pHfStream     += ulCopySize;
                    ulHeadersSize += ulCopySize;
                    ulLeftSize    -= ulCopySize;

                    pHfStream[0] = HTTP_CARRIAGE_RETURN;
                    pHfStream[1] = HTTP_LINE_FEED;

                    pHfStream     += 2;
                    ulHeadersSize += 2;
                    ulLeftSize    -= 2;
                }
            }
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    pHfStream[0] = HTTP_CARRIAGE_RETURN;
    pHfStream[1] = HTTP_LINE_FEED;

    pHfStream     += 2;
    ulHeadersSize += 2;
    ulLeftSize    -= 2;

    *pulSize = ulHeadersSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoGetHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header id to be matched.

    return:     header field object.

**********************************************************************/

ANSC_HANDLE
HttpBmoGetHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulHeaderId, HTTP_BMO_HFO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->HfoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pHttpHfo->HeaderId == ulHeaderId )
        {
            AnscReleaseLock(&pMyObject->HfoTableLock);

            return  (ANSC_HANDLE)pHttpHfo;
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoGetHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called to retrieve a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header id to be matched.

                ULONG                       ulIndex
                The index to multiple headers.

    return:     header field object.

**********************************************************************/

ANSC_HANDLE
HttpBmoGetHeaderField2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulHeaderId, HTTP_BMO_HFO_TABLE_SIZE);
    ULONG                           ulCount      = 0;

    AnscAcquireLock(&pMyObject->HfoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->HfoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pHttpHfo->HeaderId == ulHeaderId )
        {
            if ( ulCount != ulIndex )
            {
                ulCount ++;
                continue;
            }

            AnscReleaseLock(&pMyObject->HfoTableLock);

            return  (ANSC_HANDLE)pHttpHfo;
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoAddHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHfo
            );

    description:

        This function is called to add a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHfo
                Specifies the header field object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoAddHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HEADER_FIELD              pHttpHfo     = (PHTTP_HEADER_FIELD        )hHfo;
    ULONG                           ulHashIndex  = AnscHashUlong(pHttpHfo->HeaderId, HTTP_BMO_HFO_TABLE_SIZE);

    pHttpHfo->HashIndex = ulHashIndex;

    AnscAcquireLock   (&pMyObject->HfoTableLock);
    AnscQueuePushEntry(&pMyObject->HfoTable[ulHashIndex], &pHttpHfo->Linkage);
    AnscReleaseLock   (&pMyObject->HfoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoDelHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called to delete a HTTP header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header id to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoDelHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulHeaderId, HTTP_BMO_HFO_TABLE_SIZE);
    ULONG                           ulIndex      = 0;

    while ( TRUE )
    {
        pHttpHfo =
            (PHTTP_HEADER_FIELD)pMyObject->GetHeaderField2
                (
                    (ANSC_HANDLE)pMyObject,
                    ulHeaderId,
                    ulIndex
                );

        if ( !pHttpHfo )
        {
            if ( ulIndex == 0 )
            {
                return  ANSC_STATUS_CANT_FIND;
            }
            else
            {
                return ANSC_STATUS_SUCCESS;
            }
        }

        AnscAcquireLock        (&pMyObject->HfoTableLock);
        AnscQueuePopEntryByLink(&pMyObject->HfoTable[ulHashIndex], &pHttpHfo->Linkage);
        AnscReleaseLock        (&pMyObject->HfoTableLock);

        AnscFreeMemory(pHttpHfo);

        ulIndex ++;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoDelAllHfos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all the header fields.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoDelAllHfos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->HfoTableLock);

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->HfoTable[i]);

        while ( pSLinkEntry )
        {
            pHttpHfo    = ACCESS_HTTP_HEADER_FIELD(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->HfoTable[i]);

            AnscFreeMemory(pHttpHfo);
        }
    }

    AnscReleaseLock(&pMyObject->HfoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
