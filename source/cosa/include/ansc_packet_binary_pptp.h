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

    module:	ansc_packet_binary_pptp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Point-to-Point Tunneling Protocol
        (PPTP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/18/01    modify the file name from ansc_protocol_binary
                    to ansc_packet_binary.
        01/19/01    restructure the container object, add and
                    remove a few things
        09/11/01    add ppp related packet structures
        10/03/01    break it up into several header files; one for
                    each specific protocol suite

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_PPTP_
#define  _ANSC_PACKET_BINARY_PPTP_


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4200)                     /* nonstandard extension used: zero-sized array in struct/union */
#endif


/*
 * All network protocol messages must be defined without any packing. While most protocols were
 * designed with the packing in mind, i.e. multi-byte values are always aligned on the desired
 * boudary, there're plenty protocols didn't follow this rule. Following are two of typical ways
 * to control the byte bounary in C/C++ programs:
 */
#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
                 DATA STRUCTURES USED BY PPTP
***********************************************************/

/*
 * Each PPTP Control Connection message begins with an 8 octet fixed header portion. This fixed
 * header contains the following: the total length of the message, the PPTP Message Type indicator,
 * and a "Magic Cookie".
 */
#define  PPTP_MESSAGE_TYPE_CONTROL                  0x0001
#define  PPTP_MESSAGE_TYPE_MANAGEMENT               0x0002

#define  PPTP_MAGIC_COOKIE                          0x1A2B3C4D
#define  PPTP_PROTOCOL_VERSION                      0x0100

/*
 * Control Connection Messages are used to establish and clear user sessions. The first set of
 * Control Connection messages are used to maintain the control connection itself. The control
 * connection is initiated by either the PNS or PAC after they establish the underlying TCP
 * connection.
 */
#define  PPTP_CTRL_MESSAGE_SCC_REQUEST              1
#define  PPTP_CTRL_MESSAGE_SCC_REPLY                2
#define  PPTP_CTRL_MESSAGE_TCC_REQUEST              3
#define  PPTP_CTRL_MESSAGE_TCC_REPLY                4
#define  PPTP_CTRL_MESSAGE_ECHO_REQUEST             5
#define  PPTP_CTRL_MESSAGE_ECHO_REPLY               6

#define  PPTP_CTRL_MESSAGE_OC_REQUEST               7
#define  PPTP_CTRL_MESSAGE_OC_REPLY                 8
#define  PPTP_CTRL_MESSAGE_IC_REQUEST               9
#define  PPTP_CTRL_MESSAGE_IC_REPLY                 10
#define  PPTP_CTRL_MESSAGE_IC_CONNECTED             11
#define  PPTP_CTRL_MESSAGE_CC_REQUEST               12
#define  PPTP_CTRL_MESSAGE_CD_NOTIFY                13

#define  PPTP_CTRL_MESSAGE_WE_NOTIFY                14
#define  PPTP_CTRL_MESSAGE_SL_INFO                  15

/*
 * PPTP defines a set of messages sent as TCP data on the control connection between a PNS and a
 * given PAC. The TCP session for the control session is established by initiating a TCP connection
 * to port 1723. The source port is assigned to any unused port number.
 */
typedef  struct
_PPTP_HEADER
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
}_struct_pack_
PPTP_HEADER,  *PPPTP_HEADER;

#define  AnscPptpGetLength(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Length     )) )
#define  AnscPptpGetMessageType(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->MessageType)) )
#define  AnscPptpGetMagicCookie(p)                  ( AnscUlongFromNToH (AnscReadUlong (&p->MagicCookie)) )
#define  AnscPptpGetControlType(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->ControlType)) )
#define  AnscPptpGetReserved0(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved0  )) )
#define  AnscPptpGetReserved1(p)                    ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1  )) )
#define  AnscPptpIsAligned(p)                       ( AnscPptpGetMagicCookie(p) == 0x1A2B3C4D )

#define  AnscPptpSetLength(p, l)                    AnscWriteUshort(&p->Length,      AnscUshortFromHToN(l))
#define  AnscPptpSetMessageType(p, t)               AnscWriteUshort(&p->MessageType, AnscUshortFromHToN(t))
#define  AnscPptpSetMagicCookie(p, c)               AnscWriteUlong (&p->MagicCookie, AnscUlongFromHToN (c))
#define  AnscPptpSetControlType(p, t)               AnscWriteUshort(&p->ControlType, AnscUshortFromHToN(t))
#define  AnscPptpSetReserved0(p, r)                 AnscWriteUshort(&p->Reserved0,   AnscUshortFromHToN(r))
#define  AnscPptpSetReserved1(p, r)                 AnscWriteUshort(&p->Reserved1,   AnscUshortFromHToN(r))


/*
 * The Start-Control-Connection-Request is a PPTP control message used to establish the control
 * connection between a PNS and a PAC. Each PNS-PAC pair requires a dedicated control connection
 * to be established. A control connection must be established before any other PPTP messages
 * can be issued.
 */
#define  PPTP_HOST_NAME_SIZE                        64
#define  PPTP_VENDOR_STRING_SIZE                    64

typedef  struct
_PPTP_SCC_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          Version;
    USHORT                          Reserved1;
    ULONG                           FramingCapabilities;
    ULONG                           BearerCapabilities;
    USHORT                          MaxChannels;
    USHORT                          FirmwareRevision;
    char                            HostName    [PPTP_HOST_NAME_SIZE];
    char                            VendorString[PPTP_VENDOR_STRING_SIZE];
}_struct_pack_
PPTP_SCC_REQUEST,  *PPPTP_SCC_REQUEST;

#define  AnscPptpSccRequestGetLength(p)             AnscPptpGetLength(p)
#define  AnscPptpSccRequestGetMessageType(p)        AnscPptpGetMessageType(p)
#define  AnscPptpSccRequestGetMagicCookie(p)        AnscPptpGetMagicCookie(p)
#define  AnscPptpSccRequestGetControlType(p)        AnscPptpGetControlType(p)
#define  AnscPptpSccRequestGetReserved0(p)          AnscPptpGetReserved0(p)

#define  AnscPptpSccRequestSetLength(p, l)          AnscPptpSetLength(p, l)
#define  AnscPptpSccRequestSetMessageType(p, t)     AnscPptpSetMessageType(p, t)
#define  AnscPptpSccRequestSetMagicCookie(p, c)     AnscPptpSetMagicCookie(p, c)
#define  AnscPptpSccRequestSetControlType(p, t)     AnscPptpSetControlType(p, t)
#define  AnscPptpSccRequestSetReserved0(p, r)       AnscPptpSetReserved0(p, r)

