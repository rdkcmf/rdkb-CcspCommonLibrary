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

    module:	bwrm_pmo_operation.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bwrm Page Manager Object.

        *   BwrmPmoEngage
        *   BwrmPmoCancel
        *   BwrmPmoCacheTimerInvoke

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


#include "bwrm_pmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoEngage
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
BwrmPmoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoCancel
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
BwrmPmoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pCacheTimerObject->Stop((ANSC_HANDLE)pCacheTimerObject);

    pMyObject->DelAllPages((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoCacheTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called whenever the cache timer is invoked.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoCacheTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty         = (PBWRM_PAGE_MANAGER_PROPERTY  )&pMyObject->Property;
    ULONG                           ulCurTime         = AnscGetTickInSeconds();

    if ( (ulCurTime - pMyObject->Timestamp) > pProperty->CacheTimeout )
    {
        pMyObject->DelAllPages((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}
