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

    module:	bwsp_sco_interface.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Bwsp Site Creator Object.

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


#ifndef  _BWSP_SCO_INTERFACE_
#define  _BWSP_SCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "bwsp_ifo_wsr.h"
#include "bwsp_ifo_csp.h"
#include "bwsp_properties.h"


/***********************************************************
   PLATFORM INDEPENDENT BWSP SITE CREATOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWSPSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWSPSCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_BWSPSCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPSCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The BWSP Site Creator Object serves the role as a central controller for the management web site.
 * It's responsible for setting up the web operation environment and creating the host object and
 * the resource owner objects accordingly. It also serves as a functional container for all the
 * other BWSP objects as well.
 */
#define  BWSP_SITE_CREATOR_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BWSP_SITE_CREATOR_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hWebHemIf;                                              \
    ANSC_HANDLE                     hBwspWsrIf;                                             \
    ANSC_HANDLE                     hBwspCspIf;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BWSPSCO_GET_IF              GetWebHemIf;                                            \
    PFN_BWSPSCO_SET_IF              SetWebHemIf;                                            \
    PFN_BWSPSCO_GET_IF              GetBwspWsrIf;                                           \
    PFN_BWSPSCO_GET_IF              GetBwspCspIf;                                           \
                                                                                            \
    PFN_BWSPSCO_GET_PROPERTY        GetProperty;                                            \
    PFN_BWSPSCO_SET_PROPERTY        SetProperty;                                            \
    PFN_BWSPSCO_RESET               ResetProperty;                                          \
    PFN_BWSPSCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BWSPSCO_ENGAGE              Engage;                                                 \
    PFN_BWSPSCO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BWSPCSPIF_CONFIG            CspConfigProperty;                                      \
                                                                                            \
    PFN_BWSPWSRIF_ATTACH1           WsrAttachWebHost;                                       \
    PFN_BWSPWSRIF_DETACH1           WsrDetachWebHost;                                       \
    PFN_BWSPWSRIF_ATTACH2           WsrAttachServlet;                                       \
    PFN_BWSPWSRIF_DETACH2           WsrDetachServlet;                                       \
    /* end of object class content */                                                       \

typedef  struct
_BWSP_SITE_CREATOR_OBJECT
{
    BWSP_SITE_CREATOR_CLASS_CONTENT
}
BWSP_SITE_CREATOR_OBJECT,  *PBWSP_SITE_CREATOR_OBJECT;

#define  ACCESS_BWSP_SITE_CREATOR_OBJECT(p)         \
         ACCESS_CONTAINER(p, BWSP_SITE_CREATOR_OBJECT, Linkage)


#endif
