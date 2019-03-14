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

    module:	tls_cco_cbcif.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the TLS Conn Controller Object.

        *   TlsCcoCbcNotifyEvent
        *   TlsCcoCbcGetConnParams
        *   TlsCcoCbcSetSessionID
        *   TlsCcoCbcSetSessionSharing
        *   TlsCcoCbcSetQuickHandshake
        *   TlsCcoCbcSetCipherSuiteArray
        *   TlsCcoCbcSetCompressionArray
        *   TlsCcoCbcGenerateAlert
        *   TlsCcoCbcChangeCipherSpec

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
        TlsCcoCbcNotifyEvent
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ULONG                       ulError,
                ANSC_HANDLE                 hEventContext
            );

    description:

        This function is called by the TLS protocol objects when
        something happens that needs controller's attention.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the type of event that is happening.

                ULONG                       ulError
                Specifies the error code associated with the event. The
                meaning of this field depends on the event type.

                ANSC_HANDLE                 hEventContext
                Specifies the event-specific context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;
    PTLS_HAND_SHAKER_OBJECT         pTlsHandShaker   = (PTLS_HAND_SHAKER_OBJECT      )pMyObject->hTlsHandShaker;
    PTLS_ALERT_HANDLER_OBJECT       pTlsAlertHandler = (PTLS_ALERT_HANDLER_OBJECT    )pMyObject->hTlsAlertHandler;
    PTLS_MEC_INTERFACE              pTlsMecIf        = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf        = (PTLS_TSA_INTERFACE           )pMyObject->hTlsTsaIf;
    PTLS_HSM_INTERFACE              pTlsHsmIf        = (PTLS_HSM_INTERFACE           )pTlsMecIf->GetTlsHsmIf(pTlsMecIf->hOwnerContext);

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    switch ( ulEvent )
    {
        case    TLS_CBC_EVENT_HANDSHAKE_SUCCEEDED :

                returnStatus =
                    pTlsTsaIf->NotifyEvent
                        (
                            pTlsTsaIf->hOwnerContext,
                            TLS_TSA_EVENT_CONNECTION_ESTABLISHED,
                            ulError,
                            hEventContext
                        );

                break;

        case    TLS_CBC_EVENT_ALERT_RECEIVED :

                if ( ulError == TLS_ALERT_DESCR_closeNotify )
                {
                    returnStatus =
                        pTlsTsaIf->NotifyEvent
                            (
                                pTlsTsaIf->hOwnerContext,
                                TLS_TSA_EVENT_CONNECTION_CLOSED,
                                ulError,
                                hEventContext
                            );
                }
                else
                {
                    /*
                     * Alert messages convey the severity of the message and a description of the
                     * alert. Alert messages with a level of fatal result in the immediate termi-
                     * nation of the connection. In this case, other connections corresponding to
                     * the session may continue, but the session identifier must be invalidated,
                     * preventing the failed session from being used to establish new connections.
                     */
                    returnStatus =
                        pTlsHsmIf->DelHandshakeSession
                            (
                                pTlsHsmIf->hOwnerContext,
                                pConnParams->HostID,
                                pConnParams->PeerID,
                                pConnParams->SessionID,
                                pConnParams->SessionIDSize
                            );

                    pMyObject->bBroken = TRUE;
                    returnStatus       =
                        pTlsTsaIf->NotifyEvent
                            (
                                pTlsTsaIf->hOwnerContext,
                                TLS_TSA_EVENT_CONNECTION_TERMINATED,
                                ulError,
                                hEventContext
                            );
                }

                break;

        case    TLS_CBC_EVENT_ALERT_GENERATED :

                if ( ulError != TLS_ALERT_DESCR_closeNotify )
                {
                    /*
                     * Alert messages convey the severity of the message and a description of the
                     * alert. Alert messages with a level of fatal result in the immediate termi-
                     * nation of the connection. In this case, other connections corresponding to
                     * the session may continue, but the session identifier must be invalidated,
                     * preventing the failed session from being used to establish new connections.
                     */
                    returnStatus =
                        pTlsHsmIf->DelHandshakeSession
                            (
                                pTlsHsmIf->hOwnerContext,
                                pConnParams->HostID,
                                pConnParams->PeerID,
                                pConnParams->SessionID,
                                pConnParams->SessionIDSize
                            );

                    pMyObject->bBroken = TRUE;
                    returnStatus       =
                        pTlsTsaIf->NotifyEvent
                            (
                                pTlsTsaIf->hOwnerContext,
                                (ulError == TLS_ALERT_DESCR_timeout)? TLS_TSA_EVENT_CONNECTION_TIMEOUT : TLS_TSA_EVENT_CONNECTION_TERMINATED,
                                ulError,
                                hEventContext
                            );
                }

                break;

        case    TLS_CBC_EVENT_CCS_RECEIVED :

                /*
                 * The change cipher spec message is sent by both the client and server to notify
                 * the receiving party that subsequent records will be protected under the newly
                 * negotiated CipherSpec and keys. Reception of this message causes the receiver to
                 * instruct the Record Layer to immediately copy the read pending state into the
                 * read current state. Immediately after sending this message, the sender should
                 * instruct the record layer to make the write pending state the write active state.
                 * The change cipher spec message is sent during the handshake after the security
                 * parameters have been agreed upon, but before the verifying finished message is
                 * sent. An unexpected change cipher spec message should generate an unexpected_
                 * message alert. When resuming a previous session, the change cipher spec message
                 * is sent after the hello messages.
                 */
                if ( pTlsHandShaker->ProgressBits & TLS_HSO_PROGRESS_BIT_changeCipherR )
                {
                    returnStatus =
                        pTlsAlertHandler->SendAlert
                            (
                                (ANSC_HANDLE)pTlsAlertHandler,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_unexpectedMessage
                            );
                }
                else
                {
                    returnStatus = pTlsHandShaker->ChangeCipherR((ANSC_HANDLE)pTlsHandShaker);
                }

                break;

        default :

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsCcoCbcGetConnParams
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the TLS protocol objects to
        retrieve the connection parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     connection parameters.

**********************************************************************/

ANSC_HANDLE
TlsCcoCbcGetConnParams
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;

    return  (ANSC_HANDLE)pConnParams;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcSetSessionID
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      session_id,
                ULONG                       session_id_size
            );

    description:

        This function is called by the TLS protocol objects to
        configure the session id of the connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      session_id
                Specifies the session_id to be configured.

                ULONG                       session_id_size
                Specifies the size of the session_id.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcSetSessionID
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;

    if ( session_id_size > TLS_MAX_SESSION_ID_SIZE )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pConnParams->SessionIDSize = session_id_size;

        if ( session_id_size > 0 )
        {
            AnscCopyMemory
                (
                    pConnParams->SessionID,
                    session_id,
                    session_id_size
                );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcSetSessionSharing
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bSessionSharing
            );

    description:

        This function is called by the TLS protocol objects to
        configure the mode of the connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bSessionSharing
                Specifies the mode of the connection.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcSetSessionSharing
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSessionSharing
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;

    pConnParams->bSessionSharing = bSessionSharing;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcSetQuickHandshake
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bQuickHandshake
            );

    description:

        This function is called by the TLS protocol objects to
        configure the mode of the connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bQuickHandshake
                Specifies the mode of the connection.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcSetQuickHandshake
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bQuickHandshake
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;

    pConnParams->bQuickHandshake = bQuickHandshake;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcSetCipherSuiteArray
            (
                ANSC_HANDLE                 hThisObject,
                void*                       item_array,
                ULONG                       item_count
            );

    description:

        This function is called by the TLS protocol objects to
        configure the cipher suite list of the connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       item_array
                Specifies the cipher suite list to be configured.

                ULONG                       item_count
                Specifies the size of cipher suite list.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcSetCipherSuiteArray
    (
        ANSC_HANDLE                 hThisObject,
        void*                       item_array,
        ULONG                       item_count
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;
    USHORT*                         pCipherSuiteList = (USHORT*                      )item_array;
    ULONG                           ulListSize       = AnscGetMin2(item_count, TLS_MAX_CIPHER_SUITE_LIST_LENGTH);
    ULONG                           i                = 0;

    pConnParams->CipherSuiteCount = ulListSize;

    for ( i = 0; i < ulListSize; i++ )
    {
        pConnParams->CipherSuiteArray[i] = pCipherSuiteList[i];
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcSetCompressionArray
            (
                ANSC_HANDLE                 hThisObject,
                void*                       item_array,
                ULONG                       item_count
            );

    description:

        This function is called by the TLS protocol objects to
        configure the compression method list of the connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       item_array
                Specifies the compress method list to be configured.

                ULONG                       item_count
                Specifies the size of compress method list.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcSetCompressionArray
    (
        ANSC_HANDLE                 hThisObject,
        void*                       item_array,
        ULONG                       item_count
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;
    UCHAR*                          pCompressionList = (UCHAR*                       )item_array;
    ULONG                           ulListSize       = AnscGetMin2(item_count, TLS_MAX_CIPHER_SUITE_LIST_LENGTH);
    ULONG                           i                = 0;

    pConnParams->CompressionCount = ulListSize;

    for ( i = 0; i < ulListSize; i++ )
    {
        pConnParams->CompressionArray[i] = pCompressionList[i];
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcGenerateAlert
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       alert_level,
                UCHAR                       alert_descr
            );

    description:

        This function is called by the TLS protocol objects to
        generate an alert.

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
TlsCcoCbcGenerateAlert
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       alert_level,
        UCHAR                       alert_descr
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_ALERT_HANDLER_OBJECT       pTlsAlertHandler = (PTLS_ALERT_HANDLER_OBJECT    )pMyObject->hTlsAlertHandler;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    return
        pTlsAlertHandler->SendAlert
            (
                (ANSC_HANDLE)pTlsAlertHandler,
                alert_level,
                alert_descr
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCbcChangeCipherSpec
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the TLS protocol objects to
        send out a change-cipher-spec message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCbcChangeCipherSpec
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CIPHER_SWITCH_OBJECT       pTlsCipherSwitch = (PTLS_CIPHER_SWITCH_OBJECT    )pMyObject->hTlsCipherSwitch;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    return pTlsCipherSwitch->SendChangeCipherSpec((ANSC_HANDLE)pTlsCipherSwitch);
}
