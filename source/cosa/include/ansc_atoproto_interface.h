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

    module:	ansc_atoproto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Ansc Ato Protocol Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/17/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ATOPROTO_INTERFACE_
#define  _ANSC_ATOPROTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"


/***********************************************************
              ANSC ATO PROTOCOL OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ATOPROTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATOPROTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_ATOPROTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATOPROTO_CREATE_TABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ATOPROTO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  char*
(*PFN_ATOPROTO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

/*
 * Most people are more comfortable dealing with names instead of numbers. This observation also
 * holds true in the programming world. It's just plain nice to be able to convert a protocol name,
 * such as TCP or HTTP, into the corresonding protocol_type/port_number.
 */
#define  ANSC_ATO_PROTOCOL_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_ATOM_TABLE_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    PFN_ATOPROTO_CREATE_TABLE       CreateTypeTable;                                        \
    PFN_ATOPROTO_CREATE_TABLE       CreateNameTable;                                        \
    PFN_ATOPROTO_GET_TYPE           GetProtocolType;                                        \
    PFN_ATOPROTO_GET_NAME           GetProtocolName;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ATO_PROTOCOL_OBJECT
{
    ANSC_ATO_PROTOCOL_CLASS_CONTENT
}
ANSC_ATO_PROTOCOL_OBJECT,  *PANSC_ATO_PROTOCOL_OBJECT;

#define  ACCESS_ANSC_ATO_PROTOCOL_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_ATO_PROTOCOL_OBJECT, Linkage)


#endif
