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

    module:	ansc_ato_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Atom Table.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_ATO_INTERNAL_API_
#define  _ANSC_ATO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_ATO_STATES.C
***********************************************************/

ULONG
AnscAtoGetIntAtomCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAtoGetStrAtomCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAtoGetIntTableSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAtoSetIntTableSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ULONG
AnscAtoGetStrTableSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAtoSetStrTableSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscAtoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_ATO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscAtoAllocateIntTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAtoAllocateStrTable
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAtoGetAtomCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAtoGetAtomByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ULONG
AnscAtoGetAtomCount2
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAtoGetAtomByIndex2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_ATO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscAtoAddAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       id,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
AnscAtoGetAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscAtoDelAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscAtoDelAllStrAtoms
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAtoAddAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id,
        char*                       name,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
AnscAtoGetAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ANSC_STATUS
AnscAtoDelAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ANSC_STATUS
AnscAtoDelAllIntAtoms
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
