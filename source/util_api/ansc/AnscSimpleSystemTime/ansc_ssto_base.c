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

    module:	ansc_ssto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic object functions
        of the Ansc Simple System Time Object.

        *   AnscSstoCreate
        *   AnscSstoRemove
        *   AnscSstoEnrollObjects
        *   AnscSstoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/21/01    Xuechen Yang    template
        11/25/02    Na Li           initial revision

**********************************************************************/


#include "ansc_ssto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscSstoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ntm Peer Association Object and
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
AnscSstoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_SIMPLE_SYS_TIME_OBJECT)AnscAllocateMemory(sizeof(ANSC_SIMPLE_SYS_TIME_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_SIMPLE_SYS_TIME_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_SIMPLE_SYS_TIME_OID;
    pBaseObject->Create            = AnscSstoCreate;
    pBaseObject->Remove            = AnscSstoRemove;
    pBaseObject->EnrollObjects     = AnscSstoEnrollObjects;
    pBaseObject->Initialize        = AnscSstoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoRemove
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
AnscSstoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    PANSC_STS_INTERFACE             pStsIf       = (PANSC_STS_INTERFACE)pMyObject->hStsIf;

    if( pStsIf )
    {
        AnscFreeMemory(pStsIf);
    }

    AnscFreeLock(&pMyObject->AccessLock        );

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoEnrollObjects
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
AnscSstoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT   pMyObject    = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    PANSC_STS_INTERFACE            pStsIf       = (PANSC_STS_INTERFACE)pMyObject->hStsIf;

    if ( !pStsIf )
    {
        pStsIf = (PANSC_STS_INTERFACE)AnscAllocateMemory(sizeof(ANSC_STS_INTERFACE));

        if ( !pStsIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hStsIf = (ANSC_HANDLE)pStsIf;
        }

        AnscCopyString(pStsIf->Name, ANSC_STS_INTERFACE_NAME);

        pStsIf->InterfaceId       = ANSC_STS_INTERFACE_ID;
        pStsIf->hOwnerContext     = (ANSC_HANDLE)pMyObject;
        pStsIf->Size              = sizeof(ANSC_STS_INTERFACE);

        pStsIf->GetLocalTime      = AnscSstoStsGetLocalTime;
        pStsIf->GetCurSeconds     = AnscSstoStsGetCurSeconds;
        pStsIf->AdjustClock1      = AnscSstoStsAdjustClock1;
        pStsIf->AdjustClock2      = AnscSstoStsAdjustClock2;
        pStsIf->GetTimeOffset     = AnscSstoStsGetTimeOffset;
        pStsIf->SetTimeOffset     = AnscSstoStsSetTimeOffset;

        pStsIf->SecondsToCalendar = AnscSstoStsSecondsToCalendar;
        pStsIf->CalendarToSeconds = AnscSstoStsCalendarToSeconds;
        pStsIf->SysTickToCalendar = AnscSstoStsSysTickToCalendar;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);


    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoInitialize
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
AnscSstoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject     = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;


    /*
     * While in C++ you don't have to initialize all the member fields inherited from the base
     * class since the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    pMyObject->Oid                      = ANSC_SIMPLE_SYS_TIME_OID;
    pMyObject->Create                   = AnscSstoCreate;
    pMyObject->Remove                   = AnscSstoRemove;
    pMyObject->EnrollObjects            = AnscSstoEnrollObjects;
    pMyObject->Initialize               = AnscSstoInitialize;

    pMyObject->LastSecond               = ANSC_SSTO_SECONDS_FROM_1900_TO_1970;
    pMyObject->LastTick                 = AnscGetTickInSeconds();
    pMyObject->iTimeOffset              = 0;

    AnscInitializeLock       (&pMyObject->AccessLock       );



    pMyObject->GetStsIf                 = AnscSstoGetStsIf;
    pMyObject->Reset                    = AnscSstoReset;
    pMyObject->GetLastSecond            = AnscSstoGetLastSecond;
    pMyObject->GetLastTick              = AnscSstoGetLastTick;
    pMyObject->GetiTimeOffset           = AnscSstoGetiTimeOffset;
    pMyObject->SetiTimeOffset           = AnscSstoSetiTimeOffset;
    pMyObject->AcquireAccess            = AnscSstoAcquireAccess;
    pMyObject->ReleaseAccess            = AnscSstoReleaseAccess;
    pMyObject->GetCurrSecond            = AnscSstoGetCurrSecond;

    pMyObject->SecondToCalendar         = AnscSstoSecondToCalendar;
    pMyObject->SecondToLocalCalendar    = AnscSstoSecondToLocalCalendar;
    pMyObject->CalendarToSecond         = AnscSstoCalendarToSecond;
    pMyObject->GetWeekDay               = AnscSstoGetWeekDay;
    pMyObject->LocalCalendarToSecond    = AnscSstoLocalCalendarToSecond;

    pMyObject->StsGetLocalTime          = AnscSstoStsGetLocalTime;
    pMyObject->StsGetCurSeconds         = AnscSstoStsGetCurSeconds;
    pMyObject->StsAdjustClock1          = AnscSstoStsAdjustClock1;
    pMyObject->StsAdjustClock2          = AnscSstoStsAdjustClock2;

    pMyObject->StsSecondsToCalendar     = AnscSstoStsSecondsToCalendar;
    pMyObject->StsCalendarToSeconds     = AnscSstoStsCalendarToSeconds;
    pMyObject->StsSysTickToCalendar     = AnscSstoStsSysTickToCalendar;


    return  ANSC_STATUS_SUCCESS;
}
