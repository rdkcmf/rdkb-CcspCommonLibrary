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

    module:	ansc_oco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Object Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_OCO_INTERFACE_
#define  _ANSC_OCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT OBJECT CONTAINER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_OCO_OPMASK_OMO_EXTERNAL               0x00000001
#define  ANSC_OCO_OPMASK_OMO_INTERNAL               0x00000002
#define  ANSC_OCO_OPMASK_TSO                        0x00000004
#define  ANSC_OCO_OPMASK_ATO                        0x00000008
#define  ANSC_OCO_OPMASK_TOO                        0x00000010
#define  ANSC_OCO_OPMASK_IPC                        0x00000020
#define  ANSC_OCO_OPMASK_COLLAPSE                   0x00000040

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_OCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_OCO_GET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_SET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_OCO_GET_MASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_SET_MASK)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMask
    );

typedef  ANSC_STATUS
(*PFN_OCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_ENROLL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCO_REGISTER1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        void*                       pfnCreate,
        void*                       pfnRemove
    );

typedef  ANSC_STATUS
(*PFN_OCO_REGISTER2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        char*                       baseObjectName,
        ULONG                       baseOid,
        char*                       derivedType,
        ULONG                       brokerOid,
        void*                       pfnCreate,
        void*                       pfnRemove
    );

typedef  ANSC_HANDLE
(*PFN_OCO_CREATE_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       objectName
    );

typedef  ANSC_HANDLE
(*PFN_OCO_CREATE_BYOID)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    );

typedef  ANSC_HANDLE
(*PFN_OCO_CREATE_BYTYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseObjectName,
        char*                       derivedType
    );

typedef  ANSC_HANDLE
(*PFN_OCO_GET_OBJECT_BYRID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    );

typedef  ANSC_STATUS
(*PFN_OCO_REGISTER_TIMER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    );

typedef  ANSC_STATUS
(*PFN_OCO_CANCEL_TIMER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_OBJECT_CONTAINER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hExternalObjectMapper;                                  \
    ANSC_HANDLE                     hInternalObjectMapper;                                  \
    ANSC_HANDLE                     hTimerScheduler;                                        \
    ANSC_HANDLE                     hIpcPort;                                               \
    ANSC_HANDLE                     hCarrierContext;                                        \
    ULONG                           OpMask;                                                 \
    BOOL                            bEngaged;                                               \
                                                                                            \
    PFN_OCO_GET_CONTEXT             GetExternalOmo;                                         \
    PFN_OCO_SET_CONTEXT             SetExternalOmo;                                         \
    PFN_OCO_GET_CONTEXT             GetInternalOmo;                                         \
    PFN_OCO_SET_CONTEXT             SetInternalOmo;                                         \
    PFN_OCO_GET_CONTEXT             GetTso;                                                 \
    PFN_OCO_SET_CONTEXT             SetTso;                                                 \
    PFN_OCO_GET_CONTEXT             GetIpcPort;                                             \
    PFN_OCO_SET_CONTEXT             SetIpcPort;                                             \
    PFN_OCO_GET_CARRIER             GetCarrier;                                             \
    PFN_OCO_SET_CARRIER             SetCarrier;                                             \
    PFN_OCO_GET_MASK                GetOpMask;                                              \
    PFN_OCO_SET_MASK                SetOpMask;                                              \
    PFN_OCO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_OCO_ENGAGE                  Engage;                                                 \
    PFN_OCO_CANCEL                  Cancel;                                                 \
    PFN_OCO_ENGAGE                  SubEngage;                                              \
    PFN_OCO_CANCEL                  SubCancel;                                              \
                                                                                            \
    PFN_OCO_ENROLL                  EnrollAllObjects;                                       \
    PFN_OCO_MANUFACTURE             ManufactureAllObjects;                                  \
    PFN_OCO_DESTROY                 DestroyAllObjects;                                      \
                                                                                            \
    PFN_OCO_ENROLL                  EnrollExtensionObjects;                                 \
    PFN_OCO_MANUFACTURE             ManufactureExtensionObjects;                            \
    PFN_OCO_DESTROY                 DestroyExtensionObjects;                                \
                                                                                            \
    PFN_OCO_ENROLL                  EnrollFeatureObjects;                                   \
    PFN_OCO_MANUFACTURE             ManufactureFeatureObjects;                              \
    PFN_OCO_DESTROY                 DestroyFeatureObjects;                                  \
                                                                                            \
    PFN_OCO_REGISTER1               RegisterObject1;                                        \
    PFN_OCO_REGISTER2               RegisterObject2;                                        \
    PFN_OCO_CREATE_BYNAME           CreateObjectByName;                                     \
    PFN_OCO_CREATE_BYOID            CreateObjectByOid;                                      \
    PFN_OCO_CREATE_BYTYPE           CreateObjectByType;                                     \
    PFN_OCO_GET_OBJECT_BYRID        GetObjectByRid;                                         \
                                                                                            \
    PFN_OCO_REGISTER_TIMER          RegisterTimer;                                          \
    PFN_OCO_CANCEL_TIMER            CancelTimer;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_OBJECT_CONTAINER_OBJECT
{
    ANSC_OBJECT_CONTAINER_CLASS_CONTENT
}
ANSC_OBJECT_CONTAINER_OBJECT,  *PANSC_OBJECT_CONTAINER_OBJECT;

#define  ACCESS_ANSC_OBJECT_CONTAINER_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_OBJECT_CONTAINER_OBJECT, Linkage)


#endif
