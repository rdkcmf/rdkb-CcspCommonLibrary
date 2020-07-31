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

    module:	dslh_objeo_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh Obj Entity Object.

        *   DslhObjeoCreate
        *   DslhObjeoRemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        01/11/2011    Bin added "GetVarEntity";
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#include "dslh_objeo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhObjeoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Obj Entity Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/
#ifdef _DEBUG
ULONG  g_ObjEntity  = 0;
#endif

ANSC_HANDLE
DslhObjeoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_OBJ_ENTITY_OBJECT)AnscAllocateMemory(sizeof(DSLH_OBJ_ENTITY_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->hParentObjEntity   = (ANSC_HANDLE)NULL;
    pMyObject->ObjDescr           = NULL;
    pMyObject->LastName           = NULL;
    pMyObject->bForbidNotify      = FALSE;
    pMyObject->InsUpdateTimestamp = AnscGetTickInSeconds();

    pMyObject->EnableForbidNotify = DslhObjeoEnableForbidNotify;
    pMyObject->Reset              = DslhObjeoReset;
    pMyObject->Remove             = DslhObjeoRemove;

    pMyObject->GetObjEntity2      = DslhObjeoGetObjEntity2;
    pMyObject->GetObjEntity3      = DslhObjeoGetObjEntity3;
    pMyObject->AddObjEntity       = DslhObjeoAddObjEntity;
    pMyObject->DelObjEntity       = DslhObjeoDelObjEntity;
    pMyObject->AddVarEntity       = DslhObjeoAddVarEntity;
    pMyObject->DelVarEntity       = DslhObjeoDelVarEntity;
    pMyObject->GetVarEntity       = DslhObjeoGetVarEntity;

    pMyObject->GetObjRecordCount  = DslhObjeoGetObjRecordCount;
    pMyObject->GetObjRecordCount2 = DslhObjeoGetObjRecordCount2;
    pMyObject->GetObjRecord       = DslhObjeoGetObjRecord;
    pMyObject->GetObjRecord2      = DslhObjeoGetObjRecord2;
    pMyObject->AddObjRecord       = DslhObjeoAddObjRecord;
    pMyObject->DelObjRecord       = DslhObjeoDelObjRecord;
    pMyObject->PopulateObjRecordByName     
                                  = DslhObjeoPopulateObjRecordByName;
    pMyObject->DeleteObjRecordByName   
                                  = DslhObjeoDeleteObjRecordByName;

    AnscQueueInitializeHeader(&pMyObject->ObjInsQueue);
    AnscQueueInitializeHeader(&pMyObject->ObjeoQueue );

    AnscQueueInitializeHeader(&pMyObject->VareoTable);

#ifdef _DEBUG
    g_ObjEntity ++;
#endif


    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjeoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjeoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_ENTITY_OBJECT         pMyObject    = (PDSLH_OBJ_ENTITY_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

