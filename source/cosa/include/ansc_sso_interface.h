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

    module:	ansc_sso_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Syslog Sender Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        11/20/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SSO_INTERFACE_
#define  _ANSC_SSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "ansc_ifo_smt.h"
#include "ansc_scuowo_interface.h"


/***********************************************************
    PLATFORM INDEPENDENT SYSLOG SENDER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SSO_MAX_MESSAGE_SIZE                  1024

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  PUCHAR
(*PFN_SSO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_SSO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_SSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * In its most simplistic terms, the syslog protocol provides a transport to allow a machine to
 * send event notification messages across IP networks to event message collectors - also known as
 * syslog servers. Since each process, application and operating system was written somewhat
 * independently, there's little uniformity to the content of syslog messages.
 */
#define  ANSC_SYSLOG_SENDER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSimpleClientUdp;                                       \
    ANSC_HANDLE                     hClientUdpWorker;                                       \
    ANSC_HANDLE                     hAnscSmtIf;                                             \
    BOOL                            bLoggingEnabled;                                        \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_SSO_GET_ADDR                GetClientAddress;                                       \
    PFN_SSO_SET_ADDR                SetClientAddress;                                       \
    PFN_SSO_GET_PORT                GetClientPort;                                          \
    PFN_SSO_SET_PORT                SetClientPort;                                          \
    PFN_SSO_GET_ADDR                GetServerAddress;                                       \
    PFN_SSO_SET_ADDR                SetServerAddress;                                       \
    PFN_SSO_GET_PORT                GetServerPort;                                          \
    PFN_SSO_SET_PORT                SetServerPort;                                          \
                                                                                            \
    PFN_SSO_GET_CONTEXT             GetSimpleClientUdp;                                     \
    PFN_SSO_GET_CONTEXT             GetClientUdpWorker;                                     \
    PFN_SSO_GET_IF                  GetAnscSmtIf;                                           \
                                                                                            \
    PFN_SSO_ENGAGE                  Engage;                                                 \
    PFN_SSO_CANCEL                  Cancel;                                                 \
                                                                                            \
    PFN_ANSCSMTIF_ISSUE             SmtIssueMessage;                                        \
    PFN_ANSCSMTIF_ISSUE2            SmtIssueMessage2;                                       \
    PFN_ANSCSMTIF_ENABLE            SmtEnableLogging;                                       \
                                                                                            \
    PFN_SCUOWO_QUERY                ScuowoQuery;                                            \
    PFN_SCUOWO_PROCESS              ScuowoProcess;                                          \
    PFN_SCUOWO_NOTIFY               ScuowoNotify;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SYSLOG_SENDER_OBJECT
{
    ANSC_SYSLOG_SENDER_CLASS_CONTENT
}
ANSC_SYSLOG_SENDER_OBJECT,  *PANSC_SYSLOG_SENDER_OBJECT;

#define  ACCESS_ANSC_SYSLOG_SENDER_OBJECT(p)        \
         ACCESS_CONTAINER(p, ANSC_SYSLOG_SENDER_OBJECT, Linkage)


#endif
