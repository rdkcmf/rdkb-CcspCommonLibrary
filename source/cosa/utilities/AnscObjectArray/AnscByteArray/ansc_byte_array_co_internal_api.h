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

    module:	ansc_byte_array_co_internal_api.h

        For Byte Array object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Object Array component
        object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/03/02    initial revision.


    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/

#ifndef  _ANSC_BYTE_ARRAY_CO_INTERNAL_API
#define  _ANSC_BYTE_ARRAY_CO_INTERNAL_API


ULONG
AnscByteArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    );


void
AnscByteArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    );


void
AnscByteArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_OBJECT_ARRAY_DATA
AnscByteArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );


void
AnscByteArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    );

void
AnscByteArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    );


void
AnscByteArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    );


ULONG
AnscByteArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    );


void
AnscByteArrayEnlargeStorage
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );

LONG
AnscByteArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    );

ANSC_HANDLE
AnscByteArrayGetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

BOOL
AnscByteArraySetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pBuf,
        ULONG                       ulSize,
        BOOL                        bForeignBuffer
    );

#endif

