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

    module:	ansc_ifo_ftc.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the File Transfer Client interface.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/16/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_IFO_FTC_
#define  _ANSC_IFO_FTC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
        ANSC FILE TRANSFER CLIENT INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  ANSC_FTC_INTERFACE_NAME                    "anscFileTransferClientIf"
#define  ANSC_FTC_INTERFACE_ID                      0

#define  ANSC_FTC_FAIL_REASON_NOT_DEFINED           100
#define  ANSC_FTC_FAIL_REASON_PKT_TIMEOUT           101
#define  ANSC_FTC_FAIL_REASON_PKT_INVALID           102
#define  ANSC_FTC_FAIL_REASON_PKT_OUT_OF_ORDER      103
#define  ANSC_FTC_FAIL_REASON_RESOURCES             104
#define  ANSC_FTC_FAIL_REASON_EXPIRED               105

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_ANSCFTCIF_SUCCEED)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       fileData,
        ULONG                       fileSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ANSCFTCIF_FAIL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulReason,
        char*                       errorMsg,
        ANSC_HANDLE                 hReserved
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  ANSC_FTC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_ANSCFTCIF_SUCCEED           Succeed;                                                \
    PFN_ANSCFTCIF_FAIL              Fail;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_FTC_INTERFACE
{
    ANSC_FTC_INTERFACE_CLASS_CONTENT
}
ANSC_FTC_INTERFACE,  *PANSC_FTC_INTERFACE;

#define  ACCESS_ANSC_FTC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, ANSC_FTC_INTERFACE, Linkage)


#endif