#define  AnscPptpSccRequestGetVersion(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->Version            )) )
#define  AnscPptpSccRequestGetReserved1(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1          )) )
#define  AnscPptpSccRequestGetFraming(p)            ( AnscUlongFromNToH (AnscReadUlong (&p->FramingCapabilities)) )
#define  AnscPptpSccRequestGetBearer(p)             ( AnscUlongFromNToH (AnscReadUlong (&p->BearerCapabilities )) )
#define  AnscPptpSccRequestGetMaxChannels(p)        ( AnscUshortFromNToH(AnscReadUshort(&p->MaxChannels        )) )
#define  AnscPptpSccRequestGetFwRevision(p)         ( AnscUshortFromNToH(AnscReadUshort(&p->FirmwareRevision   )) )
#define  AnscPptpSccRequestGetHostName(p)           ( p->HostName )
#define  AnscPptpSccRequestGetVendorString(p)       ( p->VendorString )

#define  AnscPptpSccRequestSetVersion(p, v)         AnscWriteUshort(&p->Version,             AnscUshortFromHToN(v))
#define  AnscPptpSccRequestSetReserved1(p, r)       AnscWriteUshort(&p->Reserved1,           AnscUshortFromHToN(r))
#define  AnscPptpSccRequestSetFraming(p, f)         AnscWriteUlong (&p->FramingCapabilities, AnscUlongFromHToN (f))
#define  AnscPptpSccRequestSetBearer(p, b)          AnscWriteUlong (&p->BearerCapabilities,  AnscUlongFromHToN (b))
#define  AnscPptpSccRequestSetMaxChannels(p, n)     AnscWriteUshort(&p->MaxChannels,         AnscUshortFromHToN(n))
#define  AnscPptpSccRequestSetFwRevision(p, r)      AnscWriteUshort(&p->FirmwareRevision,    AnscUshortFromHToN(r))
#define  AnscPptpSccRequestSetHostName(p, s)        AnscCopyString (&p->HostName[0],         s                    )
#define  AnscPptpSccRequestSetVendorString(p, s)    AnscCopyString (&p->VendorString[0],     s                    )

/*
 * The Start-Control-Connection Reply is a PPTP control message sent in reply to a received Start-
 * Control-Connection-Request message. This message contains a result code indicating the result
 * of the control connection establishment attempt.
 */
typedef  struct
_PPTP_SCC_REPLY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          Version;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    ULONG                           FramingCapabilities;
    ULONG                           BearerCapabilities;
    USHORT                          MaxChannels;
    USHORT                          FirmwareRevision;
    char                            HostName    [PPTP_HOST_NAME_SIZE];
    char                            VendorString[PPTP_VENDOR_STRING_SIZE];
}_struct_pack_
PPTP_SCC_REPLY,  *PPPTP_SCC_REPLY;

#define  AnscPptpSccReplyGetLength(p)               AnscPptpGetLength(p)
#define  AnscPptpSccReplyGetMessageType(p)          AnscPptpGetMessageType(p)
#define  AnscPptpSccReplyGetMagicCookie(p)          AnscPptpGetMagicCookie(p)
#define  AnscPptpSccReplyGetControlType(p)          AnscPptpGetControlType(p)
#define  AnscPptpSccReplyGetReserved0(p)            AnscPptpGetReserved0(p)

#define  AnscPptpSccReplySetLength(p, l)            AnscPptpSetLength(p, l)
#define  AnscPptpSccReplySetMessageType(p, t)       AnscPptpSetMessageType(p, t)
#define  AnscPptpSccReplySetMagicCookie(p, c)       AnscPptpSetMagicCookie(p, c)
#define  AnscPptpSccReplySetControlType(p, t)       AnscPptpSetControlType(p, t)
#define  AnscPptpSccReplySetReserved0(p, r)         AnscPptpSetReserved0(p, r)

#define  AnscPptpSccReplyGetVersion(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Version            )) )
#define  AnscPptpSccReplyGetResultCode(p)           ( p->ResultCode )
#define  AnscPptpSccReplyGetErrorCode(p)            ( p->ErrorCode  )
#define  AnscPptpSccReplyGetFraming(p)              ( AnscUlongFromNToH (AnscReadUlong (&p->FramingCapabilities)) )
#define  AnscPptpSccReplyGetBearer(p)               ( AnscUlongFromNToH (AnscReadUlong (&p->BearerCapabilities )) )
#define  AnscPptpSccReplyGetMaxChannels(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->MaxChannels        )) )
#define  AnscPptpSccReplyGetFwRevision(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->FirmwareRevision   )) )
#define  AnscPptpSccReplyGetHostName(p)             ( p->HostName     )
#define  AnscPptpSccReplyGetVendorString(p)         ( p->VendorString )

#define  AnscPptpSccReplySetVersion(p, v)           AnscWriteUshort(&p->Version,             AnscUshortFromHToN(v))
#define  AnscPptpSccReplySetResultCode(p, c)        ( p->ResultCode = c )
#define  AnscPptpSccReplySetErrorCode(p, c)         ( p->ErrorCode  = c )
#define  AnscPptpSccReplySetFraming(p, f)           AnscWriteUlong (&p->FramingCapabilities, AnscUlongFromHToN (f))
#define  AnscPptpSccReplySetBearer(p, b)            AnscWriteUlong (&p->BearerCapabilities,  AnscUlongFromHToN (b))
#define  AnscPptpSccReplySetMaxChannels(p, n)       AnscWriteUshort(&p->MaxChannels,         AnscUshortFromHToN(n))
#define  AnscPptpSccReplySetFwRevision(p, r)        AnscWriteUshort(&p->FirmwareRevision,    AnscUshortFromHToN(r))
#define  AnscPptpSccReplySetHostName(p, s)          AnscCopyString (&p->HostName[0],         s                    )
#define  AnscPptpSccReplySetVendorString(p, s)      AnscCopyString (&p->VendorString[0],     s                    )

/*
 * The Stop-Control-Connection Request is a PPTP control message sent by one peer of a PAC-PNS
 * control connection to inform the other peer that the control connection should be closed. In
 * addition to closing the control connection, all active user calls are implicitly cleared. The
 * reason for issuing this request is indicated in the Reason field.
 */
