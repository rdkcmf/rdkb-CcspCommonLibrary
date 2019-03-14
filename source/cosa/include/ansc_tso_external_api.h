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

    module:	ansc_tso_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the timer scheduler.

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


#ifndef  _ANSC_TSO_EXTERNAL_API_
#define  _ANSC_TSO_EXTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TSO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateTimerScheduler
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_TSO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscTsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
AnscTsoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTsoShutdown
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TDO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateTimerDescriptor
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_TDO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscTdoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
AnscTdoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTdoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


#if defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_)

ANSC_HANDLE
AnscCreateTimerDescriptor2
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved,
        char *                      file,
        int                         lineno
    );

ANSC_HANDLE
AnscTdoCreate2
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved,
        char *                      file,
        int                         lineno
    );

ANSC_STATUS
AnscTdoInitialize2
    (
        ANSC_HANDLE                 hThisObject,
        char *                      file,
        int                         lineno
    );

#define AnscCreateTimerDescriptor(cctx, octx, rsv) AnscCreateTimerDescriptor2(cctx, octx, rsv, __FILE__, __LINE__)
#define AnscTdoCreate(cctx, octx, rsv) AnscTdoCreate2(cctx, octx, rsv, __FILE__, __LINE__)
#define AnscTdoInitialize(hThis) AnscTdoInitialize2(hThis, __FILE__, __LINE__)

#endif /* defined(_ANSC_SPINLOCK_PROFILING_) || defined(_ANSC_TDO_PROFILING_) */

#ifdef _ANSC_TDO_PROFILING_

void
AnscTdoProfilingPrint
    (
        void
    );

void 
AnscTdoProfilingEnable
    (
        BOOLEAN                     bEnable,
        ULONG                       ulStart,
        ULONG                       ulStop
    );

void
AnscTdoProfilingCleanup
    (
        void
    );

#endif /* _ANSC_TDO_PROFILING_ */

#endif /* _ANSC_TSO_EXTERNAL_API_ */

