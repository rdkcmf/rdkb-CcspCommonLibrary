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

    module:	ansc_co_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the general Ansc Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_CO_INTERFACE_
#define  _ANSC_CO_INTERFACE_


/***********************************************************
           GENERAL NSM FUNCTION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_COMPONENT_OBJECT_NAME                 "abstractComponentObject"
#define  ANSC_COMPONENT_OBJECT_ID                   0xFFFFFFFF

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCCO_CREATE)
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

typedef  ANSC_STATUS
(*PFN_ANSCCO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCO_ENROLL_OBJECTS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCO_INITIALIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ANSCCO_GET_OWNER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCO_SET_OWNER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwner
    );

typedef  char*
(*PFN_ANSCCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ANSCCO_GET_OID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ANSCCO_GET_RID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCO_DISPATCH_EVENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEvent
    );

/*
 * The Component Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSCCO_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hContainerContext;                                      \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ULONG                           Oid;                                                    \
    ULONG                           Rid;                                                    \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
                                                                                            \
    PFN_ANSCCO_CREATE               Create;                                                 \
    PFN_ANSCCO_REMOVE               Remove;                                                 \
    PFN_ANSCCO_ENROLL_OBJECTS       EnrollObjects;                                          \
    PFN_ANSCCO_INITIALIZE           Initialize;                                             \
    /*                                                                                      \
    PFN_ANSCCO_GET_OWNER            GetOwner;                                               \
    PFN_ANSCCO_SET_OWNER            SetOwner;                                               \
    PFN_ANSCCO_GET_NAME             GetName;                                                \
    PFN_ANSCCO_GET_OID              GetOid;                                                 \
    PFN_ANSCCO_GET_RID              GetRid;                                                 \
    PFN_ANSCCO_DISPATCH_EVENT       DispatchEvent;                                          \
    */                                                                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_COMPONENT_OBJECT
{
    ANSCCO_CLASS_CONTENT
}
ANSC_COMPONENT_OBJECT,  *PANSC_COMPONENT_OBJECT;

#define  ACCESS_ANSC_COMPONENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_COMPONENT_OBJECT, Linkage)


#endif
