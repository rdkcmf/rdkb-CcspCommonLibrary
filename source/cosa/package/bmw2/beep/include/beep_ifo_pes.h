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

    module:	beep_ifo_pes.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BEEP Page Execution Service Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_IFO_PES_
#define  _BEEP_IFO_PES_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
       BEEP PAGE EXECUTION SERVICE INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_PES_INTERFACE_NAME                    "beepPageExecutionServiceIf"
#define  BEEP_PES_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPPESIF_PREPARE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    );

typedef  ANSC_STATUS
(*PFN_BEEPPESIF_EXECUTE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    );

typedef  BOOL
(*PFN_BEEPPESIF_QUALIFY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        ULONG                       page_type
    );

typedef  BOOL
(*PFN_BEEPPESIF_CONVERT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       org_page_path,
        char**                      new_page_path
    );

/*
 * BEEP is the realization of a web-based execution environment, which is built on top of the SLAP
 * layer (Aggregation Plan). BEEP provides an extensible framework that allows new Sever-side Page
 * Generation technologies to be added very easily. For such an Page-Generation Technology, a
 * BEEP_PES interface MUST be implemented and registered to the BEEP module. The registration
 * process establishes the assocation between the Web Resource Identification (e.g., web file
 * extension: '.bsp', '.cgi', ..., etc). BREE (BSP Run-time Execution Engine) is one example of
 * such Page-Generation Technology.
 */
#define  BEEP_PES_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPPESIF_PREPARE           PreparePage;                                            \
    PFN_BEEPPESIF_EXECUTE           ExecutePage;                                            \
    PFN_BEEPPESIF_QUALIFY           QualifyPage;                                            \
    PFN_BEEPPESIF_CONVERT           ConvertPath;                                            \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_PES_INTERFACE
{
    BEEP_PES_INTERFACE_CLASS_CONTENT
}
BEEP_PES_INTERFACE,  *PBEEP_PES_INTERFACE;

#define  ACCESS_BEEP_PES_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_PES_INTERFACE, Linkage)


#endif
