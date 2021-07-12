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

    module:	sys_iro_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Info Repository Object.

        *   SysIroCreate
        *   SysIroRemove
        *   SysIroEnrollObjects
        *   SysIroInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/04/02    initial revision.

**********************************************************************/


#include "sys_iro_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysIroCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Info Repository Object and
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
SysIroCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_INFO_REPOSITORY_OBJECT)AnscAllocateMemory(sizeof(SYS_INFO_REPOSITORY_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, SYS_INFO_REPOSITORY_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_INFO_REPOSITORY_OID;
    pBaseObject->Create            = SysIroCreate;
    pBaseObject->Remove            = SysIroRemove;
    pBaseObject->EnrollObjects     = SysIroEnrollObjects;
    pBaseObject->Initialize        = SysIroInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroRemove
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
SysIroRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_IRA_INTERFACE              pIraIf       = (PSYS_IRA_INTERFACE           )pMyObject->hIraIf;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pIraIf )
    {
        AnscFreeMemory(pIraIf);
    }

    if ( pRepDriver )
    {
        pRepDriver->Remove((ANSC_HANDLE)pRepDriver);

        pMyObject->hRepDriver = (ANSC_HANDLE)NULL;
    }

    AnscFreeSemaphore(&pMyObject->AccessSemaphore);
    AnscFreeLock     (&pMyObject->AccessLock     );
    AnscCoRemove     ((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroEnrollObjects
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
SysIroEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_IRA_INTERFACE              pIraIf       = (PSYS_IRA_INTERFACE           )pMyObject->hIraIf;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;
    errno_t   rc    = -1;

    if ( !pRepDriver )
    {
        pRepDriver =
            (PSYS_REPOSITORY_DRIVER_OBJECT)SysCreateRepositoryDriver
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pRepDriver )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRepDriver = (ANSC_HANDLE)pRepDriver;
        }
    }

    if ( !pIraIf )
    {
        pIraIf = (PSYS_IRA_INTERFACE)AnscAllocateMemory(sizeof(SYS_IRA_INTERFACE));

        if ( !pIraIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hIraIf = (ANSC_HANDLE)pIraIf;
        }

        rc = STRCPY_S_NOCLOBBER(pIraIf->Name, sizeof(pIraIf->Name), SYS_IRA_INTERFACE_NAME);
        ERR_CHK(rc);

        pIraIf->InterfaceId        = SYS_IRA_INTERFACE_ID;
        pIraIf->hOwnerContext      = (ANSC_HANDLE)pMyObject;
        pIraIf->Size               = sizeof(SYS_IRA_INTERFACE);

        pIraIf->AcqWriteAccess     = SysIroIraAcqWriteAccess;
        pIraIf->RelWriteAccess     = SysIroIraRelWriteAccess;

        pIraIf->AcqThreadLock      = SysIroIraAcqThreadLock;
        pIraIf->RelThreadLock      = SysIroIraRelThreadLock;

        pIraIf->AddSysFolder       = SysIroIraAddSysFolder;
        pIraIf->AddFolder          = SysIroIraAddFolder;
        pIraIf->AddFolder2         = SysIroIraAddFolder2;
        pIraIf->DelFolder          = SysIroIraDelFolder;
        pIraIf->DelFolder2         = SysIroIraDelFolder2;

        pIraIf->OpenFolder         = SysIroIraOpenFolder;
        pIraIf->OpenFolder2        = SysIroIraOpenFolder2;
        pIraIf->CloseFolder        = SysIroIraCloseFolder;
        pIraIf->ClearFolder        = SysIroIraClearFolder;

        pIraIf->GenerateFolder     = SysIroIraGenerateFolder;
        pIraIf->PopulateFolder     = SysIroIraPopulateFolder;

        pIraIf->QueryFolder        = SysIroIraQueryFolder;
        pIraIf->GetSubFolderCount  = SysIroIraGetSubFolderCount;
        pIraIf->EnumSubFolder      = SysIroIraEnumSubFolder;
        pIraIf->TestSubFolder      = SysIroIraTestSubFolder;

        pIraIf->AddRecord          = SysIroIraAddRecord;
        pIraIf->AddRecord2         = SysIroIraAddRecord2;
        pIraIf->DelRecord          = SysIroIraDelRecord;
        pIraIf->GetRecord          = SysIroIraGetRecord;
        pIraIf->GetRecordByIndex   = SysIroIraGetRecordByIndex;
        pIraIf->SetRecord          = SysIroIraSetRecord;

        pIraIf->QueryRecord        = SysIroIraQueryRecord;
        pIraIf->GetRecordCount     = SysIroIraGetRecordCount;
        pIraIf->EnumRecord         = SysIroIraEnumRecord;
        pIraIf->TestRecord         = SysIroIraTestRecord;

        pIraIf->GetRfoUserContext  = SysIroIraGetRfoUserContext;
        pIraIf->SetRfoUserContext  = SysIroIraSetRfoUserContext;
        pIraIf->GetRfoUserReserved = SysIroIraGetRfoUserReserved;
        pIraIf->SetRfoUserReserved = SysIroIraSetRfoUserReserved;
        pIraIf->GetRfoFolderType   = SysIroIraGetRfoFolderType;
        pIraIf->SetRfoFolderType   = SysIroIraSetRfoFolderType;
        pIraIf->GetRfoPermission   = SysIroIraGetRfoPermission;
        pIraIf->SetRfoPermission   = SysIroIraSetRfoPermission;
        pIraIf->GetRfoRenderAttr   = SysIroIraGetRfoRenderAttr;
        pIraIf->SetRfoRenderAttr   = SysIroIraSetRfoRenderAttr;

        pIraIf->GetRroUserContext  = SysIroIraGetRroUserContext;
        pIraIf->SetRroUserContext  = SysIroIraSetRroUserContext;
        pIraIf->GetRroUserReserved = SysIroIraGetRroUserReserved;
        pIraIf->SetRroUserReserved = SysIroIraSetRroUserReserved;
        pIraIf->GetRroPermission   = SysIroIraGetRroPermission;
        pIraIf->SetRroPermission   = SysIroIraSetRroPermission;
        pIraIf->GetRroRenderAttr   = SysIroIraGetRroRenderAttr;
        pIraIf->SetRroRenderAttr   = SysIroIraSetRroRenderAttr;

        pIraIf->GetCurRecycleBin   = SysIroIraGetCurRecycleBin;
        pIraIf->AttachRecycleBin   = SysIroIraAttachRecycleBin;
        pIraIf->DetachRecycleBin   = SysIroIraDetachRecycleBin;

        pIraIf->SortSubFolders     = SysIroIraSortSubFolders;
        pIraIf->SortRecords        = SysIroIraSortRecords;

        pIraIf->GetSysRamIf        = SysIroIraGetSysRamIf;
        pIraIf->SetSysRamIf        = SysIroIraSetSysRamIf;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroInitialize
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
SysIroInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                   = SYS_INFO_REPOSITORY_OID;
    pMyObject->Create                = SysIroCreate;
    pMyObject->Remove                = SysIroRemove;
    pMyObject->EnrollObjects         = SysIroEnrollObjects;
    pMyObject->Initialize            = SysIroInitialize;

    pMyObject->hSysRamIf             = NULL;
    pMyObject->bActive               = FALSE;

    pMyObject->GetIraIf              = SysIroGetIraIf;
    pMyObject->GetRepDriver          = SysIroGetRepDriver;
    pMyObject->GetProperty           = SysIroGetProperty;
    pMyObject->SetProperty           = SysIroSetProperty;
    pMyObject->ResetProperty         = SysIroResetProperty;
    pMyObject->Reset                 = SysIroReset;

    pMyObject->Engage                = SysIroEngage;
    pMyObject->Cancel                = SysIroCancel;
    pMyObject->AcqAccess             = SysIroAcqAccess;
    pMyObject->RelAccess             = SysIroRelAccess;

    pMyObject->IraAcqWriteAccess     = SysIroIraAcqWriteAccess;
    pMyObject->IraRelWriteAccess     = SysIroIraRelWriteAccess;

    pMyObject->IraAddSysFolder       = SysIroIraAddSysFolder;
    pMyObject->IraAddFolder          = SysIroIraAddFolder;
    pMyObject->IraAddFolder2         = SysIroIraAddFolder2;
    pMyObject->IraDelFolder          = SysIroIraDelFolder;
    pMyObject->IraDelFolder2         = SysIroIraDelFolder2;

    pMyObject->IraOpenFolder         = SysIroIraOpenFolder;
    pMyObject->IraOpenFolder2        = SysIroIraOpenFolder2;
    pMyObject->IraCloseFolder        = SysIroIraCloseFolder;
    pMyObject->IraClearFolder        = SysIroIraClearFolder;

    pMyObject->IraGenerateFolder     = SysIroIraGenerateFolder;
    pMyObject->IraPopulateFolder     = SysIroIraPopulateFolder;

    pMyObject->IraQueryFolder        = SysIroIraQueryFolder;
    pMyObject->IraGetSubFolderCount  = SysIroIraGetSubFolderCount;
    pMyObject->IraEnumSubFolder      = SysIroIraEnumSubFolder;
    pMyObject->IraTestSubFolder      = SysIroIraTestSubFolder;

    pMyObject->IraAddRecord          = SysIroIraAddRecord;
    pMyObject->IraAddRecord2         = SysIroIraAddRecord2;
    pMyObject->IraDelRecord          = SysIroIraDelRecord;
    pMyObject->IraGetRecord          = SysIroIraGetRecord;
    pMyObject->IraSetRecord          = SysIroIraSetRecord;

    pMyObject->IraQueryRecord        = SysIroIraQueryRecord;
    pMyObject->IraGetRecordCount     = SysIroIraGetRecordCount;
    pMyObject->IraEnumRecord         = SysIroIraEnumRecord;
    pMyObject->IraTestRecord         = SysIroIraTestRecord;

    pMyObject->IraGetRfoUserContext  = SysIroIraGetRfoUserContext;
    pMyObject->IraSetRfoUserContext  = SysIroIraSetRfoUserContext;
    pMyObject->IraGetRfoUserReserved = SysIroIraGetRfoUserReserved;
    pMyObject->IraSetRfoUserReserved = SysIroIraSetRfoUserReserved;
    pMyObject->IraGetRfoFolderType   = SysIroIraGetRfoFolderType;
    pMyObject->IraSetRfoFolderType   = SysIroIraSetRfoFolderType;
    pMyObject->IraGetRfoPermission   = SysIroIraGetRfoPermission;
    pMyObject->IraSetRfoPermission   = SysIroIraSetRfoPermission;
    pMyObject->IraGetRfoRenderAttr   = SysIroIraGetRfoRenderAttr;
    pMyObject->IraSetRfoRenderAttr   = SysIroIraSetRfoRenderAttr;

    pMyObject->IraGetRroUserContext  = SysIroIraGetRroUserContext;
    pMyObject->IraSetRroUserContext  = SysIroIraSetRroUserContext;
    pMyObject->IraGetRroUserReserved = SysIroIraGetRroUserReserved;
    pMyObject->IraSetRroUserReserved = SysIroIraSetRroUserReserved;
    pMyObject->IraGetRroPermission   = SysIroIraGetRroPermission;
    pMyObject->IraSetRroPermission   = SysIroIraSetRroPermission;
    pMyObject->IraGetRroRenderAttr   = SysIroIraGetRroRenderAttr;
    pMyObject->IraSetRroRenderAttr   = SysIroIraSetRroRenderAttr;

    pMyObject->IraGetCurRecycleBin   = SysIroIraGetCurRecycleBin;
    pMyObject->IraAttachRecycleBin   = SysIroIraAttachRecycleBin;
    pMyObject->IraDetachRecycleBin   = SysIroIraDetachRecycleBin;

    pMyObject->IraSortSubFolders     = SysIroIraSortSubFolders;
    pMyObject->IraSortRecords        = SysIroIraSortRecords;

    pMyObject->IraGetSysRamIf        = SysIroIraGetSysRamIf;
    pMyObject->IraSetSysRamIf        = SysIroIraSetSysRamIf;

    AnscInitializeSemaphore(&pMyObject->AccessSemaphore, SYS_IRO_MAX_REP_ACCESS_COUNT, SYS_IRO_MAX_REP_ACCESS_COUNT);
    AnscInitializeLock     (&pMyObject->AccessLock);
    AnscInitializeLock     (&pMyObject->ThreadLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
