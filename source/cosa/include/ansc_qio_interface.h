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

    module:	ansc_qio_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Query Interface Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/11/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_QIO_INTERFACE_
#define  _ANSC_QIO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT QUERY INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_QIO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_QIO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_QIO_GET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_QIO_SET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_QIO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_QIO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_QIO_ADD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_QIO_DEL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

typedef  ANSC_HANDLE
(*PFN_QIO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

/*
 * As a standard practice, each system module MUST create a Query Interface Object to provide a
 * standard interface for external compents to access its interfaces via name-handler mapping. This
 * object is usually encapsulated by an Ansc Module Container Object, whose runtime context will
 * be written into a known position in the System Information Repository.
 */
#define  ANSC_QUERY_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAtoIf;                                                 \
                                                                                            \
    PFN_QIO_ADD                     AddIf;                                                  \
    PFN_QIO_DEL                     DelIf;                                                  \
    PFN_QIO_QUERY                   QueryIf;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_QUERY_INTERFACE_OBJECT
{
    ANSC_QUERY_INTERFACE_CLASS_CONTENT
}
ANSC_QUERY_INTERFACE_OBJECT,  *PANSC_QUERY_INTERFACE_OBJECT;

#define  ACCESS_ANSC_QUERY_INTERFACE_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_QUERY_INTERFACE_OBJECT, Linkage)


#endif
