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

    module:	sys_rfo_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Repository Driver Object.

        *   SysRfoCreate
        *   SysRfoRemove
        *   SysRfoEnrollObjects
        *   SysRfoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/06/02    initial revision.

**********************************************************************/


#include "sys_rfo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysRfoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Repository Driver Object and
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

ANSC_HANDLE
SysRfoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_REPOSITORY_FOLDER_OBJECT)AnscAllocateMemory(sizeof(SYS_REPOSITORY_FOLDER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_LIGHT_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_REPOSITORY_FOLDER_OID;
    pBaseObject->Create            = SysRfoCreate;
    pBaseObject->Remove            = SysRfoRemove;
    pBaseObject->EnrollObjects     = SysRfoEnrollObjects;
    pBaseObject->Initialize        = SysRfoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoRemove
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
SysRfoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT)hThisObject;
    PSYS_RFO_RENDER_ATTR            pRenderAttr  = (PSYS_RFO_RENDER_ATTR         )pMyObject->hRenderAttr;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE           )pMyObject->hSelfRcaIf;
    PSYS_RCG_INTERFACE              pSelfRcgIf   = (PSYS_RCG_INTERFACE           )pMyObject->hSelfRcgIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE           )pMyObject->hSelfRcmIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pRenderAttr )
    {
        SysFreeRfoRenderAttr(pRenderAttr);
    }

    if ( pSelfRcaIf )
    {
        AnscFreeMemory(pSelfRcaIf);

        pMyObject->hSelfRcaIf = (ANSC_HANDLE)NULL;
    }

    if ( pSelfRcgIf )
    {
        AnscFreeMemory(pSelfRcgIf);

        pMyObject->hSelfRcgIf = (ANSC_HANDLE)NULL;
    }

    if ( pSelfRcmIf )
    {
        AnscFreeMemory(pSelfRcmIf);

        pMyObject->hSelfRcmIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeTsLock(&pMyObject->AccessTsLock   );
    AnscFreeLock  (&pMyObject->FolderTableLock);
    AnscFreeLock  (&pMyObject->RecordTableLock);
    AnscLcoRemove ((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT)hThisObject;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE           )pMyObject->hSelfRcaIf;
    PSYS_RCG_INTERFACE              pSelfRcgIf   = (PSYS_RCG_INTERFACE           )pMyObject->hSelfRcgIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE           )pMyObject->hSelfRcmIf;
    errno_t   rc  = -1;

    if ( !pSelfRcaIf )
    {
        pSelfRcaIf = (PSYS_RCA_INTERFACE)AnscAllocateMemory(sizeof(SYS_RCA_INTERFACE));

        if ( !pSelfRcaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSelfRcaIf = (ANSC_HANDLE)pSelfRcaIf;
        }

        rc = STRCPY_S_NOCLOBBER(pSelfRcaIf->Name, sizeof(pSelfRcaIf->Name), SYS_RCA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSelfRcaIf->InterfaceId   = SYS_RCA_INTERFACE_ID;
        pSelfRcaIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pSelfRcaIf->Size          = sizeof(SYS_RCA_INTERFACE);

        pSelfRcaIf->Notify        = SysRfoRcaNotify;
        pSelfRcaIf->Review        = SysRfoRcaReview;
    }

    if ( !pSelfRcgIf )
    {
        pSelfRcgIf = (PSYS_RCG_INTERFACE)AnscAllocateMemory(sizeof(SYS_RCG_INTERFACE));

        if ( !pSelfRcgIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSelfRcgIf = (ANSC_HANDLE)pSelfRcgIf;
        }

        rc = STRCPY_S_NOCLOBBER(pSelfRcgIf->Name, sizeof(pSelfRcgIf->Name), SYS_RCG_INTERFACE_NAME);
        ERR_CHK(rc);

        pSelfRcgIf->InterfaceId   = SYS_RCG_INTERFACE_ID;
        pSelfRcgIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pSelfRcgIf->Size          = sizeof(SYS_RCG_INTERFACE);

        pSelfRcgIf->NeedsUpdate   = SysRfoRcgNeedsUpdate;
        pSelfRcgIf->Synchronize   = SysRfoRcgSynchronize;
    }

    if ( !pSelfRcmIf )
    {
        pSelfRcmIf = (PSYS_RCM_INTERFACE)AnscAllocateMemory(sizeof(SYS_RCM_INTERFACE));

        if ( !pSelfRcmIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSelfRcmIf = (ANSC_HANDLE)pSelfRcmIf;
        }

        rc = STRCPY_S_NOCLOBBER(pSelfRcmIf->Name, sizeof(pSelfRcmIf->Name), SYS_RCM_INTERFACE_NAME);
        ERR_CHK(rc);

        pSelfRcmIf->InterfaceId   = SYS_RCM_INTERFACE_ID;
        pSelfRcmIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pSelfRcmIf->Size          = sizeof(SYS_RCM_INTERFACE);

        pSelfRcmIf->Notify        = SysRfoRcmNotify;
        pSelfRcmIf->Init          = SysRfoRcmInit;
        pSelfRcmIf->Stop          = SysRfoRcmStop;
    }

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT)hThisObject;
    ULONG                           i            = 0;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscLcoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid               = SYS_REPOSITORY_FOLDER_OID;
    pMyObject->Create            = SysRfoCreate;
    pMyObject->Remove            = SysRfoRemove;
    pMyObject->EnrollObjects     = SysRfoEnrollObjects;
    pMyObject->Initialize        = SysRfoInitialize;

    pMyObject->HashIndex         = 0;
    pMyObject->GlobalFid         = 0;
    pMyObject->GlobalRid         = 0;
    pMyObject->BirthTime         = AnscGetTickInSeconds();
    pMyObject->Timestamp         = AnscGetTickInSeconds();
    pMyObject->hRenderAttr       = (ANSC_HANDLE)NULL;
    pMyObject->hRecycleBin       = (ANSC_HANDLE)NULL;
    pMyObject->hParentFolder     = (ANSC_HANDLE)NULL;
    pMyObject->hUserContext      = (ANSC_HANDLE)NULL;
    pMyObject->hOpenRcaIf        = (ANSC_HANDLE)NULL;
    pMyObject->hOpenRcgIf        = (ANSC_HANDLE)NULL;
    pMyObject->hOpenRcmIf        = (ANSC_HANDLE)NULL;
    pMyObject->RefCount          = 0;
    pMyObject->bActive           = FALSE;

    pMyObject->FolderSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
    pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    pMyObject->FolderArray       = NULL;
    pMyObject->FolderArrayMaxLen = SYS_RFO_DEF_ARRAY_SIZE;
    pMyObject->FolderArrayCurLen = 0;

    pMyObject->RecordSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
    pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    pMyObject->RecordArray       = NULL;
    pMyObject->RecordArrayMaxLen = SYS_RFO_DEF_ARRAY_SIZE;
    pMyObject->RecordArrayCurLen = 0;

    pMyObject->GetBirthTime      = SysRfoGetBirthTime;
    pMyObject->SetBirthTime      = SysRfoSetBirthTime;
    pMyObject->GetRenderAttr     = SysRfoGetRenderAttr;
    pMyObject->SetRenderAttr     = SysRfoSetRenderAttr;
    pMyObject->GetRecycleBin     = SysRfoGetRecycleBin;
    pMyObject->SetRecycleBin     = SysRfoSetRecycleBin;
    pMyObject->GetParentFolder   = SysRfoGetParentFolder;
    pMyObject->SetParentFolder   = SysRfoSetParentFolder;
    pMyObject->GetUserContext    = SysRfoGetUserContext;
    pMyObject->SetUserContext    = SysRfoSetUserContext;
    pMyObject->GetUserReserved   = SysRfoGetUserReserved;
    pMyObject->SetUserReserved   = SysRfoSetUserReserved;
    pMyObject->GetRcaIf          = SysRfoGetRcaIf;
    pMyObject->SetRcaIf          = SysRfoSetRcaIf;
    pMyObject->GetRcgIf          = SysRfoGetRcgIf;
    pMyObject->SetRcgIf          = SysRfoSetRcgIf;
    pMyObject->GetRcmIf          = SysRfoGetRcmIf;
    pMyObject->SetRcmIf          = SysRfoSetRcmIf;

    pMyObject->GetFolderName     = SysRfoGetFolderName;
    pMyObject->SetFolderName     = SysRfoSetFolderName;
    pMyObject->GetFolderType     = SysRfoGetFolderType;
    pMyObject->SetFolderType     = SysRfoSetFolderType;
    pMyObject->GetPermission     = SysRfoGetPermission;
    pMyObject->SetPermission     = SysRfoSetPermission;
    pMyObject->GetContentType    = SysRfoGetContentType;
    pMyObject->SetContentType    = SysRfoSetContentType;

    pMyObject->GetProperty       = SysRfoGetProperty;
    pMyObject->SetProperty       = SysRfoSetProperty;
    pMyObject->ResetProperty     = SysRfoResetProperty;
    pMyObject->Reset             = SysRfoReset;

    pMyObject->AcquireAccess     = SysRfoAcquireAccess;
    pMyObject->ReleaseAccess     = SysRfoReleaseAccess;
    pMyObject->Engage            = SysRfoEngage;
    pMyObject->Cancel            = SysRfoCancel;
    pMyObject->Delete            = SysRfoDelete;
    pMyObject->IssueKey          = SysRfoIssueKey;
    pMyObject->ClearKey          = SysRfoClearKey;

    pMyObject->DelAllFolders     = SysRfoDelAllFolders;
    pMyObject->DelAllRecords     = SysRfoDelAllRecords;
    pMyObject->AddRfoIntoArray   = SysRfoAddRfoIntoArray;
    pMyObject->DelRfoFromArray   = SysRfoDelRfoFromArray;
    pMyObject->CleanUpRfoArray   = SysRfoCleanUpRfoArray;
    pMyObject->AddRroIntoArray   = SysRfoAddRroIntoArray;
    pMyObject->DelRroFromArray   = SysRfoDelRroFromArray;
    pMyObject->CleanUpRroArray   = SysRfoCleanUpRroArray;

    pMyObject->SortFolderByTime  = SysRfoSortFolderByTime;
    pMyObject->SortFolderByName  = SysRfoSortFolderByName;
    pMyObject->SortRecordByTime  = SysRfoSortRecordByTime;
    pMyObject->SortRecordByName  = SysRfoSortRecordByName;

    pMyObject->QueryAbsPath      = SysRfoQueryAbsPath;
    pMyObject->IsRemovable       = SysRfoIsRemovable;
    pMyObject->GrantAccess       = SysRfoGrantAccess;
    pMyObject->GetFolderCount    = SysRfoGetFolderCount;
    pMyObject->GetRecordCount    = SysRfoGetRecordCount;

    pMyObject->AddFolder         = SysRfoAddFolder;
    pMyObject->DelFolder         = SysRfoDelFolder;
    pMyObject->GetFolderByIndex  = SysRfoGetFolderByIndex;
    pMyObject->GetFolderByName   = SysRfoGetFolderByName;

    pMyObject->AddRecord         = SysRfoAddRecord;
    pMyObject->DelRecord         = SysRfoDelRecord;
    pMyObject->GetRecordByIndex  = SysRfoGetRecordByIndex;
    pMyObject->GetRecordByName   = SysRfoGetRecordByName;
    pMyObject->SetRecord         = SysRfoSetRecord;

    pMyObject->GenerateContent   = SysRfoGenerateContent;
    pMyObject->PopulateContent   = SysRfoPopulateContent;
    pMyObject->OpenFirst         = SysRfoOpenFirst;
    pMyObject->CloseLast         = SysRfoCloseLast;

    pMyObject->RcaNotify         = SysRfoRcaNotify;
    pMyObject->RcaReview         = SysRfoRcaReview;

    pMyObject->RcgNeedsUpdate    = SysRfoRcgNeedsUpdate;
    pMyObject->RcgSynchronize    = SysRfoRcgSynchronize;

    pMyObject->RcmNotify         = SysRfoRcmNotify;
    pMyObject->RcmInit           = SysRfoRcmInit;
    pMyObject->RcmStop           = SysRfoRcmStop;

    /* CID 137529 Data race condition */
    AnscInitializeTsLock(&pMyObject->AccessTsLock);

    for ( i = 0; i < SYS_RFO_RFO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->FolderTable[i]);
    }
    AnscInitializeLock(&pMyObject->FolderTableLock);

    for ( i = 0; i < SYS_RFO_RRO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->RecordTable[i]);
    }
    AnscInitializeLock(&pMyObject->RecordTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
