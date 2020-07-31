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

    module:	ansc_tso_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Timer Scheduler Object.

        *   AnscTsoCreate
        *   AnscTsoRemove
        *   AnscTsoEnrollObjects
        *   AnscTsoInitialize
        *   AnscTsoShutdown

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/01    initial revision.

**********************************************************************/


#include "ansc_tso_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscTsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Timer Scheduler Object and
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
AnscTsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_TIMER_SCHEDULER_OBJECT)AnscAllocateMemory(sizeof(ANSC_TIMER_SCHEDULER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_TIMER_SCHEDULER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_TIMER_SCHEDULER_OID;
    pBaseObject->Create            = AnscTsoCreate;
    pBaseObject->Remove            = AnscTsoRemove;
    pBaseObject->EnrollObjects     = AnscTsoEnrollObjects;
    pBaseObject->Initialize        = AnscTsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoRemove
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
AnscTsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    pMyObject->Stop((ANSC_HANDLE)pMyObject);

    AnscCoVer3Remove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoEnrollObjects
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
AnscTsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    AnscCoVer3EnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoInitialize
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
AnscTsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoVer3Initialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid            = ANSC_TIMER_SCHEDULER_OID;
    pMyObject->Create         = AnscTsoCreate;
    pMyObject->Remove         = AnscTsoRemove;
    pMyObject->EnrollObjects  = AnscTsoEnrollObjects;
    pMyObject->Initialize     = AnscTsoInitialize;
    pMyObject->Shutdown       = AnscTsoShutdown;

    pMyObject->SchedulingUnit = ANSC_TSO_DEF_SCHEDULING_UNIT;
    pMyObject->hCurInvokedTdo = (ANSC_HANDLE)NULL;
    pMyObject->bStarted       = FALSE;

    pMyObject->Reset          = AnscTsoReset;
    pMyObject->ScheduleTask   = AnscTsoScheduleTask;
    pMyObject->Start          = AnscTsoStart;
    pMyObject->Stop           = AnscTsoStop;

    pMyObject->RegisterTimer  = AnscTsoRegisterTimer;
    pMyObject->CancelTimer    = AnscTsoCancelTimer;
    pMyObject->ScheduleTimer  = AnscTsoScheduleTimer;

    AnscInitializeTsLock     (&pMyObject->SyncTsLock      );
    AnscQueueInitializeHeader(&pMyObject->TdoQueue        );
    AnscInitializeSpinLock   (&pMyObject->TdoQueueSpinLock);

    /*
     * The essential operation of the Timer Scheduling is to simulate the timer management via
     * serial event notification in a background low-priority task. The Timer Scheduler Object
     * employs a single task to manage all the registered timer descriptors.
     */
    /*
    pMyObject->Start((ANSC_HANDLE)pMyObject);
    */

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscTsoShutdown
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
AnscTsoShutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TIMER_SCHEDULER_OBJECT    pMyObject    = (PANSC_TIMER_SCHEDULER_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeTsLock    (&pMyObject->SyncTsLock      );
    AnscFreeSpinLock  (&pMyObject->TdoQueueSpinLock);
    AnscCoVer3Shutdown((ANSC_HANDLE)pMyObject      );

    return  ANSC_STATUS_SUCCESS;
}
