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

    module:	dslh_objro_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh Obj Record Object.

        *   DslhObjroCreate
        *   DslhObjroRemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/26/2005    initial revision.
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#include "dslh_objro_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhObjroCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Obj Record Object and
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
ULONG  g_ObjRecord  = 0;
#endif

ANSC_HANDLE
DslhObjroCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PDSLH_OBJ_RECORD_OBJECT         pMyObject    = NULL;
    PDSLH_RVQ_INTERFACE             pDslhRvqIf   = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_OBJ_RECORD_OBJECT)AnscAllocateMemory(sizeof(DSLH_OBJ_RECORD_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->hParentObjRecord            = (ANSC_HANDLE)NULL;
    pMyObject->hDslhCpeController          = (ANSC_HANDLE)NULL;
    pMyObject->hDslhObjController          = (ANSC_HANDLE)NULL;
    pMyObject->hDslhObjEntity              = (ANSC_HANDLE)NULL;
    pMyObject->hIrepFoWmpDatabase          = (ANSC_HANDLE)NULL;
    pMyObject->LastName                    = NULL;
    pMyObject->FullName                    = NULL;
    pMyObject->NextInstanceNumber          = 1;

    pMyObject->SetDslhObjEntity            = DslhObjroSetDslhObjEntity;
    pMyObject->SetLastName                 = DslhObjroSetLastName;
    pMyObject->SetFullName                 = DslhObjroSetFullName;
    pMyObject->Destroy                     = DslhObjroDestroy;
    pMyObject->Reset                       = DslhObjroReset;
	pMyObject->Remove					   = DslhObjroRemove;

    pMyObject->PopulateObjRecords          = DslhObjroPopulateObjRecords;
    pMyObject->PopulateVarRecords          = DslhObjroPopulateVarRecords;
   /*pMyObject->ParamValueChanged           = DslhObjroParamValueChanged; */
    pMyObject->ChildObjectCreated          = DslhObjroChildObjectCreated;
    pMyObject->ChildObjectRemoved          = DslhObjroChildObjectRemoved;
    pMyObject->PopulateObjRecordByName     = DslhObjroPopulateObjRecordByName;
    pMyObject->DeleteObjRecordByName       = DslhObjroDeleteObjRecordByName;

    pMyObject->GetObjRecordCount           = DslhObjroGetObjRecordCount;
    pMyObject->GetObjRecord                = DslhObjroGetObjRecord;
    pMyObject->GetObjRecord2               = DslhObjroGetObjRecord2;
    pMyObject->AddObjRecord                = DslhObjroAddObjRecord;
    pMyObject->DelAllObjRecords            = DslhObjroDelAllObjRecords;
    pMyObject->GetVarRecordCount           = DslhObjroGetVarRecordCount;
    pMyObject->GetVarRecord                = DslhObjroGetVarRecord;
    pMyObject->AddVarRecord                = DslhObjroAddVarRecord;

    pMyObject->GetTotalParamNumber         = DslhObjroGetTotalParamNumber;
    pMyObject->GetAllParamValues           = DslhObjroGetAllParamValues;
    pMyObject->GetAllParamInfos            = DslhObjroGetAllParamInfos;
    pMyObject->GetAllParamAttribs          = DslhObjroGetAllParamAttribs;
    pMyObject->SetAllParamAttribs          = DslhObjroSetAllParamAttribs;

    pMyObject->VerifyChanges               = DslhObjroVerifyChanges;
    pMyObject->CommitChanges               = DslhObjroCommitChanges;
    pMyObject->CommitChanges2              = DslhObjroCommitChanges2;
    pMyObject->CancelChanges               = DslhObjroCancelChanges;
    pMyObject->AddChildObject              = DslhObjroAddChildObject;
    pMyObject->DelChildObject              = DslhObjroDelChildObject;

    AnscQueueInitializeHeader(&pMyObject->ObjroQueue);

    if( TRUE ) /* for ( i = 0; i < DSLH_OBJRO_VARRO_TABLE_SIZE; i++ ) */
    {
        AnscQueueInitializeHeader(&pMyObject->VarroTable);
    }

    if ( !pDslhRvqIf )
    {
        pDslhRvqIf = (PDSLH_RVQ_INTERFACE)AnscAllocateMemory(sizeof(DSLH_RVQ_INTERFACE));

        if ( pDslhRvqIf )
        {
            pMyObject->hDslhRvqIf = (ANSC_HANDLE)pDslhRvqIf;

			rc = STRCPY_S_NOCLOBBER(pDslhRvqIf->Name, sizeof(pDslhRvqIf->Name), DSLH_RVQ_INTERFACE_NAME);
			ERR_CHK(rc);

			pDslhRvqIf->InterfaceId              = DSLH_RVQ_INTERFACE_ID;
			pDslhRvqIf->hOwnerContext            = (ANSC_HANDLE)pMyObject;
			pDslhRvqIf->Size                     = sizeof(DSLH_RVQ_INTERFACE);

			pDslhRvqIf->IsParamUpdated           = DslhObjroRvqIsParamUpdated;
            pDslhRvqIf->ClearParamUpdated        = DslhObjroRvqClearParamUpdated;
			pDslhRvqIf->GetParamValue            = DslhObjroRvqGetParamValue;
			pDslhRvqIf->GetParamValueString      = DslhObjroRvqGetParamValueString;
			pDslhRvqIf->GetParamValueInt         = DslhObjroRvqGetParamValueInt;
			pDslhRvqIf->GetParamValueUint32      = DslhObjroRvqGetParamValueUint32;
			pDslhRvqIf->GetParamValueBool        = DslhObjroRvqGetParamValueBool;
			pDslhRvqIf->GetParamValueUint32Array = DslhObjroRvqGetParamValueUint32Array;
			pDslhRvqIf->GetParamValueUcharArray  = DslhObjroRvqGetParamValueUcharArray;
			pDslhRvqIf->GetParamValueHandle      = DslhObjroRvqGetParamValueHandle;
		}
    }

#ifdef _DEBUG
	g_ObjRecord ++;
#endif

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroRemove
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
DslhObjroRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject    = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_RVQ_INTERFACE             pDslhRvqIf   = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pDslhRvqIf )
    {
        AnscFreeMemory(pDslhRvqIf);

        pMyObject->hDslhRvqIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

