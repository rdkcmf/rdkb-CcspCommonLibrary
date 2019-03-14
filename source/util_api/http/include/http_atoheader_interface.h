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

    module:	http_atoheader_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Ato Header Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_ATOHEADER_INTERFACE_
#define  _HTTP_ATOHEADER_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"


/***********************************************************
               HTTP ATO HEADER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ATOHEADER_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATOHEADER_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_ATOHEADER_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATOHEADER_CREATE_TABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ATOHEADER_GET_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  char*
(*PFN_ATOHEADER_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_ATO_HEADER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSC_ATOM_TABLE_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    PFN_ATOHEADER_CREATE_TABLE      CreateCodeTable;                                        \
    PFN_ATOHEADER_CREATE_TABLE      CreateNameTable;                                        \
    PFN_ATOHEADER_GET_CODE          GetAtomCode;                                            \
    PFN_ATOHEADER_GET_NAME          GetAtomName;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_ATO_HEADER_OBJECT
{
    HTTP_ATO_HEADER_CLASS_CONTENT
}
HTTP_ATO_HEADER_OBJECT,  *PHTTP_ATO_HEADER_OBJECT;

#define  ACCESS_HTTP_ATO_HEADER_OBJECT(p)           \
         ACCESS_CONTAINER(p, HTTP_ATO_HEADER_OBJECT, Linkage)


#endif
