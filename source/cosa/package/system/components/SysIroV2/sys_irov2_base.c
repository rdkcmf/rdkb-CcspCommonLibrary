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

    module:	sys_irov2_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Info Repository Object.

        *   SysIroV2Create
        *   SysIroV2Remove
        *   SysIroV2EnrollObjects
        *   SysIroV2Initialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/30/06    initial revision.

**********************************************************************/


#include "sys_irov2_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysIroV2Create
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
SysIroV2Create
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSYS_IROV2_OBJECT               pMyObject    = NULL;
    errno_t  rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_IROV2_OBJECT)AnscAllocateMemory(sizeof(SYS_IROV2_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), SYS_INFO_REPOSITORY_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_INFO_REPOSITORY_OID;
    pBaseObject->Create            = SysIroV2Create;
    pBaseObject->Remove            = SysIroV2Remove;
    pBaseObject->EnrollObjects     = SysIroV2EnrollObjects;
    pBaseObject->Initialize        = SysIroV2Initialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2Remove
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
SysIroV2Remove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IRA_INTERFACE              pIraIf       = (PSYS_IRA_INTERFACE           )pMyObject->hIraIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pIraIf )
    {
        AnscFreeMemory(pIraIf);
    }

    AnscFreeLock     (&pMyObject->AccessLock     );
    AnscCoRemove     ((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2EnrollObjects
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
SysIroV2EnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IRA_INTERFACE              pIraIf       = (PSYS_IRA_INTERFACE           )pMyObject->hIraIf;
    PSYS_IROV2_FOLDER_ENTRY         pRootFolder  = (PSYS_IROV2_FOLDER_ENTRY      )pMyObject->hRootFolder;
    errno_t    rc   = -1;

    if ( !pRootFolder )
    {
        pRootFolder = (PSYS_IROV2_FOLDER_ENTRY)AnscAllocateMemory(sizeof(SYS_IROV2_FOLDER_ENTRY) + _ansc_strlen("/") + 1);

        if ( !pRootFolder )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRootFolder = (ANSC_HANDLE)pRootFolder;

            SysIroV2InitFolderEntry(pRootFolder);

            pRootFolder->FolderName[0] = '/';
            pRootFolder->FolderName[1] = '\0';
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

        pIraIf->AcqWriteAccess     = SysIroV2IraAcqWriteAccess;
        pIraIf->RelWriteAccess     = SysIroV2IraRelWriteAccess;

        pIraIf->AddSysFolder       = SysIroV2IraAddSysFolder;
        pIraIf->AddFolder          = SysIroV2IraAddFolder;
        pIraIf->AddFolder2         = SysIroV2IraAddFolder2;
        pIraIf->DelFolder          = SysIroV2IraDelFolder;
        pIraIf->DelFolder2         = SysIroV2IraDelFolder2;

        pIraIf->OpenFolder         = SysIroV2IraOpenFolder;
        pIraIf->OpenFolder2        = SysIroV2IraOpenFolder2;
        pIraIf->CloseFolder        = SysIroV2IraCloseFolder;
        pIraIf->ClearFolder        = SysIroV2IraClearFolder;

        pIraIf->GenerateFolder     = SysIroV2IraGenerateFolder;
        pIraIf->PopulateFolder     = SysIroV2IraPopulateFolder;

        pIraIf->QueryFolder        = SysIroV2IraQueryFolder;
        pIraIf->GetSubFolderCount  = SysIroV2IraGetSubFolderCount;
        pIraIf->EnumSubFolder      = SysIroV2IraEnumSubFolder;
        pIraIf->TestSubFolder      = SysIroV2IraTestSubFolder;

        pIraIf->AddRecord          = SysIroV2IraAddRecord;
        pIraIf->AddRecord2         = SysIroV2IraAddRecord2;
        pIraIf->DelRecord          = SysIroV2IraDelRecord;
        pIraIf->GetRecord          = SysIroV2IraGetRecord;
        pIraIf->SetRecord          = SysIroV2IraSetRecord;

        pIraIf->QueryRecord        = SysIroV2IraQueryRecord;
        pIraIf->GetRecordCount     = SysIroV2IraGetRecordCount;
        pIraIf->EnumRecord         = SysIroV2IraEnumRecord;
        pIraIf->TestRecord         = SysIroV2IraTestRecord;

        pIraIf->GetRfoUserContext  = SysIroV2IraGetRfoUserContext;
        pIraIf->SetRfoUserContext  = SysIroV2IraSetRfoUserContext;
        pIraIf->GetRfoUserReserved = SysIroV2IraGetRfoUserReserved;
        pIraIf->SetRfoUserReserved = SysIroV2IraSetRfoUserReserved;
        pIraIf->GetRfoFolderType   = SysIroV2IraGetRfoFolderType;
        pIraIf->SetRfoFolderType   = SysIroV2IraSetRfoFolderType;
        pIraIf->GetRfoPermission   = SysIroV2IraGetRfoPermission;
        pIraIf->SetRfoPermission   = SysIroV2IraSetRfoPermission;
        pIraIf->GetRfoRenderAttr   = SysIroV2IraGetRfoRenderAttr;
        pIraIf->SetRfoRenderAttr   = SysIroV2IraSetRfoRenderAttr;

        pIraIf->GetRroUserContext  = SysIroV2IraGetRroUserContext;
        pIraIf->SetRroUserContext  = SysIroV2IraSetRroUserContext;
        pIraIf->GetRroUserReserved = SysIroV2IraGetRroUserReserved;
        pIraIf->SetRroUserReserved = SysIroV2IraSetRroUserReserved;
        pIraIf->GetRroPermission   = SysIroV2IraGetRroPermission;
        pIraIf->SetRroPermission   = SysIroV2IraSetRroPermission;
        pIraIf->GetRroRenderAttr   = SysIroV2IraGetRroRenderAttr;
        pIraIf->SetRroRenderAttr   = SysIroV2IraSetRroRenderAttr;

        pIraIf->GetCurRecycleBin   = SysIroV2IraGetCurRecycleBin;
        pIraIf->AttachRecycleBin   = SysIroV2IraAttachRecycleBin;
        pIraIf->DetachRecycleBin   = SysIroV2IraDetachRecycleBin;

        pIraIf->SortSubFolders     = SysIroV2IraSortSubFolders;
        pIraIf->SortRecords        = SysIroV2IraSortRecords;

        pIraIf->GetSysRamIf        = SysIroV2IraGetSysRamIf;
        pIraIf->SetSysRamIf        = SysIroV2IraSetSysRamIf;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2Initialize
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
SysIroV2Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT          )hThisObject;

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
    pMyObject->Oid                      = SYS_INFO_REPOSITORY_OID;
    pMyObject->Create                   = SysIroV2Create;
    pMyObject->Remove                   = SysIroV2Remove;
    pMyObject->EnrollObjects            = SysIroV2EnrollObjects;
    pMyObject->Initialize               = SysIroV2Initialize;

    pMyObject->hSysRamIf                = NULL;
    pMyObject->bActive                  = FALSE;

    AnscInitializeLock       (&pMyObject->AccessLock);

    pMyObject->GetIraIf                 = SysIroV2GetIraIf;
    pMyObject->GetProperty              = SysIroV2GetProperty;
    pMyObject->SetProperty              = SysIroV2SetProperty;
    pMyObject->ResetProperty            = SysIroV2ResetProperty;
    pMyObject->Reset                    = SysIroV2Reset;

    pMyObject->Engage                   = SysIroV2Engage;
    pMyObject->Cancel                   = SysIroV2Cancel;
    pMyObject->AcqAccess                = SysIroV2AcqAccess;
    pMyObject->RelAccess                = SysIroV2RelAccess;

    pMyObject->AddFolder                = SysIroV2AddFolder;
    pMyObject->AddFolder2               = SysIroV2AddFolder2;
    pMyObject->DelFolder                = SysIroV2DelFolder;
	pMyObject->DelFolder2				= SysIroV2DelFolder2;
    pMyObject->ResetFolder              = SysIroV2ResetFolder;
    pMyObject->GetFolderByName          = SysIroV2GetFolderByName;
    pMyObject->GetFolderByName2         = SysIroV2GetFolderByName2;
    pMyObject->GetFolderByIndex         = SysIroV2GetFolderByIndex;
    pMyObject->AddRecord                = SysIroV2AddRecord;
    pMyObject->DelRecord                = SysIroV2DelRecord;
    pMyObject->SetRecord                = SysIroV2SetRecord;
    pMyObject->GetRecordByName          = SysIroV2GetRecordByName;
    pMyObject->GetRecordByIndex         = SysIroV2GetRecordByIndex;

    pMyObject->IraAcqWriteAccess        = SysIroV2IraAcqWriteAccess;
    pMyObject->IraRelWriteAccess        = SysIroV2IraRelWriteAccess;

    pMyObject->IraAddSysFolder          = SysIroV2IraAddSysFolder;
    pMyObject->IraAddFolder             = SysIroV2IraAddFolder;
    pMyObject->IraAddFolder2            = SysIroV2IraAddFolder2;
    pMyObject->IraDelFolder             = SysIroV2IraDelFolder;
    pMyObject->IraDelFolder2            = SysIroV2IraDelFolder2;

    pMyObject->IraOpenFolder            = SysIroV2IraOpenFolder;
    pMyObject->IraOpenFolder2           = SysIroV2IraOpenFolder2;
    pMyObject->IraCloseFolder           = SysIroV2IraCloseFolder;
    pMyObject->IraClearFolder           = SysIroV2IraClearFolder;

    pMyObject->IraGenerateFolder        = SysIroV2IraGenerateFolder;
    pMyObject->IraPopulateFolder        = SysIroV2IraPopulateFolder;

    pMyObject->IraQueryFolder           = SysIroV2IraQueryFolder;
    pMyObject->IraGetSubFolderCount     = SysIroV2IraGetSubFolderCount;
    pMyObject->IraEnumSubFolder         = SysIroV2IraEnumSubFolder;
    pMyObject->IraTestSubFolder         = SysIroV2IraTestSubFolder;

    pMyObject->IraAddRecord             = SysIroV2IraAddRecord;
    pMyObject->IraAddRecord2            = SysIroV2IraAddRecord2;
    pMyObject->IraDelRecord             = SysIroV2IraDelRecord;
    pMyObject->IraGetRecord             = SysIroV2IraGetRecord;
    pMyObject->IraSetRecord             = SysIroV2IraSetRecord;

    pMyObject->IraQueryRecord           = SysIroV2IraQueryRecord;
    pMyObject->IraGetRecordCount        = SysIroV2IraGetRecordCount;
    pMyObject->IraEnumRecord            = SysIroV2IraEnumRecord;
    pMyObject->IraTestRecord            = SysIroV2IraTestRecord;

    pMyObject->IraGetRfoUserContext     = SysIroV2IraGetRfoUserContext;
    pMyObject->IraSetRfoUserContext     = SysIroV2IraSetRfoUserContext;
    pMyObject->IraGetRfoUserReserved    = SysIroV2IraGetRfoUserReserved;
    pMyObject->IraSetRfoUserReserved    = SysIroV2IraSetRfoUserReserved;
    pMyObject->IraGetRfoFolderType      = SysIroV2IraGetRfoFolderType;
    pMyObject->IraSetRfoFolderType      = SysIroV2IraSetRfoFolderType;
    pMyObject->IraGetRfoPermission      = SysIroV2IraGetRfoPermission;
    pMyObject->IraSetRfoPermission      = SysIroV2IraSetRfoPermission;
    pMyObject->IraGetRfoRenderAttr      = SysIroV2IraGetRfoRenderAttr;
    pMyObject->IraSetRfoRenderAttr      = SysIroV2IraSetRfoRenderAttr;

    pMyObject->IraGetRroUserContext     = SysIroV2IraGetRroUserContext;
    pMyObject->IraSetRroUserContext     = SysIroV2IraSetRroUserContext;
    pMyObject->IraGetRroUserReserved    = SysIroV2IraGetRroUserReserved;
    pMyObject->IraSetRroUserReserved    = SysIroV2IraSetRroUserReserved;
    pMyObject->IraGetRroPermission      = SysIroV2IraGetRroPermission;
    pMyObject->IraSetRroPermission      = SysIroV2IraSetRroPermission;
    pMyObject->IraGetRroRenderAttr      = SysIroV2IraGetRroRenderAttr;
    pMyObject->IraSetRroRenderAttr      = SysIroV2IraSetRroRenderAttr;

    pMyObject->IraGetCurRecycleBin      = SysIroV2IraGetCurRecycleBin;
    pMyObject->IraAttachRecycleBin      = SysIroV2IraAttachRecycleBin;
    pMyObject->IraDetachRecycleBin      = SysIroV2IraDetachRecycleBin;

    pMyObject->IraSortSubFolders        = SysIroV2IraSortSubFolders;
    pMyObject->IraSortRecords           = SysIroV2IraSortRecords;

    pMyObject->IraGetSysRamIf           = SysIroV2IraGetSysRamIf;
    pMyObject->IraSetSysRamIf           = SysIroV2IraSetSysRamIf;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
