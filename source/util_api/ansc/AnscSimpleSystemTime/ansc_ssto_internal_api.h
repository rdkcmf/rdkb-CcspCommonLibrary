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

    module:	ansc_ssto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Simple System Time 
        Object.

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


#ifndef  _ANSC_SSTO_INTERNAL_API_
#define  _ANSC_SSTO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_SSTO_STATE.C
***********************************************************/
ANSC_HANDLE
AnscSstoGetStsIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSstoReset
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
AnscSstoGetLastSecond
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscSstoGetLastTick
    (
        ANSC_HANDLE                 hThisObject
    );

INT
AnscSstoGetiTimeOffset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSstoSetiTimeOffset
    (
        ANSC_HANDLE                 hThisObject,
        INT                         value
    );

ANSC_STATUS
AnscSstoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject

    );

ANSC_STATUS
AnscSstoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject

    );

ULONG
AnscSstoGetCurrSecond
    (
        ANSC_HANDLE                 hThisObject

    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_SSTO_STSIF.C
***********************************************************/

ANSC_STATUS
AnscSstoStsGetLocalTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar
    );

ULONG
AnscSstoStsGetCurSeconds
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSstoStsGetUTCTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscSstoStsAdjustClock1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds,
        int                         iTimeOffset
    );

ANSC_STATUS
AnscSstoStsAdjustClock2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUniversalTime,
        int                         iTimeOffset
    );

ANSC_STATUS
AnscSstoStsSecondsToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscSstoStsCalendarToSeconds
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    );

ANSC_STATUS
AnscSstoStsSysTickToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTicks,
        ANSC_HANDLE                 hCalendar
    );

INT
AnscSstoStsGetTimeOffset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSstoStsSetTimeOffset
    (
        ANSC_HANDLE                 hThisObject,
        INT                         value
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_SSTO_PROCESS.C
***********************************************************/

ANSC_STATUS
AnscSstoCalendarToSecond
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    );

ANSC_STATUS
AnscSstoLocalCalendarToSecond
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar,
        PULONG                      pTimeStampI
    );

ANSC_STATUS
AnscSstoSecondToCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscSstoSecondToLocalCalendar
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       TimeStampI,
        ANSC_HANDLE                 hCalendar
    );

ANSC_STATUS
AnscSstoGetWeekDay
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCalendar

    );


#endif
