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

    module:	ansc_ip4fo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Ipv4 Fragmenter Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/31/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_IP4FO_INTERFACE_
#define  _ANSC_IP4FO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_sco_interface.h"
#include "ansc_sco_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT IPV4 FRAGMENTER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_IP4FO_DEFAULT_MPS                     1500

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IP4FO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IP4FO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_IP4FO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_IP4FO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_IP4FO_FRAGMENT)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        PSLIST_HEADER               pDescriptors
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_IP4_FRAGMENTER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSC_SIMPLEX_CONNECTOR_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ULONG                           MaxPayloadSize;                                         \
                                                                                            \
    PFN_IP4FO_GET_SIZE              GetMaxPayloadSize;                                      \
    PFN_IP4FO_SET_SIZE              SetMaxPayloadSize;                                      \
    PFN_IP4FO_FRAGMENT              Fragment;                                               \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IP4_FRAGMENTER_OBJECT
{
    ANSC_IP4_FRAGMENTER_CLASS_CONTENT
}
ANSC_IP4_FRAGMENTER_OBJECT,  *PANSC_IP4_FRAGMENTER_OBJECT;

#define  ACCESS_ANSC_IP4_FRAGMENTER_OBJECT(p)       \
         ACCESS_CONTAINER(p, ANSC_IP4_FRAGMENTER_OBJECT, Linkage)


#endif
