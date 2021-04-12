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

    module:	web_roofsm_do_stuff.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Roo Fsm Object.

        *   WebRooFsmDoGet
        *   WebRooFsmDoHead
        *   WebRooFsmDoPost

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#include "web_roofsm_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooFsmDoGet
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooFsmDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PWEB_ROO_FSM_OBJECT             pMyObject             = (PWEB_ROO_FSM_OBJECT        )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq               = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep               = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PANSC_TOKEN_CHAIN               pPathTokenChain       = (PANSC_TOKEN_CHAIN          )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    ANSC_HANDLE                     hMappedFile           = (ANSC_HANDLE                )NULL;
    PHTTP_HFO_CONTENT_LENGTH        pHttpHfoContentLength = NULL;
    PHTTP_HFO_CONTENT_TYPE          pHttpHfoContentType   = NULL;
    PVOID                           pFileContent          = NULL;
    ULONG                           ulFileSize            = 0;
    ULONG                           ulMimeMediaType       = 0;
    ULONG                           ulMimeSubType         = 0;

    hMappedFile =
        pMyObject->MapDiskFile
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pPathTokenChain
            );

    if ( !hMappedFile )
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_NOT_FOUND,
                    (ANSC_HANDLE)NULL
                );

        return  returnStatus;
    }
    else
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_OK,
                    (ANSC_HANDLE)NULL
                );
    }

    /*
     * The Content-Length entity-header field indicates the size of the entity-body, in decimal
     * number of OCTETs, sent to the recipient or, in the case of the HEAD method, the size of the
     * entity-body that would have been sent had the request been a GET.
     *
     *      Content-Length    = "Content-Length" ":" 1*DIGIT
     *
     * An example is
     *
     *      Content-Length: 3495
     *
     * Applications SHOULD use this field to indicate the transfer-length of the message-body,
     * unless this is prohibited by the rules.
     */
    pHttpHfoContentLength = (PHTTP_HFO_CONTENT_LENGTH)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LENGTH));

    if ( !pHttpHfoContentLength )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoContentLength->HashIndex = 0;
        pHttpHfoContentLength->Flags     = 0;
        pHttpHfoContentLength->HeaderId  = HTTP_HEADER_ID_CONTENT_LENGTH;

        pHttpHfoContentLength->BodySize  = AnscGetFileSize(hMappedFile);
    }

    returnStatus =
        pBmoRep->AddHeaderField
            (
                (ANSC_HANDLE)pBmoRep,
                (ANSC_HANDLE)pHttpHfoContentLength
            );

    /*
     * The Content-Type entity-header field indicates the media type of the entity-body sent to the
     * recipient or, in the case of the HEAD method, the media type that would have been sent had
     * the request been a GET.
     *
     *      Content-Type   = "Content-Type" ":" media-type
     */
    returnStatus =
        pMyObject->GetMimeType
            (
                (ANSC_HANDLE)pMyObject,
                hMappedFile,
                &ulMimeMediaType,
                &ulMimeSubType
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pHttpHfoContentType = (PHTTP_HFO_CONTENT_TYPE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_TYPE));

        if ( !pHttpHfoContentType )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoContentType->HashIndex = 0;
            pHttpHfoContentType->Flags     = 0;
            pHttpHfoContentType->HeaderId  = HTTP_HEADER_ID_CONTENT_TYPE;

            pHttpHfoContentType->MediaType = ulMimeMediaType;
            pHttpHfoContentType->SubType   = ulMimeSubType;
        }

        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoContentType
                );
    }

    /*
     * The GET method menas retrieve whatever information (in the form of an entity) is identified
     * by the Request-URI. If the Request-URI refers to a data-producing process, it is the
     * produced data which shall be returned as the entity in the response and not the source text
     * of the process, unless that text happens to be the output of the process.
     */
    /*CID :61576 Argument cannot be negative - Added negative check in def part*/
    ulFileSize = (ULONG) AnscGetFileSize(hMappedFile);
        
    if ( ulFileSize == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pFileContent = AnscAllocateMemory(ulFileSize);

        if ( !pFileContent )
        {
            returnStatus =
                pMyObject->ReplyReq
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBmoReq,
                        (ANSC_HANDLE)pBmoRep,
                        HTTP_STATUS_SERVICE_UNAVAILABLE,
                        (ANSC_HANDLE)NULL
                    );

            return  returnStatus;
        }

        AnscReadFile
            (
                hMappedFile,
                pFileContent,
                &ulFileSize
            );
    }

    returnStatus =
        pBmoRep->SaveAsBody
            (
                (ANSC_HANDLE)pBmoRep,
                pFileContent,
                ulFileSize
            );

    AnscCloseFile(hMappedFile);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooFsmDoHead
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooFsmDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_ROO_FSM_OBJECT             pMyObject       = (PWEB_ROO_FSM_OBJECT        )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_OK,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooFsmDoPost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooFsmDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_ROO_FSM_OBJECT             pMyObject       = (PWEB_ROO_FSM_OBJECT        )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 3;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}