typedef  struct
_PPTP_TCC_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    UCHAR                           Reason;
    UCHAR                           Reserved1;
    USHORT                          Reserved2;
}_struct_pack_
PPTP_TCC_REQUEST,  *PPPTP_TCC_REQUEST;

#define  AnscPptpTccRequestGetLength(p)             AnscPptpGetLength(p)
#define  AnscPptpTccRequestGetMessageType(p)        AnscPptpGetMessageType(p)
#define  AnscPptpTccRequestGetMagicCookie(p)        AnscPptpGetMagicCookie(p)
#define  AnscPptpTccRequestGetControlType(p)        AnscPptpGetControlType(p)
#define  AnscPptpTccRequestGetReserved0(p)          AnscPptpGetReserved0(p)

#define  AnscPptpTccRequestSetLength(p, l)          AnscPptpSetLength(p, l)
#define  AnscPptpTccRequestSetMessageType(p, t)     AnscPptpSetMessageType(p, t)
#define  AnscPptpTccRequestSetMagicCookie(p, c)     AnscPptpSetMagicCookie(p, c)
#define  AnscPptpTccRequestSetControlType(p, t)     AnscPptpSetControlType(p, t)
#define  AnscPptpTccRequestSetReserved0(p, r)       AnscPptpSetReserved0(p, r)

#define  AnscPptpTccRequestGetReason(p)             ( p->Reason )
#define  AnscPptpTccRequestGetReserved1(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1)) )
#define  AnscPptpTccRequestGetReserved2(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved2)) )

#define  AnscPptpTccRequestSetReason(p, r)          ( p->Reason = r )
#define  AnscPptpTccRequestSetReserved1(p, r)       AnscWriteUshort(&p->Reserved1, AnscUshortFromHToN(r))
#define  AnscPptpTccRequestSetReserved2(p, r)       AnscWriteUshort(&p->Reserved2, AnscUshortFromHToN(r))

/*
 * The Stop-Control-Connection-Reply is a PPTP control message sent by one peer of a PAC-PNS
 * control connection upon receipt of a Stop-Control-Connection-Request from the other peer.
 */
typedef  struct
_PPTP_TCC_REPLY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    USHORT                          Reserved1;
}_struct_pack_
PPTP_TCC_REPLY,  *PPPTP_TCC_REPLY;

#define  AnscPptpTccReplyGetLength(p)               AnscPptpGetLength(p)
#define  AnscPptpTccReplyGetMessageType(p)          AnscPptpGetMessageType(p)
#define  AnscPptpTccReplyGetMagicCookie(p)          AnscPptpGetMagicCookie(p)
#define  AnscPptpTccReplyGetControlType(p)          AnscPptpGetControlType(p)
#define  AnscPptpTccReplyGetReserved0(p)            AnscPptpGetReserved0(p)

#define  AnscPptpTccReplySetLength(p, l)            AnscPptpSetLength(p, l)
#define  AnscPptpTccReplySetMessageType(p, t)       AnscPptpSetMessageType(p, t)
#define  AnscPptpTccReplySetMagicCookie(p, c)       AnscPptpSetMagicCookie(p, c)
#define  AnscPptpTccReplySetControlType(p, t)       AnscPptpSetControlType(p, t)
#define  AnscPptpTccReplySetReserved0(p, r)         AnscPptpSetReserved0(p, r)

#define  AnscPptpTccReplyGetResultCode(p)           ( p->ResultCode )
#define  AnscPptpTccReplyGetErrorCode(p)            ( p->ErrorCode  )
#define  AnscPptpTccReplyGetReserved1(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1)) )

#define  AnscPptpTccReplySetResultCode(p, c)        ( p->ResultCode = c )
#define  AnscPptpTccReplySetErrorCode(p, c)         ( p->ErrorCode  = c )
#define  AnscPptpTccReplySetReserved1(p, r)         AnscWriteUshort(&p->Reserved1, AnscUshortFromHToN(r))

/*
 * The Echo-Request is a PPTP control message sent by either peer of a PAC-PNS control connection.
 * This control message is used as a "keep-alive" for the control connection. The receiving peer
 * issues an Echo-Reply to each Echo-Request received.
 */
typedef  struct
_PPTP_ECHO_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    ULONG                           Identifier;
}_struct_pack_
PPTP_ECHO_REQUEST,  *PPPTP_ECHO_REQUEST;

#define  AnscPptpEchoRequestGetLength(p)            AnscPptpGetLength(p)
#define  AnscPptpEchoRequestGetMessageType(p)       AnscPptpGetMessageType(p)
#define  AnscPptpEchoRequestGetMagicCookie(p)       AnscPptpGetMagicCookie(p)
#define  AnscPptpEchoRequestGetControlType(p)       AnscPptpGetControlType(p)
#define  AnscPptpEchoRequestGetReserved0(p)         AnscPptpGetReserved0(p)

#define  AnscPptpEchoRequestSetLength(p, l)         AnscPptpSetLength(p, l)
#define  AnscPptpEchoRequestSetMessageType(p, t)    AnscPptpSetMessageType(p, t)
#define  AnscPptpEchoRequestSetMagicCookie(p, c)    AnscPptpSetMagicCookie(p, c)
#define  AnscPptpEchoRequestSetControlType(p, t)    AnscPptpSetControlType(p, t)
#define  AnscPptpEchoRequestSetReserved0(p, r)      AnscPptpSetReserved0(p, r)

#define  AnscPptpEchoRequestGetIdentifier(p)        ( AnscUlongFromNToH(AnscReadUlong(&p->Identifier)) )

#define  AnscPptpEchoRequestSetIdentifier(p, id)    AnscWriteUlong(&p->Identifier, AnscUlongFromHToN(id))

/*
 * The Echo-Reply is a PPTP control message sent by either peer of a PAC-PNS control connection
 * in response to the receipt of an Echo-Request.
 */
typedef  struct
_PPTP_ECHO_REPLY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    ULONG                           Identifier;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    USHORT                          Reserved1;
}_struct_pack_
PPTP_ECHO_REPLY,  *PPPTP_ECHO_REPLY;

#define  AnscPptpEchoReplyGetLength(p)              AnscPptpGetLength(p)
#define  AnscPptpEchoReplyGetMessageType(p)         AnscPptpGetMessageType(p)
#define  AnscPptpEchoReplyGetMagicCookie(p)         AnscPptpGetMagicCookie(p)
#define  AnscPptpEchoReplyGetControlType(p)         AnscPptpGetControlType(p)
#define  AnscPptpEchoReplyGetReserved0(p)           AnscPptpGetReserved0(p)

