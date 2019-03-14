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

    module:	bree_spo_interface.h

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/


#ifndef  _BREE_SPO_INTERFACE_
#define  _BREE_SPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "beep_ifo_pes.h"
#include "bree_ifo_csp.h"

#include "bspeng_ifo_soa.h"


/***********************************************************
        BWRM COOKED PAGE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

#define  BREE_SPO_COMPONENT_NAME                    "BreeSpoComponent"
#define  BREE_SPO_COMPONENT_OID                     ANSC_OBJECT_OID_NULL
#define  BREE_SPO_COMPONENT_TYPE                    ANSC_OBJECT_TYPE_NULL

#define  BREE_SPO_ENGINE_NAME                       "BREE 2.0"
#define  BREE_SPO_PAGE_EXTENSIONS                   "bsp, cbsp"
#define  BREE_SPO_STATIC_RES_EXTENSIONS             "htm, html, jpg, jpeg, gif, bmp, png, js, java, class, svg, css, xml, cfg, txt"

#define  BREE_SPO_STATIC_ENGINE_NAME                "Static Resources Engine 1.0"

#define  BREE_SPO_BAD_PAGE_PATH_NAME                "//"
#define  BREE_SPO_MAX_PAGE_PATH_SEG                 64

#define  BREE_SPO_RES_FILTER_Off                    0
#define  BREE_SPO_RES_FILTER_ProtectExcept          1
#define  BREE_SPO_RES_FILTER_ProtectOnly            2

#define  BREE_SOA_INTERFACE_NAME                    "breeSlapObjectAccessIf"
#define  BREE_SOA_INTERFACE_ID                      0

typedef  struct
_BREE_SPO_RES_FILTER
{
    SINGLE_LINK_ENTRY               Linkage;        /* used to link into the upper level list */
    BOOL                            bDirectory;     /* directory name specified */
    PUCHAR                          pDFName;        /* directory or file name */
}
BREE_SPO_RES_FILTER, *PBREE_SPO_RES_FILTER;


#define  ACCESS_BREE_SPO_RES_FILTER(p)              \
         ACCESS_CONTAINER(p, BREE_SPO_RES_FILTER, Linkage)


/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BREESPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_BREESPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_CREATE_BEEP_DESC)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_CONFIG_RES_FILTER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ResFilterMode,
        PUCHAR                      pSupervisorPage
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_REG_FILTER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

typedef  PUCHAR
(*PFN_BREESPO_GET_FNAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    );

typedef  BOOL
(*PFN_BREESPO_IN_FILTER_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath,
        BOOL                        bExemptionList
    );

typedef  PUCHAR
(*PFN_BREESPO_PREPARE_DFNAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    );

typedef  ANSC_STATUS
(*PFN_BREESPO_ENFORCE_CACHE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf,
        ULONG                       ulSeconds
    );

typedef  ANSC_HANDLE
(*PFN_BREESPO_CREATE_SOAIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf
    );


/*
 * The BSP Run-time Execution Engine Service Provider object contains a BEEP engine
 * descriptor for BSP scripts. As well it embraces all the information that BSP
 * engine needs to access underlying objects such as BEEP PEC interface etc.
 */
#define  BREE_SPO_CLASS_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBeepEngineDesc;                                        \
    ANSC_HANDLE                     hStaticResEngineDesc;                                   \
    BOOL                            bActive;                                                \
    ANSC_HANDLE                     hBeepSerIf;                                             \
    ANSC_HANDLE                     hBeepCspIf;                                             \
    ANSC_HANDLE                     hBreeSoaIf;                                             \
    ULONG                           ResFilterMode;                                          \
    PUCHAR                          SupervisorPage;                                         \
    SLIST_HEADER                    ResFilterList;                                          \
    SLIST_HEADER                    ResFilterListExemption;                                 \
                                                                                            \
    /* APIs */                                                                              \
    PFN_BREESPO_ENGAGE              Engage;                                                 \
    PFN_BREESPO_CANCEL              Cancel;                                                 \
    PFN_BREESPO_SET_IF              SetBeepSerIf;                                           \
    PFN_BREESPO_SET_IF              SetBeepCspIf;                                           \
    /* for dynamic BSP pages */                                                             \
    PFN_BREESPO_SET_IF              SetBeepEngineDesc;                                      \
    PFN_BREESPO_GET_IF              GetBeepEngineDesc;                                      \
    PFN_BREESPO_CREATE_BEEP_DESC    CreateBeepEngineDesc;                                   \
    PFN_BEEPPESIF_PREPARE           PreparePage;                                            \
    PFN_BEEPPESIF_EXECUTE           ExecutePage;                                            \
    /* for static resources */                                                              \
    PFN_BREESPO_SET_IF              SetStaticResEngineDesc;                                 \
    PFN_BREESPO_GET_IF              GetStaticResEngineDesc;                                 \
    PFN_BREESPO_CREATE_BEEP_DESC    CreateStaticResEngineDesc;                              \
    PFN_BEEPPESIF_PREPARE           PrepareStaticRes;                                       \
    PFN_BEEPPESIF_EXECUTE           ExecuteStatisRes;                                       \
                                                                                            \
    PFN_BEEPPESIF_QUALIFY           QualifyPage;                                            \
                                                                                            \
    PFN_BREESPO_CONFIG_RES_FILTER   ConfigResFilter;                                        \
    PFN_BREESPO_REG_FILTER          RegResFilterDir;                                        \
    PFN_BREESPO_REG_FILTER          RegResFilterDirExemption;                               \
    PFN_BREESPO_REG_FILTER          RegResFilterFile;                                       \
    PFN_BREESPO_REG_FILTER          RegResFilterFileExemption;                              \
    PFN_BREESPO_GET_FNAME           GetFormalPagePath;                                      \
    PFN_BREESPO_IN_FILTER_LIST      IsPagePathInFilterList;                                 \
    PFN_BREESPO_PREPARE_DFNAME      PrepareDFName;                                          \
                                                                                            \
    PFN_BREESPO_ENFORCE_CACHE       EnforceCache;                                           \
                                                                                            \
    PFN_BREESPO_CREATE_SOAIF        CreateBreeSoaIf;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BREE_SPO_OBJECT
{
    BREE_SPO_CLASS_CONTENT
}
BREE_SPO_OBJECT,  *PBREE_SPO_OBJECT;

#define  ACCESS_BREE_SPO_OBJECT(p)          \
         ACCESS_CONTAINER(p, BREE_SPO_OBJECT, Linkage)


#endif
