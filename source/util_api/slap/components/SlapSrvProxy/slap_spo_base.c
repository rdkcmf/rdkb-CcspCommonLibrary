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

    module:	slap_spo_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Srv Proxy Object.

        *   SlapSpoCreate
        *   SlapSpoRemove
        *   SlapSpoEnrollObjects
        *   SlapSpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#include "slap_spo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapSpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Srv Proxy Object and
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
SlapSpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_SRV_PROXY_OBJECT)AnscAllocateMemory(sizeof(SLAP_SRV_PROXY_OBJECT));

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
    pBaseObject->Oid               = SLAP_SRV_PROXY_OID;
    pBaseObject->Create            = SlapSpoCreate;
    pBaseObject->Remove            = SlapSpoRemove;
    pBaseObject->EnrollObjects     = SlapSpoEnrollObjects;
    pBaseObject->Initialize        = SlapSpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoRemove
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
SlapSpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf   = (PSLAP_GOA_INTERFACE   )pMyObject->hSlapGoaIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    switch ( pMyObject->ObjMode )
    {
        case    SLAP_SPO_OBJ_MODE_acquired :

                if ( pMyObject->hSlapGoaObj )
                {
                    pSlapGoaIf->RelObjectAccess
                            (
                                pSlapGoaIf->hOwnerContext,
                                pMyObject->hSlapGoaObj
                            );
                }

                break;

        case    SLAP_SPO_OBJ_MODE_referenced :

                break;

        case    SLAP_SPO_OBJ_MODE_created :

                if ( pMyObject->hSlapGoaObj )
                {
                    pSlapGoaIf->DeleteObject
                            (
                                pSlapGoaIf->hOwnerContext,
                                pMyObject->hSlapGoaObj
                            );
                }

                break;

        default :

                break;
    }

    if ( pMyObject->ObjName )
    {
        AnscFreeMemory(pMyObject->ObjName);

        pMyObject->ObjName = NULL;
    }

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoEnrollObjects
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
SlapSpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoInitialize
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
SlapSpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

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
    pMyObject->Oid               = SLAP_SRV_PROXY_OID;
    pMyObject->Create            = SlapSpoCreate;
    pMyObject->Remove            = SlapSpoRemove;
    pMyObject->EnrollObjects     = SlapSpoEnrollObjects;
    pMyObject->Initialize        = SlapSpoInitialize;

    pMyObject->CallTimeout       = 0;
    pMyObject->ObjName           = NULL;
    pMyObject->ObjType           = 0;
    pMyObject->ObjMode           = SLAP_SPO_OBJ_MODE_acquired;

    pMyObject->hSlapGoaObj       = (ANSC_HANDLE)NULL;
    pMyObject->hSlapUoaIf        = (ANSC_HANDLE)NULL;
    pMyObject->hSlapGoaIf        = (ANSC_HANDLE)NULL;
    pMyObject->hSlapPoaIf        = (ANSC_HANDLE)NULL;

    pMyObject->GetCallTimeout    = SlapSpoGetCallTimeout;
    pMyObject->SetCallTimeout    = SlapSpoSetCallTimeout;
    pMyObject->GetObjName        = SlapSpoGetObjName;
    pMyObject->SetObjName        = SlapSpoSetObjName;
    pMyObject->GetObjType        = SlapSpoGetObjType;
    pMyObject->SetObjType        = SlapSpoSetObjType;
    pMyObject->GetObjMode        = SlapSpoGetObjMode;
    pMyObject->SetObjMode        = SlapSpoSetObjMode;

    pMyObject->GetSlapGoaObj     = SlapSpoGetSlapGoaObj;
    pMyObject->SetSlapGoaObj     = SlapSpoSetSlapGoaObj;
    pMyObject->GetSlapUoaIf      = SlapSpoGetSlapUoaIf;
    pMyObject->SetSlapUoaIf      = SlapSpoSetSlapUoaIf;
    pMyObject->GetSlapGoaIf      = SlapSpoGetSlapGoaIf;
    pMyObject->SetSlapGoaIf      = SlapSpoSetSlapGoaIf;
    pMyObject->GetSlapPoaIf      = SlapSpoGetSlapPoaIf;
    pMyObject->SetSlapPoaIf      = SlapSpoSetSlapPoaIf;
    pMyObject->Reset             = SlapSpoReset;

    pMyObject->Engage            = SlapSpoEngage;
    pMyObject->Cancel            = SlapSpoCancel;
    pMyObject->InvokeDispatch    = SlapSpoInvokeDispatch;

    pMyObject->PoaToGoaParamList = SlapSpoPoaToGoaParamList;
    pMyObject->GoaToPoaParamList = SlapSpoGoaToPoaParamList;
    pMyObject->PoaToGoaVariable  = SlapSpoPoaToGoaVariable;
    pMyObject->GoaToPoaVariable  = SlapSpoGoaToPoaVariable;

    return  ANSC_STATUS_SUCCESS;
}