#define  AnscPptpEchoReplySetLength(p, l)           AnscPptpSetLength(p, l)
#define  AnscPptpEchoReplySetMessageType(p, t)      AnscPptpSetMessageType(p, t)
#define  AnscPptpEchoReplySetMagicCookie(p, c)      AnscPptpSetMagicCookie(p, c)
#define  AnscPptpEchoReplySetControlType(p, t)      AnscPptpSetControlType(p, t)
#define  AnscPptpEchoReplySetReserved0(p, r)        AnscPptpSetReserved0(p, r)

#define  AnscPptpEchoReplyGetIdentifier(p)          ( AnscUlongFromNToH (AnscReadUlong (&p->Identifier)) )
#define  AnscPptpEchoReplyGetResultCode(p)          ( p->ResultCode )
#define  AnscPptpEchoReplyGetErrorCode(p)           ( p->ErrorCode  )
#define  AnscPptpEchoReplyGetReserved1(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1 )) )

#define  AnscPptpEchoReplySetIdentifier(p, id)      AnscWriteUlong (&p->Identifier, AnscUlongFromHToN (id))
#define  AnscPptpEchoReplySetResultCode(p, c)       ( p->ResultCode = c )
#define  AnscPptpEchoReplySetErrorCode(p, c)        ( p->ErrorCode  = c )
#define  AnscPptpEchoReplySetReserved1(p, r)        AnscWriteUshort(&p->Reserved1,  AnscUshortFromHToN(r ))

/*
 * The Outgoing-Call-Request is a PPTP control message sent by the PNS to the PAC to indicate that
 * an outbound call from the PAC is to be established. This request provides the PAC with
 * information required to make the call. It also provides information to the PAC that is used to
 * regulate the transmission of data to the PNS for this session once it is established.
 */
#define  PPTP_PHONE_NUMBER_SIZE                     64
#define  PPTP_SUBADDRESS_SIZE                       64

typedef  struct
_PPTP_OC_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    USHORT                          CallSerialNumber;
    ULONG                           MinBps;
    ULONG                           MaxBps;
    ULONG                           BearerType;
    ULONG                           FramingType;
    USHORT                          RecvWindowSize;
    USHORT                          ProcessingDelay;
    USHORT                          PhoneNumberLength;
    USHORT                          Reserved1;
    char                            PhoneNumber[PPTP_PHONE_NUMBER_SIZE];
    char                            Subaddress [PPTP_SUBADDRESS_SIZE];
}_struct_pack_
PPTP_OC_REQUEST,  *PPPTP_OC_REQUEST;

#define  AnscPptpOcRequestGetLength(p)              AnscPptpGetLength(p)
#define  AnscPptpOcRequestGetMessageType(p)         AnscPptpGetMessageType(p)
#define  AnscPptpOcRequestGetMagicCookie(p)         AnscPptpGetMagicCookie(p)
#define  AnscPptpOcRequestGetControlType(p)         AnscPptpGetControlType(p)
#define  AnscPptpOcRequestGetReserved0(p)           AnscPptpGetReserved0(p)

#define  AnscPptpOcRequestSetLength(p, l)           AnscPptpSetLength(p, l)
#define  AnscPptpOcRequestSetMessageType(p, t)      AnscPptpSetMessageType(p, t)
#define  AnscPptpOcRequestSetMagicCookie(p, c)      AnscPptpSetMagicCookie(p, c)
#define  AnscPptpOcRequestSetControlType(p, t)      AnscPptpSetControlType(p, t)
#define  AnscPptpOcRequestSetReserved0(p, r)        AnscPptpSetReserved0(p, r)

#define  AnscPptpOcRequestGetCallId(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->CallId          )) )
#define  AnscPptpOcRequestGetCallSerialNumber(p)    ( AnscUshortFromNToH(AnscReadUshort(&p->CallSerialNumber)) )
#define  AnscPptpOcRequestGetMinBps(p)              ( AnscUlongFromNToH (AnscReadUlong (&p->MinBps          )) )
#define  AnscPptpOcRequestGetMaxBps(p)              ( AnscUlongFromNToH (AnscReadUlong (&p->MaxBps          )) )
#define  AnscPptpOcRequestGetBearerType(p)          ( AnscUlongFromNToH (AnscReadUlong (&p->BearerType      )) )
#define  AnscPptpOcRequestGetFramingType(p)         ( AnscUlongFromNToH (AnscReadUlong (&p->FramingType     )) )
#define  AnscPptpOcRequestGetWindowSize(p)          ( AnscUshortFromNToH(AnscReadUshort(&p->RecvWindowSize  )) )
#define  AnscPptpOcRequestGetDelay(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->ProcessingDelay )) )

#define  AnscPptpOcRequestSetCallId(p, id)          AnscWriteUshort(&p->CallId,             AnscUshortFromHToN(id))
#define  AnscPptpOcRequestSetCallSerialNumber(p, n) AnscWriteUshort(&p->CallSerialNumber,   AnscUshortFromHToN(n ))
#define  AnscPptpOcRequestSetMinBps(p, n)           AnscWriteUlong (&p->MinBps,             AnscUlongFromHToN (n ))
#define  AnscPptpOcRequestSetMaxBps(p, n)           AnscWriteUlong (&p->MaxBps,             AnscUlongFromHToN (n ))
#define  AnscPptpOcRequestSetBearerType(p, t)       AnscWriteUlong (&p->BearerType,         AnscUlongFromHToN (t ))
#define  AnscPptpOcRequestSetFramingType(p, t)      AnscWriteUlong (&p->FramingType,        AnscUlongFromHToN (t ))
#define  AnscPptpOcRequestSetWindowSize(p, s)       AnscWriteUshort(&p->RecvWindowSize,     AnscUshortFromHToN(s ))
#define  AnscPptpOcRequestSetDelay(p, d)            AnscWriteUshort(&p->ProcessingDelay,    AnscUshortFromHToN(d ))

#define  AnscPptpOcRequestSetPhoneNumberLength(p, n)                                        \
         AnscWriteUshort(&p->PhoneNumberLength,  AnscUshortFromHToN(n ))
