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

    module:	ansc_ncco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Npt (Network Packet
        Transformation) Command Carrier Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/11/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_NCCO_INTERFACE_
#define  _ANSC_NCCO_INTERFACE_


/***********************************************************
 PLATFORM INDEPENDENT NPT COMMAND CARRIER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_NCCO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_NCCO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_APPEND_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmd
    );

typedef  ANSC_STATUS
(*PFN_NCCO_INSERT_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmd,
        ANSC_HANDLE                 hAfterCmd
    );

typedef  ANSC_STATUS
(*PFN_NCCO_REMOVE_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmd
    );

typedef  ANSC_STATUS
(*PFN_NCCO_DELETE_ALL_CMDS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_NCCO_POP_CMD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_NCCO_PEEK_HEAD_CMD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_NCCO_PEEK_NEXT_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisCmd
    );

typedef  ANSC_HANDLE
(*PFN_NCCO_PEEK_TAIL_CMD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_TRANSFER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_INCORPORATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_TEST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCCO_FOLD)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The packet processing in bisga architecture is greatly different from the legacy host-based
 * software protocol-stack implementation, which usually makes copies of a packet along the way
 * and manipulates packet content. In bisga, we isolate the packet-manipulation and packet-
 * classification. When a packet is received, it's first handed over to a classification engine
 * (i.e., policy search engine), which in turns returns a set of manipulation commands back to
 * the packet transformation engine. It then transforms the packet based on the commands.
 */
