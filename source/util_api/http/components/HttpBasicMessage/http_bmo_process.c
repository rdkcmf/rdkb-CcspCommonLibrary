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

    module:	http_bmo_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Basic Message Object.

        *   HttpBmoExamine
        *   HttpBmoProcess
        *   HttpBmoCloseUp
        *   HttpBmoProduce
        *   HttpBmoOutputHeaders
        *   HttpBmoOutputBody

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

        ULONG
        HttpBmoExamine
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
HttpBmoExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject      = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody   = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    ULONG                           ulTestBmoState = HTTP_BMO_STATE_EMPTY;
    ULONG                           ulTestMboState = HTTP_MBO_STATE_EMPTY;
    BOOL                            bHfComplete    = FALSE;

    switch ( pMyObject->State )
    {
        case    HTTP_BMO_STATE_EMPTY :
        case    HTTP_BMO_STATE_PART_HEADER :

                AnscHttpIsHfComplete(buffer, ulSize, bHfComplete);

                if ( bHfComplete )
                {
                    ulTestBmoState = HTTP_BMO_STATE_HEADER_ARRIVED;
                }
                else
                {
                    ulTestBmoState = HTTP_BMO_STATE_PART_HEADER;
                }

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :
        case    HTTP_BMO_STATE_PART_BODY :

                if ( !pMessageBody )
                {
                    ulTestBmoState = HTTP_BMO_STATE_UNKNOWN;
                }
                else
                {
                    ulTestMboState =
                        pMessageBody->Examine
                            (
                                (ANSC_HANDLE)pMessageBody,
                                buffer,
                                ulSize
                            );

                    if ( ulTestMboState == HTTP_MBO_STATE_COMPLETE )
                    {
                        ulTestBmoState = HTTP_BMO_STATE_COMPLETE;
                    }
                    else if ( ulTestMboState == HTTP_MBO_STATE_OVER_PACKED )
                    {
                        ulTestBmoState = HTTP_BMO_STATE_OVER_PACKED;
                    }
                    else
                    {
                        ulTestBmoState = HTTP_BMO_STATE_PART_BODY;
                    }
                }

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                ulTestBmoState = HTTP_BMO_STATE_OVER_PACKED;

        default :

                break;
    }

    return  ulTestBmoState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoProcess
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
HttpBmoProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhIf       = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhIf;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = (PANSC_BUFFER_DESCRIPTOR   )hBdo;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo   = NULL;
    PANSC_BUFFER_DESCRIPTOR         pNewBodyBdo  = NULL;
    ULONG                           ulHeaderSize = 0;

    switch ( pMyObject->State )
    {
        case    HTTP_BMO_STATE_EMPTY :
        case    HTTP_BMO_STATE_PART_HEADER :

                AnscHttpGetHfSize
                    (
                        AnscBdoGetBlock    (pBufferDesp),
                        AnscBdoGetBlockSize(pBufferDesp),
                        ulHeaderSize
                    );

                if ( ulHeaderSize == AnscBdoGetBlockSize(pBufferDesp) )
                {
                    /*
                     * The buffer descriptor only contains the HTTP header fields, not even one bit
                     * of the message body (chances are that this message doesn't have a body). Two
                     * events must be indicated if no body will follow:
                     *
                     *      $ event_arrived_header
                     *      $ event_completed
                     *
                     * If the body size cannot be predicted or can be predicted to have a non-zero
                     * size, two events must be indicated:
                     *
                     *      $ event_arrived_header
                     *      $ event_end_of_header
                     */
                    pHeaderBdo = pBufferDesp;

                    returnStatus = pMyObject->SetHeaderBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pHeaderBdo);
                    returnStatus = pMyObject->ParseHeaders((ANSC_HANDLE)pMyObject);

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
#ifdef   _ANSC_HTTP_SUPPORT_PIPELING
                        return  returnStatus;
#else
                        pTmhIf->Notify
                            (
                                pTmhIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                HTTP_TMH_EVENT_GARBAGE_DATA_DROPPED,
                                (ANSC_HANDLE)pHeaderBdo
                            );

                        return  ANSC_STATUS_SUCCESS;    /* silient drop garbage data */
#endif
                    }

                    returnStatus =
                        pTmhIf->Notify
                            (
                                pTmhIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                HTTP_TMH_EVENT_ARRIVED_HEADER,
                                (ANSC_HANDLE)pHeaderBdo
                            );

                    if ( pMyObject->MboMode == HTTP_MBO_MODE_EMPTY )
                    {
                        pMyObject->State = HTTP_BMO_STATE_COMPLETE;
                        returnStatus     =
                            pTmhIf->Notify
                                (
                                    pTmhIf->hOwnerContext,
                                    (ANSC_HANDLE)pMyObject,
                                    HTTP_TMH_EVENT_COMPLETED,
                                    (ANSC_HANDLE)pHeaderBdo
                                );
                    }
                    else if ( pMyObject->PredictBodySize((ANSC_HANDLE)pMyObject) > 0 )
                    {
                        pMyObject->State = HTTP_BMO_STATE_HEADER_ARRIVED;
                        returnStatus     =
                            pTmhIf->Notify
                                (
                                    pTmhIf->hOwnerContext,
                                    (ANSC_HANDLE)pMyObject,
                                    HTTP_TMH_EVENT_END_OF_HEADER,
                                    (ANSC_HANDLE)pHeaderBdo
                                );

                        pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT)pMyObject->CreateMessageBody((ANSC_HANDLE)pMyObject);

                        if ( !pMessageBody )
                        {
                            return  ANSC_STATUS_INTERNAL_ERROR;
                        }
                        else
                        {
                            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

                            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
                            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
                            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
                            pMessageBody->SetFumIf((ANSC_HANDLE)pMessageBody, pMyObject->hFumIf  );
                        }
                    }
                    else
                    {
                        pMyObject->State = HTTP_BMO_STATE_COMPLETE;
                        returnStatus     =
                            pTmhIf->Notify
                                (
                                    pTmhIf->hOwnerContext,
                                    (ANSC_HANDLE)pMyObject,
                                    HTTP_TMH_EVENT_COMPLETED,
                                    (ANSC_HANDLE)pHeaderBdo
                                );
                    }
                }
                else
                {
                    /*
                     * The buffer descriptor not only contains the HTTP header fields, but also
                     * some message body parts or part of following messages (a new feature in
                     * HTTP/1.1, messages can be pipe-lined). There're several possibilities:
                     *
                     *      (1) The payload data following the header fields is body part,
                     *          but it's not complete. Following actions MUST be taken -
                     *
                     *          $ break the original buffer descriptor into one header
                     *            descriptor and one body descriptor.
                     *          $ set the header descriptor and append body descriptor.
                     *          $ parse the headers, of course.
                     *          $ two events must be indicated: event_arrived_header and
                     *            event_arrived_body_part.
                     *
                     *      (2) The payload data following the header field is body part,
                     *          and it's complete. Following actions MUST be taken -
                     *
                     *          $ break the original buffer descriptor into one header
                     *            descriptor and one body descriptor.
                     *          $ set the header descriptor and append body descriptor.
                     *          $ parse the headers, of course.
                     *          $ three events must be indicated: event_arrived_header,
                     *            event_arrived_body, and event_completed.
                     *
                     *      (3) The payload data contains more than just message body,
                     *          some piece of following messages is also included -
                     *
                     *          $ break the original buffer descriptor into one header
                     *            descriptor, one body descriptor and one extra buffer
                     *            descriptor.
                     *          $ set the header descriptor and append body descriptor.
                     *          $ parse the headers, of course.
                     *          $ three events must be indicated: event_arrived_header,
                     *            event_arrived_body, and event_over_packed.
                     *
                     *      (4) The payload data contains no body for the current message,
                     *          but only stuff for following messages -
                     *
                     *          $ break the original buffer descriptor into one header
                     *            descriptor and one extra buffer descriptor.
                     *          $ set the header descriptor.
                     *          $ parse the headers, of course.
                     *          $ two events must be indicated: event_arrived_header and
                     *            and event_over_packed.
                     */
                    returnStatus =
                        AnscBdoBreakAt
                            (
                                (ANSC_HANDLE)pBufferDesp,
                                ulHeaderSize,
                                (PANSC_HANDLE)&pHeaderBdo,
                                (PANSC_HANDLE)&pNewBodyBdo
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                        return  returnStatus;
                    }

                    returnStatus = pMyObject->SetHeaderBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pHeaderBdo);
                    returnStatus = pMyObject->ParseHeaders((ANSC_HANDLE)pMyObject);

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        pMyObject->SetHeaderBdo((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);
                        AnscBdoIncRefCount(pBufferDesp, 1);

                        if ( pHeaderBdo )
                        {
                            AnscFreeBdo(pHeaderBdo);
                        }

                        if ( pNewBodyBdo )
                        {
                            AnscFreeBdo(pNewBodyBdo);
                        }

#ifdef   _ANSC_HTTP_SUPPORT_PIPELING
                        return  returnStatus;
#else
                        pTmhIf->Notify
                            (
                                pTmhIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                HTTP_TMH_EVENT_GARBAGE_DATA_DROPPED,
                                (ANSC_HANDLE)NULL
                            );

                        return  ANSC_STATUS_SUCCESS;    /*siliently drop garbage data */
#endif
                    }

                    returnStatus =
                        pTmhIf->Notify
                            (
                                pTmhIf->hOwnerContext,
                                (ANSC_HANDLE)pMyObject,
                                HTTP_TMH_EVENT_ARRIVED_HEADER,
                                (ANSC_HANDLE)pHeaderBdo
                            );

                    if ( pMyObject->MboMode == HTTP_MBO_MODE_EMPTY )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pNewBodyBdo);

                        pMyObject->State = HTTP_BMO_STATE_COMPLETE;
                        returnStatus     =
                            pTmhIf->Notify
                                (
                                    pTmhIf->hOwnerContext,
                                    (ANSC_HANDLE)pMyObject,
                                    HTTP_TMH_EVENT_COMPLETED,
                                    (ANSC_HANDLE)pHeaderBdo
                                );
                    }
                    else if ( pMyObject->PredictBodySize((ANSC_HANDLE)pMyObject) > 0 )
                    {
                        pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT)pMyObject->CreateMessageBody((ANSC_HANDLE)pMyObject);

                        if ( !pMessageBody )
                        {
                            AnscFreeBdo((ANSC_HANDLE)pNewBodyBdo);

                            return  ANSC_STATUS_INTERNAL_ERROR;
                        }
                        else
                        {
                            pMyObject->hMessageBody = (ANSC_HANDLE)pMessageBody;

                            pMessageBody->SetMode ((ANSC_HANDLE)pMessageBody, pMyObject->MboMode );
                            pMessageBody->SetHfpIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pHfpIf);
                            pMessageBody->SetBccIf((ANSC_HANDLE)pMessageBody, (ANSC_HANDLE)pBccIf);
                            pMessageBody->SetFumIf((ANSC_HANDLE)pMessageBody, pMyObject->hFumIf  );
                        }

                        pMyObject->State = HTTP_BMO_STATE_PART_BODY;
                        returnStatus     =
                            pMessageBody->Process
                                (
                                    (ANSC_HANDLE)pMessageBody,
                                    (ANSC_HANDLE)pNewBodyBdo
                                );
                    }
                    else
                    {
                        pMyObject->State = HTTP_BMO_STATE_OVER_PACKED;
                        returnStatus     =
                            pTmhIf->Notify
                                (
                                    pTmhIf->hOwnerContext,
                                    (ANSC_HANDLE)pMyObject,
                                    HTTP_TMH_EVENT_OVER_PACKED,
                                    (ANSC_HANDLE)pNewBodyBdo
                                );
                    }
                }

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :
        case    HTTP_BMO_STATE_PART_BODY :

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
                if ( !pMessageBody )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                    return  ANSC_STATUS_INTERNAL_ERROR;
                }

                returnStatus =
                    pMessageBody->Process
                        (
                            (ANSC_HANDLE)pMessageBody,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                pMyObject->State = HTTP_BMO_STATE_OVER_PACKED;
                returnStatus     =
                    pTmhIf->Notify
                        (
                            pTmhIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            HTTP_TMH_EVENT_OVER_PACKED,
                            (ANSC_HANDLE)pBufferDesp
                        );
        //CID: 69885 Missing break in switch
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
        HttpBmoCloseUp
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
HttpBmoCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject           = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody        = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR   )hBdo;
    
    switch ( pMyObject->State )
    {
        case    HTTP_BMO_STATE_EMPTY :
        case    HTTP_BMO_STATE_PART_HEADER :

                /*
                 * This is not a valid state under normal circumstances. If this buffer descriptor
                 * does include the rest of the header fields, the previous bmo_examine() and
                 * bmo_process() must have already incorporated it.
                 */
                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                returnStatus = ANSC_STATUS_BAD_PAYLOAD;

                break;

        case    HTTP_BMO_STATE_HEADER_ARRIVED :
        case    HTTP_BMO_STATE_PART_BODY :

                /*
                 * Apparently, we couldn't predict the body size either because the sending party
                 * didn't indicate the body length (i.e., Content-Length header field was not found
                 * in the message) or because we don't understand the Transfer-Encoding mechanism
                 * used by the sending party (e.g., "chunked" encoding may cause problem for us).
                 * Either way, closing-connection signals the end of message body.
                 */
                if ( !pMessageBody )
                {
                    if ( pBufferDesp )
                    {
                        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                    }

                    return  ANSC_STATUS_INTERNAL_ERROR;
                }

                returnStatus =
                    pMessageBody->CloseUp
                        (
                            (ANSC_HANDLE)pMessageBody,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    HTTP_BMO_STATE_COMPLETE :
        case    HTTP_BMO_STATE_OVER_PACKED :

                if ( pBufferDesp )
                {
                    AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
                }

                returnStatus = ANSC_STATUS_UNAPPLICABLE;
                //CID: 63236 Missing break in switch
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
        HttpBmoProduce
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to synthesize a message from its inter-
        mediate format to standard transfer format.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the output payload data buffer.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied message as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoProduce
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject     = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    ULONG                           ulMessageSize = pMyObject->GetMessageSize((ANSC_HANDLE)pMyObject);
    ULONG                           ulCopySize    = 0;
    ULONG                           ulLeftSize    = *pulSize;

    if ( *pulSize < ulMessageSize )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }

    ulMessageSize = 0;
    ulCopySize    = ulLeftSize;
    returnStatus  =
        pMyObject->CopyHeadersFrom
            (
                (ANSC_HANDLE)pMyObject,
                (PVOID)((ULONG)buffer + ulMessageSize),
                &ulCopySize
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        ulMessageSize += ulCopySize;
        ulLeftSize    -= ulCopySize;
    }

    ulCopySize    = ulLeftSize;
    returnStatus  =
        pMyObject->CopyBodyFrom
            (
                (ANSC_HANDLE)pMyObject,
                (PVOID)((ULONG)buffer + ulMessageSize),
                &ulCopySize
            );

    ulMessageSize += ulCopySize;
    ulLeftSize    -= ulCopySize;

    *pulSize = ulMessageSize;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoOutputHeaders
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
HttpBmoOutputHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject         = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhIf            = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhIf;
    PANSC_BUFFER_DESCRIPTOR         pHeaderBdo        = (PANSC_BUFFER_DESCRIPTOR   )pMyObject->hHeaderBdo;
    PVOID                           pSerializedBuffer = NULL;
    ULONG                           ulSerializedSize  = 0;

    if ( pHeaderBdo )
    {
        returnStatus =
            pTmhIf->Serialize
                (
                    pTmhIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    AnscBdoGetBlock    (pHeaderBdo),
                    AnscBdoGetBlockSize(pHeaderBdo),
                    hSerializeContext
                );
    }
    else
    {
        ulSerializedSize  = pMyObject->GetHeadersSize((ANSC_HANDLE)pMyObject);
        pSerializedBuffer = AnscAllocateMemory(ulSerializedSize);

        if ( !pSerializedBuffer )
        {
        	AnscTrace("HttpBmoOutputHeaders - serialized header too long %lu!\n", ulSerializedSize);
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            returnStatus =
                pMyObject->CopyHeadersFrom
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSerializedBuffer,
                        &ulSerializedSize
                    );
        }

        returnStatus =
            pTmhIf->Serialize
                (
                    pTmhIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    pSerializedBuffer,
                    ulSerializedSize,
                    hSerializeContext
                );

        AnscFreeMemory(pSerializedBuffer);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoOutputBody
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
HttpBmoOutputBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_MESSAGE_BODY_OBJECT       pMessageBody = (PHTTP_MESSAGE_BODY_OBJECT )pMyObject->hMessageBody;

    if ( pMessageBody )
    {
        PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = (PHTTP_WEBS_TRANS_OBJECT       )pMyObject->hHttpWebsTrans;
        PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pWebsTrans?pWebsTrans->GetWebSocket((ANSC_HANDLE)pWebsTrans):NULL;
        PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE           )pMyObject->hBccIf;
        ULONG                           ulBodySize   = pMessageBody->GetBodySize((ANSC_HANDLE)pMessageBody);
        char                            chunkMsg[16];
        BOOL                            bChunkedMode = pMyObject->IsChunkedCoding((ANSC_HANDLE)pMyObject);
        errno_t                         rc           = -1;

        if ( bChunkedMode && ulBodySize != 0 )
        {
            rc = sprintf_s(chunkMsg, sizeof(chunkMsg), "%X   \r\n", (unsigned int)ulBodySize);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }

            pBccIf->Serialize
                    (
                        pBccIf->hOwnerContext,
                        (PVOID)chunkMsg,
                        AnscSizeOfString(chunkMsg),
                        (ANSC_HANDLE)pWebSocket
                    );
        }

        pMessageBody->Output
                (
                    (ANSC_HANDLE)pMessageBody,
                    hSerializeContext
                );

        if ( bChunkedMode && ulBodySize != 0 )
        {
            pBccIf->Serialize
                    (
                        pBccIf->hOwnerContext,
                        (PVOID)"\r\n",
                        2,
                        (ANSC_HANDLE)pWebSocket
                    );
        }

    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        HttpBmoSendComplete
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to send out last chunk if chunked
        mode is specified as Transfer Encoding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSerializeContext
                Specifies the serialize context for output.

    return:     status of operation.

**********************************************************************/

void
HttpBmoSendComplete
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT    )hThisObject;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = (PHTTP_WEBS_TRANS_OBJECT       )pMyObject->hHttpWebsTrans;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pWebsTrans?pWebsTrans->GetWebSocket((ANSC_HANDLE)pWebsTrans):NULL;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE           )pMyObject->hBccIf;
    char                            LastChunk[]  = "0 \r\n\r\n";

    if ( pMyObject->IsChunkedCoding((ANSC_HANDLE)pMyObject) )
    {
        pBccIf->Serialize
                (
                    pBccIf->hOwnerContext,
                    (PVOID)LastChunk,
                    AnscSizeOfString(LastChunk),
                    (ANSC_HANDLE)pWebSocket
                );
    }

/*    return  ANSC_STATUS_SUCCESS;
*/
}

