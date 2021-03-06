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

    module:	bwrm_eco_operation.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bwrm Env Controller Object.

        *   BwrmEcoEngage
        *   BwrmEcoCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#include "bwrm_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_FILE_MANAGER_OBJECT       pBwrmFileManager = (PBWRM_FILE_MANAGER_OBJECT    )pMyObject->hBwrmFileManager;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( TRUE )
    {
        pBwrmFileManager->Engage((ANSC_HANDLE)pBwrmFileManager);
        pBwrmPageManager->Engage((ANSC_HANDLE)pBwrmPageManager);
    }

    if ( TRUE )
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_FILE_MANAGER_OBJECT       pBwrmFileManager = (PBWRM_FILE_MANAGER_OBJECT    )pMyObject->hBwrmFileManager;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    if ( TRUE )
    {
        pBwrmFileManager->Cancel((ANSC_HANDLE)pBwrmFileManager);
        pBwrmPageManager->Cancel((ANSC_HANDLE)pBwrmPageManager);
    }

    return  ANSC_STATUS_SUCCESS;
}
