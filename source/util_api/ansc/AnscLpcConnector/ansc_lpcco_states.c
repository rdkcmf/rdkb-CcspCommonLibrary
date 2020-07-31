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

    module:	ansc_lpcco_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoGetAnscImcIf
        *   AnscLpccoGetAnscCpcIf
        *   AnscLpccoSetAnscCpcIf
        *   AnscLpccoGetPartyAddrSize
        *   AnscLpccoGetCallTimeout
        *   AnscLpccoSetCallTimeout
        *   AnscLpccoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/24/03    initial revision.

**********************************************************************/


#include "ansc_lpcco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoGetAnscImcIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscLpccoGetAnscImcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    return  pMyObject->hAnscImcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoGetAnscCpcIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscLpccoGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    return  pMyObject->hAnscCpcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoSetAnscCpcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoSetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf   = (PANSC_CPC_INTERFACE       )pMyObject->hAnscCpcIf;

    if ( hInterface )
    {
        *pAnscCpcIf = *(PANSC_CPC_INTERFACE)hInterface;
    }
    else
    {
        pAnscCpcIf->CallDispatch     = AnscLpccoCpcCallDispatch;
        pAnscCpcIf->FreeOutputBuffer = AnscLpccoCpcFreeOutputBuffer;
        pAnscCpcIf->NotifyEvent      = AnscLpccoCpcNotifyEvent;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscLpccoGetPartyAddrSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscLpccoGetPartyAddrSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  sizeof(ANSC_LPC_PARTY_ADDR);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscLpccoGetCallTimeout
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscLpccoGetCallTimeout
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    return  pMyObject->CallTimeout;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoSetCallTimeout
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       timeout
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       timeout
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoSetCallTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    pMyObject->CallTimeout = timeout;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    pMyObject->DelAllPartyAddrs  ((ANSC_HANDLE)pMyObject);
    pMyObject->PopAllPendingCalls((ANSC_HANDLE)pMyObject, ANSC_LPC_ERROR_connectorReset);

    if ( pMyObject->PartyName )
    {
        AnscFreeMemory(pMyObject->PartyName);

        pMyObject->PartyName = NULL;
    }

    pMyObject->bConnected = FALSE;

    return  ANSC_STATUS_SUCCESS;
}
