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

    module:	slap_lbo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Loam Broker Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_LBO_INTERFACE_
#define  _SLAP_LBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_ifo_cpc.h"

#include "slap_properties.h"


/***********************************************************
         SLAP LOAM BROKER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_LBO_OBO_TABLE_SIZE                    128

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPLBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLBO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLBO_GET_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_ADD_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char*                       party_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_DEL_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_DEL_OBO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hCallInfo
    );

typedef  ANSC_STATUS
(*PFN_SLAPLBO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    );

/*
 * The Slap Loam Broker Object is responsible for processing IMCP-based SLAP object location/query
 * requests. It resolves the mapping from SLAP objects to the corresponding SLAP environments. It
 * serves as a central LPC server to keep trace of all the local LPC connectors by accepting every
 * HELLO message it receives.
 */
#define  SLAP_LOAM_BROKER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_LOAM_BROKER_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hAnscCpcIf;                                             \
    ANSC_HANDLE                     hAnscLpcConnector;                                      \
    BOOL                            bActive;                                                \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    QUEUE_HEADER                    OboTable[SLAP_LBO_OBO_TABLE_SIZE];                      \
    ANSC_LOCK                       OboTableLock;                                           \
                                                                                            \
    PFN_SLAPLBO_GET_IF              GetAnscCpcIf;                                           \
    PFN_SLAPLBO_GET_CONTEXT         GetAnscLpcConnector;                                    \
                                                                                            \
    PFN_SLAPLBO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPLBO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPLBO_RESET               ResetProperty;                                          \
    PFN_SLAPLBO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPLBO_ACQ_ACCESS          AcqAccess;                                              \
    PFN_SLAPLBO_REL_ACCESS          RelAccess;                                              \
    PFN_SLAPLBO_ENGAGE              Engage;                                                 \
    PFN_SLAPLBO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_SLAPLBO_GET_OBO             GetObjBinding;                                          \
    PFN_SLAPLBO_ADD_OBO             AddObjBinding;                                          \
    PFN_SLAPLBO_DEL_OBO             DelObjBinding;                                          \
    PFN_SLAPLBO_DEL_OBO2            DelObjBinding2;                                         \
    PFN_SLAPLBO_DEL_ALL             DelAllObjBindings;                                      \
                                                                                            \
    PFN_SLAPLBO_ASYNC_TASK          AsyncCallTaskLoco;                                      \
    PFN_SLAPLBO_PROCESS             ProcessCallAcqc;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallRelc;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallLoco;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallDelo;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallInvo;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallAcqo;                                        \
    PFN_SLAPLBO_PROCESS             ProcessCallRelo;                                        \
                                                                                            \
    PFN_ANSCCPCIF_DISPATCH          CpcCallDispatch;                                        \
    PFN_ANSCCPCIF_FREE              CpcFreeOutputBuffer;                                    \
    PFN_ANSCCPCIF_NOTIFY            CpcNotifyEvent;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_LOAM_BROKER_OBJECT
{
    SLAP_LOAM_BROKER_CLASS_CONTENT
}
SLAP_LOAM_BROKER_OBJECT,  *PSLAP_LOAM_BROKER_OBJECT;

#define  ACCESS_SLAP_LOAM_BROKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, SLAP_LOAM_BROKER_OBJECT, Linkage)


/***********************************************************
     SLAP LOAM BROKER ASYNC CALL INFORMATION DEFINITION
***********************************************************/

/*
 * To speed up the process of locating a SLAP object, we maintain a hash table interally for all
 * obj_name and party_name pairs. By centralizing the caching for name resolution, the time and
 * overhead involved in the communications between LPC parties are effectively reduced. Note that
 * the performance can be further improved if we maintain a separate cache for each LPC party at
 * the price of consuming much more memory.
 */
typedef  struct
_SLAP_LBO_OBJECT_BINDING
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    char*                           ObjectName;
    char*                           PartyName;
}
SLAP_LBO_OBJECT_BINDING,  *PSLAP_LBO_OBJECT_BINDING;

#define  ACCESS_SLAP_LBO_OBJECT_BINDING(p)          \
         ACCESS_CONTAINER(p, SLAP_LBO_OBJECT_BINDING, Linkage)

#define  SlapLboCleanObjBinding(obj_binding)                                                \
         {                                                                                  \
            if ( obj_binding->ObjectName )                                                  \
            {                                                                               \
                AnscFreeMemory(obj_binding->ObjectName);                                    \
                                                                                            \
                obj_binding->ObjectName = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( obj_binding->PartyName )                                                   \
            {                                                                               \
                AnscFreeMemory(obj_binding->PartyName);                                     \
                                                                                            \
                obj_binding->PartyName = NULL;                                              \
            }                                                                               \
         }

#define  SlapLboFreeObjBinding(obj_binding)                                                 \
         {                                                                                  \
            SlapLboCleanObjBinding(obj_binding);                                            \
            AnscFreeMemory        (obj_binding);                                            \
         }

/*
 * When serving a object-location call, we simutaneouly send a SLAP_LOCO REQUEST to all the LPC
 * parties instead serializing every query/answer transaction. To do that we need to spawn an async
 * task for every query.
 */
typedef  struct
_SLAP_LBO_ASYNC_CALL_INFO
{
    ANSC_HANDLE                     hThisObject;
    ULONG                           ObjectType;
    char*                           ObjectName;
    ANSC_HANDLE                     hPartyAddr;
    ANSC_HANDLE                     hCallSemaphore;
    ANSC_HANDLE*                    phDstPartyAddr;
}
SLAP_LBO_ASYNC_CALL_INFO,  *PSLAP_LBO_ASYNC_CALL_INFO;


#endif
