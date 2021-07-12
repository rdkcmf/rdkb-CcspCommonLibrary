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

    module:	sys_rbo_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Recycle Bin Object.

        *   SysRboCreate
        *   SysRboRemove
        *   SysRboEnrollObjects
        *   SysRboInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/02    initial revision.

**********************************************************************/


#include "sys_rbo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysRboCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Recycle Bin Object and
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
SysRboCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = NULL;
    errno_t     rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_RECYCLE_BIN_OBJECT)AnscAllocateMemory(sizeof(SYS_RECYCLE_BIN_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), SYS_RECYCLE_BIN_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_RECYCLE_BIN_OID;
    pBaseObject->Create            = SysRboCreate;
    pBaseObject->Remove            = SysRboRemove;
    pBaseObject->EnrollObjects     = SysRboEnrollObjects;
    pBaseObject->Initialize        = SysRboInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboRemove
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
SysRboRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->FolderQueueLock);
    AnscFreeLock(&pMyObject->RecordQueueLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboEnrollObjects
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
SysRboEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboInitialize
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
SysRboInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT)hThisObject;

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
    pMyObject->Oid              = SYS_RECYCLE_BIN_OID;
    pMyObject->Create           = SysRboCreate;
    pMyObject->Remove           = SysRboRemove;
    pMyObject->EnrollObjects    = SysRboEnrollObjects;
    pMyObject->Initialize       = SysRboInitialize;

    pMyObject->hServedFolder    = (ANSC_HANDLE)NULL;
    pMyObject->hUserContext     = (ANSC_HANDLE)NULL;

    pMyObject->GetServedFolder  = SysRboGetServedFolder;
    pMyObject->SetServedFolder  = SysRboSetServedFolder;
    pMyObject->GetUserContext   = SysRboGetUserContext;
    pMyObject->SetUserContext   = SysRboSetUserContext;

    pMyObject->GetProperty      = SysRboGetProperty;
    pMyObject->SetProperty      = SysRboSetProperty;
    pMyObject->ResetProperty    = SysRboResetProperty;
    pMyObject->Reset            = SysRboReset;

    pMyObject->DelAllFolders    = SysRboDelAllFolders;
    pMyObject->DelAllRecords    = SysRboDelAllRecords;

    pMyObject->GetFolderCount   = SysRboGetFolderCount;
    pMyObject->GetRecordCount   = SysRboGetRecordCount;

    pMyObject->AddFolder        = SysRboAddFolder;
    pMyObject->GetFolderByIndex = SysRboGetFolderByIndex;
    pMyObject->AddRecord        = SysRboAddRecord;
    pMyObject->GetRecordByIndex = SysRboGetRecordByIndex;

    AnscQueueInitializeHeader(&pMyObject->FolderQueue    );
    AnscInitializeLock       (&pMyObject->FolderQueueLock);
    AnscQueueInitializeHeader(&pMyObject->RecordQueue    );
    AnscInitializeLock       (&pMyObject->RecordQueueLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
