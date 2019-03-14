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

    module:	tls_hso_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Hand Shaker Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#ifndef  _TLS_HSO_INTERFACE_
#define  _TLS_HSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "tls_rco_interface.h"
#include "tls_rco_exported_api.h"
#include "tls_properties.h"


/***********************************************************
     PLATFORM INDEPENDENT HAND SHAKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  TLS_HSO_DEF_STATE_TIMEOUT                  20 * 1000   /* 20 seconds, in milli-seconds */
#define  TLS_HSO_DEF_MSG_BUFFER_SIZE                16384

#define  TLS_HSO_PROGRESS_BIT_calculateKeys         0x00000001
#define  TLS_HSO_PROGRESS_BIT_changeCipherR         0x00000002
#define  TLS_HSO_PROGRESS_BIT_changeCipherW         0x00000004
#define  TLS_HSO_PROGRESS_BIT_recvFinish            0x00000008
#define  TLS_HSO_PROGRESS_BIT_sendFinish            0x00000010
#define  TLS_HSO_PROGRESS_BIT_established           0x00000020

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TLSHSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TLSHSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_TLSHSO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_ABORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_ESTABLISH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_AGREE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bResumed
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_CALCULATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_CHANGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_SAVE)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TLSHSO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The TLS Handshake Protocol consists of a suite of three sub-protocols which are used to allow
 * peers to agree upon security parameters for the record layer, authenticate themselves, instan-
 * tiate negotiated security parameters, and report error conditions to each other. The Handshake
 * Protocol is responsible for negotiating a session, which consists of the following items:
 *
 *      session identifier
 *          An arbitrary byte sequence chosen by the server to identify an active or
 *          resumable session state.
 *      peer certificate
 *          X509v3 certificate of the peer. This element of the state may be null.
 *      compression method
 *          The algorithm used to compress data prior to encryption.
 *      cipher spec
 *          Specifies the bulk data encryption algorithm (such as null, DES, etc.) and
 *          a MAC algorithm (such as MD5 or SHA). It also defines cryptographic
 *          attributes such as the hash_size.
 *      master secret
 *          48-byte secret shared between the client and server.
 *      is resumable
 *          A flag indicating whether the session can be used to initiate new connections.
 *
 * These items are then used to create security parameters for use by the Record Layer when
 * protecting application data. Many connections can be instantiated using the same session through
 * the resumption feature of the TLS Handshake Protocol.
 */
#define  TLS_HAND_SHAKER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    TLS_RECORD_CLIENT_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hStateTimerObj;                                         \
    ANSC_HANDLE                     hStateTimerIf;                                          \
    TLS_SESSION_STATE               SessionState;                                           \
    TLS_RECORD_STATE                RecordStateW;                                           \
    TLS_RECORD_STATE                RecordStateR;                                           \
    ULONG                           ProgressBits;                                           \
    ANSC_LOCK                       SyncLock;                                               \
                                                                                            \
    void*                           MsgBuffer;                                              \
    ULONG                           MsgBufferSize;                                          \
    ULONG                           MsgOffset;                                              \
                                                                                            \
    void*                           FragMsgBuffer;                                          \
    ULONG                           FragMsgBufferSize;                                      \
    ULONG                           FragMsgOffset;                                          \
                                                                                            \
    PFN_TLSHSO_START                Start;                                                  \
    PFN_TLSHSO_ABORT                Abort;                                                  \
    PFN_TLSHSO_ESTABLISH            Establish;                                              \
    PFN_TLSHSO_AGREE                Agree;                                                  \
                                                                                            \
    PFN_TLSHSO_CALCULATE            CalKeys30;                                              \
    PFN_TLSHSO_CALCULATE            CalKeys31;                                              \
    PFN_TLSHSO_CALCULATE            CalMasterSecret30;                                      \
    PFN_TLSHSO_CALCULATE            CalMasterSecret31;                                      \
    PFN_TLSHSO_CHANGE               ChangeCipherW;                                          \
    PFN_TLSHSO_CHANGE               ChangeCipherR;                                          \
                                                                                            \
    PFN_TLSHSO_SAVE                 SaveHandshakeMsg;                                       \
    PFN_TLSHSO_SAVE                 SaveFragHandshakeMsg;                                   \
    PFN_TLSHSO_INVOKE               StateTimerInvoke;                                       \
    /* end of object class content */                                                       \

typedef  struct
_TLS_HAND_SHAKER_OBJECT
{
    TLS_HAND_SHAKER_CLASS_CONTENT
}
TLS_HAND_SHAKER_OBJECT,  *PTLS_HAND_SHAKER_OBJECT;

#define  ACCESS_TLS_HAND_SHAKER_OBJECT(p)           \
         ACCESS_CONTAINER(p, TLS_HAND_SHAKER_OBJECT, Linkage)


#endif
