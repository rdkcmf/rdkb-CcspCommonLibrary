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

    module:	nsm_ipc_params.h

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the input and output structures
        for communications between Sce and Nsm.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/15/01    initial revision.

**********************************************************************/


#ifndef  _NSM_IPC_PARAMS_
#define  _NSM_IPC_PARAMS_


/***********************************************************
        POLICY FIELDS DEFINITION FOR INTER-MODULE IPC
***********************************************************/

/*
 * The inter-modules (or even inter-containers) communication is implemented as system-independent
 * IPC (Inter-Process Communication). The underlying Ansc platform provides a unified IPC interface
 * to all the embedded modules while the wrapper implements system-dependent communication methods
 * for each specific software/hardware architecture. Each module must register one and only one
 * IPC port object to the wrapper. The wrapper then creates virtual switch matrix for each cross-
 * connection between any two port objects. The primary difference between this IPC and the socket
 * based provisioning/configuration/monitoring mechanisms is that the IPC channel is considered to
 * be fast and reliable.
 */

/*
 * Any module who wants to be involved in the IPC communication must register a port object with a
 * unique port name or port identifier. Currently, we use the port name to identify the target and
 * source port objects since it's easier this way to maintain the straight relationship with the
 * module provisioning identifications.
 */
#define  NSM_IPC_PORT_NAME                          "/bisga/ipc/port/nsm/"


/***********************************************************
  GENERAL PARAMS AND RETURN DEFINITION FOR NSM CONNECTION
***********************************************************/

/*
 * Communication between components is probably one of the most important aspect in any distributed
 * system. BroadWay employs different communication mechanisms for between embedded platform and
 * the central server and between the Sce and other component modules. Devising another set of text
 * based protocols for intra-box communication (inter-cpu/inter-mode still) is certainly not a good
 * choice due to the poor performance and complex decoder on both ends. Instead, we use a binary
 * based IPC mechanism similar to the device I/O control (or file system I/O).
 */
#define  NSM_IPC_PARAMS_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_PARAMS_CONTENT                                                                 \

typedef  struct
_NSM_IPC_PARAMS
{
    NSM_IPC_PARAMS_CONTENT
}
NSM_IPC_PARAMS,  *PNSM_IPC_PARAMS;

#define  ACCESS_NSM_IPC_PARAMS(p)                   \
         ACCESS_CONTAINER(p, NSM_IPC_PARAMS, Linkage)

/*
 * For a typical exchange, the initiator constructs the params data structures and sends it to the
 * responder. The responser examines the request parameters and then fills the response information
 * in the return data structure.
 */
#define  NSM_IPC_RETURN_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_RETURN_CONTENT                                                                 \

typedef  struct
_NSM_IPC_RETURN
{
    NSM_IPC_RETURN_CONTENT
}
NSM_IPC_RETURN,  *PNSM_IPC_RETURN;

#define  ACCESS_NSM_IPC_RETURN(p)                   \
         ACCESS_CONTAINER(p, NSM_IPC_RETURN, Linkage)


/***********************************************************
     LIST OF TARGET NAME DEFINITIONS FOR NSM CONNECTION
***********************************************************/

/*
 * Very similar to how device I/O control (or file system I/O) works: we define a list of target
 * names, which will be embedded in the request structure sent from the caller to the callee. The
 * callee then examines the target name to determine where the request should be dispatched to.
 */
#define  NSM_IPC_RECEIVER_CONTAINER                 "container"
#define  NSM_IPC_RECEIVER_SLEE                      "slee"
#define  NSM_IPC_RECEIVER_PSE                       "pse"
#define  NSM_IPC_RECEIVER_GPP                       "gpp"
#define  NSM_IPC_RECEIVER_SPP                       "spp"
#define  NSM_IPC_RECEIVER_SPA                       "spa"
#define  NSM_IPC_RECEIVER_PQA                       "pqa"


/***********************************************************
     LIST OF CONTROL CODE DEFINITIONS FOR NSM CONNECTION
***********************************************************/

/*
 * Very similar to how device I/O control (or file system I/O) works: we define a list of control
 * codes, which will be embedded in the request structure sent from the caller to the callee. The
 * callee then examines the control code to determine which action to take or which information
 * shall be returned. Note that the control code is not defined in a flat name space since the
 * different callee may share the same code name.
 */
#define  NSM_IPC_CONTROL_CODE_BASE                  ANSC_IPC_CONTROL_CODE_BASE  + 0x0100

