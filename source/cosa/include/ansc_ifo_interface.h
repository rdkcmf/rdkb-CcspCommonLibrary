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

    module:	ansc_ifo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the general Ansc Interface Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/24/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_IFO_INTERFACE_
#define  _ANSC_IFO_INTERFACE_


/***********************************************************
           GENERAL ANSC INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_INTERFACE_OBJECT_NAME                 "abstractInterfaceObject"
#define  ANSC_INTERFACE_OBJECT_ID                   0xFFFFFFFE

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_ANSCIFO_ATTACH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCIFO_DETACH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ANSCIFO_GET_OWNER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ANSCIFO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Interface Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSCIFO_CLASS_CONTENT                                                              \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    char                            Name[ANSC_OBJ_IF_NAME_SIZE];                            \
    ULONG                           InterfaceId;                                            \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ULONG                           Size;                                                   \
                                                                                            \
    /*                                                                                      \
    PFN_ANSCIFO_ATTACH              IfAttach;                                               \
    PFN_ANSCIFO_DETACH              IfDetach;                                               \
    PFN_ANSCIFO_GET_OWNER           IfGetOwner;                                             \
    PFN_ANSCIFO_GET_SIZE            IfGetSize;                                              \
    */                                                                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_INTERFACE_OBJECT
{
    ANSCIFO_CLASS_CONTENT
}
ANSC_INTERFACE_OBJECT,  *PANSC_INTERFACE_OBJECT;

#define  ACCESS_ANSC_INTERFACE_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_INTERFACE_OBJECT, Linkage)


#endif
