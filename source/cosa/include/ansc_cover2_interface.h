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

    module:	ansc_cover2_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the general Ansc CoVer2 Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/13/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_COVER2_INTERFACE_
#define  _ANSC_COVER2_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
        GENERAL ANSC COMPONENT OBJECT VER2 DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_CO_VER2_NAME                          "coVer2"
#define  ANSC_CO_VER2_OID                           0xFFFFFFFF

#define  ansc_co_begin_atomic_exec(p)               AnscAcquireLock(&p->CoAtomLock);
#define  ansc_co_end_atomic_exec(p)                 AnscReleaseLock(&p->CoAtomLock);
#define  ansc_co_enter_section(p)                   AnscAcquireLock(&p->CoSyncLock);
#define  ansc_co_leave_section(p)                   AnscReleaseLock(&p->CoSyncLock);

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_ANSCCOVER2_BEGIN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER2_END)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER2_ENTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER2_LEAVE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Component Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSCCO_VER2_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ULONG                           CoTaskHandle;                                           \
    ANSC_LOCK                       CoAtomLock;                                             \
    ANSC_LOCK                       CoSyncLock;                                             \
                                                                                            \
    PFN_ANSCCOVER2_BEGIN            CoBeginAtomicExec;                                      \
    PFN_ANSCCOVER2_END              CoEndAtomicExec;                                        \
                                                                                            \
    PFN_ANSCCOVER2_ENTER            CoEnterCriticalSection;                                 \
    PFN_ANSCCOVER2_LEAVE            CoLeaveCriticalSection;                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_CO_VER2_OBJECT
{
    ANSCCO_VER2_CLASS_CONTENT
}
ANSC_CO_VER2_OBJECT,  *PANSC_CO_VER2_OBJECT;

#define  ACCESS_ANSC_CO_VER2_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_CO_VER2_OBJECT, Linkage)


#endif
