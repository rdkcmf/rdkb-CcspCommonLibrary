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

    module:	ansc_obj_array_co_interface.h

        For Ansc String,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        string implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _ANSC_OBJECT_ARRAY_INTERFACE_DEC
#define  _ANSC_OBJECT_ARRAY_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  ANSC_OBJECT_ARRAY_COMPONENT_NAME           "ObjectArrayComponent"
#define  ANSC_OBJECT_ARRAY_COMPONENT_OID            ANSC_OBJECT_OID_NULL
#define  ANSC_OBJECT_ARRAY_COMPONENT_TYPE           ANSC_OBJECT_TYPE_NULL

typedef  ANSC_HANDLE                                ANSC_OBJECT_ARRAY_DATA, PANSC_OBJECT_ARRAY_DATA;

#define  ANSC_OBJECT_ARRAY_DEFAULT_GROWTH           8

typedef
ULONG
(*PFN_ANSC_OBJARRAY_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_ANSC_OBJARRAY_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    );

typedef
void
(*PFN_ANSC_OBJARRAY_REMOVE_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_OBJECT_ARRAY_DATA
(*PFN_ANSC_OBJARRAY_GET_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef
void
(*PFN_ANSC_OBJARRAY_SET_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    );

typedef
void
(*PFN_ANSC_OBJARRAY_INSERT_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    );

typedef
void
(*PFN_ANSC_OBJARRAY_REMOVE_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    );

typedef
ULONG
(*PFN_ANSC_OBJARRAY_ADD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    );

typedef
void
(*PFN_ANSC_OBJARRAY_SET_GROWTH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulGrowth
    );

typedef
LONG
(*PFN_ANSC_OBJARRAY_FIND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_OBJECT_ARRAY_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ULONG                           ulGrowth;                                           \
    /* start of methods */                                                              \
    PFN_ANSC_OBJARRAY_GET_SIZE      GetSize;                                            \
    PFN_ANSC_OBJARRAY_SET_SIZE      SetSize;                                            \
    PFN_ANSC_OBJARRAY_REMOVE_ALL    RemoveAll;                                          \
    PFN_ANSC_OBJARRAY_GET_AT        GetAt;                                              \
    PFN_ANSC_OBJARRAY_SET_AT        SetAt;                                              \
    PFN_ANSC_OBJARRAY_INSERT_AT     InsertAt;                                           \
    PFN_ANSC_OBJARRAY_REMOVE_AT     RemoveAt;                                           \
    PFN_ANSC_OBJARRAY_ADD           Add;                                                \
    PFN_ANSC_OBJARRAY_SET_GROWTH    SetGrowth;                                          \
    PFN_ANSC_OBJARRAY_FIND          Find;                                               \


/* content filtering basic message object  */
typedef  struct
_ANSC_OBJECT_ARRAY_OBJECT
{
    ANSC_OBJECT_ARRAY_CLASS_CONTENT
}
ANSC_OBJECT_ARRAY_OBJECT,    *PANSC_OBJECT_ARRAY_OBJECT;


#define  ACCESS_ANSC_OBJECT_ARRAY_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_OBJECT_ARRAY_OBJECT, Linkage)

#endif

