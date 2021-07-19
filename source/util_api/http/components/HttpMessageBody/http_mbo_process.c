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

    module:	http_mbo_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Message Body Object.

        *   HttpMboExamine
        *   HttpMboProcess
        *   HttpMboCloseUp
        *   HttpMboOutput
        *   HttpMboRemoveCoding

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


#include "http_mbo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboExamine
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
HttpMboExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject           = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE      )pMyObject->hBccIf;
    ULONG                           ulTestMboState      = HTTP_MBO_STATE_EMPTY;
    ULONG                           ulPredictedBodySize = 0;
    ULONG                           ulArrivedBodySize   = 0;

    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                ulPredictedBodySize = pBccIf   ->PredictBodySize(pBccIf->hOwnerContext );
                ulArrivedBodySize   = pMyObject->GetBodySize    ((ANSC_HANDLE)pMyObject);

                if (ulPredictedBodySize == 0xFFFFFFFF )
                {
                    ulTestMboState = HTTP_MBO_STATE_PART_BODY;
                }
                else if ( ulPredictedBodySize == (ulArrivedBodySize + ulSize) )
                {
                    ulTestMboState = HTTP_MBO_STATE_COMPLETE;
                }
                else if ( ulPredictedBodySize < (ulArrivedBodySize + ulSize) )
                {
                    ulTestMboState = HTTP_MBO_STATE_OVER_PACKED;
                }
                else
                {
                    ulTestMboState = HTTP_MBO_STATE_PART_BODY;
                }

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
        HttpMboProcess
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
HttpMboProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject           = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf              = (PHTTP_HFP_INTERFACE      )pMyObject->hHfpIf;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE      )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR  )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pNewBodyBdo         = NULL;
    PANSC_BUFFER_DESCRIPTOR         pExtraBdo           = NULL;
    ULONG                           ulPredictedBodySize = 0;
    ULONG                           ulArrivedBodySize   = 0;
    ULONG                           ulTotalPackedSize   = 0;
    PHTTP_BASIC_MESSAGE_OBJECT      pBmo                = (PHTTP_BASIC_MESSAGE_OBJECT)pMyObject->hOwnerContext;
    errno_t                         rc                  = -1;

    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                if ( pBmo->Oid == HTTP_BMO_REQ_OID )
                {
                    PHTTP_FUM_INTERFACE         pFumIf       = (PHTTP_FUM_INTERFACE )pMyObject->hFumIf;
                    PHTTP_BMO_REQ_OBJECT        pBmoRequest  = (PHTTP_BMO_REQ_OBJECT)pBmo;
                    PHTTP_RCP_INTERFACE         pRcpIf       = (PHTTP_RCP_INTERFACE          )pBmoRequest->hRcpIf;
                    PUCHAR                      pReqUri      = (PUCHAR)pRcpIf->GetPathInfo(pRcpIf->hOwnerContext, (ANSC_HANDLE)pBmoRequest);
                    PHTTP_MDH_INTERFACE         pMdhIf       = (PHTTP_MDH_INTERFACE )pFumIf->GetMdhIf(pFumIf->hOwnerContext, pReqUri);

                    if ( pMdhIf )
                    {
                        PHTTP_HFO_CONTENT_TYPE  pContentType;

                        pContentType =
                            (PHTTP_HFO_CONTENT_TYPE)pBccIf->GetHeaderField
                                (
                                    pBccIf->hOwnerContext,
                                    HTTP_HEADER_ID_CONTENT_TYPE
                                );

                        if ( (pMyObject->Mode != HTTP_MBO_MODE_STORE_EXTERNAL || pMyObject->pBoundaryStr == NULL ) &&
                             (pContentType->MediaType == IANA_MEDIA_TYPE_CODE_MULTIPART ) &&
                             (pContentType->SubType   == IANA_MT_MP_STYPE_CODE_FORM_DATA) )
                        {
                            PUCHAR      pBoundaryStr = NULL;

                            pBoundaryStr = (PUCHAR)
                                pHfpIf->GetHeaderParam
                                    (
                                        pHfpIf->hOwnerContext,
                                        pContentType->Parameters,
                                        "boundary"
                                    );

                            if ( pBoundaryStr )
                            {
                                pMyObject->pBoundaryStr = (PUCHAR)AnscAllocateMemory(2 + AnscSizeOfString((const char*)pBoundaryStr) + 1);
                                if ( !pMyObject->pBoundaryStr )
                                {
                                    returnStatus = ANSC_STATUS_RESOURCES;
                                }
                                else
                                {
                                    pMyObject->SetMode((ANSC_HANDLE)pMyObject, HTTP_MBO_MODE_STORE_EXTERNAL);

                                    pMyObject->pBoundaryStr[0] = pMyObject->pBoundaryStr[1] = '-';
                                    rc = strcpy_s((char*)(pMyObject->pBoundaryStr + 2), (2 + AnscSizeOfString((const char*)pBoundaryStr) + 1), (char*)pBoundaryStr);/*Need to confirm*/
                                    ERR_CHK(rc);

                                    returnStatus = pMdhIf->Begin((ANSC_HANDLE)pMdhIf, pBmo->GetWebSessionId((ANSC_HANDLE)pBmo));
                                }

                                AnscFreeMemory(pBoundaryStr);

                                if ( returnStatus != ANSC_STATUS_SUCCESS )
                                {
                                    return returnStatus;
                                }
                            }
                        }
                    }
                }

                /*
                 * If we're currently in these states, we're expecting more body parts for this
                 * message. Three possibilities need to be considered:
                 *
                 *      (1) The received buffer descriptor only contains body parts for
                 *          the current message, and there're more parts to come. Only
                 *          one event must be indicated: event_arrived_body_part.
                 *
                 *      (2) The payload contained by the buffer completes the message
                 *          body we've been waiting for, and it does so without any bonus
                 *          data. Two events must be indicated: event_arrived_body and
                 *          event_completed.
                 *
                 *      (3) Not only the payload completes the current message body, it
                 *          also carries data for the next transaction. Two events must
                 *          be indicated: event_arrived_body and event_over_packed.
                 */
                ulPredictedBodySize = pBccIf   ->PredictBodySize(pBccIf->hOwnerContext );
                ulArrivedBodySize   = pMyObject->GetBodySize    ((ANSC_HANDLE)pMyObject);
                ulTotalPackedSize   = ulArrivedBodySize + AnscBdoGetBlockSize(pBufferDesp);

                if ( ulPredictedBodySize == ulTotalPackedSize )
                {
                    pNewBodyBdo = pBufferDesp;
                    pExtraBdo   = NULL;

                    pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pNewBodyBdo);

                    pMyObject->State = HTTP_MBO_STATE_COMPLETE;
                    returnStatus     =
                        pBccIf->Notify
                            (
                                pBccIf->hOwnerContext,
                                HTTP_BCC_EVENT_ARRIVED_BODY,
                                (ANSC_HANDLE)pNewBodyBdo
                            );

                    returnStatus     =
                        pBccIf->Notify
                            (
                                pBccIf->hOwnerContext,
                                HTTP_BCC_EVENT_COMPLETED,
                                (ANSC_HANDLE)pNewBodyBdo
                            );
                }
                else if ( ulPredictedBodySize > ulTotalPackedSize )
                {
                    pNewBodyBdo = pBufferDesp;
                    pExtraBdo   = NULL;

                    returnStatus = pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pNewBodyBdo);

                    if ( returnStatus == ANSC_STATUS_SUCCESS )
                    {
                        pMyObject->State = HTTP_MBO_STATE_PART_BODY;
                        returnStatus     =
                            pBccIf->Notify
                                (
                                    pBccIf->hOwnerContext,
                                    HTTP_BCC_EVENT_ARRIVED_BODY_PART,
                                    (ANSC_HANDLE)pNewBodyBdo
                                );
                    }
                }
                else
                {
                    returnStatus =
                        AnscBdoBreakAt
                            (
                                (ANSC_HANDLE)pBufferDesp,
                                ulPredictedBodySize - ulArrivedBodySize,
                                (PANSC_HANDLE)&pNewBodyBdo,
                                (PANSC_HANDLE)&pExtraBdo
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                        return  returnStatus;
                    }
                    else
                    {
                        pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pNewBodyBdo);
                    }

                    pMyObject->State = HTTP_MBO_STATE_OVER_PACKED;
                    returnStatus     =
                        pBccIf->Notify
                            (
                                pBccIf->hOwnerContext,
                                HTTP_BCC_EVENT_ARRIVED_BODY,
                                (ANSC_HANDLE)pNewBodyBdo
                            );

                    returnStatus     =
                        pBccIf->Notify
                            (
                                pBccIf->hOwnerContext,
                                HTTP_BCC_EVENT_OVER_PACKED,
                                (ANSC_HANDLE)pExtraBdo
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
                //CID:72117 Missing break in switch
		break;
        default :

                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboCloseUp
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
HttpMboCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject           = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf              = (PHTTP_BCC_INTERFACE      )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR  )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pNewBodyBdo         = NULL;
    
    switch ( pMyObject->State )
    {
        case    HTTP_MBO_STATE_EMPTY :
        case    HTTP_MBO_STATE_PART_BODY :

                /*
                 * Apparently, we couldn't predict the body size either because the sending party
                 * didn't indicate the body length (i.e., Content-Length header field was not found
                 * in the message) or because we don't understand the Transfer-Encoding mechanism
                 * used by the sending party (e.g., "chunked" encoding may cause problem for us).
                 * Either way, closing-connection signals the end of message body.
                 */
                if ( pBufferDesp )
                {
                    if ( AnscBdoGetBlockSize(pBufferDesp) > 0 )
                    {
                        pNewBodyBdo = pBufferDesp;

                        pMyObject->AppendBodyBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pNewBodyBdo);
                    }
                    else
                    {
                        pNewBodyBdo = NULL;

                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }
                }
                else
                {
                    pNewBodyBdo = NULL;
                }

                pMyObject->State = HTTP_MBO_STATE_COMPLETE;
                returnStatus     =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_ARRIVED_BODY,
                            (ANSC_HANDLE)pNewBodyBdo
                        );

                returnStatus     =
                    pBccIf->Notify
                        (
                            pBccIf->hOwnerContext,
                            HTTP_BCC_EVENT_COMPLETED,
                            (ANSC_HANDLE)pNewBodyBdo
                        );

                break;

        case    HTTP_MBO_STATE_COMPLETE :
        case    HTTP_MBO_STATE_OVER_PACKED :

                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                returnStatus = ANSC_STATUS_UNAPPLICABLE;
               // CID: 71131 Missing break in switch
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
        HttpMboOutput
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSerializeContext
            );

    description:

        This function is called to serialize the current HTTP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSerializeContext
                Specifies the serialize context for output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboOutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject         = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf            = (PHTTP_BCC_INTERFACE      )pMyObject->hBccIf;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo          = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = NULL;
    
    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        pBccIf->Serialize
                (
                    pBccIf->hOwnerContext,
                    AnscBdoGetBlock    (pBodyBdo),
                    AnscBdoGetBlockSize(pBodyBdo),
                    hSerializeContext
                );
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboRemoveCoding
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
HttpMboRemoveCoding
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
