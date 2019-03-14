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

    module:	ansc_ifo_imc.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Inter-Module Communication Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/22/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_IFO_IMC_
#define  _ANSC_IFO_IMC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    ANSC INTER-MODULE COMMUNICATION INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  ANSC_IMC_INTERFACE_NAME                    "anscInterModuleCommunicationIf"
#define  ANSC_IMC_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCIMCIF_GEN_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_ANSCIMCIF_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  char*
(*PFN_ANSCIMCIF_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  BOOL
(*PFN_ANSCIMCIF_IS_CONNECTED)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        BOOL                        bOutgoingConn
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_CALL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOputputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    );

typedef  ANSC_STATUS
(*PFN_ANSCIMCIF_CALL2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOputputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  ANSC_IMC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_ANSCIMCIF_GEN_ADDR          GenPartyAddr;                                           \
    PFN_ANSCIMCIF_GET_MODE          GetLpcOpmode;                                           \
    PFN_ANSCIMCIF_SET_MODE          SetLpcOpmode;                                           \
    PFN_ANSCIMCIF_GET_NAME          GetPartyName;                                           \
    PFN_ANSCIMCIF_SET_NAME          SetPartyName;                                           \
    PFN_ANSCIMCIF_IS_CONNECTED      IsPartyConnected;                                       \
                                                                                            \
    PFN_ANSCIMCIF_ENGAGE            Engage;                                                 \
    PFN_ANSCIMCIF_CANCEL            Cancel;                                                 \
    PFN_ANSCIMCIF_CALL              CallRequest;                                            \
    PFN_ANSCIMCIF_CALL2             CallRequest2;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IMC_INTERFACE
{
    ANSC_IMC_INTERFACE_CLASS_CONTENT
}
ANSC_IMC_INTERFACE,  *PANSC_IMC_INTERFACE;

#define  ACCESS_ANSC_IMC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, ANSC_IMC_INTERFACE, Linkage)


#endif
