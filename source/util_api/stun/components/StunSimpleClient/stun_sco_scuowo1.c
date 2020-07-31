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

    module:	stun_sco_scuowo1.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Stun Simple Client Object.

        *   StunScoScuowo1Query
        *   StunScoScuowo1Process
        *   StunScoScuowo1Notify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/31/08    initial revision.

**********************************************************************/


#include "stun_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        StunScoScuowo1Query
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called by the client object to query how
        to proceed on procesing an incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be judged.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     processing mode.

**********************************************************************/

ULONG
StunScoScuowo1Query
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_SCUOWO_PMODE_DISCARD;
    }

    return  ANSC_SCUOWO_PMODE_PROCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoScuowo1Process
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called by the client object to process an
        incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoScuowo1Process
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_BSM_INTERFACE             pStunBsmIf        = (PSTUN_BSM_INTERFACE           )pMyObject->hStunBsmIf;
    PSTUN_HEADER                    pStunHeader       = (PSTUN_HEADER                  )buffer;
    PUCHAR                          pMsgFirstByte     = (PUCHAR                        )buffer;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_DISCARD;
    }
    else if ( (*pMsgFirstByte != 0) &&
              (*pMsgFirstByte != 1) )
    {
        returnStatus =
            pStunBsmIf->RecvMsg1
                (
                    pStunBsmIf->hOwnerContext,
                    buffer,
                    ulSize
                );

        return  returnStatus;
    }
    else if ( ulSize < STUN_MIN_MSG_SIZE )
    {
        return  ANSC_STATUS_DISCARD;
    }

    switch ( AnscStunGetMessageType(pStunHeader) )
    {
        case    STUN_MSG_TYPE_BindingResponse :

                returnStatus =
                    pMyObject->RecvBindingResponse1
                        (
                            (ANSC_HANDLE)pMyObject,
                            buffer,
                            ulSize
                        );

                break;

        case    STUN_MSG_TYPE_BindingErrorResponse :

                returnStatus =
                    pMyObject->RecvBindingErrorResponse1
                        (
                            (ANSC_HANDLE)pMyObject,
                            buffer,
                            ulSize
                        );

                break;

        default :

                if ( pStunBsmIf )
                {
                    returnStatus =
                        pStunBsmIf->Notify
                            (
                                pStunBsmIf->hOwnerContext,
                                STUN_BSM_EVENT_unknownMsgReceived,
                                (ANSC_HANDLE)pStunHeader
                            );
                }

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoScuowo1Notify
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the client object to process an
        socket-related event.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the event to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the event-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoScuowo1Notify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(ulEvent);
    UNREFERENCED_PARAMETER(hReserved);
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_BSM_INTERFACE             pStunBsmIf        = (PSTUN_BSM_INTERFACE           )pMyObject->hStunBsmIf;

    if ( pStunBsmIf )
    {
        pStunBsmIf->Notify
                (
                    pStunBsmIf->hOwnerContext,
                    STUN_BSM_EVENT_serverUnreachable,
                    (ANSC_HANDLE)NULL
                );
    }

    return  ANSC_STATUS_SUCCESS;
}
