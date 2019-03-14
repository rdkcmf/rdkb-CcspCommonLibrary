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

    module:	bwsp_ifo_wsr.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwsp Web Service Registrar (WSR) Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_IFO_WSR_
#define  _BWSP_IFO_WSR_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
   BWSP WEB SERVICE REGISTRAR INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BWSP_WSR_INTERFACE_NAME                    "bwspWebServiceRegistrarIf"
#define  BWSP_WSR_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BWSPWSRIF_ATTACH1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostDescr
    );

typedef  ANSC_STATUS
(*PFN_BWSPWSRIF_DETACH1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name
    );

typedef  ANSC_STATUS
(*PFN_BWSPWSRIF_ATTACH2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        ANSC_HANDLE                 hServletDescr
    );

typedef  ANSC_STATUS
(*PFN_BWSPWSRIF_DETACH2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        char*                       path_name
    );

/*
 * The BWSP framework is built on top of the baseline ISC web server architecture. While the ISC
 * web server offers great flexibility via the generic Virtual Host Object (VHO) and Resource Owner
 * Object (ROO), which can be derived to encapsulate various type web servers, it requires the web
 * service developers to understand the ANSC/BISGA programming convention and methodology. Moreover,
 * it's not very flexible in supporting the DLL-type extension mechanism, which is supported by all
 * popular desktop operating systems. BWSP was designed to address all these issues.
 */
#define  BWSP_WSR_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BWSPWSRIF_ATTACH1           AttachWebHost;                                          \
    PFN_BWSPWSRIF_DETACH1           DetachWebHost;                                          \
    PFN_BWSPWSRIF_ATTACH2           AttachServlet;                                          \
    PFN_BWSPWSRIF_DETACH2           DetachServlet;                                          \
    /* end of object class content */                                                       \

typedef  struct
_BWSP_WSR_INTERFACE
{
    BWSP_WSR_INTERFACE_CLASS_CONTENT
}
BWSP_WSR_INTERFACE,  *PBWSP_WSR_INTERFACE;

#define  ACCESS_BWSP_WSR_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BWSP_WSR_INTERFACE, Linkage)


#endif
