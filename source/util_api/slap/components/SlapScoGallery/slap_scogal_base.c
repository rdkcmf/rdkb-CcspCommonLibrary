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

    module:	slap_scogal_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Sco Gallery Object.

        *   SlapScoGalCreate
        *   SlapScoGalRemove
        *   SlapScoGalEnrollObjects
        *   SlapScoGalInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/03    initial revision.

**********************************************************************/


#include "slap_scogal_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapScoGalCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Sco Gallery Object and
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
SlapScoGalCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_SCO_GALLERY_OBJECT)AnscAllocateMemory(sizeof(SLAP_SCO_GALLERY_OBJECT));

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
    pBaseObject->Oid               = SLAP_SCO_GALLERY_OID;
    pBaseObject->Create            = SlapScoGalCreate;
    pBaseObject->Remove            = SlapScoGalRemove;
    pBaseObject->EnrollObjects     = SlapScoGalEnrollObjects;
    pBaseObject->Initialize        = SlapScoGalInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalRemove
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
SlapScoGalRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject    = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;

    if ( pAtoVariable )
    {
        pMyObject->DelAllVariables((ANSC_HANDLE)pMyObject);

        pAtoVariable->Remove((ANSC_HANDLE)pAtoVariable);

        pMyObject->hAtoVariable = (ANSC_HANDLE)NULL;
    }

    SlapScoColRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalEnrollObjects
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
SlapScoGalEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject    = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;

    if ( !pAtoVariable )
    {
        pAtoVariable =
            (PANSC_ATOM_TABLE_OBJECT)AnscCreateAtomTable
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pAtoVariable )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAtoVariable = (ANSC_HANDLE)pAtoVariable;
        }
    }

    SlapScoColEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalInitialize
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
SlapScoGalInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject    = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    SlapScoColInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid             = SLAP_SCO_GALLERY_OID;
    pMyObject->Create          = SlapScoGalCreate;
    pMyObject->Remove          = SlapScoGalRemove;
    pMyObject->EnrollObjects   = SlapScoGalEnrollObjects;
    pMyObject->Initialize      = SlapScoGalInitialize;

    pMyObject->Reset           = SlapScoGalReset;

    pMyObject->GetCount        = SlapScoGalGetCount;

    pMyObject->bOlmEnabled     = FALSE;

    pMyObject->GetSlapGoaIf    = SlapScoGalGetSlapGoaIf;
    pMyObject->GetOlmContainer = SlapScoGalGetOlmContainer;

    pMyObject->GetVariable     = SlapScoGalGetVariable;
    pMyObject->SetVariable     = SlapScoGalSetVariable;
    pMyObject->AddVariable     = SlapScoGalAddVariable;
    pMyObject->DelVariable     = SlapScoGalDelVariable;
    pMyObject->DelAllVariables = SlapScoGalDelAllVariables;

    pMyObject->JoinGallery     = SlapScoGalJoinGallery;
    pMyObject->QuitGallery     = SlapScoGalQuitGallery;

    return  ANSC_STATUS_SUCCESS;
}