#define  AnscPptpOcRequestSetPhoneNumber(p, s, l)                                           \
         {                                                                                  \
            AnscCopyMemory(&p->PhoneNumber, s, l);                                          \
            AnscPptpOcRequestSetPhoneNumberLength(p, l);                                    \
         }
#define  AnscPptpOcRequestSetSubaddress(p, s, l)    AnscCopyMemory (&p->Subaddress, s, l) 

/*
 * The Outgoing-Call-Reply is a PPTP control message sent by the PAC to the PNS in response to a
 * received Outgoing-Call-Request message. The reply indicates the result of the outgoing call
 * attempt. It also provides information to the PNS about particular parameters used for the call.
 * It provides information to allow the PNS to regulate the transmission of data to the PAC for
 * this session.
 */
typedef  struct
_PPTP_OC_REPLY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    USHORT                          PeerCallId;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    USHORT                          CauseCode;
    ULONG                           ConnectSpeed;
    USHORT                          RecvWindowSize;
    USHORT                          ProcessingDelay;
    ULONG                           PhysicalChannelId;
}_struct_pack_
PPTP_OC_REPLY,  *PPPTP_OC_REPLY;

#define  AnscPptpOcReplyGetLength(p)                AnscPptpGetLength(p)
#define  AnscPptpOcReplyGetMessageType(p)           AnscPptpGetMessageType(p)
#define  AnscPptpOcReplyGetMagicCookie(p)           AnscPptpGetMagicCookie(p)
#define  AnscPptpOcReplyGetControlType(p)           AnscPptpGetControlType(p)
#define  AnscPptpOcReplyGetReserved0(p)             AnscPptpGetReserved0(p)

#define  AnscPptpOcReplySetLength(p, l)             AnscPptpSetLength(p, l)
#define  AnscPptpOcReplySetMessageType(p, t)        AnscPptpSetMessageType(p, t)
#define  AnscPptpOcReplySetMagicCookie(p, c)        AnscPptpSetMagicCookie(p, c)
#define  AnscPptpOcReplySetControlType(p, t)        AnscPptpSetControlType(p, t)
#define  AnscPptpOcReplySetReserved0(p, r)          AnscPptpSetReserved0(p, r)

#define  AnscPptpOcReplyGetCallId(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->CallId           )) )
#define  AnscPptpOcReplyGetPeerCallId(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->PeerCallId       )) )
#define  AnscPptpOcReplyGetResultCode(p)            ( p->ResultCode )
#define  AnscPptpOcReplyGetErrorCode(p)             ( p->ErrorCode  )
#define  AnscPptpOcReplyGetCauseCode(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->CauseCode        )) )
#define  AnscPptpOcReplyGetConnectSpeed(p)          ( AnscUlongFromNToH (AnscReadUlong (&p->ConnectSpeed     )) )
#define  AnscPptpOcReplyGetWindowSize(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->RecvWindowSize   )) )
#define  AnscPptpOcReplyGetDelay(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->ProcessingDelay  )) )
#define  AnscPptpOcReplyGetChannelId(p)             ( AnscUlongFromNToH (AnscReadUlong (&p->PhysicalChannelId)) )

#define  AnscPptpOcReplySetCallId(p, id)            AnscWriteUshort(&p->CallId,            AnscUshortFromHToN(id))
#define  AnscPptpOcReplySetPeerCallId(p, id)        AnscWriteUshort(&p->PeerCallId,        AnscUshortFromHToN(id))
#define  AnscPptpOcReplySetResultCode(p, c)         ( p->ResultCode = c )
#define  AnscPptpOcReplySetErrorCode(p, c)          ( p->ErrorCode  = c )
#define  AnscPptpOcReplySetCauseCode(p, c)          AnscWriteUshort(&p->CauseCode,         AnscUshortFromHToN(c ))
#define  AnscPptpOcReplySetConnectSpeed(p, s)       AnscWriteUlong (&p->ConnectSpeed,      AnscUlongFromHToN (s ))
#define  AnscPptpOcReplySetWindowSize(p, s)         AnscWriteUshort(&p->RecvWindowSize,    AnscUshortFromHToN(s ))
#define  AnscPptpOcReplySetDelay(p, d)              AnscWriteUshort(&p->ProcessingDelay,   AnscUshortFromHToN(d ))
#define  AnscPptpOcReplySetChannelId(p, id)         AnscWriteUlong (&p->PhysicalChannelId, AnscUlongFromHToN (id))

/*
 * The Incoming-Call-Request is a PPTP control message sent by the PAC to the PNS to indicate that
 * an inbound call is to be established from the PAC. This request provides the PNS with parameter
 * information for the incoming call.
 */
typedef  struct
_PPTP_IC_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    USHORT                          CallSerialNumber;
    ULONG                           BearerType;
    ULONG                           PhysicalChannelId;
    USHORT                          DialedNumberLength;
    USHORT                          DialingNumberLength;
    char                            DialedNumber [PPTP_PHONE_NUMBER_SIZE];
    char                            DialingNumber[PPTP_PHONE_NUMBER_SIZE];
    char                            Subaddress   [PPTP_SUBADDRESS_SIZE];
}_struct_pack_
PPTP_IC_REQUEST,  *PPPTP_IC_REQUEST;

#define  AnscPptpIcRequestGetLength(p)              AnscPptpGetLength(p)
#define  AnscPptpIcRequestGetMessageType(p)         AnscPptpGetMessageType(p)
#define  AnscPptpIcRequestGetMagicCookie(p)         AnscPptpGetMagicCookie(p)
#define  AnscPptpIcRequestGetControlType(p)         AnscPptpGetControlType(p)
#define  AnscPptpIcRequestGetReserved0(p)           AnscPptpGetReserved0(p)

#define  AnscPptpIcRequestSetLength(p, l)           AnscPptpSetLength(p, l)
#define  AnscPptpIcRequestSetMessageType(p, t)      AnscPptpSetMessageType(p, t)
#define  AnscPptpIcRequestSetMagicCookie(p, c)      AnscPptpSetMagicCookie(p, c)
#define  AnscPptpIcRequestSetControlType(p, t)      AnscPptpSetControlType(p, t)
#define  AnscPptpIcRequestSetReserved0(p, r)        AnscPptpSetReserved0(p, r)

