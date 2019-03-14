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

    module:	ansc_tso_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Timer Scheduler.

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


#ifndef  _ANSC_TSO_INTERNAL_API_
#define  _ANSC_TSO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_TSO_STATES.C
***********************************************************/

ANSC_STATUS
AnscTsoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TSO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscTsoScheduleTask
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTsoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTsoStop
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TSO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
AnscTsoRegisterTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );

ANSC_STATUS
AnscTsoCancelTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );

ANSC_STATUS
AnscTsoScheduleTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_TDO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscTdoGetScheduler
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetScheduler
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hScheduler
    );

ANSC_HANDLE
AnscTdoGetClient
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    );

ULONG
AnscTdoGetStartTime
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetStartTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

ULONG
AnscTdoGetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetTimestamp
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

ULONG
AnscTdoGetTimerType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetTimerType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimerType
    );

ULONG
AnscTdoGetTimerMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetTimerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimerMode
    );

ULONG
AnscTdoGetInterval
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetInterval
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInterval
    );

ULONG
AnscTdoGetTimeToFire
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetTimeToFire
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulDelay
    );

ULONG
AnscTdoGetCallCounter
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetCallCounter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCounter
    );

BOOL
AnscTdoGetState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoSetState
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bState
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TDO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscTdoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoStop
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
