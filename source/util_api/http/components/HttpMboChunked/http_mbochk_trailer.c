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

    module:	http_mbochk_trailer.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Mbo Chunked Object.

        *   HttpMboChkAppendTrailer
        *   HttpMboChkParseTrailer

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

        ANSC_STATUS
        HttpMboChkAppendTrailer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRawTrailer,
                ULONG                       ulTrailerSize
            );

    description:

        This function is called to append the trailer data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRawTrailer
                Specifies the text buffer holding the raw trailer line.

                ULONG                       ulTrailerSize
                Specifies the size of the raw trailer line.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkAppendTrailer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject    = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = NULL;

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                ulTrailerSize * 2,
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
                pRawTrailer,
                ulTrailerSize
            );

        AnscBdoSetBlockSize(pBufferDesp, ulTrailerSize            );
        AnscBdoSetReserved1(pBufferDesp, HTTP_MBOCHK_BTYPE_TRAILER);
    }

    pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pBufferDesp);

    returnStatus =
        pMyObject->ParseTrailer
            (
                (ANSC_HANDLE)pMyObject,
                pRawTrailer,
                ulTrailerSize
            );

    returnStatus =
        pBccIf->Notify
            (
                pBccIf->hOwnerContext,
                HTTP_BCC_EVENT_ARRIVED_BODY,
                (ANSC_HANDLE)pBufferDesp
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboChkParseTrailer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRawTrailer,
                ULONG                       ulTrailerSize
            );

    description:

        This function is called to parse the trailer data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRawTrailer
                Specifies the text buffer holding the raw trailer line.

                ULONG                       ulTrailerSize
                Specifies the size of the raw trailer line.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboChkParseTrailer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawTrailer,
        ULONG                       ulTrailerSize
    )
{
    PHTTP_MBO_CHUNKED_OBJECT        pMyObject     = (PHTTP_MBO_CHUNKED_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE     )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf        = (PHTTP_BCC_INTERFACE     )pMyObject->hBccIf;
    PHTTP_HEADER_FIELD              pHttpHfo      = NULL;
    char*                           pHfStart      = (char*)pRawTrailer;
    char*                           pRawHfLine    = (char*)pRawTrailer;
    char*                           pStdHfLine    = (char*)pMyObject->ScratchPad2;
    ULONG                           ulRawLineSize = 0;
    ULONG                           ulStdLineSize = 0;
    ULONG                           ulBufferSize  = ulTrailerSize;

    if ( ulTrailerSize == 2 )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pHfStart   = pRawTrailer;
        pRawHfLine = pHfStart;
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

            if ( !pHttpHfo )
            {
                return  ANSC_STATUS_BAD_PAYLOAD;
            }
            else
            {
                pBccIf->AddHeaderField
                        (
                            pBccIf->hOwnerContext,
                            (ANSC_HANDLE)pHttpHfo
                        );
            }
        }

        pRawHfLine   += ulRawLineSize;
        ulBufferSize -= ulRawLineSize;
    }

    return  ANSC_STATUS_SUCCESS;
}
