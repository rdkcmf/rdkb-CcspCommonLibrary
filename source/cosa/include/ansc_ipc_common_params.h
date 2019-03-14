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

    module:	ansc_ipc_common_params.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the common input and
        output structures for communications between different
        ansc-compatible modules.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_IPC_COMMON_PARAMS_
#define  _ANSC_IPC_COMMON_PARAMS_


/***********************************************************
      GENERAL COMMUNICATION STATUS DEFINITION FOR IPC
***********************************************************/

/*
 * Unlike the normal IPC scenarios on embedded platforms, which can be considered as permanent
 * communication channel, the inter-modules communication on the bisga platform must take into
 * account of the situations where the message pipe may break. The communication pipe may be
 * broken because the local cpu on one of the line cards crashes. The transmitter is responsible
 * for detecting any internal link failure and report to the port object, which in turn may
 * broadcast the status to all the receivers.
 */
#define  ANSC_IPC_STATUS_UP                         0x00000001
#define  ANSC_IPC_STATUS_DOWN                       0x00000002

/*
 * In addition to indicating the simple "link up" or "link down" status, we need to display and
 * log the reason caused the status change so human administrators can diagnose the problem if
 * necessary. This feature is very important for performing high-end profiling in a pre-production
 * testing environment.
 */
#define  ANSC_IPC_STATUS_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ULONG                           StatusCode;                                             \
    ULONG                           ReasonCode;                                             \
    char                            ReasonText[ANSC_MAX_STRING_SIZE];                       \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_STATUS
{
    ANSC_IPC_STATUS_CONTENT
}
ANSC_IPC_STATUS,  *PANSC_IPC_STATUS;

#define  ANSC_IPC_STATUS(p)                         \
         ACCESS_CONTAINER(p, ANSC_IPC_STATUS, Linkage)


/***********************************************************
        GENERAL PARAMS AND RETURN DEFINITION FOR IPC
***********************************************************/

/*
 * The callee can either be addressed by its code name or the runtime object id. The code name is
 * usually used for initial provisioning requests on the platform objects, for instance, the Nsm
 * Container and Slee component; while the runtime object id is used for configuration.
 */
#define  ANSC_IPC_ADDRESS_NAME                      0x00000001
#define  ANSC_IPC_ADDRESS_RID                       0x00000002

/*
 * Communication between components is probably one of the most important aspect in any distributed
 * system. BroadWay employs different communication mechanisms for between embedded platform and
 * the central server and between the Sce and other component modules. Devising another set of text
 * based protocols for intra-box communication (inter-cpu/inter-mode still) is certainly not a good
 * choice due to the poor performance and complex decoder on both ends. Instead, we use a binary
 * based IPC mechanism similar to the device I/O control (or file system I/O).
 */
#define  ANSC_IPC_PARAMS_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ULONG                           SeqNumber;                                              \
    ULONG                           Size;                                                   \
    ULONG                           ControlCode;                                            \
    ULONG                           AddressMask;                                            \
    char                            Name[ANSC_OBJECT_NAME_SIZE];                            \
    ULONG                           Rid;                                                    \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_PARAMS
{
    ANSC_IPC_PARAMS_CONTENT
}
ANSC_IPC_PARAMS,  *PANSC_IPC_PARAMS;

#define  ANSC_IPC_PARAMS(p)                         \
         ACCESS_CONTAINER(p, ANSC_IPC_PARAMS, Linkage)

/*
 * For a typical exchange, the initiator constructs the params data structures and sends it to the
 * responder. The responser examines the request parameters and then fills the response information
 * in the return data structure.
 */
#define  ANSC_IPC_RETURN_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ULONG                           SeqNumber;                                              \
    ULONG                           Size;                                                   \
    ANSC_STATUS                     Status;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_RETURN
{
    ANSC_IPC_RETURN_CONTENT
}
ANSC_IPC_RETURN,  *PANSC_IPC_RETURN;

#define  ANSC_IPC_RETURN(p)                         \
         ACCESS_CONTAINER(p, ANSC_IPC_RETURN, Linkage)


/***********************************************************
           LIST OF COMMON CONTROL CODE DEFINITIONS
***********************************************************/

