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

    module:	sys_rro_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Repository Record Object.

        *   SysRroCreate
        *   SysRroRemove
        *   SysRroEnrollObjects
        *   SysRroInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/07/02    initial revision.

**********************************************************************/


#include "sys_rro_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysRroCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Repository Record Object and
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
SysRroCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_REPOSITORY_RECORD_OBJECT)AnscAllocateMemory(sizeof(SYS_REPOSITORY_RECORD_OBJECT));

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
    pBaseObject->Oid               = SYS_REPOSITORY_RECORD_OID;
    pBaseObject->Create            = SysRroCreate;
    pBaseObject->Remove            = SysRroRemove;
    pBaseObject->EnrollObjects     = SysRroEnrollObjects;
    pBaseObject->Initialize        = SysRroInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroRemove
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
SysRroRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;
    PSYS_RRO_RENDER_ATTR            pRenderAttr  = (PSYS_RRO_RENDER_ATTR         )pMyObject->hRenderAttr;

    if ( pRenderAttr )
    {
        SysFreeRroRenderAttr(pRenderAttr);
    }

    if ( pMyObject->RecordName )
    {
        AnscFreeMemory(pMyObject->RecordName);

        pMyObject->RecordName = NULL;
    }

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroEnrollObjects
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
SysRroEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroInitialize
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
SysRroInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

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
    pMyObject->Oid              = SYS_REPOSITORY_RECORD_OID;
    pMyObject->Create           = SysRroCreate;
    pMyObject->Remove           = SysRroRemove;
    pMyObject->EnrollObjects    = SysRroEnrollObjects;
    pMyObject->Initialize       = SysRroInitialize;

    pMyObject->HashIndex        = 0;
    pMyObject->BirthTime        = AnscGetTickInSeconds();
    pMyObject->Timestamp        = AnscGetTickInSeconds();
    pMyObject->hRenderAttr      = (ANSC_HANDLE)NULL;
    pMyObject->hParentFolder    = (ANSC_HANDLE)NULL;
    pMyObject->hUserContext     = (ANSC_HANDLE)NULL;

    pMyObject->RecordName       = NULL;
    pMyObject->RecordType       = SYS_REP_RECORD_TYPE_UINT;
    pMyObject->RecordSize       = sizeof(ULONG);
    pMyObject->Permission       = SYS_RRO_PERMISSION_ALL;

    pMyObject->GetBirthTime     = SysRroGetBirthTime;
    pMyObject->SetBirthTime     = SysRroSetBirthTime;
    pMyObject->GetRenderAttr    = SysRroGetRenderAttr;
    pMyObject->SetRenderAttr    = SysRroSetRenderAttr;
    pMyObject->GetParentFolder  = SysRroGetParentFolder;
    pMyObject->SetParentFolder  = SysRroSetParentFolder;
    pMyObject->GetUserContext   = SysRroGetUserContext;
    pMyObject->SetUserContext   = SysRroSetUserContext;
    pMyObject->GetUserReserved  = SysRroGetUserReserved;
    pMyObject->SetUserReserved  = SysRroSetUserReserved;

    pMyObject->GetRecordName    = SysRroGetRecordName;
    pMyObject->SetRecordName    = SysRroSetRecordName;
    pMyObject->GetRecordType    = SysRroGetRecordType;
    pMyObject->SetRecordType    = SysRroSetRecordType;
    pMyObject->GetPermission    = SysRroGetPermission;
    pMyObject->SetPermission    = SysRroSetPermission;
    pMyObject->GetContentType   = SysRroGetContentType;
    pMyObject->SetContentType   = SysRroSetContentType;

    pMyObject->GrantAccess      = SysRroGrantAccess;
    pMyObject->GetRecordSize    = SysRroGetRecordSize;
    pMyObject->GetRecordData    = SysRroGetRecordData;
    pMyObject->SetRecordData    = SysRroSetRecordData;

    return  ANSC_STATUS_SUCCESS;
}
