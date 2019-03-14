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

    module:	bspeng_co_cmif.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        string implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        05/13/03    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _BSPENG_CACHE_MGR_DEC
#define  _BSPENG_CACHE_MGR_DEC

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/*
 * We define the cache entry structure so that any
 * cache interface implementation must use this
 * structure. No exceptions! Because this template
 * engine doesn't have any ideas of your scenarios.
 * For the first element, you can use it to chain
 * the entry into single list. Of course, if you
 * don't want to use it, no problems.
 */
typedef  struct
_BSP_TEMPLATE_CACHE_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;        /* used to link into the upper level list */
    PUCHAR                          pName;          /* Template name */
    ULONG                           ulCount;        /* Template count */
    ANSC_HANDLE                     hTemplates;     /* Template(s) */
}
BSP_TEMPLATE_CACHE_ENTRY, *PBSP_TEMPLATE_CACHE_ENTRY;

#define  ACCESS_BSP_TEMPLATE_CACHE_ENTRY_OBJECT(p)          \
         ACCESS_CONTAINER(p, BSP_TEMPLATE_CACHE_ENTRY, Linkage)


typedef
ANSC_HANDLE
(*PFN_BSPTMPL_CMIF_GET_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTemplateName
    );

typedef
ANSC_STATUS
(*PFN_BSPTMPL_CMIF_ADD_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCacheEntry
    );

typedef
ANSC_STATUS
(*PFN_BSPTMPL_CMIF_EMPTY)
    (
        ANSC_HANDLE                 hThisObject
    );


#define  BSP_TEMPLATE_CACHE_MGR_INTERFACE_CONTENT                                           \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    PFN_BSPTMPL_CMIF_GET_TEMPLATE   GetTemplate;                                            \
    PFN_BSPTMPL_CMIF_ADD_TEMPLATE   AddTemplate;                                            \
    PFN_BSPTMPL_CMIF_EMPTY          Empty;                                                  \
    /* end of object class content */
    

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_CACHE_MGR_INTERFACE
{
    BSP_TEMPLATE_CACHE_MGR_INTERFACE_CONTENT
}
BSP_TEMPLATE_CACHE_MGR_INTERFACE, *PBSP_TEMPLATE_CACHE_MGR_INTERFACE;


#define  ACCESS_BSP_TEMPLATE_CACHE_MGR_INTERFACE(p)            \
         ACCESS_CONTAINER(p, BSP_TEMPLATE_CACHE_MGR_INTERFACE, Linkage)

#endif


