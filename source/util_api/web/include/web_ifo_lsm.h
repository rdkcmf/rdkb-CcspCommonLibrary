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

    module:	web_ifo_lsm.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Web Logical Session Manager (LSM) Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/18/02    initial revision.

**********************************************************************/


#ifndef  _WEB_IFO_LSM_
#define  _WEB_IFO_LSM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
   WEB LOGICAL SESSION MANAGER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  WEB_LSM_INTERFACE_NAME                     "webLogicalSessionManagerIf"
#define  WEB_LSM_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_WEBLSMIF_NEW)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_WEBLSMIF_CLASSIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_WEBLSMIF_EXPIRE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_WEBLSMIF_END)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

/*
 * Currently, HTTP servers respond to each client request without relating that request to previous
 * or subsequent requests; the technique allows clients and servers that wish to exchange state
 * information to place HTTP requests and responses within a larger context, which we term a
 * "session". This context might be used to create, for example, a "shopping cart", in which user
 * selections can be aggregated before purchase, or a magazine browsing system, in which a user's
 * previous reading effects which offerings are presented.
 */
#define  WEB_LSM_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_WEBLSMIF_NEW                NewContact;                                             \
    PFN_WEBLSMIF_CLASSIFY           ClassifyClient;                                         \
    PFN_WEBLSMIF_EXPIRE             ExpireSession;                                          \
    PFN_WEBLSMIF_END                EndSession;                                             \
    /* end of object class content */                                                       \

typedef  struct
_WEB_LSM_INTERFACE
{
    WEB_LSM_INTERFACE_CLASS_CONTENT
}
WEB_LSM_INTERFACE,  *PWEB_LSM_INTERFACE;

#define  ACCESS_WEB_LSM_INTERFACE(p)                \
         ACCESS_CONTAINER(p, WEB_LSM_INTERFACE, Linkage)


#endif
