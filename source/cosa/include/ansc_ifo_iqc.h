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

    module:	ansc_ifo_iqc.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Interface Query Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_IFO_IQC_
#define  _ANSC_IFO_IQC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    ANSC INTERFACE QUERY CONTROLLER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  ANSC_IQC_INTERFACE_NAME                    "anscInterfaceQueryControllerIf"
#define  ANSC_IQC_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCIQCIF_QUERY_IF)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

typedef  char*
(*PFN_ANSCIQCIF_QUERY_MODULE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

/*
 * As a standard practice, each system module MUST create a Query Interface Object to provide a
 * standard interface for external compents to access its interfaces via name-handler mapping. This
 * object is usually encapsulated by an Ansc Module Container Object, whose runtime context will
 * be written into a known position in the System Information Repository. Each Module Carrier
 * Object constructs this IQC interface to shield each module from knowing the dependency.
 */
#define  ANSC_IQC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_ANSCIQCIF_QUERY_IF          QueryIf;                                                \
    PFN_ANSCIQCIF_QUERY_MODULE      QueryModule;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IQC_INTERFACE
{
    ANSC_IQC_INTERFACE_CLASS_CONTENT
}
ANSC_IQC_INTERFACE,  *PANSC_IQC_INTERFACE;

#define  ACCESS_ANSC_IQC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, ANSC_IQC_INTERFACE, Linkage)


#endif
