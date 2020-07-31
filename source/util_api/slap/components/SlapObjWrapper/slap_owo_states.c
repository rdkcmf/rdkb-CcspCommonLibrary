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

    module:	slap_owo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Obj Wrapper Object.

        *   SlapOwoSetObjName
        *   SlapOwoSetLpcPartyAddr
        *   SlapOwoSetLpcPartyName
        *   SlapOwoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/04/03    initial revision.

**********************************************************************/


#include "slap_owo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoSetObjName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoSetObjName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;

    if ( pMyObject->ObjName )
    {
        AnscFreeMemory(pMyObject->ObjName);

        pMyObject->ObjName = NULL;
    }

    if ( obj_name )
    {
        pMyObject->ObjName = AnscCloneString(obj_name);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoSetLpcPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 party_addr
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 party_addr
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoSetLpcPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 party_addr
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR    )pMyObject->LpcPartyAddr;

    if ( pPartyAddr )
    {
        pPartyAddr->RefCount--;
    }

    pMyObject->LpcPartyAddr = party_addr;
    pPartyAddr              = (PANSC_LPC_PARTY_ADDR)pMyObject->LpcPartyAddr;

    if ( pPartyAddr )
    {
        pPartyAddr->RefCount++;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoSetLpcPartyName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoSetLpcPartyName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;

    if ( pMyObject->LpcPartyName )
    {
        AnscFreeMemory(pMyObject->LpcPartyName);

        pMyObject->LpcPartyName = NULL;
    }

    if ( party_name )
    {
        pMyObject->LpcPartyName = AnscCloneString(party_name);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR    )pMyObject->LpcPartyAddr;

    if ( pMyObject->ObjName )
    {
        AnscFreeMemory(pMyObject->ObjName);

        pMyObject->ObjName = NULL;
    }

    if ( pMyObject->LpcPartyName )
    {
        AnscFreeMemory(pMyObject->LpcPartyName);

        pMyObject->LpcPartyName = NULL;
    }

    if ( pPartyAddr )
    {
        pPartyAddr->RefCount--;

        pMyObject->LpcPartyAddr = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
