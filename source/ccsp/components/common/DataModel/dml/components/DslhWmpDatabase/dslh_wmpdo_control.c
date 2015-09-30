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

    module:	dslh_wmpdo_control.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Dslh Wmp Database Object.

        *   DslhWmpdoCreateRecordTree
        *   DslhWmpdoConfigRecordTree
        *   DslhWmpdoEnableMonitor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/26/05    initial revision.

**********************************************************************/


#include "dslh_wmpdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoCreateRecordTree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to build the runtime record tree.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoCreateRecordTree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT  )hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_OBJ_RECORD_OBJECT         pRootObjRecord     = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hRootObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pMyObject->hRootObjEntity;


    AnscAcquireTsLock(&pMyObject->AccessTsLock);

    pRootObjRecord->hIrepFoWmpDatabase = pMyObject->hPoamIrepFoWmpDatabase;

    returnStatus = pRootObjRecord->PopulateObjRecords((ANSC_HANDLE)pRootObjRecord);
    returnStatus = pRootObjRecord->PopulateVarRecords((ANSC_HANDLE)pRootObjRecord);

    AnscReleaseTsLock(&pMyObject->AccessTsLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoConfigRecordTree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to load the configuration from the
        system registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoConfigRecordTree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;

    returnStatus = pMyObject->EnableMonitor((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhWmpdoEnableMonitor
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to enable/disable the monitor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies whether the monitor should be enabled or
                disabled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoEnableMonitor
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_WMP_DATABASE_OBJECT       pMyObject          = (PDSLH_WMP_DATABASE_OBJECT    )hThisObject;
    PDSLH_WMP_DATABASE_PROPERTY     pProperty          = (PDSLH_WMP_DATABASE_PROPERTY  )&pMyObject->Property;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT  )pMyObject->hDslhCpeController;
    PDSLH_OBJ_ENTITY_OBJECT         pRootObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT      )pMyObject->hRootObjEntity;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pMonitorTimerObj   = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hMonitorTimerObj;

    pProperty->bMonitorEnabled = bEnabled;

    if ( pProperty->bMonitorEnabled )
    {
        pMonitorTimerObj->SetInterval((ANSC_HANDLE)pMonitorTimerObj, pProperty->MonitorTimerInterval * 1000);
        pMonitorTimerObj->Start      ((ANSC_HANDLE)pMonitorTimerObj);
    }
    else
    {
        pMonitorTimerObj->Stop       ((ANSC_HANDLE)pMonitorTimerObj);
    }

    return  ANSC_STATUS_SUCCESS;
}
