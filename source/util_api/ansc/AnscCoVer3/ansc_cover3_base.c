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

    module:	ansc_cover3_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc CoVer3 Object.

        *   AnscCoVer3Create
        *   AnscCoVer3Remove
        *   AnscCoVer3EnrollObjects
        *   AnscCoVer3Initialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/30/02    initial revision.

**********************************************************************/


#include "ansc_cover3_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscCoVer3Create
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc CoVer3 Object and
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
AnscCoVer3Create
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_CO_VER3_OBJECT            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_CO_VER3_OBJECT)AnscAllocateMemory(sizeof(ANSC_CO_VER3_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_CO_VER3_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_CO_VER3_OID;
    pBaseObject->Create            = AnscCoVer3Create;
    pBaseObject->Remove            = AnscCoVer3Remove;
    pBaseObject->EnrollObjects     = AnscCoVer3EnrollObjects;
    pBaseObject->Initialize        = AnscCoVer3Initialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3Remove
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
AnscCoVer3Remove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT)hThisObject;

    /* signal the retreat */
    pMyObject->bShuttingDown = TRUE;

    /* wait for all tasks to quit */
    while ( pMyObject->EngineTaskCount > 0 )
    {
        AnscSleep(100);
    }

    /* go ahead with shutdown */
    pMyObject->Shutdown((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3EnrollObjects
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
AnscCoVer3EnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3Initialize
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
AnscCoVer3Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT)hThisObject;

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
    pMyObject->Oid             = ANSC_CO_VER3_OID;
    pMyObject->Create          = AnscCoVer3Create;
    pMyObject->Remove          = AnscCoVer3Remove;
    pMyObject->EnrollObjects   = AnscCoVer3EnrollObjects;
    pMyObject->Initialize      = AnscCoVer3Initialize;
    pMyObject->Shutdown        = AnscCoVer3Shutdown;

    pMyObject->bShuttingDown   = FALSE;

    pMyObject->EngineTaskCount = 0;

    pMyObject->SpawnTask       = AnscCoVer3SpawnTask;
    pMyObject->SpawnTask2      = AnscCoVer3SpawnTask2;
    pMyObject->SpawnTask3      = AnscCoVer3SpawnTask3;
    pMyObject->EngineTask      = AnscCoVer3EngineTask;

    AnscInitializeLock       (&pMyObject->EtoAccessLock    );
    AnscSListInitializeHeader(&pMyObject->TaskInfoSList    );
    AnscInitializeLock       (&pMyObject->TaskInfoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoVer3Shutdown
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function really destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoVer3Shutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CO_VER3_OBJECT            pMyObject    = (PANSC_CO_VER3_OBJECT)hThisObject;

    AnscFreeLock(&pMyObject->EtoAccessLock    );
    AnscFreeLock(&pMyObject->TaskInfoSListLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject       );

    return  ANSC_STATUS_SUCCESS;
}
