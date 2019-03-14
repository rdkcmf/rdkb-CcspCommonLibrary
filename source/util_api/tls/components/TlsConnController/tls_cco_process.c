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

    module:	tls_cco_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Conn Controller Object.

        *   TlsCcoRecvTlsMessage
        *   TlsCcoSendAppMessage

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#include "tls_cco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoRecvTlsMessage
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an incoming TLS message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the message buffer to be processed.

                ULONG                       ulSize
                Specifies the message size to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the message-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoRecvTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( pMyObject->bBroken )
    {
        return  ANSC_STATUS_FAILURE;
    }

    return
        pTlsRecordKeeper->Recv
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                buffer,
                ulSize,
                hReserved
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoSendAppMessage
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing APP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the message buffer to be processed.

                ULONG                       ulSize
                Specifies the message size to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoSendAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_APP_CONNECTOR_OBJECT       pTlsAppConnector = (PTLS_APP_CONNECTOR_OBJECT    )pMyObject->hTlsAppConnector;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( pMyObject->bBroken )
    {
        return  ANSC_STATUS_FAILURE;
    }

    return
        pTlsAppConnector->Send
            (
                (ANSC_HANDLE)pTlsAppConnector,
                buffer,
                ulSize
            );
}
