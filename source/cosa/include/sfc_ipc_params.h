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

    module:	sfc_ipc_params.h

        For Switch Fabric Controller Container (SFCC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the input and output structures
        for communications between Sce and Sfc.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/10/01    initial revision.

**********************************************************************/


#ifndef  _SFC_IPC_PARAMS_
#define  _SFC_IPC_PARAMS_


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
#define  SFC_IPC_PORT_NAME                          "/bisga/ipc/port/sfc/"


/***********************************************************
  GENERAL PARAMS AND RETURN DEFINITION FOR SFC CONNECTION
***********************************************************/

/*
 * Communication between components is probably one of the most important aspect in any distributed
 * system. BroadWay employs different communication mechanisms for between embedded platform and
 * the central server and between the Sce and other component modules. Devising another set of text
 * based protocols for intra-box communication (inter-cpu/inter-mode still) is certainly not a good
 * choice due to the poor performance and complex decoder on both ends. Instead, we use a binary
 * based IPC mechanism similar to the device I/O control (or file system I/O).
 */
#define  SFC_IPC_PARAMS_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_PARAMS_CONTENT                                                                 \

typedef  struct
_SFC_IPC_PARAMS
{
    SFC_IPC_PARAMS_CONTENT
}
SFC_IPC_PARAMS,  *PSFC_IPC_PARAMS;

#define  ACCESS_SFC_IPC_PARAMS(p)                   \
         ACCESS_CONTAINER(p, SFC_IPC_PARAMS, Linkage)

/*
 * For a typical exchange, the initiator constructs the params data structures and sends it to the
 * responder. The responser examines the request parameters and then fills the response information
 * in the return data structure.
 */
#define  SFC_IPC_RETURN_CONTENT                                                             \
    /* duplication of the base object class content */                                      \
    ANSC_IPC_RETURN_CONTENT                                                                 \

typedef  struct
_SFC_IPC_RETURN
{
    SFC_IPC_RETURN_CONTENT
}
SFC_IPC_RETURN,  *PSFC_IPC_RETURN;

#define  ACCESS_SFC_IPC_RETURN(p)                   \
         ACCESS_CONTAINER(p, SFC_IPC_RETURN, Linkage)


/***********************************************************
     LIST OF TARGET NAME DEFINITIONS FOR SFC CONNECTION
***********************************************************/

/*
 * Very similar to how device I/O control (or file system I/O) works: we define a list of target
 * names, which will be embedded in the request structure sent from the caller to the callee. The
 * callee then examines the target name to determine where the request should be dispatched to.
 */
#define  SFC_IPC_RECEIVER_CONTAINER                 "container"


/***********************************************************
     LIST OF CONTROL CODE DEFINITIONS FOR SFC CONNECTION
***********************************************************/

/*
 * Very similar to how device I/O control (or file system I/O) works: we define a list of control
 * codes, which will be embedded in the request structure sent from the caller to the callee. The
 * callee then examines the control code to determine which action to take or which information
 * shall be returned. Note that the control code is not defined in a flat name space since the
 * different callee may share the same code name.
 */
#define  SFC_IPC_CONTROL_CODE_BASE                  ANSC_IPC_CONTROL_CODE_BASE  + 0x0100

#define  SFC_IPC_QUERY_OBJECTS_COUNT                SFC_IPC_CONTROL_CODE_BASE   + 0x0001
#define  SFC_IPC_QUERY_OBJECTS_MAP                  SFC_IPC_CONTROL_CODE_BASE   + 0x0002
#define  SFC_IPC_MANUFACTURE_OBJECTS                SFC_IPC_CONTROL_CODE_BASE   + 0x0003
#define  SFC_IPC_DESTROY_OBJECT                     SFC_IPC_CONTROL_CODE_BASE   + 0x0004

#define  SFC_IPC_EMPLOY                             SFC_IPC_CONTROL_CODE_BASE   + 0x0101
#define  SFC_IPC_COMMIT                             SFC_IPC_CONTROL_CODE_BASE   + 0x0102
#define  SFC_IPC_UNLINK                             SFC_IPC_CONTROL_CODE_BASE   + 0x0103
#define  SFC_IPC_CONFIGURE                          SFC_IPC_CONTROL_CODE_BASE   + 0x0104
#define  SFC_IPC_RESET                              SFC_IPC_CONTROL_CODE_BASE   + 0x0105

/***********************************************************
      DEFFINITIONS FOR OM PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The object map consists of all the component objects that are registered to the Slee Container,
 * including Service Matrix, Router Stack, Layer, Protocol, Bundle, and all service objects. The
 * object map simply indicates the capability on what the corresponding module can do.
 */
#define  SFC_IPC_PARAMS_OBJECTS_MAP_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_OBJECTS_MAP
{
    SFC_IPC_PARAMS_OBJECTS_MAP_CONTENT
}
SFC_IPC_PARAMS_OBJECTS_MAP,  *PSFC_IPC_PARAMS_OBJECTS_MAP;

#define  SFC_IPC_RETURN_OBJECTS_MAP_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           ObjectsCount;                                           \
    ANSC_OBJECT_TAG                 ObjectsMap[ANSC_ZERO_ARRAY_SIZE];                       \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_OBJECTS_MAP
{
    SFC_IPC_RETURN_OBJECTS_MAP_CONTENT
}
SFC_IPC_RETURN_OBJECTS_MAP,  *PSFC_IPC_RETURN_OBJECTS_MAP;


/***********************************************************
  DEFFINITIONS FOR MANUFACTURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The agent object removes the indirect object reference by the provisioning server to the base
 * object and converts the derived relationship into a direct object reference. All manufacture
 * request will be carried out on an object id.
 */
#define  SFC_IPC_PARAMS_MANUFACTURE_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           Oid;                                                    \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_MANUFACTURE
{
    SFC_IPC_PARAMS_MANUFACTURE_CONTENT
}
SFC_IPC_PARAMS_MANUFACTURE,  *PSFC_IPC_PARAMS_MANUFACTURE;

#define  SFC_IPC_RETURN_MANUFACTURE_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Volume;                                                 \
    ULONG                           RidArray[ANSC_ZERO_ARRAY_SIZE];                         \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_MANUFACTURE
{
    SFC_IPC_RETURN_MANUFACTURE_CONTENT
}
SFC_IPC_RETURN_MANUFACTURE,  *PSFC_IPC_RETURN_MANUFACTURE;


/***********************************************************
   DEFFINITIONS FOR DESTROY PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * While multiple objects can be manufactured in a single call, they have to destroyed via multiple
 * individual calls. Also note that this provisioning command actually overlap with the one defined
 * in the configuration command set.
 */
#define  SFC_IPC_PARAMS_DESTROY_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           Mode;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_DESTROY
{
    SFC_IPC_PARAMS_DESTROY_CONTENT
}
SFC_IPC_PARAMS_DESTROY,  *PSFC_IPC_PARAMS_DESTROY;

#define  SFC_IPC_RETURN_DESTROY_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_DESTROY
{
    SFC_IPC_RETURN_DESTROY_CONTENT
}
SFC_IPC_RETURN_DESTROY,  *PSFC_IPC_RETURN_DESTROY;


/***********************************************************
    DEFFINITIONS FOR EMPLOY PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SFC_IPC_PARAMS_EMPLOY_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_EMPLOY
{
    SFC_IPC_PARAMS_EMPLOY_CONTENT
}
SFC_IPC_PARAMS_EMPLOY,  *PSFC_IPC_PARAMS_EMPLOY;

#define  SFC_IPC_RETURN_EMPLOY_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_EMPLOY
{
    SFC_IPC_RETURN_EMPLOY_CONTENT
}
SFC_IPC_RETURN_EMPLOY,  *PSFC_IPC_RETURN_EMPLOY;


/***********************************************************
    DEFFINITIONS FOR COMMIT PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SFC_IPC_PARAMS_COMMIT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_COMMIT
{
    SFC_IPC_PARAMS_COMMIT_CONTENT
}
SFC_IPC_PARAMS_COMMIT,  *PSFC_IPC_PARAMS_COMMIT;

#define  SFC_IPC_RETURN_COMMIT_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_COMMIT
{
    SFC_IPC_RETURN_COMMIT_CONTENT
}
SFC_IPC_RETURN_COMMIT,  *PSFC_IPC_RETURN_COMMIT;


/***********************************************************
    DEFFINITIONS FOR UNLINK PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SFC_IPC_PARAMS_UNLINK_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           RefRid;                                                 \
    ANSC_COEC_CONTRACT              Contract;                                               \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_UNLINK
{
    SFC_IPC_PARAMS_UNLINK_CONTENT
}
SFC_IPC_PARAMS_UNLINK,  *PSFC_IPC_PARAMS_UNLINK;

#define  SFC_IPC_RETURN_UNLINK_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_UNLINK
{
    SFC_IPC_RETURN_UNLINK_CONTENT
}
SFC_IPC_RETURN_UNLINK,  *PSFC_IPC_RETURN_UNLINK;


/***********************************************************
   DEFFINITIONS FOR CONFIGURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SFC_IPC_PARAMS_CONFIGURE_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           CfgSize;                                                \
    char                            CfgData[ANSC_ZERO_ARRAY_SIZE];                          \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_CONFIGURE
{
    SFC_IPC_PARAMS_CONFIGURE_CONTENT
}
SFC_IPC_PARAMS_CONFIGURE,  *PSFC_IPC_PARAMS_CONFIGURE;

#define  SFC_IPC_RETURN_CONFIGURE_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_CONFIGURE
{
    SFC_IPC_RETURN_CONFIGURE_CONTENT
}
SFC_IPC_RETURN_CONFIGURE,  *PSFC_IPC_RETURN_CONFIGURE;


/***********************************************************
    DEFFINITIONS FOR RESET PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SFC_IPC_PARAMS_RESET_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    SFC_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_PARAMS_RESET
{
    SFC_IPC_PARAMS_RESET_CONTENT
}
SFC_IPC_PARAMS_RESET,  *PSFC_IPC_PARAMS_RESET;

#define  SFC_IPC_RETURN_RESET_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    SFC_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SFC_IPC_RETURN_RESET
{
    SFC_IPC_RETURN_RESET_CONTENT
}
SFC_IPC_RETURN_RESET,  *PSFC_IPC_RETURN_RESET;


#endif
