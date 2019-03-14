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

    module:	beep_ifo_csp.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Beep Configuration Service Provider (CSP)
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


#ifndef  _BEEP_IFO_CSP_
#define  _BEEP_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_csp.h"


/***********************************************************
  BEEP CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_CSP_INTERFACE_NAME                    "beepConfigurationServiceProviderIf"
#define  BEEP_CSP_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPCSPIF_GET_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       vserver_name,
        USHORT                      vserver_port
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_ADD_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       vserver_name,
        USHORT                      vserver_port,
        char*                       webroot_path,
        char*                       servlet_path,
        char*                       defpage_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_ADD_SERVER2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       vserver_name,
        USHORT                      vserver_port,
        char*                       webroot_path,
        char*                       servlet_path,
        char*                       defpage_path,
        BOOL                        upnp_support
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_DEL_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       vserver_name,
        USHORT                      vserver_port
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BEEPCSPIF_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet,
        char*                       host_name
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSPIF_ADD_BINDING)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMasterServlet,
        char*                       relative_path,
        char*                       mapped_page
    );

/*
 * The BEEP framework is built on top of the baseline ISC web server architecture. While the ISC
 * web server offers great flexibility via the generic Virtual Host Object (VHO) and Resource Owner
 * Object (ROO), which can be derived to encapsulate various type web servers, it requires the web
 * service developers to understand the ANSC/BISGA programming convention and methodology. Moreover,
 * it's not very flexible in supporting the DLL-type extension mechanism, which is supported by all
 * popular desktop operating systems. BEEP was designed to address all these issues.
 */
#define  BEEP_CSP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPCSPIF_GET_SERVER        GetVirtualServer;                                       \
    PFN_BEEPCSPIF_ADD_SERVER        AddVirtualServer;                                       \
    PFN_BEEPCSPIF_ADD_SERVER2       AddVirtualServer2;                                      \
    PFN_BEEPCSPIF_DEL_SERVER        DelVirtualServer;                                       \
    PFN_BEEPCSPIF_DEL_ALL           DelAllVirtualServers;                                   \
                                                                                            \
    PFN_BEEPCSPIF_GET_NAME          GetHostName;                                            \
    PFN_BEEPCSPIF_SET_NAME          SetHostName;                                            \
    PFN_BEEPCSPIF_GET_NAME          GetWebRootPath;                                         \
    PFN_BEEPCSPIF_SET_NAME          SetWebRootPath;                                         \
    PFN_BEEPCSPIF_GET_NAME          GetServletPath;                                         \
    PFN_BEEPCSPIF_SET_NAME          SetServletPath;                                         \
    PFN_BEEPCSPIF_GET_NAME          GetDefPagePath;                                         \
    PFN_BEEPCSPIF_SET_NAME          SetDefPagePath;                                         \
    PFN_BEEPCSPIF_ADD_BINDING       AddPageBinding;                                         \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_CSP_INTERFACE
{
    BEEP_CSP_INTERFACE_CLASS_CONTENT
}
BEEP_CSP_INTERFACE,  *PBEEP_CSP_INTERFACE;

#define  ACCESS_BEEP_CSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_CSP_INTERFACE, Linkage)


#endif
