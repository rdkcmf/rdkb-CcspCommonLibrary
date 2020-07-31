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

    module:	tls_aho_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Alert Handler Object.

        *   TlsAhoRecv
        *   TlsAhoSend
        *   TlsAhoRecvAlert
        *   TlsAhoSendAlert

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


#include "tls_aho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAhoRecv
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
TlsAhoRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PTLS_ALERT_HANDLER_OBJECT       pMyObject          = (PTLS_ALERT_HANDLER_OBJECT  )hThisObject;

    return
        pMyObject->RecvAlert
            (
                (ANSC_HANDLE)pMyObject,
                buffer,
                ulSize
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAhoSend
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
TlsAhoSend
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAhoRecvAlert
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
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

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsAhoRecvAlert
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_ALERT_HANDLER_OBJECT       pMyObject          = (PTLS_ALERT_HANDLER_OBJECT  )hThisObject;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_ALERT_HEADER               pTlsAlertHeader    = (PTLS_ALERT_HEADER          )buffer;

    if ( ulSize != sizeof(TLS_ALERT_HEADER) )
    {
        pTlsCbcIf->GenerateAlert
            (
                pTlsCbcIf->hOwnerContext,
                TLS_ALERT_LEVEL_fatal,
                TLS_ALERT_DESCR_unexpectedMessage
            );

        return  ANSC_STATUS_DISCARD;
    }

    AnscTrace
        (
            "*****\nTLS receive alert: level = %d \ndesp = %d\n*****\n", 
            TlsAlertGetLevel      (pTlsAlertHeader),
            TlsAlertGetDescription(pTlsAlertHeader)
        );

    return
        pTlsCbcIf->NotifyEvent
            (
                pTlsCbcIf->hOwnerContext,
                TLS_CBC_EVENT_ALERT_RECEIVED,
                TlsAlertGetDescription(pTlsAlertHeader),
                (ANSC_HANDLE)pTlsAlertHeader
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsAhoSendAlert
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       alert_level,
                UCHAR                       alert_descr
            );

    description:

        This function is called to generate an outgoing alert.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       alert_level
                Specifies the level of the alert to be generated.

                UCHAR                       alert_descr
                Specifies the description of the alert to be generated.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsAhoSendAlert
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       alert_level,
        UCHAR                       alert_descr
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_ALERT_HANDLER_OBJECT       pMyObject          = (PTLS_ALERT_HANDLER_OBJECT  )hThisObject;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT  )pMyObject->hTlsRecordKeeper;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    TLS_ALERT_HEADER                tlsAlertHeader;

    AnscTrace
        (
            "*****\nTLS generate alert: level = %d \ndesp = %d\n*****\n", 
            alert_level,
            alert_descr
        );

    TlsAlertSetLevel      ((&tlsAlertHeader), alert_level);
    TlsAlertSetDescription((&tlsAlertHeader), alert_descr);

    if ( alert_descr < TLS_ALERT_DESCR_timeout )
    {
        returnStatus =
            pTlsRecordKeeper->Send
                (
                    (ANSC_HANDLE)pTlsRecordKeeper,
                    pMyObject->RecordType,
                    &tlsAlertHeader,
                    sizeof(TLS_ALERT_HEADER)
                );
    }

    if ( alert_descr != TLS_ALERT_DESCR_closeNotify )
    {
        returnStatus =
            pTlsCbcIf->NotifyEvent
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_CBC_EVENT_ALERT_GENERATED,
                    alert_descr,
                    (ANSC_HANDLE)&tlsAlertHeader
                );
    }

    return  returnStatus;
}
