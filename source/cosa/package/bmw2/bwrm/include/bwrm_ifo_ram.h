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

    module:	bwrm_ifo_ram.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwrm Resource Access Manager (RAM)
        Objects.

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


#ifndef  _BWRM_IFO_RAM_
#define  _BWRM_IFO_RAM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  BWRM RESOURCE ACCESS MANAGER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BWRM_RAM_INTERFACE_NAME                    "bwrmResourceAccessManagerIf"
#define  BWRM_RAM_INTERFACE_ID                      0

#define  BWRM_RAM_CTRL_CODE_enable                  1
#define  BWRM_RAM_CTRL_CODE_disable                 2
#define  BWRM_RAM_CTRL_CODE_reset                   3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_BWRMRAMIF_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_size
    );

typedef  ULONG
(*PFN_BWRMRAMIF_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_timeout
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_CONTROL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ctrl_code
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_GET_PAGE1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        void**                      page_data,
        PULONG                      page_size
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_GET_PAGE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE*                phCookedPage
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_ADD_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE                 hCookedPage
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_DEL_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    );

typedef  BOOL
(*PFN_BWRMRAMIF_IS_PCF)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BWRMRAMIF_FORCE_PC)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        BOOL                        bCacheForced
    );


/*
 * The BWRM framework provides a generic read/write interface for accessing static web pages. This
 * is done by using the undering file system I/O wrappers. In addition to file access, the BWRM_RAM
 * interface provides a cache control for those modules that require significant time to process/
 * parse static web pages.
 */
#define  BWRM_RAM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BWRMRAMIF_GET_SIZE          GetCacheNumber;                                         \
    PFN_BWRMRAMIF_SET_SIZE          SetCacheNumber;                                         \
    PFN_BWRMRAMIF_GET_SIZE          GetCacheSize;                                           \
    PFN_BWRMRAMIF_SET_SIZE          SetCacheSize;                                           \
    PFN_BWRMRAMIF_GET_TIME          GetCacheTimeout;                                        \
    PFN_BWRMRAMIF_SET_TIME          SetCacheTimeout;                                        \
    PFN_BWRMRAMIF_CONTROL           ControlCache;                                           \
                                                                                            \
    PFN_BWRMRAMIF_GET_PAGE1         GetRawPage;                                             \
    PFN_BWRMRAMIF_GET_PAGE2         GetCookedPage;                                          \
    PFN_BWRMRAMIF_ADD_PAGE          AddCookedPage;                                          \
    PFN_BWRMRAMIF_DEL_PAGE          DelCookedPage;                                          \
                                                                                            \
    PFN_BWRMRAMIF_FORCE_PC          ForcePageCache;                                         \
    PFN_BWRMRAMIF_IS_PCF            IsPageCacheForced;                                      \
    /* end of object class content */                                                       \

typedef  struct
_BWRM_RAM_INTERFACE
{
    BWRM_RAM_INTERFACE_CLASS_CONTENT
}
BWRM_RAM_INTERFACE,  *PBWRM_RAM_INTERFACE;

#define  ACCESS_BWRM_RAM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BWRM_RAM_INTERFACE, Linkage)


#endif
