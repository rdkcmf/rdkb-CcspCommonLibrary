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

    module:	beep_ifo_rmg.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BEEP Response Message Generation Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/16/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_IFO_RMG_
#define  _BEEP_IFO_RMG_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    BEEP RESPONSE MESSAGE GENERATION INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_RMG_INTERFACE_NAME                    "beepResponseMessageGenerationIf"
#define  BEEP_RMG_INTERFACE_ID                      0

#define  BEEP_RMG_HEADER_OPTION_statusLine          0x00000001
#define  BEEP_RMG_HEADER_OPTION_server              0x00000002
#define  BEEP_RMG_HEADER_OPTION_host                0x00000004
#define  BEEP_RMG_HEADER_OPTION_content             0x00000008

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BEEPRMGIF_REPLY_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep,
        ULONG                       ulStatusCode,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_BEEPRMGIF_REPLY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_BEEPRMGIF_ADD_HEADERS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep,
        ULONG                       ulOptions
    );

/*
 * Instead of hardcode the list of dynamic Page Execution Engines, BEEP provides the RMG interface
 * to allow service modules to dynamically register external execution engines. These registered
 * engines will be instantiated internally as the Beep Service Bean Objects. Instead of requiring
 * all engines must implement their own reply handling routines, the master servlet provides a set
 * of APIs for engines to use to generate reply messages.
 */
#define  BEEP_RMG_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPRMGIF_REPLY_REQ         ReplyReq;                                               \
    PFN_BEEPRMGIF_REPLY             Reply300;                                               \
    PFN_BEEPRMGIF_REPLY             Reply301;                                               \
    PFN_BEEPRMGIF_REPLY             Reply302;                                               \
    PFN_BEEPRMGIF_REPLY             Reply303;                                               \
    PFN_BEEPRMGIF_REPLY             Reply304;                                               \
    PFN_BEEPRMGIF_REPLY             Reply305;                                               \
    PFN_BEEPRMGIF_REPLY             Reply307;                                               \
    PFN_BEEPRMGIF_REPLY             Reply400;                                               \
    PFN_BEEPRMGIF_REPLY             Reply403;                                               \
    PFN_BEEPRMGIF_REPLY             Reply404;                                               \
    PFN_BEEPRMGIF_REPLY             Reply405;                                               \
    PFN_BEEPRMGIF_REPLY             Reply409;                                               \
    PFN_BEEPRMGIF_REPLY             Reply410;                                               \
    PFN_BEEPRMGIF_REPLY             Reply415;                                               \
    PFN_BEEPRMGIF_REPLY             Reply417;                                               \
    PFN_BEEPRMGIF_REPLY             Reply500;                                               \
    PFN_BEEPRMGIF_REPLY             Reply501;                                               \
    PFN_BEEPRMGIF_REPLY             Reply503;                                               \
    PFN_BEEPRMGIF_REPLY             Reply505;                                               \
    PFN_BEEPRMGIF_ADD_HEADERS       AddDefRepHeaders;                                       \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_RMG_INTERFACE
{
    BEEP_RMG_INTERFACE_CLASS_CONTENT
}
BEEP_RMG_INTERFACE,  *PBEEP_RMG_INTERFACE;

#define  ACCESS_BEEP_RMG_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_RMG_INTERFACE, Linkage)


#endif