#define  NSM_IPC_QUERY_LAYOUT                       NSM_IPC_CONTROL_CODE_BASE   + 0x0001
#define  NSM_IPC_QUERY_SERVICES_MAP                 NSM_IPC_CONTROL_CODE_BASE   + 0x0002
#define  NSM_IPC_QUERY_OBJECTS_COUNT                NSM_IPC_CONTROL_CODE_BASE   + 0x0003
#define  NSM_IPC_QUERY_OBJECTS_MAP                  NSM_IPC_CONTROL_CODE_BASE   + 0x0004
#define  NSM_IPC_MANUFACTURE_OBJECTS                NSM_IPC_CONTROL_CODE_BASE   + 0x0005
#define  NSM_IPC_DESTROY_OBJECT                     NSM_IPC_CONTROL_CODE_BASE   + 0x0006

#define  NSM_IPC_EMPLOY                             NSM_IPC_CONTROL_CODE_BASE   + 0x0101
#define  NSM_IPC_COMMIT                             NSM_IPC_CONTROL_CODE_BASE   + 0x0102
#define  NSM_IPC_UNLINK                             NSM_IPC_CONTROL_CODE_BASE   + 0x0103
#define  NSM_IPC_CONFIGURE                          NSM_IPC_CONTROL_CODE_BASE   + 0x0104
#define  NSM_IPC_RESET                              NSM_IPC_CONTROL_CODE_BASE   + 0x0105


/***********************************************************
    DEFFINITIONS FOR LAYOUT PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The layout mask indicates which component is currently implemented in the Nsm module. However,
 * the mask value doesn't say anything about how a component is implemented.
 */
#define  NSM_LAYOUT_MASK_SLEE                       0x00000001
#define  NSM_LAYOUT_MASK_PSE                        0x00000002
#define  NSM_LAYOUT_MASK_GPP                        0x00000004
#define  NSM_LAYOUT_MASK_SPP                        0x00000008
#define  NSM_LAYOUT_MASK_SPA                        0x00000010
#define  NSM_LAYOUT_MASK_PQA                        0x00000020

#define  NSM_MODE_LIQUIFIED                         0x00000001
#define  NSM_MODE_PETRIFIED                         0x00000002

/*
 * The layout of Nsm is maintained by the container, of course. It indicates the way of how the
 * Nsm is implemented, and has a big impact on the rest of the system.
 */
#define  NSM_IPC_PARAMS_LAYOUT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_LAYOUT
{
    NSM_IPC_PARAMS_LAYOUT_CONTENT
}
NSM_IPC_PARAMS_LAYOUT,  *PNSM_IPC_PARAMS_LAYOUT;

#define  NSM_IPC_RETURN_LAYOUT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           LayoutMask;                                             \
    ULONG                           SleeMode;                                               \
    ULONG                           PseMode;                                                \
    ULONG                           GppMode;                                                \
    ULONG                           SppMode;                                                \
    ULONG                           SpaMode;                                                \
    ULONG                           PqaMode;                                                \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_LAYOUT
{
    NSM_IPC_RETURN_LAYOUT_CONTENT
}
NSM_IPC_RETURN_LAYOUT,  *PNSM_IPC_RETURN_LAYOUT;


/***********************************************************
      DEFFINITIONS FOR OM PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The object map consists of all the component objects that are registered to the Slee Container,
 * including Service Matrix, Router Stack, Layer, Protocol, Bundle, and all service objects. The
 * object map simply indicates the capability on what the corresponding module can do.
 */
#define  NSM_IPC_PARAMS_OBJECTS_MAP_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_OBJECTS_MAP
{
    NSM_IPC_PARAMS_OBJECTS_MAP_CONTENT
}
NSM_IPC_PARAMS_OBJECTS_MAP,  *PNSM_IPC_PARAMS_OBJECTS_MAP;

#define  NSM_IPC_RETURN_OBJECTS_MAP_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           ObjectsCount;                                           \
    ANSC_OBJECT_TAG                 ObjectsMap[ANSC_ZERO_ARRAY_SIZE];                       \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_OBJECTS_MAP
{
    NSM_IPC_RETURN_OBJECTS_MAP_CONTENT
}
NSM_IPC_RETURN_OBJECTS_MAP,  *PNSM_IPC_RETURN_OBJECTS_MAP;


/***********************************************************
  DEFFINITIONS FOR MANUFACTURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The agent object removes the indirect object reference by the provisioning server to the base
 * object and converts the derived relationship into a direct object reference. All manufacture
 * request will be carried out on an object id.
 */
