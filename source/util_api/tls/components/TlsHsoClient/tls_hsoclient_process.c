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

    module:	tls_hsoclient_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Hso Client Object.

        *   TlsHsoClientRecv
        *   TlsHsoClientSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/02/03    initial revision.

**********************************************************************/


#include "tls_hsoclient_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecv
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
TlsHsoClientRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject           = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj      = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_CBC_INTERFACE              pTlsCbcIf           = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_HANDSHAKE_HEADER           pTlsHandshakeHeader = (PTLS_HANDSHAKE_HEADER        )buffer;
    BOOL                            bValidTlsHandshake  = (BOOL                         )TRUE;
    ULONG                           ulRestSize          = (ULONG                        )ulSize;
    void*                           pMsgBuffer          = (void*                        )buffer;
    ULONG                           ulMsgSize           = (ULONG                        )0;
    BOOL                            bNewMsgBuffer       = (BOOL                         )FALSE;
    void*                           pOrgMsgBuffer       = (void*                        )pMsgBuffer;

    pStateTimerObj->Stop((ANSC_HANDLE)pStateTimerObj);

    /*
     * If the "FragMsgBuffer" is not NULL, we've already received fragments of the current hand-
     * shake message, which was sent in several records by the peer. We need to append the newly
     * received record to the previous fragments, and process from the beginning.
     */
    if ( pMyObject->FragMsgBuffer )
    {
        returnStatus =
            pMyObject->SaveFragHandshakeMsg
                (
                    (ANSC_HANDLE)pMyObject,
                    pMsgBuffer,
                    ulRestSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }

        /*
         * Now the "FragMsgBuffer" field has all the fragments we've received so far, to simplify
         * the processing logic, we detach the pointer from the object. Note that if this message
         * is NOT the last fragment, the wrong buffer will be saved again. Inefficient, yes! But
         * less prone to problems.
         */
        pMsgBuffer          = pMyObject->FragMsgBuffer;
        pTlsHandshakeHeader = (PTLS_HANDSHAKE_HEADER)pMsgBuffer;
        ulRestSize          = ulSize = pMyObject->FragMsgBufferSize;
        bNewMsgBuffer       = TRUE;
        pOrgMsgBuffer       = pMsgBuffer;

        pMyObject->FragMsgBuffer     = NULL;
        pMyObject->FragMsgBufferSize = TLS_HSO_DEF_MSG_BUFFER_SIZE;
        pMyObject->FragMsgOffset     = 0;
    }

    /*
     * The record layer fragments information blocks into TLSPlaintext records carrying data in
     * chunks of 2^14 bytes or less. Client message boundaries are not preserved in the record
     * layer (i.e., multiple client messages of the same ContentType may be coalesced into a single
     * TLSPlaintext record, or a single message may be fragmented across several records). To
     * simplify the implementation, we make a reasonable assumption: multiple handshake messages
     * may be coalesced into a single record, but a single handshake message may not be fragmented.
     */
    while ( ulRestSize >= sizeof(TLS_HANDSHAKE_HEADER) )
    {
        pTlsHandshakeHeader = (PTLS_HANDSHAKE_HEADER)pMsgBuffer;
        ulMsgSize           = TlsHandshakeGetSize(pTlsHandshakeHeader);

        if ( ulRestSize < ulMsgSize )
        {
            /*
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decodeError
                    );
            */
            /*
             * If we are here, the peer has broken a long handshake message into several records.
             * We must reassemble all the fragments before processing the handshake message.
             */
            returnStatus =
                pMyObject->SaveFragHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pMsgBuffer,
                        ulRestSize
                    );

            goto  EXIT1;
        }
        else
        {
            ulRestSize -= ulMsgSize;
            pMsgBuffer  = (void*)((ULONG)pMsgBuffer + ulMsgSize);
        }

        TlsHandshakeValidate(pTlsHandshakeHeader, ulMsgSize, bValidTlsHandshake);

        if ( !bValidTlsHandshake )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decodeError
                    );

            goto  EXIT1;
        }

        if ( (TlsHandshakeGetMsgType(pTlsHandshakeHeader) != TLS_HANDSHAKE_TYPE_helloRequest) &&
             (TlsHandshakeGetMsgType(pTlsHandshakeHeader) != TLS_HANDSHAKE_TYPE_finished    ) )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHandshakeHeader,
                        ulMsgSize
                    );
        }

        switch ( TlsHandshakeGetMsgType(pTlsHandshakeHeader) )
        {
            case    TLS_HANDSHAKE_TYPE_helloRequest :

                    returnStatus =
                        pMyObject->RecvHelloRequest
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_serverHello :

                    returnStatus =
                        pMyObject->RecvServerHello
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_certificate :

                    returnStatus =
                        pMyObject->RecvCertificate
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_serverKeyExchange :

                    returnStatus =
                        pMyObject->RecvServerKeyExchange
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_certificateRequest :

                    returnStatus =
                        pMyObject->RecvCertRequest
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_serverHelloDone :

                    returnStatus =
                        pMyObject->RecvServerHelloDone
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            case    TLS_HANDSHAKE_TYPE_finished :

                    returnStatus =
                        pMyObject->RecvFinished
                            (
                                (ANSC_HANDLE)pMyObject,
                                TlsHandshakeGetBody  (pTlsHandshakeHeader),
                                TlsHandshakeGetLength(pTlsHandshakeHeader)
                            );

                    break;

            default :

                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_unexpectedMessage
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    break;
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        if ( ulRestSize != 0 )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decodeError
                    );
        }
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( bNewMsgBuffer && pOrgMsgBuffer )
    {
        AnscFreeMemory(pOrgMsgBuffer);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientSend
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
TlsHsoClientSend
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
