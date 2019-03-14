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

    module:	dslh_objco_table_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Objco Table Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_OBJCO_TABLE_INTERFACE_
#define  _DSLH_OBJCO_TABLE_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "dslh_objco_interface.h"
#include "dslh_objco_exported_api.h"


/***********************************************************
        DSLH OBJCO TABLE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_DSLHOBJCOTABLE_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCOTABLE_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_DSLHOBJCOTABLE_IS_STH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCOTABLE_SYNC)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_DSLHOBJCOTABLE_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJCOTABLE_GET_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG*                      pulInsNumber
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJCOTABLE_NEW_ENTRY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCOTABLE_ADD_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj,
        ULONG*                      pulInsNumber
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCOTABLE_DEL_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryObj
    );

/*
 * In some ways, the DslhObjController object reassembles the idea of a SLAP object. Both of them
 * are created in runtime and serve as a bridging layer between the Management Applications and the
 * service modules underneath. However, the DSLH Data Management Model has much less overhead when
 * comparing to the sophisticated SLAP Component Programming Model.
 */
#define  DSLH_OBJCO_TABLE_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    DSLH_OBJ_CONTROLLER_CLASS_CONTENT                                                       \
    /* start of object class content */                                                     \
    PFN_DSLHOBJCOTABLE_IS_STH       IsUpdated;                                              \
    PFN_DSLHOBJCOTABLE_SYNC         Synchronize;                                            \
    PFN_DSLHOBJCOTABLE_ACTION       Lock;                                                   \
    PFN_DSLHOBJCOTABLE_ACTION       Unlock;                                                 \
    ULONG                           uReqCounter;                                            \
                                                                                            \
    PFN_DSLHOBJCOTABLE_IS_STH       IsWritable;                                             \
                                                                                            \
    PFN_DSLHOBJCOTABLE_GET_COUNT    GetEntryObjectCount;                                    \
    PFN_DSLHOBJCOTABLE_GET_ENTRY    GetEntryObject;                                         \
    PFN_DSLHOBJCOTABLE_NEW_ENTRY    NewEntryObject;                                         \
    PFN_DSLHOBJCOTABLE_ADD_ENTRY    AddEntryObject;                                         \
    PFN_DSLHOBJCOTABLE_DEL_ENTRY    DelEntryObject;                                         \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_OBJCO_TABLE_OBJECT
{
    DSLH_OBJCO_TABLE_CLASS_CONTENT
}
DSLH_OBJCO_TABLE_OBJECT,  *PDSLH_OBJCO_TABLE_OBJECT;

#define  ACCESS_DSLH_OBJCO_TABLE_OBJECT(p)          \
         ACCESS_CONTAINER(p, DSLH_OBJCO_TABLE_OBJECT, Linkage)
#define  ACCESS_DSLH_OBJCO_TABLE_FIELD(f)           \
         ACCESS_FIELD_OFFSET(DSLH_OBJCO_TABLE_OBJECT, f)


#endif
