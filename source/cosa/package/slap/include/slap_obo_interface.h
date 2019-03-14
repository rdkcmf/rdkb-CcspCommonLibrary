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

    module:	slap_obo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Obj Broker Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OBO_INTERFACE_
#define  _SLAP_OBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"
#include "slap_properties.h"


/***********************************************************
          SLAP OBJ BROKER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_OBO_OBO_TABLE_SIZE                    16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOBO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_SLAPOBO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SLAPOBO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOBO_GET_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOBO_ADD_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_DEL_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_SLAPOBO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * SLAP supports a hierarchical object name space, which can be extended to support new objects and
 * remote object invocation. SLAP Obj Broker Object is designed to manage such name space by employ-
 * ing a tree-like structure to maintain all the registered OBJECT_DESCRIPTORS. This object is very
 * similar to WebUriPath object in many aspects.
 */
#define  SLAP_OBJ_BROKER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    char*                           PathName;                                               \
    ANSC_HANDLE                     hSlapObjEntity;                                         \
    ANSC_HANDLE                     hSlapObjProxy;                                          \
    ULONG                           HashIndex;                                              \
                                                                                            \
    QUEUE_HEADER                    OboTable[SLAP_OBO_OBO_TABLE_SIZE];                      \
    ANSC_LOCK                       OboTableLock;                                           \
                                                                                            \
    PFN_SLAPOBO_IS_SOMETHING        IsRemoveable;                                           \
    PFN_SLAPOBO_SET_NAME            SetPathName;                                            \
                                                                                            \
    PFN_SLAPOBO_GET_OBO             GetObjBroker;                                           \
    PFN_SLAPOBO_ADD_OBO             AddObjBroker;                                           \
    PFN_SLAPOBO_DEL_OBO             DelObjBroker;                                           \
    PFN_SLAPOBO_DEL_ALL             DelAllObjBrokers;                                       \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OBJ_BROKER_OBJECT
{
    SLAP_OBJ_BROKER_CLASS_CONTENT
}
SLAP_OBJ_BROKER_OBJECT,  *PSLAP_OBJ_BROKER_OBJECT;

#define  ACCESS_SLAP_OBJ_BROKER_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_OBJ_BROKER_OBJECT, Linkage)


#endif
