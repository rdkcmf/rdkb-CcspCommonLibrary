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

    module:	ansc_lpcco_imcif.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoImcGenPartyAddr
        *   AnscLpccoImcGetLpcOpmode
        *   AnscLpccoImcSetLpcOpmode
        *   AnscLpccoImcGetPartyName
        *   AnscLpccoImcSetPartyName
        *   AnscLpccoImcIsPartyConnected
        *   AnscLpccoImcEngage
        *   AnscLpccoImcCancel
        *   AnscLpccoImcCallRequest
        *   AnscLpccoImcCallRequest2

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
        AnscLpccoImcGenPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to construct a 'party_addr' structure
        from the raw buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the raw address information to be used.

                ULONG                       ulSize
                Specifies the size of the raw address information.

    return:     party addr.

**********************************************************************/

ANSC_HANDLE
AnscLpccoImcGenPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;

    if ( ulSize < sizeof(ANSC_LPC_PARTY_ADDR) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pPartyAddr = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(ulSize);
    }

    if ( !pPartyAddr )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        AnscCopyMemory
            (
                pPartyAddr,
                buffer,
                ulSize
            );
    }

    return  (ANSC_HANDLE)pPartyAddr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscLpccoImcGetLpcOpmode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the operating mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     operating mode.

**********************************************************************/

ULONG
AnscLpccoImcGetLpcOpmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    return  pMyObject->LpcOpmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcSetLpcOpmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure the operating mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the operating mode to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcSetLpcOpmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    pMyObject->LpcOpmode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscLpccoImcGetPartyName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the local party name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     local party name.

**********************************************************************/

char*
AnscLpccoImcGetPartyName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    return  pMyObject->PartyName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcSetPartyName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to configure the local party name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the local party name to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcSetPartyName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    if ( pMyObject->PartyName )
    {
        AnscFreeMemory(pMyObject->PartyName);

        pMyObject->PartyName = NULL;
    }

    if ( party_name )
    {
        pMyObject->PartyName = AnscCloneString(party_name);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscLpccoImcIsPartyConnected
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name,
                BOOL                        bOutgoingConn
            );

    description:

        This function is called to check whether the specified party
        is connected.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the remote party name to be checked.

                BOOL                        bOutgoingConn
                Specifies which connection direction to check.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
AnscLpccoImcIsPartyConnected
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        BOOL                        bOutgoingConn
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;

    pPartyAddr =
        (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr
            (
                (ANSC_HANDLE)pMyObject,
                party_name
            );

    if ( !pPartyAddr )
    {
        return  FALSE;
    }
    else if ( bOutgoingConn )
    {
        if ( pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedOut )
        {
            return  TRUE;
        }
    }
    else
    {
        if ( pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedIn )
        {
            return  TRUE;
        }
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the IMC activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    returnStatus = pMyObject->Engage((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the IMC activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    returnStatus = pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcCallRequest
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                ANSC_HANDLE*                phOutputBdo,
                PULONG                      pulErrorCode,
                ULONG                       ulTimeout
            );

    description:

        This function is called to issue a LPC call request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name,
                Specifies the 'party_name' of the target party.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                ANSC_HANDLE*                phOutputBdo
                Specifies the output buffer descriptor of the call to
                be returned.

                PULONG                      pulErrorCode
                Specifies the erorr code of the operation.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcCallRequest
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(party_name);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);
    UNREFERENCED_PARAMETER(pulErrorCode);
    UNREFERENCED_PARAMETER(ulTimeout);
    *phOutputBdo = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoImcCallRequest2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                ANSC_HANDLE*                phOutputBdo,
                PULONG                      pulErrorCode,
                ULONG                       ulTimeout
            );

    description:

        This function is called to issue a LPC call request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPartyAddr
                Specifies the 'party_name' of the target party.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                ANSC_HANDLE*                phOutputBdo
                Specifies the output buffer descriptor of the call to
                be returned.

                PULONG                      pulErrorCode
                Specifies the erorr code of the operation.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoImcCallRequest2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);
    UNREFERENCED_PARAMETER(pulErrorCode);
    UNREFERENCED_PARAMETER(ulTimeout);
    *phOutputBdo = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_UNAPPLICABLE;
}
