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

    module:	dslh_wmpdo_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoGetDslhMprIf
        *   DslhWmpdoGetDslhMpaIf
        *   DslhWmpdoGetSlapVarConverter
        *   DslhWmpdoGetSlapVarMapper
        *   DslhWmpdoGetDslhCpeController
        *   DslhWmpdoSetDslhCpeController
        *   DslhWmpdoGetProperty
        *   DslhWmpdoSetProperty
        *   DslhWmpdoResetProperty
        *   DslhWmpdoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/06/2005    initial revision.
        01/12/2011    Bin added "pRootObjName".

**********************************************************************/


#include "dslh_wmpdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoGetDslhMprIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoGetDslhMprIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    return  pMyObject->hDslhMprIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoGetDslhMpaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoGetDslhMpaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    return  pMyObject->hDslhMpaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoGetSlapVarConverter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoGetSlapVarConverter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    return  pMyObject->hSlapVarConverter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoGetSlapVarMapper
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoGetSlapVarMapper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    return  pMyObject->hSlapVarMapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhWmpdoGetDslhCpeController
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhWmpdoGetDslhCpeController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    return  pMyObject->hDslhCpeController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoSetDslhCpeController
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCpeController
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCpeController
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoSetDslhCpeController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCpeController
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;

    pMyObject->hDslhCpeController = hCpeController;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty    = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;

    *(PDSLH_WMP_DATABASE_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty    = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;

    *pProperty = *(PDSLH_WMP_DATABASE_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject    = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty    = (PDSLH_WMP_DATABASE_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(DSLH_WMP_DATABASE_PROPERTY));

    pProperty->bMonitorEnabled      = TRUE;
    pProperty->MonitorTimerInterval = DSLH_DEF_MONITOR_TIMER_INTERVAL;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoReset
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
DslhWmpdoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_WMP_DATABASE_OBJECT       pMyObject      = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hRootObjEntity;
    PDSLH_WMPDO_MONITOR_PARAM       pMonitorParam  = (PDSLH_WMPDO_MONITOR_PARAM  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY         )NULL;

    pMyObject->ResetObjVarArray(hThisObject);

    if ( pMyObject->LockedEntity )
    {
        AnscFreeMemory(pMyObject->LockedEntity);

        pMyObject->LockedEntity = NULL;
    }

    if ( pMyObject->pRootObjName)
    {
        AnscFreeMemory(pMyObject->pRootObjName);

        pMyObject->pRootObjName = NULL;
    }

    if ( pRootObjRecord )
    {
        pRootObjRecord->Remove((ANSC_HANDLE)pRootObjRecord);

        pMyObject->hRootObjRecord = (ANSC_HANDLE)NULL;
    }

    if ( pRootObjEntity )
    {
        pRootObjEntity->Remove((ANSC_HANDLE)pRootObjEntity);

        pMyObject->hRootObjEntity = (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock(&pMyObject->MpoQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->MpoQueue);

    while ( pSLinkEntry )
    {
        pMonitorParam = ACCESS_DSLH_WMPDO_MONITOR_PARAM(pSLinkEntry);
        pSLinkEntry   = AnscQueuePopEntry(&pMyObject->MpoQueue);

        DslhWmpdoFreeMonitorParam(pMonitorParam);
    }

    AnscReleaseLock(&pMyObject->MpoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
