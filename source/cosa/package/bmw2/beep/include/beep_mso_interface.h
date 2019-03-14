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

    module:	beep_mso_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Master Servlet Object.

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


#ifndef  _BEEP_MSO_INTERFACE_
#define  _BEEP_MSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "beep_properties.h"
#include "beep_ifo_ser.h"
#include "beep_ifo_rmg.h"
#include "bwsp_ifo_wsh.h"


/***********************************************************
       BEEP MASTER SERVLET COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPMSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPMSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BEEPMSO_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path_name
    );

typedef  BOOL
(*PFN_BEEPMSO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Beep Master Servlet Objet is a derived instance of Bwsp Web Servlet Object, though this derived
 * relationship is not very clear duo to the fact that the BWSP_WSR interface separates the
 * internal Web Servlet implementation from the external modules. Note that unlike the BWSP module
 * and the underlying Web Server module, BEEP has intentionally REMOVED the hierarchical structure
 * for request handling. Beep Master Servlet literally presents a a file-based web resource owner
 * interface to the web server.
 */
#define  BEEP_MASTER_SERVLET_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BEEP_MASTER_SERVLET_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hBeepSerIf;                                             \
    ANSC_HANDLE                     hBeepRmgIf;                                             \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hBeepSrvController;                                     \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BEEPMSO_GET_IF              GetBeepSerIf;                                           \
    PFN_BEEPMSO_GET_IF              GetBeepRmgIf;                                           \
    PFN_BEEPMSO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPMSO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPMSO_GET_CONTEXT         GetBeepSrvController;                                   \
                                                                                            \
    PFN_BEEPMSO_GET_PATH            GetWebRootPath;                                         \
    PFN_BEEPMSO_SET_PATH            SetWebRootPath;                                         \
    PFN_BEEPMSO_GET_PATH            GetServletPath;                                         \
    PFN_BEEPMSO_SET_PATH            SetServletPath;                                         \
    PFN_BEEPMSO_GET_PATH            GetDefPagePath;                                         \
    PFN_BEEPMSO_SET_PATH            SetDefPagePath;                                         \
    PFN_BEEPMSO_GET_BOOL            GetUpnpSupport;                                         \
    PFN_BEEPMSO_SET_BOOL            SetUpnpSupport;                                         \
                                                                                            \
    PFN_BEEPMSO_GET_PROPERTY        GetProperty;                                            \
    PFN_BEEPMSO_SET_PROPERTY        SetProperty;                                            \
    PFN_BEEPMSO_RESET               ResetProperty;                                          \
    PFN_BEEPMSO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPMSO_ENGAGE              Engage;                                                 \
    PFN_BEEPMSO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BWSPWSHIF_DO_OPTIONS        WshDoOptions;                                           \
    PFN_BWSPWSHIF_DO_GET            WshDoGet;                                               \
    PFN_BWSPWSHIF_DO_HEAD           WshDoHead;                                              \
    PFN_BWSPWSHIF_DO_POST           WshDoPost;                                              \
    PFN_BWSPWSHIF_DO_PUT            WshDoPut;                                               \
    PFN_BWSPWSHIF_DO_DELETE         WshDoDelete;                                            \
    PFN_BWSPWSHIF_DO_TRACE          WshDoTrace;                                             \
    PFN_BWSPWSHIF_DO_CONNECT        WshDoConnect;                                           \
                                                                                            \
    PFN_BWSPWSHIF_DO_NOTIFY         WshDoNotify;                                            \
    PFN_BWSPWSHIF_DO_SEARCH         WshDoSearch;                                            \
    PFN_BWSPWSHIF_DO_M_SEARCH       WshDoMSearch;                                           \
    PFN_BWSPWSHIF_DO_M_POST         WshDoMPost;                                             \
    PFN_BWSPWSHIF_DO_SUBSCRIBE      WshDoSubscribe;                                         \
    PFN_BWSPWSHIF_DO_UNSUBSCRIBE    WshDoUnsubscribe;                                       \
                                                                                            \
    PFN_BEEPRMGIF_REPLY_REQ         RmgReplyReq;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply300;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply301;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply302;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply303;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply304;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply305;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply307;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply400;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply403;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply404;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply405;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply409;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply410;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply415;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply417;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply500;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply501;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply503;                                            \
    PFN_BEEPRMGIF_REPLY             RmgReply505;                                            \
    PFN_BEEPRMGIF_ADD_HEADERS       RmgAddDefRepHeaders;                                    \
                                                                                            \
    PFN_BEEPSERIF_ATTACH            SerAttachEngine;                                        \
    PFN_BEEPSERIF_DETACH            SerDetachEngine;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_MASTER_SERVLET_OBJECT
{
    BEEP_MASTER_SERVLET_CLASS_CONTENT
}
BEEP_MASTER_SERVLET_OBJECT,  *PBEEP_MASTER_SERVLET_OBJECT;

#define  ACCESS_BEEP_MASTER_SERVLET_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_MASTER_SERVLET_OBJECT, Linkage)


#endif
