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

    module:	dslh_objco_table_internal_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Dslh Objco Table
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/03/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_OBJCO_TABLE_INTERNAL_API_
#define  _DSLH_OBJCO_TABLE_INTERNAL_API_


/***********************************************************
     FUNCTIONS IMPLEMENTED IN DSLH_OBJCO_TABLE_ACCESS.C
***********************************************************/

BOOL
DslhObjcoTableIsUpdated
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjcoTableSynchronize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjcoTableLock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjcoTableUnlock
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
DslhObjcoTableGetEntryObjectCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
DslhObjcoTableGetEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG*                      pulInsNumber
    );

ANSC_HANDLE
DslhObjcoTableNewEntryObject
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhObjcoTableAddEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj,
        ULONG*                      pulInsNumber
    );

ANSC_STATUS
DslhObjcoTableDelEntryObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj
    );

#endif
