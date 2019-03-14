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

    module:	beep_ifo_pem.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BEEP Plane Element Manager Objects.

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


#ifndef  _BEEP_IFO_PEM_
#define  _BEEP_IFO_PEM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
       BEEP PLANE ELEMENT MANAGER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_PEM_INTERFACE_NAME                    "beepPlaneElementManagerIf"
#define  BEEP_PEM_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPPEMIF_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPEMIF_GET_IF2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPEMIF_GET_IF3)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet,
        char*                       page_path
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPEMIF_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPEMIF_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPEMIF_GET_CONTEXT2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet,
        char*                       page_path
    );

/*
 * The BEEP_PEM interface serves as a backbone communication channel for all BEEP module objects.
 * This interface centralizes the resource management and resolves external dependencies by
 * providing consistent access to various external and internal resources. It simplies the process
 * of resolving interface dependencies and resource access.
 */
#define  BEEP_PEM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPPEMIF_GET_IF            GetBeepCspIf;                                           \
    PFN_BEEPPEMIF_GET_IF2           GetBeepRmgIf;                                           \
    PFN_BEEPPEMIF_GET_IF            GetWebHemIf;                                            \
    PFN_BEEPPEMIF_GET_IF            GetBwspWsrIf;                                           \
    PFN_BEEPPEMIF_GET_IF            GetBwrmRamIf;                                           \
    PFN_BEEPPEMIF_GET_IF            GetSlapGoaIf;                                           \
    PFN_BEEPPEMIF_GET_IF            GetSlapUoaIf;                                           \
    PFN_BEEPPEMIF_GET_IF3           GetBeepPesIf;                                           \
                                                                                            \
    PFN_BEEPPEMIF_GET_CONTEXT       GetSlapContainerApp;                                    \
    PFN_BEEPPEMIF_GET_CONTEXT       GetSlapContainerServices;                               \
    PFN_BEEPPEMIF_SET_CONTEXT       SetSlapContainerServices;                               \
    PFN_BEEPPEMIF_GET_CONTEXT       GetSlapBeepApp;                                         \
    PFN_BEEPPEMIF_GET_CONTEXT       GetSlapBeepServer;                                      \
    PFN_BEEPPEMIF_GET_CONTEXT2      GetBeepServiceBean;                                     \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_PEM_INTERFACE
{
    BEEP_PEM_INTERFACE_CLASS_CONTENT
}
BEEP_PEM_INTERFACE,  *PBEEP_PEM_INTERFACE;

#define  ACCESS_BEEP_PEM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_PEM_INTERFACE, Linkage)


#endif
