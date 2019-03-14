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

    module:	sys_ifo_rcg.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Content Generator
        Interface Objects.

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


#ifndef  _SYS_IFO_RCG_
#define  _SYS_IFO_RCG_


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
#define  SYS_RCG_INTERFACE_NAME                     "sysRepositoryContentGeneratorIf"
#define  SYS_RCG_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_SYSRCGIF_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSRCGIF_SYNCHRONIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

/*
 * Unlike some other central system repositories that you experienced before (e.g., NT registry),
 * the Ansc System Repository does NOT assume any role for the permanent storage. In other word,
 * its primary use is for runtime information management. While a simple tree-like data structure
 * is sufficient in most cases, we like to extend its functionality to support the storage for
 * content that can be dynamically generated. A module can associate such an interface with any
 * folder that stores and manages dynamic content.
 */
#define  SYS_RCG_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SYSRCGIF_IS_SOMETHING       NeedsUpdate;                                            \
    PFN_SYSRCGIF_SYNCHRONIZE        Synchronize;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SYS_RCG_INTERFACE
{
    SYS_RCG_INTERFACE_CLASS_CONTENT
}
SYS_RCG_INTERFACE,  *PSYS_RCG_INTERFACE;

#define  ACCESS_SYS_RCG_INTERFACE(p)                \
         ACCESS_CONTAINER(p, SYS_RCG_INTERFACE, Linkage)


#endif