/*
 * Very similar to how device I/O control (or file system I/O) works: we define a list of control
 * codes, which will be embedded in the request structure sent from the caller to the callee. The
 * callee then examines the control code to determine which action to take or which information
 * shall be returned. Note that the control code is not defined in a flat name space since the
 * different callee may share the same code name.
 */
#define  ANSC_IPC_CONTROL_CODE_BASE                 0x00000000
#define  ANSC_IPC_QUERY_OBJECTS_COUNT               ANSC_IPC_CONTROL_CODE_BASE  + 0x0001
#define  ANSC_IPC_QUERY_OBJECTS_MAP                 ANSC_IPC_CONTROL_CODE_BASE  + 0x0002
#define  ANSC_IPC_MANUFACTURE                       ANSC_IPC_CONTROL_CODE_BASE  + 0x0003
#define  ANSC_IPC_DESTROY                           ANSC_IPC_CONTROL_CODE_BASE  + 0x0004


/***********************************************************
      DEFFINITIONS FOR OM PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The object map consists of all the component objects that are registered to the Slee Container,
 * including Service Matrix, Router Stack, Layer, Protocol, Bundle, and all service objects. The
 * object map simply indicates the capability on what the corresponding module can do.
 */
#define  ANSC_IPC_PARAMS_OBJECTS_MAP_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_PARAMS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_PARAMS_OBJECTS_MAP
{
    ANSC_IPC_PARAMS_OBJECTS_MAP_CONTENT
}
ANSC_IPC_PARAMS_OBJECTS_MAP,  *PANSC_IPC_PARAMS_OBJECTS_MAP;

#define  ANSC_IPC_RETURN_OBJECTS_MAP_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_RETURN_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           ObjectsCount;                                           \
    ANSC_OBJECT_TAG                 ObjectsMap[ANSC_ZERO_ARRAY_SIZE];                       \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_RETURN_OBJECTS_MAP
{
    ANSC_IPC_RETURN_OBJECTS_MAP_CONTENT
}
ANSC_IPC_RETURN_OBJECTS_MAP,  *PANSC_IPC_RETURN_OBJECTS_MAP;


/***********************************************************
  DEFFINITIONS FOR MANUFACTURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The agent object removes the indirect object reference by the provisioning server to the base
 * object and converts the derived relationship into a direct object reference. All manufacture
 * request will be carried out on an object id.
 */
#define  ANSC_IPC_PARAMS_MANUFACTURE_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_PARAMS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           Oid;                                                    \
    /* end of object class content */                                                       \

typedef  struct
ANSC_IPC_PARAMS_MANUFACTURE
{
    ANSC_IPC_PARAMS_MANUFACTURE_CONTENT
}
ANSC_IPC_PARAMS_MANUFACTURE,  *PANSC_IPC_PARAMS_MANUFACTURE;

#define  ANSC_IPC_RETURN_MANUFACTURE_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_RETURN_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           RidArray[ANSC_ZERO_ARRAY_SIZE];                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_RETURN_MANUFACTURE
{
    ANSC_IPC_RETURN_MANUFACTURE_CONTENT
}
ANSC_IPC_RETURN_MANUFACTURE,  *PANSC_IPC_RETURN_MANUFACTURE;


/***********************************************************
   DEFFINITIONS FOR DESTROY PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * While multiple objects can be manufactured in a single call, they have to destroyed via multiple
 * individual calls. Also note that this provisioning command actually overlap with the one defined
 * in the configuration command set.
 */
#define  ANSC_IPC_PARAMS_DESTROY_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_PARAMS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           Mode;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_PARAMS_DESTROY
{
    ANSC_IPC_PARAMS_DESTROY_CONTENT
}
ANSC_IPC_PARAMS_DESTROY,  *PANSC_IPC_PARAMS_DESTROY;

#define  ANSC_IPC_RETURN_DESTROY_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_RETURN_CONTENT                                                                 \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_RETURN_DESTROY
{
    ANSC_IPC_RETURN_DESTROY_CONTENT
}
ANSC_IPC_RETURN_DESTROY,  *PANSC_IPC_RETURN_DESTROY;


#endif
