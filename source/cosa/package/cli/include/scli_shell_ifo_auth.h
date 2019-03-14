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

    module:	scli_shell_ifo_auth.h

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the SCLI Shell Command Execution.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/05/05    initial revision.

**********************************************************************/


#ifndef  _SCLI_SHELL_IFO_AUTH_
#define  _SCLI_SHELL_IFO_AUTH_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
     SCLI COMMAND AUTHENTICATION INTERFACE
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SCLIAUTHIF_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bVerifyUser,
        PUCHAR                      pUserName,
        PUCHAR                      pPassword,
        PULONG                      pulPermission,
        PBOOL                       pbPrivileged,
        PULONG                      pulTermPermission,        
        PULONG                      pulIdleTimeout,
        PULONG                      pulLockoutTimeout
    );

typedef  ANSC_STATUS
(*PFN_SCLIAUTHIF_GET_TIMEOUT)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulIdleTimeout,
        PULONG                      pulLockoutTimeout
    );

/*
 * SCLI Shell AUTH interface provides routines to do user authentication
 */
#define  SCLI_SHELL_AUTH_INTERFACE_CLASS_CONTENT                                            \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SCLIAUTHIF_AUTH             DoAuth;                                                 \
    PFN_SCLIAUTHIF_GET_TIMEOUT      GetTimeouts;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SCLI_SHELL_AUTH_INTERFACE
{
    SCLI_SHELL_AUTH_INTERFACE_CLASS_CONTENT
}
SCLI_SHELL_AUTH_INTERFACE,  *PSCLI_SHELL_AUTH_INTERFACE;

#define  ACCESS_SCLI_SHELL_AUTH_INTERFACE(p)                   \
         ACCESS_CONTAINER(p, SCLI_SHELL_AUTH_INTERFACE, Linkage)


#endif
