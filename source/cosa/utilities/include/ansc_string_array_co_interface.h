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

    module:	ansc_string_co_interface.h

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

#ifndef  _ANSC_STRING_ARRAY_INTERFACE_DEC
#define  _ANSC_STRING_ARRAY_INTERFACE_DEC

#include "ansc_obj_array_co_interface.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  ANSC_STRING_ARRAY_COMPONENT_NAME           "StringArrayComponent"
#define  ANSC_STRING_ARRAY_COMPONENT_OID            ANSC_OBJECT_OID_NULL
#define  ANSC_STRING_ARRAY_COMPONENT_TYPE           ANSC_OBJECT_TYPE_NULL


typedef  struct
_ANSC_STRING_ARRAY_FIND_DATA
{
    PUCHAR                          pStr;
    ULONG                           ulStrLen;
    BOOL                            bCaseSensitive;
}
ANSC_STRING_ARRAY_FIND_DATA, *PANSC_STRING_ARRAY_FIND_DATA;


typedef
void
(*PFN_STRING_ARRAY_ENLARGE)
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
#define  ANSC_STRING_ARRAY_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                  \
    ANSC_OBJECT_ARRAY_CLASS_CONTENT                                                     \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     hStorage;                                           \
    /* maximum number of items right now can be saved in storage (in UINT) */           \
    ULONG                           ulMaxItemCount;                                     \
    /* number of items saved in storage */                                              \
    ULONG                           ulItemCount;                                        \
    /* start of methods */                                                              \
    PFN_STRING_ARRAY_ENLARGE        EnlargeStorage;                                     \

/* content filtering basic message object  */
typedef  struct
_ANSC_STRING_ARRAY_OBJECT
{
    ANSC_STRING_ARRAY_CLASS_CONTENT
}
ANSC_STRING_ARRAY_OBJECT,    *PANSC_STRING_ARRAY_OBJECT;


#define  ACCESS_ANSC_STRING_ARRAY_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_STRING_ARRAY_OBJECT, Linkage)

#endif

