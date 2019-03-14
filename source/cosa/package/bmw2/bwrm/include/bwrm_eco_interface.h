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

    module:	bwrm_eco_interface.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwrm Env Controller Objects.

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


#ifndef  _BWRM_ECO_INTERFACE_
#define  _BWRM_ECO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwrm_properties.h"
#include "bwrm_ifo_ram.h"


/***********************************************************
        BWRM ENV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWRMECO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWRMECO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMECO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Bwrm Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * bwrm object loading, the concept of "Environment Control" is really important.
 */
#define  BWRM_ENV_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BWRM_ENV_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hBwrmRamIf;                                             \
    ANSC_HANDLE                     hBwrmFileManager;                                       \
    ANSC_HANDLE                     hBwrmPageManager;                                       \
    QUEUE_HEADER                    ForcedCachedPages;                                      \
    ANSC_LOCK                       AccessLock;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BWRMECO_GET_IF              GetBwrmRamIf;                                           \
    PFN_BWRMECO_GET_CONTEXT         GetBwrmFileManager;                                     \
    PFN_BWRMECO_GET_CONTEXT         GetBwrmPageManager;                                     \
    PFN_BWRMECO_GET_PROPERTY        GetProperty;                                            \
    PFN_BWRMECO_SET_PROPERTY        SetProperty;                                            \
    PFN_BWRMECO_RESET               ResetProperty;                                          \
    PFN_BWRMECO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BWRMECO_ENGAGE              Engage;                                                 \
    PFN_BWRMECO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BWRMRAMIF_GET_SIZE          RamGetCacheNumber;                                      \
    PFN_BWRMRAMIF_SET_SIZE          RamSetCacheNumber;                                      \
    PFN_BWRMRAMIF_GET_SIZE          RamGetCacheSize;                                        \
    PFN_BWRMRAMIF_SET_SIZE          RamSetCacheSize;                                        \
    PFN_BWRMRAMIF_GET_TIME          RamGetCacheTimeout;                                     \
    PFN_BWRMRAMIF_SET_TIME          RamSetCacheTimeout;                                     \
    PFN_BWRMRAMIF_CONTROL           RamControlCache;                                        \
                                                                                            \
    PFN_BWRMRAMIF_GET_PAGE1         RamGetRawPage;                                          \
    PFN_BWRMRAMIF_GET_PAGE2         RamGetCookedPage;                                       \
    PFN_BWRMRAMIF_ADD_PAGE          RamAddCookedPage;                                       \
    PFN_BWRMRAMIF_DEL_PAGE          RamDelCookedPage;                                       \
                                                                                            \
    PFN_BWRMRAMIF_FORCE_PC          ForcePageCache;                                         \
    PFN_BWRMRAMIF_IS_PCF            IsPageCacheForced;                                      \
    /* end of object class content */                                                       \

typedef  struct
_BWRM_ENV_CONTROLLER_OBJECT
{
    BWRM_ENV_CONTROLLER_CLASS_CONTENT
}
BWRM_ENV_CONTROLLER_OBJECT,  *PBWRM_ENV_CONTROLLER_OBJECT;

#define  ACCESS_BWRM_ENV_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BWRM_ENV_CONTROLLER_OBJECT, Linkage)


typedef  struct
_BWRM_ENV_CACHE_FORCED_PAGE
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           PagePath;
}
BWRM_ENV_CACHE_FORCED_PAGE, *PBWRM_ENV_CACHE_FORCED_PAGE;

#define  ACCESS_BWRM_ENV_CACHE_FORCED_PAGE(p)       \
         ACCESS_CONTAINER(p, BWRM_ENV_CACHE_FORCED_PAGE, Linkage)

#define  BwrmEnvRemoveCacheForcedPage(pCFPage)                      \
    do {                                                            \
        if (pCFPage->PagePath)                                      \
            AnscFreeMemory(pCFPage->PagePath);                      \
        AnscFreeMemory(pCFPage);                                    \
    } while (0)


#endif
