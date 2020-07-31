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

    module:	http_psover2_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Http Pso Ver2 Object.

        *   HttpPsoVer2AskTroBySocket
        *   HttpPsoVer2PopTroBySocket
        *   HttpPsoVer2GetLastTro
        *   HttpPsoVer2GetCurTro
        *   HttpPsoVer2AddNewTro
        *   HttpPsoVer2DelAllTros

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_psover2_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2AskTroBySocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to retrieve a trans record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the server socket object to be matched.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpPsoVer2AskTroBySocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;

    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->TroSList);

    while ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        if ( pTransRecord->hServerSocket == hSocket )
        {
            pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);

            AnscReleaseLock(&pMyObject->TroSListLock);

            return  (ANSC_HANDLE)pTransRecord;
        }
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2AskTroBySocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to unlink a trans record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the server socket object to be matched.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpPsoVer2PopTroBySocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;

    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->TroSList);

    while ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        if ( pTransRecord->hServerSocket == hSocket )
        {
            AnscSListPopEntryByLink(&pMyObject->TroSList, &pTransRecord->Linkage);

            pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);

            AnscReleaseLock(&pMyObject->TroSListLock);

            return  (ANSC_HANDLE)pTransRecord;
        }
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetLastTro
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the latest record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpPsoVer2GetLastTro
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    
    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->TroSList);

    if ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);

        pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);
    }
    else
    {
        pTransRecord = NULL;
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  (ANSC_HANDLE)pTransRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2GetCurTro
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpPsoVer2GetCurTro
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    ULONG                           ulTroState     = HTTP_TRO_STATE_INITIALIZED;

    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->TroSList);

    if ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);
        ulTroState   = pTransRecord->GetTransState((ANSC_HANDLE)pTransRecord);

        if ( (ulTroState != HTTP_TRO_STATE_ESTABLISHED) &&
             (ulTroState != HTTP_TRO_STATE_FINISHED   ) )
        {
            pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);

            AnscReleaseLock(&pMyObject->TroSListLock);

            return  (ANSC_HANDLE)pTransRecord;
        }
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    pTransRecord = (PHTTP_TRANS_RECORD_OBJECT)pMyObject->AddNewTro((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pTransRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2AddCurTro
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a new record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record object.

**********************************************************************/

ANSC_HANDLE
HttpPsoVer2AddNewTro
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT         )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT   )pMyObject->hOwnerContext;
    PHTTP_WAM_INTERFACE             pWamIf         = (PHTTP_WAM_INTERFACE           )pMyObject->hWamIf;
    PHTTP_SBC_INTERFACE             pSbcIf         = (PHTTP_SBC_INTERFACE           )pMyObject->hSbcIf;
    PHTTP_CBC_INTERFACE             pCbcIf         = (PHTTP_CBC_INTERFACE           )pMyObject->hCbcIf;
    PHTTP_PBC_INTERFACE             pPbcIf         = (PHTTP_PBC_INTERFACE           )pMyObject->hPbcIf;
    PHTTP_HFP_INTERFACE             pHfpIf         = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;

    pTransRecord = (PHTTP_TRANS_RECORD_OBJECT)pAdvancedProxy->AcquireTro((ANSC_HANDLE)pAdvancedProxy);

    if ( !pTransRecord )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pTransRecord->SetWamIf       ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pWamIf            );
        pTransRecord->SetSbcIf       ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pSbcIf            );
        pTransRecord->SetCbcIf       ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pCbcIf            );
        pTransRecord->SetPbcIf       ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pPbcIf            );
        pTransRecord->SetHfpIf       ((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pHfpIf            );
        pTransRecord->SetClientSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)pClientSocket     );
        pTransRecord->SetServerSocket((ANSC_HANDLE)pTransRecord, (ANSC_HANDLE)NULL              );
        pTransRecord->SetTransState  ((ANSC_HANDLE)pTransRecord, HTTP_TRO_STATE_CLIENT_CONNECTED);
        pTransRecord->Open           ((ANSC_HANDLE)pTransRecord);
    }

    AnscAcquireLock(&pMyObject->TroSListLock);

    AnscSListPushEntry(&pMyObject->TroSList, &pTransRecord->Linkage);

    pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  (ANSC_HANDLE)pTransRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2DelAllTros
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpPsoVer2DelAllTros
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject      = (PHTTP_PSO_VER2_OBJECT      )hThisObject;
    PHTTP_WAM_INTERFACE             pWamIf         = (PHTTP_WAM_INTERFACE        )pMyObject->hWamIf;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pServerSocket  = NULL;

    AnscAcquireLock(&pMyObject->TroSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->TroSList);

    while ( pSLinkEntry )
    {
        pTransRecord = ACCESS_HTTP_TRANS_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListPopEntry(&pMyObject->TroSList);

        pWamIf->Close
                (
                    pWamIf->hOwnerContext,
                    (ANSC_HANDLE)pTransRecord
                );

        pTransRecord->AcquireAccess((ANSC_HANDLE)pTransRecord);

        pServerSocket = (PANSC_BROKER_SOCKET_TCP_OBJECT)pTransRecord->GetServerSocket((ANSC_HANDLE)pTransRecord);

        if ( pServerSocket )
        {
            pServerSocket->SetClientContext((ANSC_HANDLE)pServerSocket, (ANSC_HANDLE)NULL        );
            pServerSocket->ToClean         ((ANSC_HANDLE)pServerSocket, TRUE, HTTP_APO_SOCKET_TTC);
        }

        pTransRecord->ReleaseAccess((ANSC_HANDLE)pTransRecord);
        pTransRecord->Close        ((ANSC_HANDLE)pTransRecord);
        pTransRecord->Return       ((ANSC_HANDLE)pTransRecord);
    }

    AnscReleaseLock(&pMyObject->TroSListLock);

    return  ANSC_STATUS_SUCCESS;
}
