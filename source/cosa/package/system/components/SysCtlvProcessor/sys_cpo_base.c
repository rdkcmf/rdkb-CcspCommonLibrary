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

    module:	sys_cpo_base.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Sys Ctlv Processor Object.

        *   SysCpoCreate
        *   SysCpoRemove
        *   SysCpoEnrollObjects
        *   SysCpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#include "sys_cpo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SysCpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Sys Ctlv Processor Object and
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
SysCpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSYS_CTLV_PROCESSOR_OBJECT)AnscAllocateMemory(sizeof(SYS_CTLV_PROCESSOR_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), SYS_CTLV_PROCESSOR_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SYS_CTLV_PROCESSOR_OID;
    pBaseObject->Create            = SysCpoCreate;
    pBaseObject->Remove            = SysCpoRemove;
    pBaseObject->EnrollObjects     = SysCpoEnrollObjects;
    pBaseObject->Initialize        = SysCpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoRemove
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
SysCpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_SFC_INTERFACE              pSysSfcIf    = (PSYS_SFC_INTERFACE        )pMyObject->hSysSfcIf;

    if ( pSysSfcIf )
    {
        AnscFreeMemory(pSysSfcIf);

        pMyObject->hSysSfcIf = (ANSC_HANDLE)NULL;
    }

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscCoRemove     ((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoEnrollObjects
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
SysCpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_SFC_INTERFACE              pSysSfcIf    = (PSYS_SFC_INTERFACE        )pMyObject->hSysSfcIf;
    errno_t   rc  = -1;

    if ( !pSysSfcIf )
    {
        pSysSfcIf = (PSYS_SFC_INTERFACE)AnscAllocateMemory(sizeof(SYS_SFC_INTERFACE));

        if ( !pSysSfcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSysSfcIf = (ANSC_HANDLE)pSysSfcIf;
        }

        rc = STRCPY_S_NOCLOBBER(pSysSfcIf->Name, sizeof(pSysSfcIf->Name), SYS_SFC_INTERFACE_NAME);
        ERR_CHK(rc);

        pSysSfcIf->InterfaceId          = SYS_SFC_INTERFACE_ID;
        pSysSfcIf->hOwnerContext        = (ANSC_HANDLE)pMyObject;
        pSysSfcIf->Size                 = sizeof(SYS_SFC_INTERFACE);

        pSysSfcIf->Verify               = SysCpoSfcVerify;
        pSysSfcIf->Provision            = SysCpoSfcProvision;
        pSysSfcIf->Serialize            = SysCpoSfcSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoInitialize
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
SysCpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;

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
    pMyObject->Oid                   = SYS_CTLV_PROCESSOR_OID;
    pMyObject->Create                = SysCpoCreate;
    pMyObject->Remove                = SysCpoRemove;
    pMyObject->EnrollObjects         = SysCpoEnrollObjects;
    pMyObject->Initialize            = SysCpoInitialize;

    pMyObject->bActive               = FALSE;

    pMyObject->SetSysIraIf           = SysCpoSetSysIraIf;
    pMyObject->GetSysSfcIf           = SysCpoGetSysSfcIf;
    pMyObject->GetProperty           = SysCpoGetProperty;
    pMyObject->SetProperty           = SysCpoSetProperty;
    pMyObject->ResetProperty         = SysCpoResetProperty;
    pMyObject->Reset                 = SysCpoReset;

    pMyObject->Engage                = SysCpoEngage;
    pMyObject->Cancel                = SysCpoCancel;

    pMyObject->SfcVerify             = SysCpoSfcVerify;
    pMyObject->SfcProvision          = SysCpoSfcProvision;
    pMyObject->SfcSerialize          = SysCpoSfcSerialize;

    pMyObject->ProvisionFolder       = SysCpoProvisionFolder;
    pMyObject->QueryFolderSize       = SysCpoQueryFolderSize;
    pMyObject->SerializeFolder       = SysCpoSerializeFolder;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
