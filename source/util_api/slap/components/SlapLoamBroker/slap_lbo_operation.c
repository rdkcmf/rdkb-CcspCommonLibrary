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

    module:	slap_lbo_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Loam Broker Object.

        *   SlapLboAcqAccess
        *   SlapLboRelAccess
        *   SlapLboEngage
        *   SlapLboCancel

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
        SlapLboAcqAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire the object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboRelAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboRelAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT)hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PSLAP_LOAM_BROKER_PROPERTY      pProperty         = (PSLAP_LOAM_BROKER_PROPERTY)&pMyObject->Property;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
    PANSC_LPCCO_TCP_OBJECT          pAnscLpccoTcp     = (PANSC_LPCCO_TCP_OBJECT    )pAnscLpcConnector;
#else
    PANSC_LPCCO_UDP_OBJECT          pAnscLpccoUdp     = (PANSC_LPCCO_UDP_OBJECT    )pAnscLpcConnector;
#endif

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pAnscLpcConnector )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( pAnscLpcConnector )
    {
        pAnscLpcConnector->SetAnscCpcIf     ((ANSC_HANDLE)pAnscLpcConnector, pMyObject->hAnscCpcIf        );
        pAnscLpcConnector->SetCallTimeout   ((ANSC_HANDLE)pAnscLpcConnector, ANSC_LPC_DEF_CALL_TIMEOUT / 2);
        pAnscLpcConnector->ImcSetLpcOpmode  ((ANSC_HANDLE)pAnscLpcConnector, ANSC_LPC_OPMODE_SERVER       );
        pAnscLpcConnector->ImcSetPartyName  ((ANSC_HANDLE)pAnscLpcConnector, ANSC_LPC_MANAGER_PARTY_NAME  );
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
        pAnscLpccoTcp    ->SetMyPort        ((ANSC_HANDLE)pAnscLpccoTcp,     ANSC_LPC_MANAGER_PORT_NUMBER );
        pAnscLpccoTcp    ->SetMyAddress     ((ANSC_HANDLE)pAnscLpccoTcp,     pProperty->MyAddress.Dot     );
        pAnscLpccoTcp    ->SetManagerAddress((ANSC_HANDLE)pAnscLpccoTcp,     pProperty->MyAddress.Dot     );
#else
        pAnscLpccoUdp    ->SetMyPort        ((ANSC_HANDLE)pAnscLpccoUdp,     ANSC_LPC_MANAGER_PORT_NUMBER );
        pAnscLpccoUdp    ->SetMyAddress     ((ANSC_HANDLE)pAnscLpccoUdp,     pProperty->MyAddress.Dot     );
        pAnscLpccoUdp    ->SetManagerAddress((ANSC_HANDLE)pAnscLpccoUdp,     pProperty->MyAddress.Dot     );
#endif

        returnStatus = pAnscLpcConnector->Engage((ANSC_HANDLE)pAnscLpcConnector);
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject         = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    if ( pAnscLpcConnector )
    {
        pAnscLpcConnector->Cancel((ANSC_HANDLE)pAnscLpcConnector);
    }

    return  ANSC_STATUS_SUCCESS;
}
