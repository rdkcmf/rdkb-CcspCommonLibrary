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

    module:	http_tro_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Trans Record Object.

        *   HttpTroTerminate
        *   HttpTroOpen
        *   HttpTroClose
        *   HttpTroAcquireAccess
        *   HttpTroReleaseAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/05/02    initial revision.

**********************************************************************/


#include "http_tro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroTerminate
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by external Controller Objects to
        terminate the session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroTerminate
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    
    pMyObject->AcquireAccess((ANSC_HANDLE)pMyObject);

    if ( pClientSocket )
    {
        pClientSocket->ToClean((ANSC_HANDLE)pClientSocket, TRUE, HTTP_APO_SOCKET_TTC);
    }

    pMyObject->ReleaseAccess((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT   )pMyObject->hOwnerContext;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket  = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    PHTTP_HFP_INTERFACE             pHfpIf         = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhClientIf   = (PHTTP_TMH_INTERFACE           )pMyObject->hTmhClientIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf   = (PHTTP_TMH_INTERFACE           )pMyObject->hTmhServerIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;

    if ( !pBmoReq )
    {
        pBmoReq = (PHTTP_BMO_REQ_OBJECT)pAdvancedProxy->AcquireBmoReq((ANSC_HANDLE)pAdvancedProxy);

        if ( !pBmoReq )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoReq = (ANSC_HANDLE)pBmoReq;
        }

        pBmoReq->SetHfpIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf           );
        pBmoReq->SetTmhIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pTmhClientIf     );
        pBmoReq->SetTransactionId((ANSC_HANDLE)pBmoReq, 0                             );
        pBmoReq->SetClientAddr   ((ANSC_HANDLE)pBmoReq, pClientSocket->PeerAddress.Dot);
        pBmoReq->SetClientPort   ((ANSC_HANDLE)pBmoReq, pClientSocket->PeerPort       );
        pBmoReq->SetDaemonAddr   ((ANSC_HANDLE)pBmoReq, pClientSocket->HostAddress.Dot);
        pBmoReq->SetDaemonPort   ((ANSC_HANDLE)pBmoReq, pClientSocket->HostPort       );
    }

    if ( !pBmoRep )
    {
        pBmoRep = (PHTTP_BMO_REP_OBJECT)pAdvancedProxy->AcquireBmoRep((ANSC_HANDLE)pAdvancedProxy);

        if ( !pBmoRep )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoRep = (ANSC_HANDLE)pBmoRep;
        }

        pBmoRep->SetHfpIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf      );
        pBmoRep->SetTmhIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pTmhServerIf);
        pBmoRep->SetTransactionId((ANSC_HANDLE)pBmoRep, 0                        );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to disable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT)pMyObject->hOwnerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT       )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep        = (PHTTP_BMO_REP_OBJECT       )pMyObject->hBmoRep;

    if ( pBmoReq )
    {
        pMyObject->hBmoReq = (ANSC_HANDLE)NULL;

          pAdvancedProxy->ReleaseBmoReq
                (
                    (ANSC_HANDLE)pAdvancedProxy,
                    (ANSC_HANDLE)pBmoReq
                );
    }

    if ( pBmoRep )
    {
        pMyObject->hBmoRep = (ANSC_HANDLE)NULL;

        pAdvancedProxy->ReleaseBmoRep
                (
                    (ANSC_HANDLE)pAdvancedProxy,
                    (ANSC_HANDLE)pBmoRep
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT  )hThisObject;
    
    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT  )hThisObject;
    
    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
