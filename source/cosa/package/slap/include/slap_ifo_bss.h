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

    module:	slap_ifo_bss.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Basic System Service Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_IFO_BSS_
#define  _SLAP_IFO_BSS_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
       SLAP BASIC SYSTEM SERVICE INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_BSS_INTERFACE_NAME                    "slapBasicSystemServiceIf"
#define  SLAP_BSS_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_SLAPBSSIF_GET_FLAG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPBSSIF_SET_FLAG)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue
    );

typedef  ULONG
(*PFN_SLAPBSSIF_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPBSSIF_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_HANDLE
(*PFN_SLAPBSSIF_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Most the internal SLAP System Service Objects are made avaiable via the open SLAP object name
 * space. However, it's not very efficient for all the internal SLAP objects to access these system
 * objects by using the UOA interface. The BSS interface offers a shortcut for those objects that
 * need to access system objects but don't want to go through UOA.
 */
#define  SLAP_BSS_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPBSSIF_GET_FLAG          GetPoolingFlag;                                         \
    PFN_SLAPBSSIF_SET_FLAG          SetPoolingFlag;                                         \
    PFN_SLAPBSSIF_GET_SIZE          GetDefPoolSize;                                         \
    PFN_SLAPBSSIF_SET_SIZE          SetDefPoolSize;                                         \
    PFN_SLAPBSSIF_GET_CONTEXT       GetVarConverter;                                        \
    PFN_SLAPBSSIF_GET_CONTEXT       GetVarMapper;                                           \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_BSS_INTERFACE
{
    SLAP_BSS_INTERFACE_CLASS_CONTENT
}
SLAP_BSS_INTERFACE,  *PSLAP_BSS_INTERFACE;

#define  ACCESS_SLAP_BSS_INTERFACE(p)               \
         ACCESS_CON(p, SLAP_BSS_INTERFACE, Linkage)


#endif
