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

    module:	tls_cco_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the TLS Conn Controller Object.

        *   TlsCcoAcqAccess
        *   TlsCcoRelAccess
        *   TlsCcoEngage
        *   TlsCcoCancel
        *   TlsCcoConfig

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
        TlsCcoAcqAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoRelAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_HAND_SHAKER_OBJECT         pTlsHandShaker   = (PTLS_HAND_SHAKER_OBJECT      )pMyObject->hTlsHandShaker;
    PTLS_ALERT_HANDLER_OBJECT       pTlsAlertHandler = (PTLS_ALERT_HANDLER_OBJECT    )pMyObject->hTlsAlertHandler;
    PTLS_CIPHER_SWITCH_OBJECT       pTlsCipherSwitch = (PTLS_CIPHER_SWITCH_OBJECT    )pMyObject->hTlsCipherSwitch;
    PTLS_APP_CONNECTOR_OBJECT       pTlsAppConnector = (PTLS_APP_CONNECTOR_OBJECT    )pMyObject->hTlsAppConnector;
    PTLS_CBC_INTERFACE              pTlsCbcIf        = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf        = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf        = (PTLS_TSA_INTERFACE           )pMyObject->hTlsTsaIf;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    /*
     * We have postponed the creation of all the protocol objects until now. Such delayed object
     * creation gives us following benefits:
     *
     *      - allow connection to be configured before object creation
     *      - flexible and efficient memory management
     */
    if ( !pTlsRecordKeeper )
    {
        pTlsRecordKeeper =
            (PTLS_RECORD_KEEPER_OBJECT)TlsCreateRecordKeeper
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsRecordKeeper )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsRecordKeeper = (ANSC_HANDLE)pTlsRecordKeeper;
        }

        pTlsRecordKeeper->hTlsCbcIf     = (ANSC_HANDLE)pTlsCbcIf;
        pTlsRecordKeeper->hTlsMecIf     = (ANSC_HANDLE)pTlsMecIf;
        pTlsRecordKeeper->hTlsTsaIf     = (ANSC_HANDLE)pTlsTsaIf;
        pTlsRecordKeeper->Engage     ((ANSC_HANDLE)pTlsRecordKeeper);
    }

    if ( !pTlsHandShaker )
    {
        if ( pConnParams->bTlsClient )
        {
            pTlsHandShaker =
                (PTLS_HAND_SHAKER_OBJECT)TlsCreateHsoClient
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
        }
        else
        {
            pTlsHandShaker =
                (PTLS_HAND_SHAKER_OBJECT)TlsCreateHsoServer
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
        }

        if ( !pTlsHandShaker )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsHandShaker = (ANSC_HANDLE)pTlsHandShaker;
        }

        pTlsHandShaker->hTlsCbcIf       = (ANSC_HANDLE)pTlsCbcIf;
        pTlsHandShaker->hTlsMecIf       = (ANSC_HANDLE)pTlsMecIf;
        pTlsHandShaker->hTlsTsaIf       = (ANSC_HANDLE)pTlsTsaIf;
        pTlsHandShaker->Engage     ((ANSC_HANDLE)pTlsHandShaker);
    }

    if ( !pTlsAlertHandler )
    {
        pTlsAlertHandler =
            (PTLS_ALERT_HANDLER_OBJECT)TlsCreateAlertHandler
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsAlertHandler )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsAlertHandler = (ANSC_HANDLE)pTlsAlertHandler;
        }

        pTlsAlertHandler->hTlsCbcIf     = (ANSC_HANDLE)pTlsCbcIf;
        pTlsAlertHandler->hTlsMecIf     = (ANSC_HANDLE)pTlsMecIf;
        pTlsAlertHandler->hTlsTsaIf     = (ANSC_HANDLE)pTlsTsaIf;
        pTlsAlertHandler->Engage     ((ANSC_HANDLE)pTlsAlertHandler);
    }

    if ( !pTlsCipherSwitch )
    {
        pTlsCipherSwitch =
            (PTLS_CIPHER_SWITCH_OBJECT)TlsCreateCipherSwitch
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsCipherSwitch )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsCipherSwitch = (ANSC_HANDLE)pTlsCipherSwitch;
        }

        pTlsCipherSwitch->hTlsCbcIf     = (ANSC_HANDLE)pTlsCbcIf;
        pTlsCipherSwitch->hTlsMecIf     = (ANSC_HANDLE)pTlsMecIf;
        pTlsCipherSwitch->hTlsTsaIf     = (ANSC_HANDLE)pTlsTsaIf;
        pTlsCipherSwitch->Engage     ((ANSC_HANDLE)pTlsCipherSwitch);
    }

    if ( !pTlsAppConnector )
    {
        pTlsAppConnector =
            (PTLS_APP_CONNECTOR_OBJECT)TlsCreateAppConnector
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsAppConnector )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsAppConnector = (ANSC_HANDLE)pTlsAppConnector;
        }

        pTlsAppConnector->hTlsCbcIf     = (ANSC_HANDLE)pTlsCbcIf;
        pTlsAppConnector->hTlsMecIf     = (ANSC_HANDLE)pTlsMecIf;
        pTlsAppConnector->hTlsTsaIf     = (ANSC_HANDLE)pTlsTsaIf;
        pTlsAppConnector->Engage     ((ANSC_HANDLE)pTlsAppConnector);
    }

    /*
     * Four record protocol clients are specified in RFC 2246: the handshake protocol, the alert
     * protocol, the change cipher spec protocol, and the application data protocol. In order to
     * allow extension of the TLS protocol, additional record types can be supported by the record
     * protocol. The TLS_CBC interface centralizes the connection control, including exception
     * handling, session management, and coordination.
     */
    if ( TRUE )
    {
        pTlsRecordKeeper->AddRecordClient((ANSC_HANDLE)pTlsRecordKeeper, (ANSC_HANDLE)pTlsHandShaker  );
        pTlsRecordKeeper->AddRecordClient((ANSC_HANDLE)pTlsRecordKeeper, (ANSC_HANDLE)pTlsAlertHandler);
        pTlsRecordKeeper->AddRecordClient((ANSC_HANDLE)pTlsRecordKeeper, (ANSC_HANDLE)pTlsCipherSwitch);
        pTlsRecordKeeper->AddRecordClient((ANSC_HANDLE)pTlsRecordKeeper, (ANSC_HANDLE)pTlsAppConnector);
    }

    if ( TRUE )
    {
        pMyObject->bActive = TRUE;
        pMyObject->bBroken = FALSE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_HAND_SHAKER_OBJECT         pTlsHandShaker   = (PTLS_HAND_SHAKER_OBJECT      )pMyObject->hTlsHandShaker;
    PTLS_ALERT_HANDLER_OBJECT       pTlsAlertHandler = (PTLS_ALERT_HANDLER_OBJECT    )pMyObject->hTlsAlertHandler;
    PTLS_CIPHER_SWITCH_OBJECT       pTlsCipherSwitch = (PTLS_CIPHER_SWITCH_OBJECT    )pMyObject->hTlsCipherSwitch;
    PTLS_APP_CONNECTOR_OBJECT       pTlsAppConnector = (PTLS_APP_CONNECTOR_OBJECT    )pMyObject->hTlsAppConnector;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
        pMyObject->bBroken = FALSE;
    }

    if ( TRUE )
    {
        if(pTlsRecordKeeper) /*RDKB-6308, CID-24258, NULL check before use*/
        {
            pTlsRecordKeeper->PopRecordClient((ANSC_HANDLE)pTlsRecordKeeper, TLS_RECORD_TYPE_handshake);
            pTlsRecordKeeper->PopRecordClient((ANSC_HANDLE)pTlsRecordKeeper, TLS_RECORD_TYPE_alert);
            pTlsRecordKeeper->PopRecordClient((ANSC_HANDLE)pTlsRecordKeeper, TLS_RECORD_TYPE_changeCipherSpec);
            pTlsRecordKeeper->PopRecordClient((ANSC_HANDLE)pTlsRecordKeeper, TLS_RECORD_TYPE_applicationData);
        }
    }

    if ( pTlsHandShaker )
    {
        pTlsHandShaker->Reset((ANSC_HANDLE)pTlsHandShaker);
        pTlsHandShaker->Cancel((ANSC_HANDLE)pTlsHandShaker);
        pTlsHandShaker->Remove((ANSC_HANDLE)pTlsHandShaker);

        pMyObject->hTlsHandShaker = (ANSC_HANDLE)NULL;
    }

    if ( pTlsAlertHandler )
    {
        pTlsAlertHandler->Cancel((ANSC_HANDLE)pTlsAlertHandler);
        pTlsAlertHandler->Remove((ANSC_HANDLE)pTlsAlertHandler);

        pMyObject->hTlsAlertHandler = (ANSC_HANDLE)NULL;
    }

    if ( pTlsCipherSwitch )
    {
        pTlsCipherSwitch->Cancel((ANSC_HANDLE)pTlsCipherSwitch);
        pTlsCipherSwitch->Remove((ANSC_HANDLE)pTlsCipherSwitch);

        pMyObject->hTlsCipherSwitch = (ANSC_HANDLE)NULL;
    }

    if ( pTlsAppConnector )
    {
        pTlsAppConnector->Cancel((ANSC_HANDLE)pTlsAppConnector);
        pTlsAppConnector->Remove((ANSC_HANDLE)pTlsAppConnector);

        pMyObject->hTlsAppConnector = (ANSC_HANDLE)NULL;
    }

    if ( pTlsRecordKeeper )
    {
        pTlsRecordKeeper->Cancel((ANSC_HANDLE)pTlsRecordKeeper);
        pTlsRecordKeeper->Remove((ANSC_HANDLE)pTlsRecordKeeper);

        pMyObject->hTlsRecordKeeper = (ANSC_HANDLE)NULL;
    }

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoConfig
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTlsConnParams
            );

    description:

        This function is called to configure the TLS connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTlsConnParams
                Specifies the connection parameters to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoConfig
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnParams
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams      = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;
    PTLS_MEC_INTERFACE              pTlsMecIf        = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_MODULE_PARAMS              pModuleParams    = (PTLS_MODULE_PARAMS           )pTlsMecIf->GetModuleParams(pTlsMecIf->hOwnerContext);
    ULONG                           i                = 0;

    *pConnParams = *(PTLS_CONNECTION_PARAMS)hTlsConnParams;

    /*
     * We allow a TLS handshake session to propose a customized list of cipher suites, which over-
     * rides the default/complete cipher suite list supported by the CryptoProvider. This list can
     * be passed in when a new TLS connection is created and MUST be a subset of the default list.
     */
    if ( (pConnParams  ->CipherSuiteCount == 0) &&
         (pModuleParams->CipherSuiteCount != 0) )
    {
        pConnParams->CipherSuiteCount = pModuleParams->CipherSuiteCount;

        for ( i = 0; i < pModuleParams->CipherSuiteCount; i++ )
        {
            pConnParams->CipherSuiteArray[i] = pModuleParams->CipherSuiteArray[i];
        }
    }
    else if ( (pConnParams  ->CipherSuiteCount == 0) &&
              (pModuleParams->CipherSuiteCount == 0) )
    {
        ULONG                       j = 0;

        if ( pModuleParams->bStrongSecurity )
        {
            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_RSA )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            pConnParams->CipherSuiteCount = j;
        }
        else if ( pModuleParams->bExportableOnly )
        {
            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_RC4_40_MD5;
            }

            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5;
            }

            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_RSA )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_EXPORT_WITH_RC4_40_MD5;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_EXPORT_WITH_DES40_CBC_SHA;
            }

            pConnParams->CipherSuiteCount = j;
        }
        else
        {
            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_RSA )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_WITH_3DES_EDE_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_DES_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_IDEA_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_RC4_128_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_RC4_128_MD5;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_WITH_DES_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_WITH_DES_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_WITH_DES_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_WITH_DES_CBC_SHA;
            }

            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_RC4_40_MD5;
            }

            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5;
            }

            if ( (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING_GT512) ||
                 (pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT_LE512) )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DH_RSA )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_NULL_MD5;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_NULL_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_WITH_3DES_EDE_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_WITH_DES_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_WITH_RC4_128_MD5;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_EXPORT_WITH_DES40_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_EXPORT_WITH_RC4_40_MD5;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_AES_128_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_WITH_AES_128_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_WITH_AES_128_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_WITH_AES_128_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_WITH_AES_128_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_WITH_AES_128_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_ENCRYPT )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_RSA_WITH_AES_256_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_DSS_WITH_AES_256_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_RSA_WITH_AES_256_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_DSS )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_DSS_WITH_AES_256_CBC_SHA;
            }

            if ( pModuleParams->CertConfigBits & TLS_CERT_BIT_RSA_SIGNING )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DHE_RSA_WITH_AES_256_CBC_SHA;
            }

            if ( TRUE )
            {
                pConnParams->CipherSuiteArray[j++] = TLS_DH_anon_WITH_AES_256_CBC_SHA;
            }

            pConnParams->CipherSuiteCount = j;
        }
    }

    if ( (pConnParams  ->CompressionCount == 0) &&
         (pModuleParams->CompressionCount != 0) )
    {
        pConnParams->CompressionCount = pModuleParams->CompressionCount;

        for ( i = 0; i < pModuleParams->CompressionCount; i++ )
        {
            pConnParams->CompressionArray[i] = pModuleParams->CompressionArray[i];
        }
    }
    else if ( (pConnParams  ->CompressionCount == 0) &&
              (pModuleParams->CompressionCount == 0) )
    {
        ULONG                       j = 0;

        pConnParams->CompressionArray[j++] = TLS_COMPRESSION_METHOD_NULL;
        pConnParams->CompressionCount      = j;
    }

    return  ANSC_STATUS_SUCCESS;
}
