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

    module:	tls_cco_control.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the TLS Conn Controller Object.

        *   TlsCcoStartHandshake
        *   TlsCcoCloseSession

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
        TlsCcoStartHandshake
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start the TLS handshake process.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoStartHandshake
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject        = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_HAND_SHAKER_OBJECT         pTlsHandShaker   = (PTLS_HAND_SHAKER_OBJECT      )pMyObject->hTlsHandShaker;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    return pTlsHandShaker->Start((ANSC_HANDLE)pTlsHandShaker);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoCloseSession
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the TLS session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCcoCloseSession
    (
        ANSC_HANDLE                 hThisObject
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
                TLS_ALERT_LEVEL_warning,
                TLS_ALERT_DESCR_closeNotify
            );
}
