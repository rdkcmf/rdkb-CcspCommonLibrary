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

    module:	web_sso_fumif.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Simple Server Object.

        *   WebSsoRegisterMdhIf
        *   WebSsoGetMdhIf

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        08/23/07    initial revision.

**********************************************************************/


#include "web_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoRegisterMdhIf
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pRequestUri,
                ANSC_HANDLE                 hMdhIf
            )

    description:

        This function is called to register MDH interface associated
        with the given Request URI.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pRequestUri
                Request URI.

                ANSC_HANDLE                 hMdhIf
                MDH Interface to be registered.

    return:     object state.

**********************************************************************/

ANSC_STATUS
WebSsoRegisterMdhIf
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri,
        ANSC_HANDLE                 hMdhIf
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT)hThisObject;
    PHTTP_FUM_INTERFACE             pFumIf       = (PHTTP_FUM_INTERFACE      )pMyObject->hFumIf;
    PHTTP_FUM_ENTITY                pEntity      = NULL;

    if ( (ANSC_HANDLE)NULL == pFumIf->GetMdhIf(pFumIf->hOwnerContext, pRequestUri) )
    {
        HttpFumEntityAlloc(pEntity, (char *)pRequestUri, hMdhIf);
        if ( !pEntity )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscAcquireLock(&pMyObject->MdhSListLock);
            AnscSListPushEntry(&pMyObject->MdhSList, &pEntity->Linkage);
            AnscReleaseLock(&pMyObject->MdhSListLock);
        }
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebSsoGetMdhIf
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pRequestUri
            )

    description:

        This function is called to get MDH interface associated
        with the given Request URI.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pRequestUri
                Request URI.

    return:     associated MDH interface, NULL returned if no match
                against the request uri.

**********************************************************************/

ANSC_HANDLE
WebSsoGetMdhIf
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT)hThisObject;
    PHTTP_FUM_ENTITY                pEntity      = NULL;
    PSINGLE_LINK_ENTRY              pEntry       = NULL;
    ANSC_HANDLE                     hMdhIf       = NULL;

    if ( !pRequestUri )
    {
        return (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock(&pMyObject->MdhSListLock);

    pEntry = AnscSListGetFirstEntry(&pMyObject->MdhSList);
    while ( pEntry )
    {
        pEntity = ACCESS_HTTP_FUM_ENTITY(pEntry);
        pEntry  = AnscSListGetNextEntry(pEntry);

        if ( strcasecmp((char *)pEntity->pUri, (char *)pRequestUri) == 0 )
        {
            hMdhIf = pEntity->hMdhIf;
            break;
        }
    }

    AnscReleaseLock(&pMyObject->MdhSListLock);

    return hMdhIf;
}


