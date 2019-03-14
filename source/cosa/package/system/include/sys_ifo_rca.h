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

    module:	sys_ifo_rca.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Content Auditor Interface
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/07/02    initial revision.

**********************************************************************/


#ifndef  _SYS_IFO_RCA_
#define  _SYS_IFO_RCA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
     SYS REPOSITORY CONTENT AUDITOR INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SYS_RCA_INTERFACE_NAME                     "sysRepositoryContentAuditorIf"
#define  SYS_RCA_INTERFACE_ID                       0

#define  SYS_RCA_EVENT_FOLDER_ADDED                 1
#define  SYS_RCA_EVENT_FOLDER_DELETED               2
#define  SYS_RCA_EVENT_RECORD_ADDED                 3
#define  SYS_RCA_EVENT_RECORD_MODIFIED              4
#define  SYS_RCA_EVENT_RECORD_DELETED               5

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SYSRCAIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SYSRCAIF_REVIEW)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

/*
 * Although not recommended, the system repository may be used by different modules to exchange run-
 * time information. For example: Web-based Configuration Modules may modify the policy settings
 * stored in the repository directly, and request such changes to be conveyed to the owner of the
 * policy settings. The Repository Content Auditor Interface is designed for such purpose. If the
 * owner (abstract concept, there's no counterpart in the implementation, at least not yet) of a
 * repository folder allows other modules to modify its configuration, it shall associate such an
 * interface with the folder object.
 */
#define  SYS_RCA_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SYSRCAIF_NOTIFY             Notify;                                                 \
    PFN_SYSRCAIF_REVIEW             Review;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_SYS_RCA_INTERFACE
{
    SYS_RCA_INTERFACE_CLASS_CONTENT
}
SYS_RCA_INTERFACE,  *PSYS_RCA_INTERFACE;

#define  ACCESS_SYS_RCA_INTERFACE(p)                \
         ACCESS_CONTAINER(p, SYS_RCA_INTERFACE, Linkage)


#endif
