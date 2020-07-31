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

    module:	tls_cso_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Cipher Switch Object.

        *   TlsCsoRecv
        *   TlsCsoSend
        *   TlsCsoRecvChangeCipherSpec
        *   TlsCsoSendChangeCipherSpec

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


#include "tls_cso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCsoRecv
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
TlsCsoRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PTLS_CIPHER_SWITCH_OBJECT       pMyObject          = (PTLS_CIPHER_SWITCH_OBJECT  )hThisObject;

    return
        pMyObject->RecvChangeCipherSpec
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
        TlsCsoSend
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
TlsCsoSend
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
        TlsCsoRecvChangeCipherSpec
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
TlsCsoRecvChangeCipherSpec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_CIPHER_SWITCH_OBJECT       pMyObject          = (PTLS_CIPHER_SWITCH_OBJECT  )hThisObject;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_CCS_HEADER                 pTlsCcsHeader      = (PTLS_CCS_HEADER            )buffer;

    if ( ulSize != sizeof(TLS_CCS_HEADER) )
    {
        pTlsCbcIf->GenerateAlert
            (
                pTlsCbcIf->hOwnerContext,
                TLS_ALERT_LEVEL_fatal,
                TLS_ALERT_DESCR_unexpectedMessage
            );

        return  ANSC_STATUS_DISCARD;
    }
    else if ( TlsCcsGetType(pTlsCcsHeader) != TLS_CCS_TYPE_changeCipherSpec )
    {
        pTlsCbcIf->GenerateAlert
            (
                pTlsCbcIf->hOwnerContext,
                TLS_ALERT_LEVEL_fatal,
                TLS_ALERT_DESCR_unexpectedMessage
            );

        return  ANSC_STATUS_DISCARD;
    }

    return
        pTlsCbcIf->NotifyEvent
            (
                pTlsCbcIf->hOwnerContext,
                TLS_CBC_EVENT_CCS_RECEIVED,
                TlsCcsGetType(pTlsCcsHeader),
                (ANSC_HANDLE)pTlsCcsHeader
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCsoSendChangeCipherSpec
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to generate a 'change-cipher-spec'
        message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCsoSendChangeCipherSpec
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CIPHER_SWITCH_OBJECT       pMyObject          = (PTLS_CIPHER_SWITCH_OBJECT  )hThisObject;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT  )pMyObject->hTlsRecordKeeper;
    TLS_CCS_HEADER                  tlsCcsHeader;

    TlsCcsSetType((&tlsCcsHeader), TLS_CCS_TYPE_changeCipherSpec);

    return
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                &tlsCcsHeader,
                sizeof(TLS_CCS_HEADER)
            );
}