#define  AnscPptpIcRequestGetCallId(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->CallId           )) )
#define  AnscPptpIcRequestGetCallSerialNumber(p)    ( AnscUshortFromNToH(AnscReadUshort(&p->CallSerialNumber )) )
#define  AnscPptpIcRequestGetBearerType(p)          ( AnscUlongFromNToH (AnscReadUlong (&p->BearerType       )) )
#define  AnscPptpIcRequestGetChannelId(p)           ( AnscUlongFromNToH (AnscReadUlong (&p->PhysicalChannelId)) )
#define  AnscPptpIcRequestGetDialedNumberLength(p)  ( AnscUshortFromNToH(AnscReadUshort(&p->DialedNumberLength )) )
#define  AnscPptpIcRequestGetDialingNumberLength(p) ( AnscUshortFromNToH(AnscReadUshort(&p->DialingNumberLength)) )
#define  AnscPptpIcRequestGetDialedNumber(p)        ( p->DialedNumber )
#define  AnscPptpIcRequestGetDialingNumber(p)       ( p->DialingNumber)
#define  AnscPptpIcRequestGetSubaddress(p)          ( p->Subaddress   )

#define  AnscPptpIcRequestSetCallId(p, id)          AnscWriteUshort(&p->CallId,            AnscUshortFromHToN(id))
#define  AnscPptpIcRequestSetCallSerialNumber(p, n) AnscWriteUshort(&p->CallSerialNumber,  AnscUshortFromHToN(n ))
#define  AnscPptpIcRequestSetBearerType(p, t)       AnscWriteUlong (&p->BearerType,        AnscUlongFromHToN (t ))
#define  AnscPptpIcRequestSetChannelId(p, id)       AnscWriteUlong (&p->PhysicalChannelId, AnscUlongFromHToN (id))

#define  AnscPptpIcRequestSetDialedNumberLength(p, n)                                       \
         AnscWriteUshort(&p->DialedNumberLength,  AnscUshortFromHToN(n ))
#define  AnscPptpIcRequestSetDialingNumberLength(p, n)                                      \
         AnscWriteUshort(&p->DialingNumberLength,  AnscUshortFromHToN(n ))
#define  AnscPptpIcRequestSetDialedNumber(p, s, l)                                          \
         {                                                                                  \
            AnscCopyMemory(&p->DialedNumber, s, l);                                         \
            AnscPptpIcRequestSetDialedNumberLength(p, l);                                   \
         }
#define  AnscPptpIcRequestSetDialingNumber(p, s, l)                                         \
         {                                                                                  \
            AnscCopyMemory(&p->DialingNumber, s, l);                                        \
            AnscPptpIcRequestSetDialingNumberLength(p, l);                                  \
         }
#define  AnscPptpIcRequestSetSubaddress(p, s, l)    AnscCopyMemory (&p->Subaddress, s, l) 

/*
 * The Incoming-Call-Reply is a PPTP control message sent by the PNS to the PAC in response to a
 * received Incoming-Call-Request message. The reply indicates the result of hte incoming call
 * attempt. It also provides information to allow the PAC to regulate the transmission of data to
 * the PNS for this session.
 */
typedef  struct
_PPTP_IC_REPLY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    USHORT                          PeerCallId;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    USHORT                          RecvWindowSize;
    USHORT                          TransmitDelay;
    USHORT                          Reserved1;
}_struct_pack_
PPTP_IC_REPLY,  *PPPTP_IC_REPLY;

#define  AnscPptpIcReplyGetLength(p)                AnscPptpGetLength(p)
#define  AnscPptpIcReplyGetMessageType(p)           AnscPptpGetMessageType(p)
#define  AnscPptpIcReplyGetMagicCookie(p)           AnscPptpGetMagicCookie(p)
#define  AnscPptpIcReplyGetControlType(p)           AnscPptpGetControlType(p)
#define  AnscPptpIcReplyGetReserved0(p)             AnscPptpGetReserved0(p)

#define  AnscPptpIcReplySetLength(p, l)             AnscPptpSetLength(p, l)
#define  AnscPptpIcReplySetMessageType(p, t)        AnscPptpSetMessageType(p, t)
#define  AnscPptpIcReplySetMagicCookie(p, c)        AnscPptpSetMagicCookie(p, c)
#define  AnscPptpIcReplySetControlType(p, t)        AnscPptpSetControlType(p, t)
#define  AnscPptpIcReplySetReserved0(p, r)          AnscPptpSetReserved0(p, r)

#define  AnscPptpIcReplyGetCallId(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->CallId        )) )
#define  AnscPptpIcReplyGetPeerCallId(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->PeerCallId    )) )
#define  AnscPptpIcReplyGetResultCode(p)            ( p->ResultCode )
#define  AnscPptpIcReplyGetErrorCode(p)             ( p->ErrorCode  )
#define  AnscPptpIcReplyGetWindowSize(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->RecvWindowSize)) )
#define  AnscPptpIcReplyGetDelay(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->TransmitDelay )) )
#define  AnscPptpIcReplyGetReserved1(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1     )) )

#define  AnscPptpIcReplySetCallId(p, id)            AnscWriteUshort(&p->CallId,         AnscUshortFromHToN(id))
#define  AnscPptpIcReplySetPeerCallId(p, id)        AnscWriteUshort(&p->PeerCallId,     AnscUshortFromHToN(id))
#define  AnscPptpIcReplySetResultCode(p, c)         ( p->ResultCode = c )
#define  AnscPptpIcReplySetErrorCode(p, c)          ( p->ErrorCode  = c )
#define  AnscPptpIcReplySetWindowSize(p, s)         AnscWriteUshort(&p->RecvWindowSize, AnscUshortFromHToN(s ))
#define  AnscPptpIcReplySetDelay(p, d)              AnscWriteUshort(&p->TransmitDelay,  AnscUshortFromHToN(d ))
#define  AnscPptpIcReplySetReserved1(p, r)          AnscWriteUshort(&p->Reserved1,      AnscUshortFromHToN(r ))

/*
 * The Incoming-Call-Connected message is a PPTP control message sent by the PAC to the PNS in
 * response to a received Incoming-Call-Reply. It provides information to the PNS about particular
 * parameters used for ths call. It also provides information to allow the PNS to regulate the
 * transmission of data to the PAC for this session.
 */
typedef  struct
_PPTP_IC_CONNECTED
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          PeerCallId;
    USHORT                          Reserved1;
    ULONG                           ConnectSpeed;
    USHORT                          RecvWindowSize;
    USHORT                          TransmitDelay;
    ULONG                           FramingType;
}_struct_pack_
PPTP_IC_CONNECTED,  *PPPTP_IC_CONNECTED;

