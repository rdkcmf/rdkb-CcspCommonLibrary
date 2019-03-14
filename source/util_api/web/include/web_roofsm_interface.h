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

    module:	web_roofsm_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Roo Fsm Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#ifndef  _WEB_ROOFSM_INTERFACE_
#define  _WEB_ROOFSM_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "web_roo_interface.h"
#include "web_roo_exported_api.h"


/***********************************************************
     PLATFORM INDEPENDENT WEB ROO FSM OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBROOFSM_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBROOFSM_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBROOFSM_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  char*
(*PFN_WEBROOFSM_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_WEBROOFSM_MAP_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_STATUS
(*PFN_WEBROOFSM_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFile,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  WEB_ROO_FSM_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    WEB_RESOURCE_OWNER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    PFN_WEBROOFSM_MAP_FILE          MapDiskFile;                                            \
    PFN_WEBROOFSM_GET_TYPE          GetMimeType;                                            \
    /* end of object class content */                                                       \

typedef  struct
_WEB_ROO_FSM_OBJECT
{
    WEB_ROO_FSM_CLASS_CONTENT
}
WEB_ROO_FSM_OBJECT,  *PWEB_ROO_FSM_OBJECT;

#define  ACCESS_WEB_ROO_FSM_OBJECT(p)               \
         ACCESS_CONTAINER(p, WEB_ROO_FSM_OBJECT, Linkage)


#endif
