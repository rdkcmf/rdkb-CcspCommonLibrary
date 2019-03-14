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

    module:	bree_srm_interface.h

        For BREE Site Resource Manager Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        BREE Site Resource Manager Object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        01/17/04    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _BREE_SRM_INTERFACE
#define  _BREE_SRM_INTERFACE


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

#define  BREE_SRMO_COMPONENT_NAME                   "BreeSrmo"
#define  BREE_SRMO_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  BREE_SRMO_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  BREE_SRM_RES_TABLE_SIZE                    32

typedef  struct
_BREE_SRM_RESOURCE
{
    PUCHAR                          pResPath;
    PUCHAR                          pResName;
    PUCHAR                          pResContent;
    PULONG                          pulContentLen;
}
BREE_SRM_RESOURCE, *PBREE_SRM_RESOURCE;

typedef  struct
_BREE_SRM_RES_ITEM
{
    SINGLE_LINK_ENTRY               Linkage;            /* used to link into the upper level list */
    PUCHAR                          pPath;
    PUCHAR                          pName;
    PUCHAR                          pContent;
    ULONG                           ulContentLen;
}
BREE_SRM_RES_ITEM, *PBREE_SRM_RES_ITEM;

#define  ACCESS_BREE_SRM_RES_ITEM_OBJECT(p)              \
         ACCESS_CONTAINER(p, BREE_SRM_RES_ITEM, Linkage)


/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BREESRMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_BREESRMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BREESRMO_REG_COOKED_RES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    );

typedef  ANSC_STATUS
(*PFN_BREESRMO_UNREG_COOKED_RES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    );

typedef  ANSC_HANDLE
(*PFN_BREESRMO_MAP_COOKED_RES)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl
    );

typedef  ANSC_STATUS
(*PFN_BREE_SRMO_LOAD_COOKED_RES)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The BREE Site Resource Manager object is responsible for managing BMW 2.0
 * cooked site resources including BSP pages, image files, HTML files etc.
 * And it also provides services to system to map a relative URL to the
 * corresponding cooked resource.
 */
#define  BREE_SRM_CLASS_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLIST_HEADER                    ResList[BREE_SRM_RES_TABLE_SIZE];                       \
                                                                                            \
    PFN_BREESRMO_REG_COOKED_RES     RegisterCookedResource;                                 \
    PFN_BREESRMO_UNREG_COOKED_RES   UnregisterCookedResource;                               \
    PFN_BREESRMO_MAP_COOKED_RES     MapCookedResource;                                      \
    PFN_BREE_SRMO_LOAD_COOKED_RES   LoadCookedResources;                                    \
    /* end of object class content */                                                       \

typedef  struct
_BREE_SRM_OBJECT
{
    BREE_SRM_CLASS_CONTENT
}
BREE_SRM_OBJECT,  *PBREE_SRM_OBJECT;

#define  ACCESS_BREE_SRM_OBJECT(p)              \
         ACCESS_CONTAINER(p, BREE_SRM_OBJECT, Linkage)


#endif
