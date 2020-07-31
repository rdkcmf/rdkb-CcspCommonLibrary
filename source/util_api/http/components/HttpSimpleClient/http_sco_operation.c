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

    module:	http_sco_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Simple Client Object.

        *   HttpScoEngage
        *   HttpScoCancel
        *   HttpScoSessionTdoInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#include "http_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT   )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo  = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pMyObject->ManufactureWcsoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureWctoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureBmoRepPool((ANSC_HANDLE)pMyObject);

    pSessionTdo->SetTimerType((ANSC_HANDLE)pSessionTdo, ANSC_TIMER_TYPE_PERIODIC      );
    pSessionTdo->SetInterval ((ANSC_HANDLE)pSessionTdo, HTTP_SCO_SESSION_TIME_INTERVAL);
    pSessionTdo->Start       ((ANSC_HANDLE)pSessionTdo);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT   )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo  = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pSessionTdo->Stop((ANSC_HANDLE)pSessionTdo);

    pMyObject->DelAllWcsos      ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyWcsoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyWctoPool  ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoReqPool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyBmoRepPool((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoSessionTdoInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Session Timer Descriptor Object
        to clean up all the expired session objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoSessionTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT   )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;
    ULONG                           ulCurTime    = AnscGetTickInSeconds();
	ULONG							ulTimeout;

	ulTimeout = ( pMyObject->SessionIdleTimeout > HTTP_WCSO_TIMEOUT ) ? pMyObject->SessionIdleTimeout : HTTP_WCSO_TIMEOUT;

    AnscAcquireLock(&pMyObject->WcsoTableLock);

    for ( i = 0; i < HTTP_SCO_WCSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WcsoTable[i]);

        while ( pSLinkEntry )
        {
            pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);
            pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

            if ( (pWebcSession->bRemoveMe                                  == TRUE                    ) ||
                 (pWebcSession->GetSessionState((ANSC_HANDLE)pWebcSession) == HTTP_WCSO_STATE_FINISHED) ||
                 ((ulCurTime - pWebcSession->Timestamp)                    >= ulTimeout               ) )
/*
            if ( (pWebcSession->GetSessionState((ANSC_HANDLE)pWebcSession) == HTTP_WCSO_STATE_FINISHED) ||
                 ((ulCurTime - pWebcSession->Timestamp)                    >= ulTimeout             ) )
*/
            {
                AnscTrace("\n   ** Return Http Webc Session Object back to the global pool! ** \n");

                AnscSListPopEntryByLink(&pMyObject->WcsoTable[i], &pWebcSession->Linkage);

                pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);
                pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);
                pWebcSession->Close        ((ANSC_HANDLE)pWebcSession);
                pWebcSession->Return       ((ANSC_HANDLE)pWebcSession);

                if ( (ULONG)pMyObject->hWebClientSession == (ULONG)pWebcSession )
                {
                    pMyObject->hWebClientSession = NULL;
                }
            }
        }
    }

    AnscReleaseLock(&pMyObject->WcsoTableLock);

    return  returnStatus;
}