#define  AnscPptpIcConnectedGetLength(p)            AnscPptpGetLength(p)
#define  AnscPptpIcConnectedGetMessageType(p)       AnscPptpGetMessageType(p)
#define  AnscPptpIcConnectedGetMagicCookie(p)       AnscPptpGetMagicCookie(p)
#define  AnscPptpIcConnectedGetControlType(p)       AnscPptpGetControlType(p)
#define  AnscPptpIcConnectedGetReserved0(p)         AnscPptpGetReserved0(p)

#define  AnscPptpIcConnectedSetLength(p, l)         AnscPptpSetLength(p, l)
#define  AnscPptpIcConnectedSetMessageType(p, t)    AnscPptpSetMessageType(p, t)
#define  AnscPptpIcConnectedSetMagicCookie(p, c)    AnscPptpSetMagicCookie(p, c)
#define  AnscPptpIcConnectedSetControlType(p, t)    AnscPptpSetControlType(p, t)
#define  AnscPptpIcConnectedSetReserved0(p, r)      AnscPptpSetReserved0(p, r)

#define  AnscPptpIcConnectedGetPeerCallId(p)        ( AnscUshortFromNToH(AnscReadUshort(&p->PeerCallId    )) )
#define  AnscPptpIcConnectedGetReserved1(p)         AnscPptpGetReserved1(p)
#define  AnscPptpIcConnectedGetConnectSpeed(p)      ( AnscUlongFromNToH (AnscReadUlong (&p->ConnectSpeed  )) )
#define  AnscPptpIcConnectedGetWindowSize(p)        ( AnscUshortFromNToH(AnscReadUshort(&p->RecvWindowSize)) )
#define  AnscPptpIcConnectedGetDelay(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->TransmitDelay )) )
#define  AnscPptpIcConnectedGetFramingType(p)       ( AnscUlongFromNToH (AnscReadUlong (&p->FramingType   )) )

#define  AnscPptpIcConnectedSetPeerCallId(p, id)    AnscWriteUshort(&p->PeerCallId,     AnscUshortFromHToN(id))
#define  AnscPptpIcConnectedSetReserved1(p, r)      AnscPptpSetReserved1(p, r)
#define  AnscPptpIcConnectedSetConnectSpeed(p, s)   AnscWriteUlong (&p->ConnectSpeed,   AnscUlongFromHToN (s ))
#define  AnscPptpIcConnectedSetWindowSize(p, s)     AnscWriteUshort(&p->RecvWindowSize, AnscUshortFromHToN(s ))
#define  AnscPptpIcConnectedSetDelay(p, d)          AnscWriteUshort(&p->TransmitDelay,  AnscUshortFromHToN(d ))
#define  AnscPptpIcConnectedSetFramingType(p, t)    AnscWriteUlong (&p->FramingType,    AnscUlongFromHToN (t ))

/*
 * The Call-Clear-Request is a PPTP control message sent by the PNS to the PAC indicating that a
 * particular call is to be disconnected. The call being cleared can be either an incoming or
 * outgoing call, in any state. The PAC responds to this message with a Call-Disconnect-Notify
 * message.
 */
typedef  struct
_PPTP_CC_REQUEST
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    USHORT                          Reserved1;
}_struct_pack_
PPTP_CC_REQUEST,  *PPPTP_CC_REQUEST;

#define  AnscPptpCcRequestGetLength(p)              AnscPptpGetLength(p)
#define  AnscPptpCcRequestGetMessageType(p)         AnscPptpGetMessageType(p)
#define  AnscPptpCcRequestGetMagicCookie(p)         AnscPptpGetMagicCookie(p)
#define  AnscPptpCcRequestGetControlType(p)         AnscPptpGetControlType(p)
#define  AnscPptpCcRequestGetReserved0(p)           AnscPptpGetReserved0(p)

#define  AnscPptpCcRequestSetLength(p, l)           AnscPptpSetLength(p, l)
#define  AnscPptpCcRequestSetMessageType(p, t)      AnscPptpSetMessageType(p, t)
#define  AnscPptpCcRequestSetMagicCookie(p, c)      AnscPptpSetMagicCookie(p, c)
#define  AnscPptpCcRequestSetControlType(p, t)      AnscPptpSetControlType(p, t)
#define  AnscPptpCcRequestSetReserved0(p, r)        AnscPptpSetReserved0(p, r)

#define  AnscPptpCcRequestGetCallId(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->CallId   )) )
#define  AnscPptpCcRequestGetReserved1(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1)) )

#define  AnscPptpCcRequestSetCallId(p, id)          AnscWriteUshort(&p->CallId,    AnscUshortFromHToN(id))
#define  AnscPptpCcRequestSetReserved1(p, r)        AnscWriteUshort(&p->Reserved1, AnscUshortFromHToN(r ))

/*
 * The Call-Disconnect-Notify message is a PPTP control message sent by the PAC to the PNS. It is
 * issued whenever a call is disconnected, due to the receipt by the PAC of a Call-Clear-Request
 * or for any other reason. Its purpose is to inform the PNS of both the disconnection and the
 * reason for it.
 */
#define  PPTP_CALL_STATISTICS_SIZE                  128

typedef  struct
_PPTP_CD_NOTIFY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          CallId;
    UCHAR                           ResultCode;
    UCHAR                           ErrorCode;
    USHORT                          CauseCode;
    USHORT                          Reserved1;
    char                            CallStatistics[PPTP_CALL_STATISTICS_SIZE];
}_struct_pack_
PPTP_CD_NOTIFY,  *PPPTP_CD_NOTIFY;

#define  AnscPptpCdNotifyGetLength(p)               AnscPptpGetLength(p)
#define  AnscPptpCdNotifyGetMessageType(p)          AnscPptpGetMessageType(p)
#define  AnscPptpCdNotifyGetMagicCookie(p)          AnscPptpGetMagicCookie(p)
#define  AnscPptpCdNotifyGetControlType(p)          AnscPptpGetControlType(p)
#define  AnscPptpCdNotifyGetReserved0(p)            AnscPptpGetReserved0(p)

#define  AnscPptpCdNotifySetLength(p, l)            AnscPptpSetLength(p, l)
#define  AnscPptpCdNotifySetMessageType(p, t)       AnscPptpSetMessageType(p, t)
#define  AnscPptpCdNotifySetMagicCookie(p, c)       AnscPptpSetMagicCookie(p, c)
#define  AnscPptpCdNotifySetControlType(p, t)       AnscPptpSetControlType(p, t)
#define  AnscPptpCdNotifySetReserved0(p, r)         AnscPptpSetReserved0(p, r)

