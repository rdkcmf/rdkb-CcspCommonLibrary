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

    module:	slap_owouoac_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Owo Uoac Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/11/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OWOUOAC_INTERFACE_
#define  _SLAP_OWOUOAC_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_owo_interface.h"
#include "slap_owo_exported_api.h"


/***********************************************************
          SLAP OWO UOAC COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_OWOUOAC_OWO_TABLE_SIZE                64

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOWOUOAC_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOWOUOAC_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_SLAPOWOUOAC_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_type
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_ATTACH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_DETACH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    );

typedef  ANSC_STATUS
(*PFN_SLAPOWOUOAC_CLEAN)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The SLAP Obj Wrapper Object is used to encapsulate a generic object presentation to the upper
 * layer modules. It wraps the actual object handle along with the LPC information of the target
 * LPC party, which is running in a different process, if the wrapped object is a remote object.
 * Note that the reason why we need a separate wrapper object instead of using SLAP_SCO directly is
 * to unify the object creation and access via SLAP_GOA interface.
 */
#define  SLAP_OWO_UOAC_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    SLAP_OBJ_WRAPPER_CLASS_CONTENT                                                          \
    /* start of object class content */                                                     \
    QUEUE_HEADER                    OwoTable[SLAP_OWOUOAC_OWO_TABLE_SIZE];                  \
    ANSC_LOCK                       OwoTableLock;                                           \
                                                                                            \
    PFN_SLAPOWOUOAC_ATTACH          AttachObjWrapper;                                       \
    PFN_SLAPOWOUOAC_DETACH          DetachObjWrapper;                                       \
    PFN_SLAPOWOUOAC_REMOVE          RemoveObjWrapper;                                       \
    PFN_SLAPOWOUOAC_CLEAN           CleanOwoTable;                                          \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OWO_UOAC_OBJECT
{
    SLAP_OWO_UOAC_CLASS_CONTENT
}
SLAP_OWO_UOAC_OBJECT,  *PSLAP_OWO_UOAC_OBJECT;

#define  ACCESS_SLAP_OWO_UOAC_OBJECT(p)             \
         ACCESS_CONTAINER(p, SLAP_OWO_UOAC_OBJECT, Linkage)
#define  ACCESS_SLAP_OWO_UOAC_FIELD(f)              \
         ACCESS_FIELD_OFFSET(SLAP_OWO_UOAC_OBJECT, f)


#endif
