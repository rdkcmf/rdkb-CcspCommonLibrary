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

    module:	sys_rdo_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Repository Driver Object.

        *   SysRdoCreate
        *   SysRdoRemove
        *   SysRdoEnrollObjects
        *   SysRdoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/05/02    initial revision.

**********************************************************************/


#include "sys_rdo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysRdoCreate
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
SysRdoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_REPOSITORY_DRIVER_OBJECT)AnscAllocateMemory(sizeof(SYS_REPOSITORY_DRIVER_OBJECT));

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
    pBaseObject->Oid               = SYS_REPOSITORY_DRIVER_OID;
    pBaseObject->Create            = SysRdoCreate;
    pBaseObject->Remove            = SysRdoRemove;
    pBaseObject->EnrollObjects     = SysRdoEnrollObjects;
    pBaseObject->Initialize        = SysRdoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoRemove
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
SysRdoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT)hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->hRootFolder;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pRootFolder )
    {
        pRootFolder->Remove((ANSC_HANDLE)pRootFolder);

        pMyObject->hRootFolder = (ANSC_HANDLE)NULL;
    }

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoEnrollObjects
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
SysRdoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT)hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->hRootFolder;

    if ( !pRootFolder )
    {
        pRootFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)SysCreateRepositoryFolder
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pRootFolder )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRootFolder = (ANSC_HANDLE)pRootFolder;
        }
    }

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoInitialize
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
SysRdoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT)hThisObject;

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
    pMyObject->Oid           = SYS_REPOSITORY_DRIVER_OID;
    pMyObject->Create        = SysRdoCreate;
    pMyObject->Remove        = SysRdoRemove;
    pMyObject->EnrollObjects = SysRdoEnrollObjects;
    pMyObject->Initialize    = SysRdoInitialize;

    pMyObject->bActive       = FALSE;

    pMyObject->GetRootFolder = SysRdoGetRootFolder;
    pMyObject->GetProperty   = SysRdoGetProperty;
    pMyObject->SetProperty   = SysRdoSetProperty;
    pMyObject->ResetProperty = SysRdoResetProperty;
    pMyObject->Reset         = SysRdoReset;

    pMyObject->Engage        = SysRdoEngage;
    pMyObject->Cancel        = SysRdoCancel;

    pMyObject->AddFolder1    = SysRdoAddFolder1;
    pMyObject->AddFolder2    = SysRdoAddFolder2;
    pMyObject->DelFolder1    = SysRdoDelFolder1;
    pMyObject->DelFolder2    = SysRdoDelFolder2;
    pMyObject->GetFolder1    = SysRdoGetFolder1;
    pMyObject->GetFolder2    = SysRdoGetFolder2;

    pMyObject->AddRecord     = SysRdoAddRecord;
    pMyObject->DelRecord     = SysRdoDelRecord;
    pMyObject->GetRecord     = SysRdoGetRecord;
    pMyObject->SetRecord     = SysRdoSetRecord;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
