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

    module:	ansc_ptr_co_interface.h

        For Ansc String,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

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

        03/17/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _ANSC_PTR_ARRAY_INTERFACE_DEC
#define  _ANSC_PTR_ARRAY_INTERFACE_DEC

#include "ansc_obj_array_co_interface.h"

#define  ANSC_PTR_ARRAY_ITEM_SIZE                   sizeof(ANSC_OBJECT_ARRAY_DATA)
typedef  PULONG                                     ANSC_PTR_ARRAY_DATA, *PANSC_PTR_ARRAY_DATA;

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  ANSC_PTR_ARRAY_COMPONENT_NAME             "PtrArrayComponent"
#define  ANSC_PTR_ARRAY_COMPONENT_OID              ANSC_OBJECT_OID_NULL
#define  ANSC_PTR_ARRAY_COMPONENT_TYPE             ANSC_OBJECT_TYPE_NULL


typedef
void
(*PFN_PTR_ARRAY_ENLARGE)
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_PTR_ARRAY_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                  \
    ANSC_OBJECT_ARRAY_CLASS_CONTENT                                                     \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     hStorage;                                           \
    /* maximum number of items right now can be saved in storage (in UINT) */           \
    ULONG                           ulMaxItemCount;                                     \
    /* number of items saved in storage */                                              \
    ULONG                           ulItemCount;                                        \
    /* start of methods */                                                              \
    PFN_PTR_ARRAY_ENLARGE           EnlargeStorage;                                     \

/* content filtering basic message object  */
typedef  struct
_ANSC_PTR_ARRAY_OBJECT
{
    ANSC_PTR_ARRAY_CLASS_CONTENT
}
ANSC_PTR_ARRAY_OBJECT,    *PANSC_PTR_ARRAY_OBJECT;


#define  ACCESS_ANSC_PTR_ARRAY_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_PTR_ARRAY_OBJECT, Linkage)

#endif

