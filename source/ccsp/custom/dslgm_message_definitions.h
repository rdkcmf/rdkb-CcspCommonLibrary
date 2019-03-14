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

    module:	dslgm_message_definitions.h

        For DSL Manager Model Implementation (DSLM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Dslm Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

**********************************************************************/


#ifndef  _DSLGM_MESSAGE_DEFINITIONS_
#define  _DSLGM_MESSAGE_DEFINITIONS_


/***********************************************************
        DATA TYPE AND CONSTANTS FOR DSLG/M MESSAGES
***********************************************************/

/*
 * All user-mode programs share the same Message Queue by calling msgget() with the same key value.
 * The first argument to msgget() is the key value. This key value is then compared to existing key
 * values that exist within the kernel for other message queues. At that point, the open or access
 * operation is dependent upon the contents of the msgflg argument. Instead of calling ftok() to
 * generate the key, we hard code key value as defined below.
 */
#define  DSLGM_GLOBAL_MSGQUEUE_KEY                  0x44534C47      /*DSLG*/

/*
 * A unique "message type" (i.e. the value of the 'mtype' field of the msgbuf data structure) is
 * defined for an endpoint within each user-mode program. Each user-mode program will only listen
 * to messages addressed to it. This is usually done by spawning a separate task and calling msgrcv()
 * in a while loop. Below is the list of message type values defined for each user-mode program.
 */
#define  DSLGM_IPC_MSGTYPE_ToDslm                   1
#define  DSLGM_IPC_MSGTYPE_ToSysRegistry            2
#define  DSLGM_IPC_MSGTYPE_ToSnmp                   3
#define  DSLGM_IPC_MSGTYPE_ToWps                    4
#define  DSLGM_IPC_MSGTYPE_ToSspMonitor             5

#define  DSLGM_IPC_MSGTYPE_ToThirdParty             11
#define  DSLGM_IPC_MSGTYPE_DslmToCmdLine            12
#define  DSLGM_IPC_MSGTYPE_SnmpToCmdLine            13
#define  DSLGM_IPC_MSGTYPE_WpsToCmdLine             14
#define  DSLGM_IPC_MSGTYPE_ToHotPlug                15

/* 
 *  The actual request contained in the message is determined by OpCode
 */
#define  DSLGM_IPC_OpCode_CmdLine                   1

#define  DSLGM_IPC_OpCode_AddLocalApp               101
#define  DSLGM_IPC_OpCode_DelLocalApp               102
#define  DSLGM_IPC_OpCode_DelAllLocalApps           103

#define  DSLGM_IPC_OpCode_AddDev                    201
#define  DSLGM_IPC_OpCode_RemoveDev                 202

#define  DSLGM_IPC_OpCode_Reboot                    301

/*
 *  Constants
 */
#define  DSLGM_MSG_MAX_BUFFER_SIZE                  512

/***********************************************************
        DSLG/M MESSAGE DATA STRUCTURE DEFINITIONS
***********************************************************/

#define  DSLGM_MSGBUF_CONTENT                                   \
    /* Base msgbuf content */                                   \
    unsigned long                   DstMsgType;                 \
    unsigned long                   ReplyMsgType;               \
    unsigned long                   OpCode;                     \
    unsigned long                   Status;                     \
    /* End of structure definition */

typedef  struct
_DSLGM_MSGBUF_base
{
    DSLGM_MSGBUF_CONTENT
}
DSLGM_MSGBUF_base, *PDSLGM_MSGBUF_base;


typedef  struct
_DSLGM_MSGBUF_ThirdParty2Dslm
{
    DSLGM_MSGBUF_CONTENT

    unsigned char                   ucProtocol;
    unsigned short                  usPortFrom;
    unsigned short                  usPortTo;
}
DSLGM_MSGBUF_ThirdParty2Dslm, *PDSLGM_MSGBUF_ThirdParty2Dslm;


typedef  struct
_DSLGM_MSGBUF_Dslm2ThirdParty
{
    DSLGM_MSGBUF_CONTENT
}
DSLGM_MSGBUF_Dslm2ThirdParty, *PDSLGM_MSGBUF_Dslm2ThirdParty;


typedef  struct
_DSLGM_MSGBUF_CmdLine2Dslm
{
    DSLGM_MSGBUF_CONTENT

    char                            CmdStr[256];
}
DSLGM_MSGBUF_CmdLine2Dslm, *PDSLGM_MSGBUF_CmdLine2Dslm;


typedef  struct
_DSLGM_MSGBUF_Dslm2CmdLine
{
    DSLGM_MSGBUF_CONTENT
}
DSLGM_MSGBUF_Dslm2CmdLine, *PDSLGM_MSGBUF_Dslm2CmdLine;


typedef  struct
_DSLGM_MSGBUF_Hotplug2Dslm
{
    DSLGM_MSGBUF_CONTENT

    char                            DevStr[32];
}
DSLGM_MSGBUF_Hotplug2Dslm, *PDSLGM_MSGBUF_Hotplug2Dslm;


typedef  struct
_DSLGM_MSGBUF_Dslm2Hotplug
{
    DSLGM_MSGBUF_CONTENT
}
DSLGM_MSGBUF_Dslm2Hotplug, *PDSLGM_MSGBUF_Dslm2Hotplug;

#endif

