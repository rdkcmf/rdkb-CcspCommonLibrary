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

    module:	slap_amo_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Access Manager Object.

        *   SlapAmoEngage
        *   SlapAmoCancel

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


#include "slap_amo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoEngage
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
SlapAmoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
#ifdef  _ANSC_SLAP_LPC_
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector  = (PANSC_LPC_CONNECTOR_OBJECT   )pMyObject->hAnscLpcConnector;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOAM_SERVER_OBJECT        pSlapLoamServer    = (PSLAP_LOAM_SERVER_OBJECT     )pMyObject->hSlapLoamServer;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    char                            lpcPartyName[64];
    errno_t                         rc                 = -1;
    #ifdef _SLAP_IPC_USE_TCP_SOCKET
        PANSC_LPCCO_TCP_OBJECT          pAnscLpccoTcp      = (PANSC_LPCCO_TCP_OBJECT       )pAnscLpcConnector;
    #else
        PANSC_LPCCO_UDP_OBJECT          pAnscLpccoUdp      = (PANSC_LPCCO_UDP_OBJECT       )pAnscLpcConnector;
    #endif
#endif

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if (  (pProperty->AggregationMode & SLAP_GOA_MODE_imcpServer ) &&
              !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    if ( TRUE )
    {
        if ( pProperty->AggregationMode & SLAP_GOA_MODE_enablePooling )
        {
            pSlapEnvController->BssSetPoolingFlag((ANSC_HANDLE)pSlapEnvController, TRUE);
        }
        else
        {
            pSlapEnvController->BssSetPoolingFlag((ANSC_HANDLE)pSlapEnvController, FALSE);
        }

        pSlapEnvController->Engage((ANSC_HANDLE)pSlapEnvController);
    }


#ifdef  _ANSC_SLAP_LPC_

    if ( (pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) ||
         (pProperty->AggregationMode & SLAP_GOA_MODE_imcpServer) )
    {
        if ( !pAnscLpcConnector )
        {
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
            pAnscLpcConnector =
                (PANSC_LPC_CONNECTOR_OBJECT)AnscCreateLpccoTcp
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
#else
            pAnscLpcConnector =
                (PANSC_LPC_CONNECTOR_OBJECT)AnscCreateLpccoUdp
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
#endif

            if ( !pAnscLpcConnector )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                pMyObject->hAnscLpcConnector = (ANSC_HANDLE           )pAnscLpcConnector;
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
                pAnscLpccoTcp                = (PANSC_LPCCO_TCP_OBJECT)pAnscLpcConnector;
#else
                pAnscLpccoUdp                = (PANSC_LPCCO_UDP_OBJECT)pAnscLpcConnector;
#endif
            }
        }

        if ( TRUE )
        {
            AnscZeroMemory(lpcPartyName, 64);

            rc = sprintf_s
                (
                    lpcPartyName,
                    sizeof(lpcPartyName);
                    "slapAccessManager-0x%x-0x%x@%d.%d.%d.%d",
                    (ULONG)AnscGetProcessId    (),
                    (ULONG)AnscGetCurTaskHandle(),
                    pProperty->MyAddress.Dot[0],
                    pProperty->MyAddress.Dot[1],
                    pProperty->MyAddress.Dot[2],
                    pProperty->MyAddress.Dot[3]
                );
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }

        pAnscLpcConnector->ImcSetLpcOpmode  ((ANSC_HANDLE)pAnscLpcConnector, ANSC_LPC_OPMODE_CLIENT       );
        pAnscLpcConnector->ImcSetPartyName  ((ANSC_HANDLE)pAnscLpcConnector, lpcPartyName                 );
#ifdef   _SLAP_IPC_USE_TCP_SOCKET
        pAnscLpccoTcp    ->SetMyPort        ((ANSC_HANDLE)pAnscLpccoTcp,     pProperty->LpcPartyPort      );
        pAnscLpccoTcp    ->SetMyAddress     ((ANSC_HANDLE)pAnscLpccoTcp,     pProperty->MyAddress.Dot     );
        pAnscLpccoTcp    ->SetManagerAddress((ANSC_HANDLE)pAnscLpccoTcp,     pProperty->ManagerAddress.Dot);
#else
        pAnscLpccoUdp    ->SetMyPort        ((ANSC_HANDLE)pAnscLpccoUdp,     pProperty->LpcPartyPort      );
        pAnscLpccoUdp    ->SetMyAddress     ((ANSC_HANDLE)pAnscLpccoUdp,     pProperty->MyAddress.Dot     );
        pAnscLpccoUdp    ->SetManagerAddress((ANSC_HANDLE)pAnscLpccoUdp,     pProperty->ManagerAddress.Dot);
#endif
        pAnscLpcConnector->Engage           ((ANSC_HANDLE)pAnscLpcConnector);
    }

    if ( pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient )
    {
        if ( !pSlapLoamClient )
        {
            pSlapLoamClient =
                (PSLAP_LOAM_CLIENT_OBJECT)SlapCreateLoamClient
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pSlapLoamClient )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                pMyObject->hSlapLoamClient = (ANSC_HANDLE)pSlapLoamClient;
            }
        }

        if ( pProperty->AggregationMode & SLAP_GOA_MODE_mobilitySupport )
        {
            pSlapLoamClient->SetMobilityOn((ANSC_HANDLE)pSlapLoamClient, TRUE);
        }
        else
        {
            pSlapLoamClient->SetMobilityOn((ANSC_HANDLE)pSlapLoamClient, FALSE);
        }

        pSlapLoamClient->SetAnscLpcConnector((ANSC_HANDLE)pSlapLoamClient, (ANSC_HANDLE)pAnscLpcConnector);
        pSlapLoamClient->Engage             ((ANSC_HANDLE)pSlapLoamClient);
    }

    if ( pProperty->AggregationMode & SLAP_GOA_MODE_imcpServer )
    {
        if ( !pSlapLoamServer )
        {
            pSlapLoamServer =
                (PSLAP_LOAM_SERVER_OBJECT)SlapCreateLoamServer
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pSlapLoamServer )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                pMyObject->hSlapLoamServer = (ANSC_HANDLE)pSlapLoamServer;
            }
        }

        if ( pProperty->AggregationMode & SLAP_GOA_MODE_autoExport )
        {
            pSlapLoamServer->SetMobilityOn((ANSC_HANDLE)pSlapLoamServer, TRUE);
        }
        else
        {
            pSlapLoamServer->SetMobilityOn((ANSC_HANDLE)pSlapLoamServer, FALSE);
        }

        pSlapLoamServer->SetAnscLpcConnector((ANSC_HANDLE)pSlapLoamServer, (ANSC_HANDLE)pAnscLpcConnector);
        pSlapLoamServer->SetSlapUoaIf       ((ANSC_HANDLE)pSlapLoamServer, (ANSC_HANDLE)pSlapUoaIf       );
        pSlapLoamServer->Engage             ((ANSC_HANDLE)pSlapLoamServer);
    }

