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

    module:	bree_spo_cooked_page.h

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


#ifndef  _BREE_SPO_COOKED_PAGE_
#define  _BREE_SPO_COOKED_PAGE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwrm_cpo_interface.h"
#include "bwrm_cpo_exported_api.h"


ANSC_HANDLE
BreeCreateCookedPageObj
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
        BREE COOKED PAGE COMPONENT OBJECT DEFINITION
***********************************************************/


/*
 * Definition of a BREE cooked BSP page, it derives from
 * BWRM_COOKED_PAGE_CLASS_CONTENT
 */

#define  BREE_COOKED_PAGE_CLASS_CONTENT                                                     \
    BWRM_COOKED_PAGE_CLASS_CONTENT                                                          \
    /* Archived BSP page's content */                                                       \
    void*                           pArchivedBsp;                                           \
    /* size of archived BSP page content */                                                 \
    ULONG                           ulSize;                                                 \
    /* flag to tell if pArchiveBsp is dynamically generated */                              \
    BOOL                            bDynamic;                                               \

typedef  struct
_BREE_COOKED_BSP_PAGE
{
    BREE_COOKED_PAGE_CLASS_CONTENT
}
BREE_COOKED_BSP_PAGE,  *PBREE_COOKED_BSP_PAGE;


#endif
