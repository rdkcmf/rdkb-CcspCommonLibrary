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

    module:	tls_aco_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS App Connector Object.

        *   TlsAcoRecv
        *   TlsAcoSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/03    initial revision.

**********************************************************************/


#include "tls_aco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAcoRecv
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

                ANSC_HANDLE                 hReserved
                Specifies the message-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsAcoRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PTLS_APP_CONNECTOR_OBJECT       pMyObject          = (PTLS_APP_CONNECTOR_OBJECT  )hThisObject;
    PTLS_TSA_INTERFACE              pTlsTsaIf          = (PTLS_TSA_INTERFACE         )pMyObject->hTlsTsaIf;
    PANSC_BUFFER_DESCRIPTOR         pTbpBdo            = (PANSC_BUFFER_DESCRIPTOR    )NULL;

    pTbpBdo = AnscAllocateBdo(0, 0, 0);

    if ( !pTbpBdo )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        AnscBdoSetOwner     (pTbpBdo, FALSE );
        AnscBdoSetBuffer    (pTbpBdo, buffer);
        AnscBdoSetBufferSize(pTbpBdo, ulSize);
        AnscBdoSetOffset    (pTbpBdo, 0     );
        AnscBdoSetBlockSize (pTbpBdo, ulSize);
    }

    return
        pTlsTsaIf->RecvAppMessage
            (
                pTlsTsaIf->hOwnerContext,
                (ANSC_HANDLE)pTbpBdo
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAcoSend
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the outgoing message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsAcoSend
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_APP_CONNECTOR_OBJECT       pMyObject          = (PTLS_APP_CONNECTOR_OBJECT  )hThisObject;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT  )pMyObject->hTlsRecordKeeper;

    return
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                buffer,
                ulSize
            );

}
