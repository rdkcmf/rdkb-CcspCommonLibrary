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

    module:	tls_eco_scsif.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the TLS Env Controller Object.

        *   TlsEcoScsConfigModule
        *   TlsEcoScsCreateConnection
        *   TlsEcoScsConfigConnection
        *   TlsEcoScsRemoveConnection
        *   TlsEcoScsStartConnection
        *   TlsEcoScsCloseConnection
        *   TlsEcoScsQueryConnection
        *   TlsEcoScsRecvTlsMessage
        *   TlsEcoScsSendAppMessage

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#include "tls_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsConfigModule
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsModuleParams
            );

    description:

        This function is called to configure the TLS module as a whole.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsModuleParams
                Specifies the module configuration parameters.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsConfigModule
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsModuleParams
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject        = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pDstModuleParams = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    PTLS_MODULE_PARAMS              pSrcModuleParams = (PTLS_MODULE_PARAMS          )hTlsModuleParams;

    TlsCloneModuleParams(pSrcModuleParams, pDstModuleParams);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoScsCreateConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsTsaIf
            );

    description:

        This function is called to create a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsTsaIf
                Specifies the TLS_TSA interface that is associated with
                the newly created connection.

    return:     tls connection handle.

**********************************************************************/

ANSC_HANDLE
TlsEcoScsCreateConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsTsaIf
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )NULL;

    pTlsConnController =
        (PTLS_CONN_CONTROLLER_OBJECT)TlsCreateConnController
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pTlsConnController )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pTlsConnController->hTlsMecIf       = pMyObject->hTlsMecIf;
        pTlsConnController->hTlsTsaIf       = hTlsTsaIf           ;
    }

    return  (ANSC_HANDLE)pTlsConnController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsConfigConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection,
                ANSC_HANDLE                 hTlsConnParams
            );

    description:

        This function is called to configure a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection to be configured. This
                handle MUST be the one returned by CreateConnection().

                ANSC_HANDLE                 hTlsConnParams
                Specifies the connection parameters to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsConfigConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hTlsConnParams
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return
        pTlsConnController->Config
            (
                (ANSC_HANDLE)pTlsConnController,
                hTlsConnParams
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsRemoveConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection
            );

    description:

        This function is called to remove a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection to be removed. This handle
                MUST be the one returned by CreateConnection().

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsRemoveConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pTlsConnController->AcqAccess((ANSC_HANDLE)pTlsConnController);
    pTlsConnController->RelAccess((ANSC_HANDLE)pTlsConnController);
    pTlsConnController->Cancel   ((ANSC_HANDLE)pTlsConnController);
    pTlsConnController->Remove   ((ANSC_HANDLE)pTlsConnController);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsStartConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection
            );

    description:

        This function is called to start a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection to be started. This handle
                MUST be the one returned by CreateConnection().

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsStartConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pTlsConnController->Engage        ((ANSC_HANDLE)pTlsConnController);
    pTlsConnController->StartHandshake((ANSC_HANDLE)pTlsConnController);

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsCloseConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection
            );

    description:

        This function is called to close a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection to be closed. This handle
                MUST be the one returned by CreateConnection().

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsCloseConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return pTlsConnController->CloseSession((ANSC_HANDLE)pTlsConnController);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TlsEcoScsQueryConnection
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection
            );

    description:

        This function is called to query status of a TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection to be queried. This handle
                MUST be the one returned by CreateConnection().

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsEcoScsQueryConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;

    if ( !pTlsConnController )
    {
        return  FALSE;
    }

    return  pTlsConnController->bActive;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsRecvTlsMessage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection,
                ANSC_HANDLE                 hMessageBdo
            );

    description:

        This function is called to process an incoming TLS message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection on which the message is
                received. This handle MUST be the one returned by
                CreateConnection().

                ANSC_HANDLE                 hMessageBdo
                Specifies the message buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsRecvTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PANSC_PACKET_DESCRIPTOR         pTbpPdo            = (PANSC_PACKET_DESCRIPTOR     )NULL;
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;
    PANSC_BUFFER_DESCRIPTOR         pTbpBdo            = (PANSC_BUFFER_DESCRIPTOR     )hMessageBdo;
    PANSC_BUFFER_DESCRIPTOR         pTbpBdo2           = (PANSC_BUFFER_DESCRIPTOR     )NULL;
    ULONG                           ulHeaderSize       = (ULONG                       )sizeof(TLS_RECORD_HEADER);
    ULONG                           ulRecordSize       = (ULONG                       )0;
    ULONG                           ulPacketSize       = (ULONG                       )0;
    void*                           pRecordBuffer      = (void*                       )NULL;
    PSSL2_RECORD_HEADER             pSsl2RecordHeader  = (PSSL2_RECORD_HEADER         )NULL;
    void*                           pSsl3RecordBuffer  = (void*                       )NULL;
    ANSC_HANDLE                     hMsgReserved       = (ANSC_HANDLE                 )NULL;
    BOOL                            bSsl2Message       = (BOOL                        )FALSE;
    BOOL                            bNewBuffer         = (BOOL                        )FALSE;
    BOOL                            bFreeReserved      = (BOOL                        )FALSE;
    TLS_RECORD_HEADER               tlsRecordHeader;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        /*pTlsConnController->AcqAccess((ANSC_HANDLE)pTlsConnController);*/

        pTbpPdo = (PANSC_PACKET_DESCRIPTOR)pTlsConnController->hTbpPdo;
    }

    if ( AnscBdoGetOwner(pTbpBdo) )
    {
        AnscPdoAttachBdoAtBack(pTbpPdo, pTbpBdo);
    }
    else
    {
        pTbpBdo2 =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    AnscBdoGetBufferSize(pTbpBdo),
                    AnscBdoGetOffset    (pTbpBdo),
                    AnscBdoGetBlockSize (pTbpBdo)
                );

        if ( !pTbpBdo2 )
        {
            AnscFreeBdo((ANSC_HANDLE)pTbpBdo);

            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            AnscCopyMemory
                (
                    AnscBdoGetBlock    (pTbpBdo2),
                    AnscBdoGetBlock    (pTbpBdo ),
                    AnscBdoGetBlockSize(pTbpBdo )
                );

            AnscFreeBdo((ANSC_HANDLE)pTbpBdo);
        }

        AnscPdoAttachBdoAtBack(pTbpPdo, pTbpBdo2);
    }

    /*
     * The TLS Record Protocol is a layered protocol. At each layer, messages may include fields
     * for length, description, and content. The Record Protocol takes messages to be transmitted,
     * fragments the data into manageable blocks, optionally compresses the data, applies a MAC,
     * encrypts, and transmits the result. Received data is decrypted, verified, decompressed, and
     * reassembled, then delivered to higher level clients.
     */
    do
    {
        AnscZeroMemory(&tlsRecordHeader, sizeof(TLS_RECORD_HEADER));

        ulHeaderSize = sizeof(TLS_RECORD_HEADER);
        returnStatus =
            AnscPdoCopyFrom
                (
                    (ANSC_HANDLE)pTbpPdo,
                    &tlsRecordHeader,
                    &ulHeaderSize,
                    0
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            returnStatus = ANSC_STATUS_PENDING;

            break;
        }
        else
        {
            ulRecordSize  = TlsRecordGetSize((&tlsRecordHeader));
            ulPacketSize  = AnscPdoGetPacketSize((ANSC_HANDLE)pTbpPdo);
            hMsgReserved  = (ANSC_HANDLE)NULL;
            bSsl2Message  = FALSE;
            bNewBuffer    = FALSE;
            bFreeReserved = FALSE;

            /*
             * Legacy SSLv2/v3 client implementations always initiate the handshake negotiation
             * with a SSLv2 Client Hello message, while the 'version' field in the message may be
             * actually set to 3.0. Weird, isn't it?
             */
            if ( !TlsRecordIsTypeValid((&tlsRecordHeader)) )
            {
                if ( (tlsRecordHeader.ContentType != 0x80) &&
                     (tlsRecordHeader.ContentType != 0   ) )
                {
                    AnscPdoClean((ANSC_HANDLE)pTbpPdo);

                    returnStatus = ANSC_STATUS_UNAPPLICABLE;

                    break;
                }
                else
                {
                    pSsl2RecordHeader = (PSSL2_RECORD_HEADER)&tlsRecordHeader;

                    if ( Ssl2RecordGetMsgType(pSsl2RecordHeader) != SSL2_MSG_TYPE_CLIENT_HELLO )
                    {
                        AnscPdoClean((ANSC_HANDLE)pTbpPdo);

                        returnStatus = ANSC_STATUS_UNAPPLICABLE;

                        break;
                    }
                    else
                    {
                        bSsl2Message = TRUE;
                        ulRecordSize = Ssl2RecordGetSize(pSsl2RecordHeader);
                    }
                }
            }
        }

        if ( ulPacketSize < ulRecordSize )
        {
            returnStatus = ANSC_STATUS_SUCCESS;

            break;
        }
        else if ( AnscPdoGetBdoCount(pTbpPdo) == 1 )
        {
            pTbpBdo       = AnscPdoGetHeadBdo(pTbpPdo);
            pRecordBuffer = AnscBdoGetBlock  (pTbpBdo);
            bNewBuffer    = FALSE;
        }
        else
        {
            pRecordBuffer = AnscAllocateMemory(ulRecordSize);
            bNewBuffer    = TRUE;

            if ( !pRecordBuffer )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                break;
            }
            else
            {
                returnStatus =
                    AnscPdoCopyFrom
                        (
                            (ANSC_HANDLE)pTbpPdo,
                            pRecordBuffer,
                            &ulRecordSize,
                            0
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pRecordBuffer);

                    returnStatus = ANSC_STATUS_FAILURE;

                    break;
                }
            }
        }

        /*
         * If this message is a SSLv2 message, we need to translate it into a SSLv3/TLS 1.0 message
         * before feeding the message into the TlsConnController. Note that now we only support the
         * translation for the Handshake Client Hello message.
         */
        if ( bSsl2Message )
        {
            pSsl3RecordBuffer = AnscAllocateMemory(512);

            if ( !pSsl3RecordBuffer )
            {
                if ( bNewBuffer )
                {
                    AnscFreeMemory(pRecordBuffer);
                }

                returnStatus = ANSC_STATUS_RESOURCES;

                break;
            }
            else
            {
                returnStatus =
                    pMyObject->Ssl2ToSsl3ClientHello
                        (
                            (ANSC_HANDLE)pMyObject,
                            pRecordBuffer,
                            ulRecordSize,
                            pSsl3RecordBuffer,
                            &ulRecordSize
                        );
            }

            hMsgReserved  = (ANSC_HANDLE)pRecordBuffer;
            bFreeReserved = bNewBuffer;
            pRecordBuffer = pSsl3RecordBuffer;
            bNewBuffer    = TRUE;
        }
        else
        {
            hMsgReserved  = (ANSC_HANDLE)NULL;
            bFreeReserved = FALSE;
        }

        returnStatus =
            pTlsConnController->RecvTlsMessage
                (
                    (ANSC_HANDLE)pTlsConnController,
                    pRecordBuffer,
                    ulRecordSize,
                    hMsgReserved
                );

        if ( bNewBuffer )
        {
            AnscFreeMemory(pRecordBuffer);
        }

        if ( hMsgReserved && bFreeReserved )
        {
            AnscFreeMemory(hMsgReserved);
        }

        AnscPdoShrinkRight((ANSC_HANDLE)pTbpPdo, ulRecordSize);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }
    while ( TRUE );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    /*pTlsConnController->RelAccess((ANSC_HANDLE)pTlsConnController);*/

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoScsSendAppMessage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnection,
                ANSC_HANDLE                 hMessageBdo
            );

    description:

        This function is called to process an outgoing APP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnection
                Specifies the TLS connection on which the message is
                to be sent. This handle MUST be the one returned by
                CreateConnection().

                ANSC_HANDLE                 hMessageBdo
                Specifies the message buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoScsSendAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_CONN_CONTROLLER_OBJECT     pTlsConnController = (PTLS_CONN_CONTROLLER_OBJECT )hTlsConnection;
    PANSC_BUFFER_DESCRIPTOR         pTbpBdo            = (PANSC_BUFFER_DESCRIPTOR     )hMessageBdo;

    if ( !pTlsConnController )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pTlsConnController->SendAppMessage
            (
                (ANSC_HANDLE)pTlsConnController,
                AnscBdoGetBlock    (pTbpBdo),
                AnscBdoGetBlockSize(pTbpBdo)
            );

    AnscFreeBdo((ANSC_HANDLE)pTbpBdo);

    return  returnStatus;
}
