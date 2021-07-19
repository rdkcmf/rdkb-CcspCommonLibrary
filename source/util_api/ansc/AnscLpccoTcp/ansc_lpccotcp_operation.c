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

    module:	ansc_lpccotcp_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpEnroll
        *   AnscLpccoTcpUnbind
        *   AnscLpccoTcpConnectToManager

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpEnroll
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enroll this IPC connector to the
        central LPC manager. This function also adds the local address
        information into the party table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpEnroll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer     = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer     = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket     = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hConnTimerObj;
    ULONG                           ulDaemonMode      = (ULONG                         )ANSC_DSTO_MODE_COMPACT | ANSC_DSTO_MODE_EVENT_SYNC | ANSC_DSTO_MODE_FOREIGN_BUFFER | ANSC_DSTO_MODE_NO_TIMEOUT | ANSC_DSTO_MODE_XSOCKET;
    ULONG                           ulBrokerMode      = (ULONG                         )ANSC_BSTO_MODE_COMPACT | ANSC_BSTO_MODE_EVENT_SYNC | ANSC_BSTO_MODE_FOREIGN_BUFFER | ANSC_BSTO_MODE_NO_TIMEOUT | ANSC_BSTO_MODE_XSOCKET;
    PANSC_LPC_PARTY_ADDR            pMyPartyAddr      = (PANSC_LPC_PARTY_ADDR          )NULL;
    PANSC_LPC_PARTY_ADDR            pManagerPartyAddr = (PANSC_LPC_PARTY_ADDR          )NULL;
    errno_t                         rc                = -1;

    if ( (pMyObject->LpcOpmode != ANSC_LPC_OPMODE_CLIENT) &&
         (pMyObject->LpcOpmode != ANSC_LPC_OPMODE_SERVER) )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;

        goto  EXIT1;
    }

    /*
     * Before the enrollment process starts, don't forget to add ourselves into the 'party-addr'
     * table. While this may seem unnecessary, the upper layer actually needs to know the local
     * 'party-addr' sometimes. For example, if the upper layer needs to encapsulate the 'party-addr'
     * and send to another party, it has to query the connector to know the local address.
     */
    if ( TRUE )
    {
        pMyPartyAddr = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(sizeof(ANSC_LPC_PARTY_ADDR));

        if ( !pMyPartyAddr )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            rc = strcpy_s(pMyPartyAddr->PartyName, sizeof(pMyPartyAddr->PartyName), pMyObject->PartyName);
            ERR_CHK(rc);

            pMyPartyAddr->StructSize      = sizeof(ANSC_LPC_PARTY_ADDR);
            pMyPartyAddr->RefCount        = 0;
            pMyPartyAddr->ActiveCalls     = 0;
            pMyPartyAddr->Timestamp       = 0;
            pMyPartyAddr->PartyState      = 0;
            pMyPartyAddr->PartyType       = ANSC_LPC_PARTY_TYPE_self;
            pMyPartyAddr->PartyAddr.Value = pMyObject->MyAddress.Value;
            pMyPartyAddr->PartyProtocol   = TCP_TRANSPORT;
            pMyPartyAddr->PartyPort       = pMyObject->MyPort;
            pMyPartyAddr->PartySocket     = (ANSC_HANDLE)NULL;
            pMyPartyAddr->PartyReserved   = (ANSC_HANDLE)NULL;

            pMyObject->AddPartyAddr((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pMyPartyAddr);
        }
    }

    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_CLIENT )
    {
        pManagerPartyAddr = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(sizeof(ANSC_LPC_PARTY_ADDR));

        if ( !pManagerPartyAddr )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            rc = strcpy_s(pManagerPartyAddr->PartyName,sizeof(pManagerPartyAddr->PartyName), ANSC_LPC_MANAGER_PARTY_NAME);
            ERR_CHK(rc);

            pManagerPartyAddr->StructSize      = sizeof(ANSC_LPC_PARTY_ADDR);
            pManagerPartyAddr->RefCount        = 0;
            pManagerPartyAddr->ActiveCalls     = 0;
            pManagerPartyAddr->Timestamp       = 0;
            pManagerPartyAddr->PartyState      = 0;
            pManagerPartyAddr->PartyType       = ANSC_LPC_PARTY_TYPE_manager;
            pManagerPartyAddr->PartyAddr.Value = pMyObject->ManagerAddress.Value;
            pManagerPartyAddr->PartyProtocol   = TCP_TRANSPORT;
            pManagerPartyAddr->PartyPort       = pMyObject->ManagerPort;
            pManagerPartyAddr->PartySocket     = (ANSC_HANDLE)NULL;
            pManagerPartyAddr->PartyReserved   = (ANSC_HANDLE)NULL;

            pMyObject->AddPartyAddr((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pManagerPartyAddr);
        }
    }

    /*
     * Depending on the 'opmode' of the IPC Connector, we need to engage in different enrollment
     * process: if the IPC Connector is operating in 'client' mode, we need to send a Registration
     * message to the LPC Manager along with the local address information. If the LPC Connector is
     * operating in 'server' mode, no Registration mesage needs to sent; instead, we bind to the
     * default manager port.
     */
    if ( TRUE )
    {
        pBrokerServer->SetHostAddress   ((ANSC_HANDLE)pBrokerServer, pMyObject->MyAddress.Dot    );
        pBrokerServer->SetHostPort      ((ANSC_HANDLE)pBrokerServer, 0                           );
        pBrokerServer->SetMaxMessageSize((ANSC_HANDLE)pBrokerServer, pMyObject->MaxMessageSize   );
        pBrokerServer->SetEngineCount   ((ANSC_HANDLE)pBrokerServer, pMyObject->EngineCount      );
        pBrokerServer->SetMinSocketCount((ANSC_HANDLE)pBrokerServer, pMyObject->MinSocketCount   );
        pBrokerServer->SetMaxSocketCount((ANSC_HANDLE)pBrokerServer, pMyObject->MaxSocketCount   );
        pBrokerServer->SetSocketTimeOut ((ANSC_HANDLE)pBrokerServer, pMyObject->SocketTimeOut * 5);
        pBrokerServer->SetMode          ((ANSC_HANDLE)pBrokerServer, ulBrokerMode                );
        pBrokerServer->Engage           ((ANSC_HANDLE)pBrokerServer);

        pDaemonServer->SetHostAddress   ((ANSC_HANDLE)pDaemonServer, pMyObject->MyAddress.Dot    );
        pDaemonServer->SetHostPort      ((ANSC_HANDLE)pDaemonServer, pMyObject->MyPort           );
        pDaemonServer->SetMaxMessageSize((ANSC_HANDLE)pDaemonServer, pMyObject->MaxMessageSize   );
        pDaemonServer->SetEngineCount   ((ANSC_HANDLE)pDaemonServer, pMyObject->EngineCount      );
        pDaemonServer->SetMinSocketCount((ANSC_HANDLE)pDaemonServer, pMyObject->MinSocketCount   );
        pDaemonServer->SetMaxSocketCount((ANSC_HANDLE)pDaemonServer, pMyObject->MaxSocketCount   );
        pDaemonServer->SetMode          ((ANSC_HANDLE)pDaemonServer, ulDaemonMode                );
        pDaemonServer->Engage           ((ANSC_HANDLE)pDaemonServer);
    }

    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_CLIENT )
    {
        pBrokerSocket =
            (PANSC_BROKER_SOCKET_TCP_OBJECT)pBrokerServer->AddSocket
                (
                    (ANSC_HANDLE)pBrokerServer,
                    pMyObject->ManagerAddress.Dot,
                    pMyObject->ManagerPort,
                    0,
                    (ANSC_HANDLE)pManagerPartyAddr
                );

        /*
         * What if the LPC Manager is unavailable at this moment? Shall we abort the engage()
         * process or shall we continue the operation and try to reconnect to the LPC manager later?
         * Here we continue even if the LPC manager is temporarily unavailable. Since the IMC
         * functions will try to connect to the specified LPC parties if the 'PartySocket' is NULL,
         * we don't need to do anything special here.
         */
        if ( pBrokerSocket )
        {
            returnStatus =
                pMyObject->SendHello
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBrokerSocket
                    );

            pManagerPartyAddr->PartySocket = (ANSC_HANDLE)pBrokerSocket;

            pMyObject->bConnected = TRUE;
        }
        else
        {
         /*   AnscTrace
                (
                    "\n !! AnscLpccoTcpEnroll: party %s failed to connect to LPC manager !!\n",
                    pMyObject->PartyName
                );*/

            pConnTimerObj->Stop       ((ANSC_HANDLE)pConnTimerObj);
            pConnTimerObj->SetInterval((ANSC_HANDLE)pConnTimerObj, ANSC_LPCCO_DEF_CONN_TIMEOUT);
            pConnTimerObj->Start      ((ANSC_HANDLE)pConnTimerObj);
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpUnbind
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unbind this IPC connector from the
        central LPC manager.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpUnbind
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pDaemonServer = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_LPC_PARTY_ADDR            pPartyAddr    = (PANSC_LPC_PARTY_ADDR          )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY            )NULL;
    ULONG                           i             = 0;

    /*
     * Out of courtesy, we SHOULD send a BYE message to every connected LPC party if a LPC channel
     * exists. Note that we only need to transmit the BYE message over the outgoing pipe.
     */
    AnscAcquireLock(&pMyObject->PartyTableLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[i]);

        while ( pSLinkEntry )
        {
            pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pPartyAddr->PartySocket )
            {
                    pMyObject->SendBye
                        (
                            (ANSC_HANDLE)pMyObject,
                            pPartyAddr->PartySocket
                        );
            }
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    pDaemonServer->Cancel((ANSC_HANDLE)pDaemonServer);
    pBrokerServer->Cancel((ANSC_HANDLE)pBrokerServer);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpConnectToManager
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to connect this IPC connector to the
        central LPC manager.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpConnectToManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer     = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket     = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hConnTimerObj;
    PANSC_LPC_PARTY_ADDR            pManagerPartyAddr = (PANSC_LPC_PARTY_ADDR          )NULL;

    if ( pMyObject->LpcOpmode != ANSC_LPC_OPMODE_CLIENT )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;

        goto  EXIT1;
    }
    else
    {
        pManagerPartyAddr =
            (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr
                (
                    (ANSC_HANDLE)pMyObject,
                    ANSC_LPC_MANAGER_PARTY_NAME
                );
    }

    if ( !pManagerPartyAddr )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;

        goto  EXIT1;
    }
    else
    {
        pBrokerSocket =
            (PANSC_BROKER_SOCKET_TCP_OBJECT)pBrokerServer->AddSocket
                (
                    (ANSC_HANDLE)pBrokerServer,
                    pMyObject->ManagerAddress.Dot,
                    pMyObject->ManagerPort,
                    0,
                    (ANSC_HANDLE)pManagerPartyAddr
                );

        /*
         * What if the LPC Manager is unavailable at this moment? Shall we abort the engage()
         * process or shall we continue the operation and try to reconnect to the LPC manager later?
         * Here we continue even if the LPC manager is temporarily unavailable. Since the IMC
         * functions will try to connect to the specified LPC parties if the 'PartySocket' is NULL,
         * we don't need to do anything special here.
         */
        if ( pBrokerSocket )
        {
            returnStatus =
                pMyObject->SendHello
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBrokerSocket
                    );

            pManagerPartyAddr->PartySocket = (ANSC_HANDLE)pBrokerSocket;

            pMyObject->bConnected = TRUE;
        }
        else
        {/*
            AnscTrace
                (
                    "\n !! AnscLpccoTcpConnectToManager: party %s failed to connect to LPC manager !!\n",
                    pMyObject->PartyName
                );
	*/
            pConnTimerObj->Stop       ((ANSC_HANDLE)pConnTimerObj);
            pConnTimerObj->SetInterval((ANSC_HANDLE)pConnTimerObj, ANSC_LPCCO_DEF_CONN_TIMEOUT);
            pConnTimerObj->Start      ((ANSC_HANDLE)pConnTimerObj);
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    return  returnStatus;
}
