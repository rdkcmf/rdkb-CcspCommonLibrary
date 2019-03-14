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

    module:	sys_ifo_ram.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Access Monitor Interface
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/14/04    initial revision.

**********************************************************************/


#ifndef  _SYS_IFO_RAM_
#define  _SYS_IFO_RAM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
       SYS INFO REPOSITORY ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SYS_RAM_INTERFACE_NAME                     "sysRepositoryAccessMonitorIf"
#define  SYS_RAM_INTERFACE_ID                       0

#define  SYS_RAM_EVENT_folderAdded                  1
#define  SYS_RAM_EVENT_folderUpdated                2
#define  SYS_RAM_EVENT_folderDeleted                3
#define  SYS_RAM_EVENT_folderCleared                4
#define  SYS_RAM_EVENT_recordAdded                  5
#define  SYS_RAM_EVENT_recordUpdated                6
#define  SYS_RAM_EVENT_recordDeleted                7

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SYSRAMIF_ENABLE_SYNC)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_SYSRAMIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSysRepFolder,
        ULONG                       ulEvent
    );

/*
 * As part of the effort to minimize the integration cost and unify the development of different
 * system components, we divide the product development into three stages: component development,
 * packaging, and integration. We encapsulate all the system modules inside a system package object
 * which exposes following API functions. These functions are used by customers for customization
 * and final integration.
 */
#define  SYS_RAM_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SYSRAMIF_ENABLE_SYNC        EnableFileSync;                                         \
    PFN_SYSRAMIF_NOTIFY             Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_SYS_RAM_INTERFACE
{
    SYS_RAM_INTERFACE_CLASS_CONTENT
}
SYS_RAM_INTERFACE,  *PSYS_RAM_INTERFACE;

#define  ACCESS_SYS_RAM_INTERFACE(p)                \
         ACCESS_CONTAINER(p, SYS_RAM_INTERFACE, Linkage)


#endif
