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

    module:	tls_seo_states.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the TLS Session Entry Object.

        *   TlsSeoGetSessionState
        *   TlsSeoSetSessionState
        *   TlsSeoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#include "tls_seo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSeoGetSessionState
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSessionState
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSessionState
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSeoGetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    )
{
    PTLS_SESSION_ENTRY_OBJECT       pMyObject        = (PTLS_SESSION_ENTRY_OBJECT)hThisObject;
    PTLS_SESSION_STATE              pSrcSessionState = (PTLS_SESSION_STATE       )&pMyObject->SessionState;
    PTLS_SESSION_STATE              pDstSessionState = (PTLS_SESSION_STATE       )hSessionState;

    *pDstSessionState = *pSrcSessionState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSeoSetSessionState
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSessionState
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSessionState
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSeoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    )
{
    PTLS_SESSION_ENTRY_OBJECT       pMyObject        = (PTLS_SESSION_ENTRY_OBJECT)hThisObject;
    PTLS_SESSION_STATE              pDstSessionState = (PTLS_SESSION_STATE       )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams  = (PTLS_SECURITY_PARAMS     )&pDstSessionState->SecurityParams;
    PTLS_SESSION_STATE              pSrcSessionState = (PTLS_SESSION_STATE       )hSessionState;

    *pDstSessionState = *pSrcSessionState;

    if ( TRUE )
    {
        pSecurityParams->MyProfile     = NULL;
        pSecurityParams->PeerCertChain = NULL;
        pSecurityParams->RsaKeyPub     = NULL;
        pSecurityParams->RsaKeyPrv     = NULL;
        pSecurityParams->DHKeyPubS     = NULL;
        pSecurityParams->DHKeyPrvS     = NULL;
        pSecurityParams->DHKeyPubC     = NULL;
        pSecurityParams->DHKeyPrvC     = NULL;

        pSecurityParams->RsaModulus    = NULL;
        pSecurityParams->RsaExponent   = NULL;
        pSecurityParams->DHPrime       = NULL;
        pSecurityParams->DHGenerator   = NULL;
        pSecurityParams->DHPublicS     = NULL;
        pSecurityParams->DHPublicC     = NULL;
        pSecurityParams->DHPrivate     = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSeoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSeoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_ENTRY_OBJECT       pMyObject       = (PTLS_SESSION_ENTRY_OBJECT)hThisObject;
    PTLS_SESSION_STATE              pSessionState   = (PTLS_SESSION_STATE       )&pMyObject->SessionState;

    TlsCleanSessionState(pSessionState);

    return  ANSC_STATUS_SUCCESS;
}
