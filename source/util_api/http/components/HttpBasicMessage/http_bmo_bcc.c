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

    module:	http_bmo_bcc.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced body-control functions
        of the Http Basic Message Object.

        *   HttpBmoBccGetHeaderField
        *   HttpBmoBccAddHeaderField
        *   HttpBmoBccPredictBodySize
        *   HttpBmoBccNotify
        *   HttpBmoBccSerialize

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


#include "http_bmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoBccGetHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called by the message body object to retrieve
        a header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header field id to be matched.

    return:     matched header field.

**********************************************************************/

ANSC_HANDLE
HttpBmoBccGetHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    
    return  pMyObject->GetHeaderField((ANSC_HANDLE)pMyObject, ulHeaderId);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoBccAddHeaderField
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHfo
            );

    description:

        This function is called by the message body object to add
        a new header field.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHfo
                Specifies the header field object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoBccAddHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    
    return  pMyObject->AddHeaderField((ANSC_HANDLE)pMyObject, hHfo);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoBccPredictBodySize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the message body object to predict
        the message body size.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     message body size.

**********************************************************************/

ULONG
HttpBmoBccPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    
    return  pMyObject->PredictBodySize((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoBccNotify
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the message body object to indicate
        trigger and final events.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the body carrier event to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the reserved field for future use.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoBccNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhIf       = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = (PANSC_BUFFER_DESCRIPTOR   )hReserved;

    switch ( ulEvent )
    {
        case    HTTP_BCC_EVENT_ARRIVED_BODY_PART :

                pMyObject->State = HTTP_BMO_STATE_PART_BODY;
                returnStatus     =
                    pTmhIf->Notify
                        (
                            pTmhIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            HTTP_TMH_EVENT_ARRIVED_BODY_PART,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    HTTP_BCC_EVENT_ARRIVED_BODY :

                pMyObject->State = HTTP_BMO_STATE_COMPLETE;
                returnStatus     =
                    pTmhIf->Notify
                        (
                            pTmhIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            HTTP_TMH_EVENT_ARRIVED_BODY,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    HTTP_BCC_EVENT_COMPLETED :

                pMyObject->State = HTTP_BMO_STATE_COMPLETE;
                returnStatus     =
                    pTmhIf->Notify
                        (
                            pTmhIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            HTTP_TMH_EVENT_COMPLETED,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    HTTP_BCC_EVENT_OVER_PACKED :

                pMyObject->State = HTTP_BMO_STATE_OVER_PACKED;
                returnStatus     =
                    pTmhIf->Notify
                        (
                            pTmhIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            HTTP_TMH_EVENT_OVER_PACKED,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        default :

                returnStatus = ANSC_STATUS_SUCCESS;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoBccSerialize
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hSerializeContext
            );

    description:

        This function is called by the MBO object to output the HTTP
        stream payload data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the payload data buffer to be serialized.

                ULONG                       ulSize
                Specifies the payload buffer size.

                ANSC_HANDLE                 hSerializeContext
                Specifies the context used for serialization.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoBccSerialize
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhIf       = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhIf;

    returnStatus =
        pTmhIf->Serialize
            (
                pTmhIf->hOwnerContext,
                (ANSC_HANDLE)pMyObject,
                buffer,
                ulSize,
                hSerializeContext
            );

    return  returnStatus;
}
