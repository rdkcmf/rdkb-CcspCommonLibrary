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

    module:	ansc_ato_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Ansc Atom Table Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_ATO_INTERFACE_
#define  _ANSC_ATO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
     PLATFORM INDEPENDENT ATOM TABLE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the Atom Table Object definition.
 */
#define  ANSC_ATO_ADO_TABLE_SIZE                    64

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ATO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_ATO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ATO_ALLOCATE_TABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ATO_GET_ATOM_BYIND)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

typedef  ANSC_HANDLE
(*PFN_ATO_ADD_ATOM_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       id,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_ATO_GET_ATOM_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_ATO_DEL_ATOM_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_ATO_ADD_ATOM_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id,
        char*                       name,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_ATO_GET_ATOM_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_STATUS
(*PFN_ATO_DEL_ATOM_BYID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

typedef  ANSC_STATUS
(*PFN_ATO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_ATOM_TABLE_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLIST_HEADER*                   AdoIntTable;                                            \
    ANSC_SPINLOCK                   AdoIntTableSpinLock;                                    \
    ULONG                           AdoIntTableSize;                                        \
    ULONG                           AdoIntCount;                                            \
                                                                                            \
    SLIST_HEADER*                   AdoStrTable;                                            \
    ANSC_SPINLOCK                   AdoStrTableSpinLock;                                    \
    ULONG                           AdoStrTableSize;                                        \
    ULONG                           AdoStrCount;                                            \
                                                                                            \
    PFN_ATO_GET_COUNT               GetIntAtomCount;                                        \
    PFN_ATO_GET_COUNT               GetStrAtomCount;                                        \
    PFN_ATO_GET_SIZE                GetIntTableSize;                                        \
    PFN_ATO_SET_SIZE                SetIntTableSize;                                        \
    PFN_ATO_GET_SIZE                GetStrTableSize;                                        \
    PFN_ATO_SET_SIZE                SetStrTableSize;                                        \
    PFN_ATO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_ATO_ALLOCATE_TABLE          AllocateIntTable;                                       \
    PFN_ATO_ALLOCATE_TABLE          AllocateStrTable;                                       \
    PFN_ATO_GET_COUNT               GetAtomCount;                                           \
    PFN_ATO_GET_ATOM_BYIND          GetAtomByIndex;                                         \
    PFN_ATO_GET_COUNT               GetAtomCount2;                                          \
    PFN_ATO_GET_ATOM_BYIND          GetAtomByIndex2;                                        \
                                                                                            \
    PFN_ATO_ADD_ATOM_BYNAME         AddAtomByName;                                          \
    PFN_ATO_GET_ATOM_BYNAME         GetAtomByName;                                          \
    PFN_ATO_DEL_ATOM_BYNAME         DelAtomByName;                                          \
    PFN_ATO_DEL_ALL                 DelAllStrAtoms;                                         \
                                                                                            \
    PFN_ATO_ADD_ATOM_BYID           AddAtomById;                                            \
    PFN_ATO_GET_ATOM_BYID           GetAtomById;                                            \
    PFN_ATO_DEL_ATOM_BYID           DelAtomById;                                            \
    PFN_ATO_DEL_ALL                 DelAllIntAtoms;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ATOM_TABLE_OBJECT
{
    ANSC_ATOM_TABLE_CLASS_CONTENT
}
ANSC_ATOM_TABLE_OBJECT,  *PANSC_ATOM_TABLE_OBJECT;

#define  ACCESS_ANSC_ATOM_TABLE_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_ATOM_TABLE_OBJECT, Linkage)


/***********************************************************
   PLATFORM INDEPENDENT ATOM DESCRIPTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the Atom Descriptor Object definition.
 */
typedef  struct
_ANSC_ATOM_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    char*                           AtomName;
    ULONG                           AtomId;
    ANSC_HANDLE                     hContext;
}
ANSC_ATOM_DESCRIPTOR,  *PANSC_ATOM_DESCRIPTOR;

#define  ACCESS_ANSC_ATOM_DESCRIPTOR(p)             \
         ACCESS_CONTAINER(p, ANSC_ATOM_DESCRIPTOR, Linkage)


#endif
