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

    module:	web_sso_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Simple Server Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#ifndef  _WEB_SSO_INTERFACE_
#define  _WEB_SSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "web_properties.h"

#include "web_ifo_csp.h"


/***********************************************************
  PLATFORM INDEPENDENT WEB SIMPLE SERVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBSSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBSSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBSSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  WEB_SIMPLE_SERVER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    WEB_SIMPLE_SERVER_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hHttpSsoArray[WEB_MAX_DAEMON_NUMBER];                   \
    ANSC_HANDLE                     hWebSiteManager;                                        \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hCspIf;                                                 \
    ANSC_HANDLE                     hFumIf;                                                 \
    ULONG                           ServerMode;                                             \
    BOOL                            bActive;                                                \
    SLIST_HEADER                    MdhSList;                                               \
    ANSC_LOCK                       MdhSListLock;                                           \
                                                                                            \
    PFN_WEBSSO_GET_CONTEXT          GetSiteManager;                                         \
    PFN_WEBSSO_GET_IF               GetHfpIf;                                               \
    PFN_WEBSSO_SET_IF               SetHfpIf;                                               \
    PFN_WEBSSO_GET_IF               GetFumIf;                                               \
    PFN_WEBSSO_GET_IF               GetCspIf;                                               \
    PFN_WEBSSO_GET_MODE             GetServerMode;                                          \
    PFN_WEBSSO_SET_MODE             SetServerMode;                                          \
    PFN_WEBSSO_GET_PROPERTY         GetProperty;                                            \
    PFN_WEBSSO_SET_PROPERTY         SetProperty;                                            \
    PFN_WEBSSO_RESET                ResetProperty;                                          \
    PFN_WEBSSO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBSSO_ENGAGE               Engage;                                                 \
    PFN_WEBSSO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_ANSCCSPIF_GET_MODE          CspGetCfgMode;                                          \
    PFN_ANSCCSPIF_IS_SOMETHING      CspIsReadOnly;                                          \
    PFN_ANSCCSPIF_IS_SOMETHING      CspIsSwitchOn;                                          \
    PFN_ANSCCSPIF_ENGAGE            CspEngage;                                              \
    PFN_ANSCCSPIF_CANCEL            CspCancel;                                              \
                                                                                            \
    PFN_WEBCSPIF_ADD_DAEMON         CspAddHttpDaemon;                                       \
    PFN_WEBCSPIF_DEL_DAEMON         CspDelHttpDaemon;                                       \
    PFN_WEBCSPIF_SET_MODE           CspSetHttpServerMode;                                   \
    PFN_WEBCSPIF_SET_BOOL           CspSetPersistentConn;                                   \
    PFN_WEBCSPIF_SET_NAME           CspSetProductName;                                      \
    /* end of object class content */                                                       \

typedef  struct
_WEB_SIMPLE_SERVER_OBJECT
{
    WEB_SIMPLE_SERVER_CLASS_CONTENT
}
WEB_SIMPLE_SERVER_OBJECT,  *PWEB_SIMPLE_SERVER_OBJECT;

#define  ACCESS_WEB_SIMPLE_SERVER_OBJECT(p)         \
         ACCESS_CONTAINER(p, WEB_SIMPLE_SERVER_OBJECT, Linkage)


#endif