#define  ANSC_NPT_CMD_CARRIER_CLASS_CONTENT                                                 \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
                                                                                            \
    PFN_NCCO_CREATE                 Create;                                                 \
    PFN_NCCO_REMOVE                 Remove;                                                 \
    PFN_NCCO_RESET                  Reset;                                                  \
                                                                                            \
    QUEUE_HEADER                    CmdQueue;                                               \
    ANSC_LOCK                       CmdQueueLock;                                           \
                                                                                            \
    PFN_NCCO_APPEND_CMD             AppendCmd;                                              \
    PFN_NCCO_INSERT_CMD             InsertCmd;                                              \
    PFN_NCCO_REMOVE_CMD             RemoveCmd;                                              \
    PFN_NCCO_DELETE_ALL_CMDS        DeleteAllCmds;                                          \
                                                                                            \
    PFN_NCCO_POP_CMD                PopCmd;                                                 \
    PFN_NCCO_PEEK_HEAD_CMD          PeekHeadCmd;                                            \
    PFN_NCCO_PEEK_NEXT_CMD          PeekNextCmd;                                            \
    PFN_NCCO_PEEK_TAIL_CMD          PeekTailCmd;                                            \
                                                                                            \
    PFN_NCCO_TRANSFER               Transfer;                                               \
    PFN_NCCO_INCORPORATE            Incorporate;                                            \
    PFN_NCCO_TEST                   Test;                                                   \
    PFN_NCCO_FOLD                   Fold;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_CARRIER_OBJECT
{
    ANSC_NPT_CMD_CARRIER_CLASS_CONTENT
}
ANSC_NPT_CMD_CARRIER_OBJECT,  *PANSC_NPT_CMD_CARRIER_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_CARRIER_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_CARRIER_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT NPT COMMAND OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_NPT_CMD_TYPE_BASE                     0
#define  ANSC_NPT_CMD_TYPE_NULL                     ANSC_NPT_CMD_TYPE_BASE  + 0x0001
#define  ANSC_NPT_CMD_TYPE_INSB                     ANSC_NPT_CMD_TYPE_BASE  + 0x0002
#define  ANSC_NPT_CMD_TYPE_DELB                     ANSC_NPT_CMD_TYPE_BASE  + 0x0003
#define  ANSC_NPT_CMD_TYPE_REPB                     ANSC_NPT_CMD_TYPE_BASE  + 0x0004
#define  ANSC_NPT_CMD_TYPE_INSW                     ANSC_NPT_CMD_TYPE_BASE  + 0x0005
#define  ANSC_NPT_CMD_TYPE_DELW                     ANSC_NPT_CMD_TYPE_BASE  + 0x0006
#define  ANSC_NPT_CMD_TYPE_REPW                     ANSC_NPT_CMD_TYPE_BASE  + 0x0007
#define  ANSC_NPT_CMD_TYPE_INSD                     ANSC_NPT_CMD_TYPE_BASE  + 0x0008
#define  ANSC_NPT_CMD_TYPE_DELD                     ANSC_NPT_CMD_TYPE_BASE  + 0x0009
#define  ANSC_NPT_CMD_TYPE_REPD                     ANSC_NPT_CMD_TYPE_BASE  + 0x000A
#define  ANSC_NPT_CMD_TYPE_INSS                     ANSC_NPT_CMD_TYPE_BASE  + 0x000B
#define  ANSC_NPT_CMD_TYPE_DELS                     ANSC_NPT_CMD_TYPE_BASE  + 0x000C
#define  ANSC_NPT_CMD_TYPE_REPS                     ANSC_NPT_CMD_TYPE_BASE  + 0x000D
#define  ANSC_NPT_CMD_TYPE_SWAP                     ANSC_NPT_CMD_TYPE_BASE  + 0x000E
#define  ANSC_NPT_CMD_TYPE_PLSN                     ANSC_NPT_CMD_TYPE_BASE  + 0x000F
#define  ANSC_NPT_CMD_TYPE_MINN                     ANSC_NPT_CMD_TYPE_BASE  + 0x0010
#define  ANSC_NPT_CMD_TYPE_ZERO                     ANSC_NPT_CMD_TYPE_BASE  + 0x0011
#define  ANSC_NPT_CMD_TYPE_GENP                     ANSC_NPT_CMD_TYPE_BASE  + 0x0012
#define  ANSC_NPT_CMD_TYPE_MOVP                     ANSC_NPT_CMD_TYPE_BASE  + 0x0013
#define  ANSC_NPT_CMD_TYPE_DELP                     ANSC_NPT_CMD_TYPE_BASE  + 0x0014
#define  ANSC_NPT_CMD_TYPE_EYPT                     ANSC_NPT_CMD_TYPE_BASE  + 0x0015
#define  ANSC_NPT_CMD_TYPE_DYPT                     ANSC_NPT_CMD_TYPE_BASE  + 0x0016
#define  ANSC_NPT_CMD_TYPE_ECPS                     ANSC_NPT_CMD_TYPE_BASE  + 0x0017
#define  ANSC_NPT_CMD_TYPE_DCPS                     ANSC_NPT_CMD_TYPE_BASE  + 0x0018

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_NCO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_NCO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_NCO_DUPLICATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_NCO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

/*
 * Different packet transformation command may require different parameters associated with it. The
 * parameters may come from different places as well: for byte/word/dword replacement and insertion,
 * the values can be specified within the command object while the string-related operation have to
 * use the scratch pad to do business. We're not trying to design a full-fledged packet-processing
 * silicon chip, but this architecture does give us the flexibility to adapt our technology into
 * the higher end platforms.
 */
#define  ANSC_NPT_CMD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
    ULONG                           Type;                                                   \
                                                                                            \
    PFN_NCO_CREATE                  Create;                                                 \
    PFN_NCO_REMOVE                  Remove;                                                 \
    PFN_NCO_RESET                   Reset;                                                  \
    PFN_NCO_DUPLICATE               Duplicate;                                              \
                                                                                            \
    PFN_NCO_GET_TYPE                GetType;                                                \
    PFN_NCO_SET_TYPE                SetType;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_OBJECT
{
    ANSC_NPT_CMD_CLASS_CONTENT
}
ANSC_NPT_CMD_OBJECT,  *PANSC_NPT_CMD_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_OBJECT, Linkage)


#endif