#define  AnscPptpCdNotifyGetCallId(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->CallId   )) )
#define  AnscPptpCdNotifyGetResultCode(p)           ( p->ResultCode )
#define  AnscPptpCdNotifyGetErrorCode(p)            ( p->ErrorCode  )
#define  AnscPptpCdNotifyGetCauseCode(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->CauseCode)) )
#define  AnscPptpCdNotifyGetReserved1(p)            ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1)) )

#define  AnscPptpCdNotifySetCallId(p, id)           AnscWriteUshort(&p->CallId,    AnscUshortFromHToN(id))
#define  AnscPptpCdNotifySetResultCode(p, c)        ( p->ResultCode = c )
#define  AnscPptpCdNotifySetErrorCode(p, c)         ( p->ErrorCode  = c )
#define  AnscPptpCdNotifySetCauseCode(p, c)         AnscWriteUshort(&p->CauseCode, AnscUshortFromHToN(c ))
#define  AnscPptpCdNotifySetReserved1(p, r)         AnscWriteUshort(&p->Reserved1, AnscUshortFromHToN(r ))

/*
 * The WAN-Error-Notify message is a PPTP control message sent by the PAC to the PNS to indicate
 * WAN error conditions (conditions that occur on the interface supporting PPP). The counters in
 * this message are cumulative. This message should only be sent when an error occurs, and not
 * more than once every 60 seconds. The counters are reset when a new call is established.
 */
typedef  struct
_PPTP_WE_NOTIFY
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          PeerCallId;
    USHORT                          Reserved1;
    ULONG                           CrcErrors;
    ULONG                           FramingErrors;
    ULONG                           HardwareOverruns;
    ULONG                           BufferOverruns;
    ULONG                           TimeoutErrors;
    ULONG                           AlignmentErrors;
}_struct_pack_
PPTP_WE_NOTIFY,  *PPPTP_WE_NOTIFY;

#define  AnscPptpWeNotifyGetLength(p)               AnscPptpGetLength(p)
#define  AnscPptpWeNotifyGetMessageType(p)          AnscPptpGetMessageType(p)
#define  AnscPptpWeNotifyGetMagicCookie(p)          AnscPptpGetMagicCookie(p)
#define  AnscPptpWeNotifyGetControlType(p)          AnscPptpGetControlType(p)
#define  AnscPptpWeNotifyGetReserved0(p)            AnscPptpGetReserved0(p)

#define  AnscPptpWeNotifySetLength(p, l)            AnscPptpSetLength(p, l)
#define  AnscPptpWeNotifySetMessageType(p, t)       AnscPptpSetMessageType(p, t)
#define  AnscPptpWeNotifySetMagicCookie(p, c)       AnscPptpSetMagicCookie(p, c)
#define  AnscPptpWeNotifySetControlType(p, t)       AnscPptpSetControlType(p, t)
#define  AnscPptpWeNotifySetReserved0(p, r)         AnscPptpSetReserved0(p, r)

#define  AnscPptpWeNotifyGetPeerCallId(p)           ( AnscUshortFromNToH(AnscReadUshort(&p->PeerCallId)) )

#define  AnscPptpWeNotifySetPeerCallId(p, id)       AnscWriteUshort(&p->PeerCallId, AnscUshortFromHToN(id))

/*
 * The Set-Link-Info message is a PPTP control message sent by the PNS to the PAC to set PPP-
 * negotiated options. Because these options can change at any time during the life of the call,
 * the PAC must be able to update its internal call information dynamically andp erform PPP
 * negotiation on an active PPP session.
 */
typedef  struct
_PPTP_SL_INFO
{
    USHORT                          Length;
    USHORT                          MessageType;
    ULONG                           MagicCookie;
    USHORT                          ControlType;
    USHORT                          Reserved0;
    USHORT                          PeerCallId;
    USHORT                          Reserved1;
    ULONG                           SendAccm;
    ULONG                           RecvAccm;
}_struct_pack_
PPTP_SL_INFO,  *PPPTP_SL_INFO;

#define  AnscPptpSlInfoGetLength(p)                 AnscPptpGetLength(p)
#define  AnscPptpSlInfoGetMessageType(p)            AnscPptpGetMessageType(p)
#define  AnscPptpSlInfoGetMagicCookie(p)            AnscPptpGetMagicCookie(p)
#define  AnscPptpSlInfoGetControlType(p)            AnscPptpGetControlType(p)
#define  AnscPptpSlInfoGetReserved0(p)              AnscPptpGetReserved0(p)

#define  AnscPptpSlInfoSetLength(p, l)              AnscPptpSetLength(p, l)
#define  AnscPptpSlInfoSetMessageType(p, t)         AnscPptpSetMessageType(p, t)
#define  AnscPptpSlInfoSetMagicCookie(p, c)         AnscPptpSetMagicCookie(p, c)
#define  AnscPptpSlInfoSetControlType(p, t)         AnscPptpSetControlType(p, t)
#define  AnscPptpSlInfoSetReserved0(p, r)           AnscPptpSetReserved0(p, r)

#define  AnscPptpSlInfoGetPeerCallId(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->PeerCallId)) )
#define  AnscPptpSlInfoGetReserved1(p)              ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved1 )) )
#define  AnscPptpSlInfoGetSendAccm(p)               ( AnscUlongFromNToH (AnscReadUshort(&p->SendAccm  )) )
#define  AnscPptpSlInfoGetRecvAccm(p)               ( AnscUlongFromNToH (AnscReadUshort(&p->RecvAccm  )) )

#define  AnscPptpSlInfoSetPeerCallId(p, id)         AnscWriteUshort(&p->PeerCallId, AnscUshortFromHToN(id))
#define  AnscPptpSlInfoSetReserved1(p, r)           AnscWriteUshort(&p->Reserved1,  AnscUshortFromHToN(r ))
#define  AnscPptpSlInfoSetSendAccm(p, sa)           AnscWriteUlong (&p->SendAccm,   AnscUlongFromHToN (sa))
#define  AnscPptpSlInfoSetRecvAccm(p, ra)           AnscWriteUlong (&p->RecvAccm,   AnscUlongFromHToN (ra))


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
