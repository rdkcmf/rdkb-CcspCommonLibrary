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

    module:	slap_lbo_cpcif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Loam Broker Object.

        *   SlapLboCpcCallDispatch
        *   SlapLboCpcFreeOutputBuffer
        *   SlapLboCpcNotifyEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_lbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboCpcCallDispatch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboCpcCallDispatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PIMCP_SLAP_CALL                 pImcpSlapCall     = (PIMCP_SLAP_CALL           )pInputBuffer;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;

    if ( ulInputSize < sizeof(IMCP_SLAP_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulInputSize != (sizeof(IMCP_SLAP_CALL) + ImcpSlapCallGetSize(pImcpSlapCall)) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    switch ( ImcpSlapCallGetType(pImcpSlapCall) )
    {
        case    IMCP_SLAP_CALL_TYPE_LOCO :

                returnStatus =
                    pMyObject->ProcessCallLoco
                        (
                            (ANSC_HANDLE)pMyObject,
                            hLpcPartyAddr,
                            ImcpSlapCallGetData(pImcpSlapCall),
                            ImcpSlapCallGetSize(pImcpSlapCall),
                            ppOutputBuffer,
                            pulOutputSize,
                            pulErrorCode
                        );

                break;

        default :

                *pulErrorCode = ANSC_LPC_ERROR_invalidReqType;
                returnStatus  = ANSC_STATUS_UNAPPLICABLE;


                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboCpcFreeOutputBuffer
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pOutputBuffer
            );

    description:

        This function is called by the underlying LPC connector to
        release the output buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pOutputBuffer
                Specifies the output buffer to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboCpcFreeOutputBuffer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pOutputBuffer
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    AnscFreeMemory((void*)((ULONG)pOutputBuffer - IMCP_REPLY_BUFFER_OVERHEAD));

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboCpcNotifyEvent
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the underlying LPC connector to
        report an event.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the LPC event to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the event-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboCpcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR      )hReserved;

    switch ( ulEvent )
    {
        case    ANSC_CPC_EVENT_DISCONNECTED :

                if ( pPartyAddr )
                {
                    pMyObject->DelObjBinding2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pPartyAddr->PartyName
                            );
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
