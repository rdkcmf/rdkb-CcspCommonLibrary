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

    module:	ansc_sysco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the System Clock Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/16/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SYSCO_INTERFACE_
#define  _ANSC_SYSCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_wco_interface.h"
#include "ansc_wco_external_api.h"


/***********************************************************
     PLATFORM INDEPENDENT SYSTEM CLOCK OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SYSCO_SET_ADDRESS1)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  ANSC_STATUS
(*PFN_SYSCO_SET_ADDRESS2)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      serverArray,
        ULONG                       ulCount
    );

/*
 * During the service development process, we constantly find ourselves in a situation where
 * absolute time/date has to be known in order to perform a operation. This Wall Clock Object
 * encapsulates most standard clock-related operations. This object SHOULD be considered as
 * a base object, and may be implemented differently on different operating systems. If the
 * underlying operating system already supports the time/date operation, we may inherit the
 * functions instead of implementing our own; otherwise, we have to implement NTP (Network Time
 * Protocol) based Wall Clock.
 */
#define  ANSC_SYSTEM_CLOCK_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_WALL_CLOCK_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SYSTEM_CLOCK_OBJECT
{
    ANSC_SYSTEM_CLOCK_CLASS_CONTENT
}
ANSC_SYSTEM_CLOCK_OBJECT,  *PANSC_SYSTEM_CLOCK_OBJECT;

#define  ACCESS_ANSC_SYSTEM_CLOCK_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_SYSTEM_CLOCK_OBJECT, Linkage)


#endif
