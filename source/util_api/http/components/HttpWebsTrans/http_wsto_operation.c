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

    module:	http_wsto_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Webs Trans Object.

        *   HttpWstoOpen
        *   HttpWstoClose
        *   HttpWstoAcquireAccess
        *   HttpWstoReleaseAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#include "http_wsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoOpen
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
HttpWstoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT    )pMyObject->hOwnerContext;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pWebSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hWebSocket;
    PHTTP_HFP_INTERFACE             pHfpIf        = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhIf        = (PHTTP_TMH_INTERFACE           )pMyObject->hTmhIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;
    HTTP_SIMPLE_SERVER_PROPERTY     httpSsoProperty;

    pSimpleServer->GetProperty
            (
                (ANSC_HANDLE)pSimpleServer,
                (ANSC_HANDLE)&httpSsoProperty
            );

    if ( !pBmoReq )
    {
        pBmoReq = (PHTTP_BMO_REQ_OBJECT)pSimpleServer->AcquireBmoReq((ANSC_HANDLE)pSimpleServer);

        if ( !pBmoReq )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoReq = (ANSC_HANDLE)pBmoReq;
        }

        pBmoReq->SetHfpIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf            );
        pBmoReq->SetTmhIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pTmhIf            );
        pBmoReq->SetTransactionId((ANSC_HANDLE)pBmoReq, 0                              );
        pBmoReq->SetClientAddr   ((ANSC_HANDLE)pBmoReq, pWebSocket->PeerAddress.Dot    );
        pBmoReq->SetClientPort   ((ANSC_HANDLE)pBmoReq, pWebSocket->PeerPort           );
        pBmoReq->SetDaemonAddr   ((ANSC_HANDLE)pBmoReq, httpSsoProperty.HostAddress.Dot);
        pBmoReq->SetDaemonPort   ((ANSC_HANDLE)pBmoReq, httpSsoProperty.HostPort       );
        pBmoReq->SetServerMode   ((ANSC_HANDLE)pBmoReq, pSimpleServer->ServerMode      );
    }

    if ( !pBmoRep )
    {
        pBmoRep = (PHTTP_BMO_REP_OBJECT)pSimpleServer->AcquireBmoRep((ANSC_HANDLE)pSimpleServer);

        if ( !pBmoRep )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoRep = (ANSC_HANDLE)pBmoRep;
        }

        pBmoRep->SetHfpIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf);
        pBmoRep->SetTmhIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pTmhIf);
        pBmoRep->SetTransactionId((ANSC_HANDLE)pBmoRep, 0                  );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoClose
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
HttpWstoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT   )hThisObject;
    PHTTP_SIMPLE_SERVER_OBJECT      pSimpleServer = (PHTTP_SIMPLE_SERVER_OBJECT)pMyObject->hOwnerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq       = (PHTTP_BMO_REQ_OBJECT      )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep       = (PHTTP_BMO_REP_OBJECT      )pMyObject->hBmoRep;

    if ( pBmoReq )
    {
        pMyObject->hBmoReq = (ANSC_HANDLE)NULL;

        pSimpleServer->ReleaseBmoReq
                (
                    (ANSC_HANDLE)pSimpleServer,
                    (ANSC_HANDLE)pBmoReq
                );
    }

    if ( pBmoRep )
    {
        pMyObject->hBmoRep = (ANSC_HANDLE)NULL;

        pSimpleServer->ReleaseBmoRep
                (
                    (ANSC_HANDLE)pSimpleServer,
                    (ANSC_HANDLE)pBmoRep
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoAcquireAccess
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
HttpWstoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT   )hThisObject;
    
    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoReleaseAccess
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
HttpWstoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject     = (PHTTP_WEBS_TRANS_OBJECT   )hThisObject;
    
    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
