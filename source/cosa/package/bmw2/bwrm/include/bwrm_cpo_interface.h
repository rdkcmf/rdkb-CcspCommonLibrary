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

    module:	bwrm_cpo_interface.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwrm Cooked Page Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_CPO_INTERFACE_
#define  _BWRM_CPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
        BWRM COOKED PAGE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWRMCPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWRMCPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BWRMCPO_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path_name
    );

typedef  ULONG
(*PFN_BWRMCPO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timestamp
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_BWRMCPO_MATCH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BWRMCPO_CHG_REF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_BWRMCPO_GET_REF)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Bwrm Cooked Page Object is the generic representation of any intermediate BMW2 page format.
 * For example, in order for BREE to execute a BSP page, it must first analyze the BSP page and
 * convert it into an internal format before generating any rendering result. Different execution
 * engines MUST decide the best internal representation for its own use; the 'hPageContext' field
 * should be used to point to the engine-specific object/context.
 */
#define  BWRM_COOKED_PAGE_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    char*                           RootPath;                                               \
    char*                           PagePath;                                               \
    ANSC_HANDLE                     hOwnerReserved;                                         \
    ANSC_HANDLE                     hPageContext;                                           \
    ULONG                           Timestamp;                                              \
    ANSC_LOCK                       AccessLock;                                             \
    ULONG                           RefCount;                                               \
                                                                                            \
    PFN_BWRMCPO_GET_PATH            GetRootPath;                                            \
    PFN_BWRMCPO_SET_PATH            SetRootPath;                                            \
    PFN_BWRMCPO_GET_PATH            GetPagePath;                                            \
    PFN_BWRMCPO_SET_PATH            SetPagePath;                                            \
    PFN_BWRMCPO_GET_CONTEXT         GetOwnerReserved;                                       \
    PFN_BWRMCPO_SET_CONTEXT         SetOwnerReserved;                                       \
    PFN_BWRMCPO_GET_CONTEXT         GetPageContext;                                         \
    PFN_BWRMCPO_SET_CONTEXT         SetPageContext;                                         \
    PFN_BWRMCPO_GET_TIME            GetTimestamp;                                           \
    PFN_BWRMCPO_SET_TIME            SetTimestamp;                                           \
    PFN_BWRMCPO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BWRMCPO_MATCH               MatchPath;                                              \
                                                                                            \
    PFN_BWRMCPO_CHG_REF             IncRefCount;                                            \
    PFN_BWRMCPO_CHG_REF             DecRefCount;                                            \
    PFN_BWRMCPO_GET_REF             GetRefCount;                                            \
    /* end of object class content */                                                       \

typedef  struct
_BWRM_COOKED_PAGE_OBJECT
{
    BWRM_COOKED_PAGE_CLASS_CONTENT
}
BWRM_COOKED_PAGE_OBJECT,  *PBWRM_COOKED_PAGE_OBJECT;

#define  ACCESS_BWRM_COOKED_PAGE_OBJECT(p)          \
         ACCESS_CONTAINER(p, BWRM_COOKED_PAGE_OBJECT, Linkage)


#endif