#define  NSM_IPC_PARAMS_MANUFACTURE_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           Oid;                                                    \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_MANUFACTURE
{
    NSM_IPC_PARAMS_MANUFACTURE_CONTENT
}
NSM_IPC_PARAMS_MANUFACTURE,  *PNSM_IPC_PARAMS_MANUFACTURE;

#define  NSM_IPC_RETURN_MANUFACTURE_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           RidArray[ANSC_ZERO_ARRAY_SIZE];                         \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_MANUFACTURE
{
    NSM_IPC_RETURN_MANUFACTURE_CONTENT
}
NSM_IPC_RETURN_MANUFACTURE,  *PNSM_IPC_RETURN_MANUFACTURE;


/***********************************************************
   DEFFINITIONS FOR DESTROY PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * While multiple objects can be manufactured in a single call, they have to destroyed via multiple
 * individual calls. Also note that this provisioning command actually overlap with the one defined
 * in the configuration command set.
 */
#define  NSM_IPC_PARAMS_DESTROY_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Mode;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_DESTROY
{
    NSM_IPC_PARAMS_DESTROY_CONTENT
}
NSM_IPC_PARAMS_DESTROY,  *PNSM_IPC_PARAMS_DESTROY;

#define  NSM_IPC_RETURN_DESTROY_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_DESTROY
{
    NSM_IPC_RETURN_DESTROY_CONTENT
}
NSM_IPC_RETURN_DESTROY,  *PNSM_IPC_RETURN_DESTROY;


/***********************************************************
    DEFFINITIONS FOR EMPLOY PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  NSM_IPC_PARAMS_EMPLOY_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_EMPLOY
{
    NSM_IPC_PARAMS_EMPLOY_CONTENT
}
NSM_IPC_PARAMS_EMPLOY,  *PNSM_IPC_PARAMS_EMPLOY;

#define  NSM_IPC_RETURN_EMPLOY_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_EMPLOY
{
    NSM_IPC_RETURN_EMPLOY_CONTENT
}
NSM_IPC_RETURN_EMPLOY,  *PNSM_IPC_RETURN_EMPLOY;


/***********************************************************
    DEFFINITIONS FOR COMMIT PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  NSM_IPC_PARAMS_COMMIT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_COMMIT
{
    NSM_IPC_PARAMS_COMMIT_CONTENT
}
NSM_IPC_PARAMS_COMMIT,  *PNSM_IPC_PARAMS_COMMIT;

#define  NSM_IPC_RETURN_COMMIT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_COMMIT
{
    NSM_IPC_RETURN_COMMIT_CONTENT
}
NSM_IPC_RETURN_COMMIT,  *PNSM_IPC_RETURN_COMMIT;


/***********************************************************
    DEFFINITIONS FOR UNLINK PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  NSM_IPC_PARAMS_UNLINK_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_UNLINK
{
    NSM_IPC_PARAMS_UNLINK_CONTENT
}
NSM_IPC_PARAMS_UNLINK,  *PNSM_IPC_PARAMS_UNLINK;

#define  NSM_IPC_RETURN_UNLINK_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_UNLINK
{
    NSM_IPC_RETURN_UNLINK_CONTENT
}
NSM_IPC_RETURN_UNLINK,  *PNSM_IPC_RETURN_UNLINK;


/***********************************************************
   DEFFINITIONS FOR CONFIGURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  NSM_IPC_PARAMS_CONFIGURE_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           CfgSize;                                                \
    char                            CfgData[ANSC_ZERO_ARRAY_SIZE];                          \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_CONFIGURE
{
    NSM_IPC_PARAMS_CONFIGURE_CONTENT
}
NSM_IPC_PARAMS_CONFIGURE,  *PNSM_IPC_PARAMS_CONFIGURE;

#define  NSM_IPC_RETURN_CONFIGURE_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_CONFIGURE
{
    NSM_IPC_RETURN_CONFIGURE_CONTENT
}
NSM_IPC_RETURN_CONFIGURE,  *PNSM_IPC_RETURN_CONFIGURE;


/***********************************************************
    DEFFINITIONS FOR RESET PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  NSM_IPC_PARAMS_RESET_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_PARAMS_RESET
{
    NSM_IPC_PARAMS_RESET_CONTENT
}
NSM_IPC_PARAMS_RESET,  *PNSM_IPC_PARAMS_RESET;

#define  NSM_IPC_RETURN_RESET_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_NSM_IPC_RETURN_RESET
{
    NSM_IPC_RETURN_RESET_CONTENT
}
NSM_IPC_RETURN_RESET,  *PNSM_IPC_RETURN_RESET;


#endif
