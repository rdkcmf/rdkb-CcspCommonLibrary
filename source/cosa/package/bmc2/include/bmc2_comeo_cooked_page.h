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

    module:	bmc2_comeo_cooked_page.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Com Executor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMEO_COOKED_PAGE_
#define  _BMC2_COMEO_COOKED_PAGE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwrm_cpo_interface.h"
#include "bwrm_cpo_exported_api.h"


/***********************************************************
    BMC2 COMEO COOKED PAGE COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Definition of BMC2 cooked BSP page, it derives from
 * BWRM_COOKED_PAGE_CLASS_CONTENT
 */

#define  BMC2_COOKED_PAGE_CLASS_CONTENT                                                     \
    BWRM_COOKED_PAGE_CLASS_CONTENT                                                          \
    /* Archived BSP page's content */                                                       \
    void*                           pArchivedBsp;                                           \
    /* size of archived BSP page content */                                                 \
    ULONG                           ulSize;                                                 \
    /* flag to tell if pArchiveBsp is dynamically generated */                              \
    BOOL                            bDynamic;                                               \

typedef  struct
_BMC2_COOKED_BSP_PAGE
{
    BMC2_COOKED_PAGE_CLASS_CONTENT
}
BMC2_COOKED_BSP_PAGE,  *PBMC2_COOKED_BSP_PAGE;


#endif
