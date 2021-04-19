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

    module:	http_wcso_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Webc Session Object.

        *   HttpWcsoMatchServer
        *   HttpWcsoIsReadyToRequest
        *   HttpWcsoOpen
        *   HttpWcsoClose
        *   HttpWcsoAcquireAccess
        *   HttpWcsoReleaseAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#include "http_wcso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpWcsoMatchServer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                USHORT                      port,
                ULONG                       flags
            );

    description:

        This function is called to match server's name and port.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host
                Specifies the host name of the target server.

                USHORT                      port
                Specifes the port number of the target server.

                ULONG                       flags
                Specifies the operational flags of the session.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpWcsoMatchServer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        USHORT                      port,
        ULONG                       flags
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty    = (PHTTP_WEBC_SESSION_PROPERTY)&pMyObject->Property;

    if ( strcasecmp(pProperty->PeerName, name) == 0 &&
         (pProperty->PeerPort     == port                ) &&
         (pProperty->SessionFlags == flags               ) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpWcsoIsReadyToRequest
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether this session object is
        ready to initiate (another) request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpWcsoIsReadyToRequest
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = (PHTTP_WEBC_TRANS_OBJECT    )NULL;
    BOOL                            bReadyToReq  = TRUE;

    if ( pMyObject->bRemoveMe )
    {
        return  FALSE;
    }

    switch ( pMyObject->SessionState )
    {
        case    HTTP_WCSO_STATE_INITIALIZED :

                bReadyToReq = TRUE;

                break;

        case    HTTP_WCSO_STATE_SERVER_CONNECTED :

                pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetEndWcto((ANSC_HANDLE)pMyObject);

                if ( !pWebcTrans )
                {
                    bReadyToReq = TRUE;
                }
                else if ( (pWebcTrans->GetTransState((ANSC_HANDLE)pWebcTrans) == HTTP_WCTO_STATE_FINISHED) &&
                          (pWebcTrans->IsLastTrans  ((ANSC_HANDLE)pWebcTrans) == FALSE                   ) )
                {
                    bReadyToReq = TRUE;
                }
                else
                {
                    bReadyToReq = FALSE;
                }

                if ( pWebcTrans )
                {
                    pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
                }

                break;

        case    HTTP_WCSO_STATE_FINISHED :

                bReadyToReq = FALSE;

                break;

        default :

                bReadyToReq = FALSE;

                break;
    }

    return  bReadyToReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoOpen
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
HttpWcsoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    
    if ( pMyObject->SessionState != HTTP_WCSO_STATE_INITIALIZED )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus = pMyObject->Connect((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoClose
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
HttpWcsoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = (PANSC_BUFFER_DESCRIPTOR       )pTcpSimpleClient->GetBufferContext((ANSC_HANDLE)pTcpSimpleClient);
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans       = (PHTTP_WEBC_TRANS_OBJECT       )NULL;

    if ( pMyObject->SessionState == HTTP_WCSO_STATE_FINISHED )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->SessionState = HTTP_WCSO_STATE_FINISHED;
    }

    AnscTrace("Http Webc Session Object is to be closed!\n");

    if ( !pMyObject->bRemoveMe )
    {
        pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetEndWcto((ANSC_HANDLE)pMyObject);

        if ( pWebcTrans )
        {
            if ( pWebcTrans->GetTransState((ANSC_HANDLE)pWebcTrans) != HTTP_WCTO_STATE_FINISHED )
            {
                pWebcTrans->Abort
                        (
                            (ANSC_HANDLE)pWebcTrans,
                            HTTP_BSP_EVENT_TIMEOUT
                        );
            }

            pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);
        }
    }

    pMyObject->DelAllWctos((ANSC_HANDLE)pMyObject);

    pTcpSimpleClient->Cancel((ANSC_HANDLE)pTcpSimpleClient);

    if ( pBufferDesp )
    {
        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoAcquireAccess
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
HttpWcsoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    
    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoReleaseAccess
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
HttpWcsoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    
    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
