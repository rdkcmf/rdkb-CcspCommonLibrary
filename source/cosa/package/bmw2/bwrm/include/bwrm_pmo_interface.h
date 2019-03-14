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

    module:	bwrm_pmo_interface.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwrm Page Manager Objects.

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


#ifndef  _BWRM_PMO_INTERFACE_
#define  _BWRM_PMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwrm_properties.h"


/***********************************************************
        BWRM PAGE MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BWRM_PMO_CPO_TABLE_SIZE                    64
#define  BWRM_PMO_CACHE_TIMER_INTERVAL              60 * 1000   /* 1 minute, in milliseconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWRMPMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWRMPMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  BOOL
(*PFN_BWRMPMO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ULONG
(*PFN_BWRMPMO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ULONG
(*PFN_BWRMPMO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       size
    );

typedef  ULONG
(*PFN_BWRMPMO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BWRMPMO_GET_PAGE2)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BWRMPMO_GET_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_ADD_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        ANSC_HANDLE                 hCookedPage
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_DEL_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    );

typedef  ANSC_STATUS
(*PFN_BWRMPMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Bwrm Page Manager Object provides a generic dynamic page caching mechanism. By default, the
 * dynamic pages are identified by the 'page_path' field associated with web resource. It's up to
 * each dynamic Page Execution Page to construct the derived Cooked Page Objects, so they can be
 * cached and managed in the same fashion.
 */
#define  BWRM_PAGE_MANAGER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BWRM_PAGE_MANAGER_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hCacheTimerObject;                                      \
    ANSC_HANDLE                     hCacheTimerClient;                                      \
    ULONG                           Timestamp;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    CpoTable[BWRM_PMO_CPO_TABLE_SIZE];                      \
    ANSC_LOCK                       CpoTableLock;                                           \
                                                                                            \
    PFN_BWRMPMO_GET_BOOL            GetCacheEnabled;                                        \
    PFN_BWRMPMO_SET_BOOL            SetCacheEnabled;                                        \
    PFN_BWRMPMO_GET_COUNT           GetCacheEntryCount;                                     \
    PFN_BWRMPMO_SET_COUNT           SetCacheEntryCount;                                     \
    PFN_BWRMPMO_GET_SIZE            GetCacheMemorySize;                                     \
    PFN_BWRMPMO_SET_SIZE            SetCacheMemorySize;                                     \
    PFN_BWRMPMO_GET_TIME            GetCacheTimeout;                                        \
    PFN_BWRMPMO_SET_TIME            SetCacheTimeout;                                        \
                                                                                            \
    PFN_BWRMPMO_GET_PROPERTY        GetProperty;                                            \
    PFN_BWRMPMO_SET_PROPERTY        SetProperty;                                            \
    PFN_BWRMPMO_RESET               ResetProperty;                                          \
    PFN_BWRMPMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BWRMPMO_ENGAGE              Engage;                                                 \
    PFN_BWRMPMO_CANCEL              Cancel;                                                 \
    PFN_BWRMPMO_INVOKE              CacheTimerInvoke;                                       \
                                                                                            \
    PFN_BWRMPMO_GET_COUNT           GetPageCount;                                           \
    PFN_BWRMPMO_GET_PAGE2           GetOldestPage;                                          \
    PFN_BWRMPMO_GET_PAGE            GetPage;                                                \
    PFN_BWRMPMO_ADD_PAGE            AddPage;                                                \
    PFN_BWRMPMO_DEL_PAGE            DelPage;                                                \
    PFN_BWRMPMO_DEL_ALL             DelAllPages;                                            \
    /* end of object class content */                                                       \

typedef  struct
_BWRM_PAGE_MANAGER_OBJECT
{
    BWRM_PAGE_MANAGER_CLASS_CONTENT
}
BWRM_PAGE_MANAGER_OBJECT,  *PBWRM_PAGE_MANAGER_OBJECT;

#define  ACCESS_BWRM_PAGE_MANAGER_OBJECT(p)         \
         ACCESS_CONTAINER(p, BWRM_PAGE_MANAGER_OBJECT, Linkage)


#endif
