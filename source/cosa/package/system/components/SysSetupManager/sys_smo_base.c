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

    module:	sys_smo_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Setup Manager Object.

        *   SysSmoCreate
        *   SysSmoRemove
        *   SysSmoEnrollObjects
        *   SysSmoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/14/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysSmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Setup Manager Object and
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
SysSmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_SETUP_MANAGER_OBJECT)AnscAllocateMemory(sizeof(SYS_SETUP_MANAGER_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), SYS_SETUP_MANAGER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_SETUP_MANAGER_OID;
    pBaseObject->Create            = SysSmoCreate;
    pBaseObject->Remove            = SysSmoRemove;
    pBaseObject->EnrollObjects     = SysSmoEnrollObjects;
    pBaseObject->Initialize        = SysSmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoRemove
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
SysSmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT)hThisObject;
    PSYS_RCM_INTERFACE              pSysRcmIf    = (PSYS_RCM_INTERFACE       )pMyObject->hSysRcmIf;

    if ( pSysRcmIf )
    {
        AnscFreeMemory(pSysRcmIf);
    }

    SysMcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoEnrollObjects
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
SysSmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT)hThisObject;
    PSYS_RCM_INTERFACE              pSysRcmIf    = (PSYS_RCM_INTERFACE       )pMyObject->hSysRcmIf;
    errno_t    rc    = -1;

    if ( !pSysRcmIf )
    {
        pSysRcmIf = (PSYS_RCM_INTERFACE)AnscAllocateMemory(sizeof(SYS_RCM_INTERFACE));

        if ( !pSysRcmIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSysRcmIf = (ANSC_HANDLE)pSysRcmIf;
        }

        rc = STRCPY_S_NOCLOBBER(pSysRcmIf->Name, sizeof(pSysRcmIf->Name), SYS_RCM_INTERFACE_NAME);
        ERR_CHK(rc);

        pSysRcmIf->InterfaceId   = SYS_RCM_INTERFACE_ID;
        pSysRcmIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pSysRcmIf->Size          = sizeof(SYS_RCM_INTERFACE);

        pSysRcmIf->Notify        = SysSmoRcmNotify;
        pSysRcmIf->Init          = SysSmoRcmInit;
        pSysRcmIf->Stop          = SysSmoRcmStop;
    }

    SysMcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoInitialize
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
SysSmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_SETUP_MANAGER_OBJECT       pMyObject    = (PSYS_SETUP_MANAGER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    SysMcoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                  = SYS_SETUP_MANAGER_OID;
    pMyObject->Create               = SysSmoCreate;
    pMyObject->Remove               = SysSmoRemove;
    pMyObject->EnrollObjects        = SysSmoEnrollObjects;
    pMyObject->Initialize           = SysSmoInitialize;

    pMyObject->hSetupRfo            = (ANSC_HANDLE)NULL;

    pMyObject->ResetProperty        = SysSmoResetProperty;
    pMyObject->Reset                = SysSmoReset;

    pMyObject->LoadComponents       = SysSmoLoadComponents;
    pMyObject->ResolveDependencies  = SysSmoResolveDependencies;
    pMyObject->UnloadComponents     = SysSmoUnloadComponents;
    pMyObject->SetUpEnv             = SysSmoSetUpEnv;
    pMyObject->CloseEnv             = SysSmoCloseEnv;

    pMyObject->GetSetupRfo          = SysSmoGetSetupRfo;
    pMyObject->GetSysRcmIf          = SysSmoGetSysRcmIf;

    pMyObject->GetRroTmplSizeByRrtd = SysSmoGetRroTmplSizeByRrtd;
    pMyObject->GetRfoTmplSizeByRftd = SysSmoGetRfoTmplSizeByRftd;
    pMyObject->WriteRroTmplByRrtd   = SysSmoWriteRroTmplByRrtd;
    pMyObject->AllocRfoTmplByRftd   = SysSmoAllocRfoTmplByRftd;
    pMyObject->AllocRftdByRfoTmpl   = SysSmoAllocRftdByRfoTmpl;
    pMyObject->WriteRrtdByRroTmpl   = SysSmoWriteRrtdByRroTmpl;
    pMyObject->CleanRftDescriptor   = SysSmoCleanRftDescriptor;
    pMyObject->CleanRrtDescriptor   = SysSmoCleanRrtDescriptor;

    pMyObject->ReactRfoAdded        = SysSmoReactRfoAdded;
    pMyObject->ReactRfoDeleted      = SysSmoReactRfoDeleted;
    pMyObject->ReactRroAdded        = SysSmoReactRroAdded;
    pMyObject->ReactRroModified     = SysSmoReactRroModified;
    pMyObject->ReactRroDeleted      = SysSmoReactRroDeleted;

    pMyObject->InitSetUpAccess      = SysSmoInitSetUpAccess;
    pMyObject->StopSetUpAccess      = SysSmoStopSetUpAccess;

    pMyObject->RcmNotify            = SysSmoRcmNotify;
    pMyObject->RcmInit              = SysSmoRcmInit;
    pMyObject->RcmStop              = SysSmoRcmStop;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
