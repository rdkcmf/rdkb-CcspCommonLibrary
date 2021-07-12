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

    module: dslh_dmagnt_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh DataModelAgent object;

        *   DslhDmagntCreate
        *   DslhDmagntRemove
        *   DslhDmagntEnrollObjects
        *   DslhDmagntInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.

**********************************************************************/


#include "dslh_dmagnt_global.h"
#include "safec_lib_common.h"

PDSLH_DATAMODEL_AGENT_OBJECT         g_DslhDataModelAgent  = NULL;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhDmagntCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Cpe Controller Object and
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
DslhDmagntCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_DATAMODEL_AGENT_OBJECT)AnscAllocateMemory(sizeof(DSLH_DATAMODEL_AGENT_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), "DataModel Agent");
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = 0;
    pBaseObject->Create            = DslhDmagntCreate;
    pBaseObject->Remove            = DslhDmagntRemove;
    pBaseObject->EnrollObjects     = DslhDmagntEnrollObjects;
    pBaseObject->Initialize        = DslhDmagntInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    g_DslhDataModelAgent = pMyObject;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntRemove
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
DslhDmagntRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_MWS_INTERFACE             pDslhMwsIf        = (PDSLH_MWS_INTERFACE        )pMyObject->hDslhMwsIf;
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomFunctions;

    if( pDslhMwsIf)
    {
        AnscFreeMemory(pMyObject->hDslhMwsIf);

        pMyObject->hDslhMwsIf = NULL;
    }

    if( pAtomFunctions != NULL)
    {
        pAtomFunctions->Remove(pAtomFunctions);
    }

    if( pMyObject->pDTXml)
    {
        AnscFreeMemory(pMyObject->pDTXml);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntEnrollObjects
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
DslhDmagntEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_MWS_INTERFACE             pDslhMwsIf        = (PDSLH_MWS_INTERFACE        )pMyObject->hDslhMwsIf;
    errno_t                         rc                = -1;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    if ( !pDslhMwsIf )
    {
        pDslhMwsIf = (PDSLH_MWS_INTERFACE)AnscAllocateMemory(sizeof(DSLH_MWS_INTERFACE));

        if ( !pDslhMwsIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }

        rc = STRCPY_S_NOCLOBBER(pDslhMwsIf->Name, sizeof(pDslhMwsIf->Name), DSLH_MWS_INTERFACE_NAME);
        ERR_CHK(rc);

        pDslhMwsIf->InterfaceId      = DSLH_MWS_INTERFACE_ID;
        pDslhMwsIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pDslhMwsIf->Size             = sizeof(DSLH_MWS_INTERFACE);

        pDslhMwsIf->GetResource = COSAMwsIfGetResource;

        pMyObject->hDslhMwsIf   = (ANSC_HANDLE)pDslhMwsIf;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntInitialize
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
DslhDmagntInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject    = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;

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
    pMyObject->Oid                 = DSLH_CPE_CONTROLLER_OID;
    pMyObject->Create              = DslhDmagntCreate;
    pMyObject->Remove              = DslhDmagntRemove;
    pMyObject->EnrollObjects       = DslhDmagntEnrollObjects;
    pMyObject->Initialize          = DslhDmagntInitialize;

    pMyObject->bActive             = FALSE;
    pMyObject->hDslhCpeController  = NULL;
    pMyObject->hAtomFunctions      = NULL;
    pMyObject->hIrepFolderCOSA     = NULL;
    pMyObject->hFcContext          = NULL;
    pMyObject->hDslhLcbIf          = NULL;
    pMyObject->hDslhCcdIf          = NULL;
    pMyObject->hDbusHandle         = NULL;
    pMyObject->pDTXml              = NULL;

    pMyObject->Engage              = DslhDmagntEngage;
    pMyObject->Cancel              = DslhDmagntCancel;

    pMyObject->LoadDataModelXML    = DslhDmagntLoadDataModelXML;
    pMyObject->LoadDataModelXML2   = DslhDmagntLoadDataModelXML2;
    pMyObject->UnloadDataModelXML  = DslhDmagntUnloadDataModelXML;
    pMyObject->RegisterInternalApi = DslhDmagntRegisterInternalApi;
    pMyObject->MemoryUsage         = DslhDmagntMemoryUsage;
    pMyObject->MemoryTable         = DslhDmagntMemoryTable;
    pMyObject->MemoryCheck         = DslhDmagntMemoryCheck;
    pMyObject->RegBaseDataModel    = DslhDmagntRegBaseDataModel;

    pMyObject->GetFcContext        = DslhDmagntGetFcContext;
    pMyObject->SetFcContext        = DslhDmagntSetFcContext;
    pMyObject->GenerateDTXmlString = DslhDmagntGenerateDTXmlString;

    pMyObject->bPluginUpdated      = TRUE;
    pMyObject->pPrefix             = NULL;

    /* init the plugin info list */
    AnscQueueInitializeHeader(&pMyObject->sPluginInfoList);

    return  ANSC_STATUS_SUCCESS;
}
