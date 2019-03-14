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

    module:	ansc_vnmo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Value Name Maper Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        10/22/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_VNMO_INTERFACE_
#define  _ANSC_VNMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_platform.h"
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
  PLATFORM INDEPENDENT DAEMON ENGINE TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_VNMO_MAX_NAME_SIZE                  128
#define  ANSC_VNMO_ATO_TABLE_SIZE                 8

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_VNMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );


typedef  ULONG
(*PFN_VNMO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_VNMO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       value
    );

/*
 * the value must be null-terminated string
 */
typedef  PCHAR
(*PFN_VNMO_GET_PCHAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_VNMO_SET_PCHAR)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       value
    );


typedef  ANSC_HANDLE
(*PFN_VNMO_ADD_ATOM_ID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id,
        char*                       name,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_VNMO_ADD_ATOM_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       id,
        ANSC_HANDLE                 hContext
    );

/*
 * Tcp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Tcp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_VALUE_NAME_MAPER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAtoParams;                                             \
    ANSC_HANDLE                     hSysVnmIf;                                              \
    ULONG                           IndexOffset;                                            \
    ULONG                           ParaCount;                                              \
    char                            MapperName[ANSC_VNMO_MAX_NAME_SIZE];                    \
                                                                                            \
    PFN_VNMO_GET_CONTEXT            GetAtoParams;                                           \
    PFN_VNMO_GET_CONTEXT            GetSysVnmIf;                                            \
    PFN_VNMO_GET_ULONG              GetIndexOffset;                                         \
    PFN_VNMO_SET_ULONG              SetIndexOffset;                                         \
    PFN_VNMO_GET_ULONG              GetParaCount;                                           \
    PFN_VNMO_SET_ULONG              SetParaCount;                                           \
    PFN_VNMO_GET_PCHAR              GetMapperName;                                          \
    PFN_VNMO_SET_PCHAR              SetMapperName;                                          \
                                                                                            \
    PFN_VNMO_ADD_ATOM_ID            AddAtomById;                                            \
    PFN_VNMO_ADD_ATOM_NAME          AddAtomByName;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_VALUE_NAME_MAPER_OBJECT
{
    ANSC_VALUE_NAME_MAPER_CLASS_CONTENT
}
ANSC_VALUE_NAME_MAPER_OBJECT,  *PANSC_VALUE_NAME_MAPER_OBJECT;

#define  ACCESS_ANSC_VALUE_NAME_MAPER_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_VALUE_NAME_MAPER_OBJECT, Linkage)


#endif
