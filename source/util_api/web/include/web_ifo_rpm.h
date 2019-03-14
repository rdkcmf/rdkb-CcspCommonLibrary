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

    module:	web_ifo_rpm.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Web Resource Path Mapping (RPM) Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/05    initial revision.

**********************************************************************/


#ifndef  _WEB_IFO_RPM_
#define  _WEB_IFO_RPM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    WEB RESOURCE PATH MAPPING INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  WEB_RPM_INTERFACE_NAME                     "webResourcePathMappingIf"
#define  WEB_RPM_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  char*
(*PFN_WEBRPMIF_GET_MAPPING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pReqUrlPath
    );

typedef  ANSC_STATUS
(*PFN_WEBRPMIF_ADD_MAPPING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pTargetPage,
        char*                       pReqUrlPath
    );

typedef  ANSC_STATUS
(*PFN_WEBRPMIF_DEL_MAPPING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pReqUrlPath
    );

typedef  ANSC_STATUS
(*PFN_WEBRPMIF_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * This interface allows a web resource to be mapped to one or more different URL paths. Before
 * locating the target ResourceOwner object, the VirtualHost will map the request URL contained in
 * the HTTP request message to its original web resource location. For example, the UPnP module
 * utilizes the same BSP for all service controls, and each service has its own contrl URL.
 */
#define  WEB_RPM_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_WEBRPMIF_GET_MAPPING        GetMappedPage;                                          \
    PFN_WEBRPMIF_ADD_MAPPING        AddMappedPath;                                          \
    PFN_WEBRPMIF_DEL_MAPPING        DelMappedPath;                                          \
    PFN_WEBRPMIF_DEL_ALL            DelAllMappedPaths;                                      \
    /* end of object class content */                                                       \

typedef  struct
_WEB_RPM_INTERFACE
{
    WEB_RPM_INTERFACE_CLASS_CONTENT
}
WEB_RPM_INTERFACE,  *PWEB_RPM_INTERFACE;

#define  ACCESS_WEB_RPM_INTERFACE(p)                \
         ACCESS_CONTAINER(p, WEB_RPM_INTERFACE, Linkage)


#endif
