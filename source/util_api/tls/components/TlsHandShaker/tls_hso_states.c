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

    module:	tls_hso_states.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the TLS Hand Shaker Object.

        *   TlsHsoReset

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


#include "tls_hso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoReset
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
TlsHsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject    = (PTLS_HAND_SHAKER_OBJECT)hThisObject;

    pMyObject->ProgressBits = 0;

    if ( TRUE )
    {
        TlsCleanSessionState((&pMyObject->SessionState));
        TlsCleanRecordState ((&pMyObject->RecordStateW));
        TlsCleanRecordState ((&pMyObject->RecordStateR));

        TlsInitSessionState ((&pMyObject->SessionState));
        TlsInitRecordState  ((&pMyObject->RecordStateW));
        TlsInitRecordState  ((&pMyObject->RecordStateR));
    }

    if ( pMyObject->MsgBuffer )
    {
        AnscFreeMemory(pMyObject->MsgBuffer);

        pMyObject->MsgBuffer     = NULL;
        pMyObject->MsgBufferSize = TLS_HSO_DEF_MSG_BUFFER_SIZE;
        pMyObject->MsgOffset     = 0;
    }

    if ( pMyObject->FragMsgBuffer )
    {
        AnscFreeMemory(pMyObject->FragMsgBuffer);

        pMyObject->FragMsgBuffer     = NULL;
        pMyObject->FragMsgBufferSize = TLS_HSO_DEF_MSG_BUFFER_SIZE;
        pMyObject->FragMsgOffset     = 0;
    }

    TlsRcoReset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