#endif


    pMyObject->bActive = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoCancel
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
SlapAmoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
#ifdef  _ANSC_SLAP_LPC_
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOAM_SERVER_OBJECT        pSlapLoamServer    = (PSLAP_LOAM_SERVER_OBJECT     )pMyObject->hSlapLoamServer;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector  = (PANSC_LPC_CONNECTOR_OBJECT   )pMyObject->hAnscLpcConnector;
#endif

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }


    #ifdef  _ANSC_SLAP_LPC_

    if ( pSlapLoamClient )
    {
        pSlapLoamClient->Cancel((ANSC_HANDLE)pSlapLoamClient);
        pSlapLoamClient->Remove((ANSC_HANDLE)pSlapLoamClient);

        pMyObject->hSlapLoamClient = (ANSC_HANDLE)NULL;
    }

    if ( pSlapLoamServer )
    {
        pSlapLoamServer->Cancel((ANSC_HANDLE)pSlapLoamServer);
        pSlapLoamServer->Remove((ANSC_HANDLE)pSlapLoamServer);

        pMyObject->hSlapLoamServer = (ANSC_HANDLE)NULL;
    }

    if ( pAnscLpcConnector )
    {
        pAnscLpcConnector->Cancel((ANSC_HANDLE)pAnscLpcConnector);
        pAnscLpcConnector->Remove((ANSC_HANDLE)pAnscLpcConnector);

        pMyObject->hAnscLpcConnector = (ANSC_HANDLE)NULL;
    }

    #endif

    if ( pSlapEnvController )
    {
        pSlapEnvController->Cancel((ANSC_HANDLE)pSlapEnvController);
    }


    return  ANSC_STATUS_SUCCESS;
}
