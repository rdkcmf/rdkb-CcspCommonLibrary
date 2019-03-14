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

    module:	ansc_cover2_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Ansc CoVer2 Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/13/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_COVER2_EXTERNAL_API_
#define  _ANSC_COVER2_EXTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_COVER2_BASE.C
***********************************************************/

ANSC_HANDLE
AnscCoVer2Create
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
AnscCoVer2Remove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCoVer2EnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCoVer2Initialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_COVER2_SYNC.C
***********************************************************/

ANSC_STATUS
AnscCoVer2BeginAtomicExec
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCoVer2EndAtomicExec
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCoVer2EnterCriticalSection
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscCoVer2LeaveCriticalSection
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
