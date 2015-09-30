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

    module:	ansc_packet_binary_tftp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the Trivial File Transfer Protocol
        (TFTP).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/15/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_TFTP_
#define  _ANSC_PACKET_BINARY_TFTP_


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
                DATA STRUCTURES USED BY TFTP
***********************************************************/

/*
 * The TFTP header consists of a 2 byte opcode field which indicate the packet's type. These
 * opcodes and the format of the various types of packets are defined below.
 */
#define  TFTP_OPCODE_RRQ                            1
#define  TFTP_OPCODE_WRQ                            2
#define  TFTP_OPCODE_DATA                           3
#define  TFTP_OPCODE_ACK                            4
#define  TFTP_OPCODE_ERROR                          5

typedef  struct
_TFTP_HEADER
{
    USHORT                          Opcode;
}_struct_pack_
TFTP_HEADER,  *PTFTP_HEADER;

#define  AnscTftpGetOpcode(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Opcode)) )
#define  AnscTftpSetOpcode(p, c)                    AnscWriteUshort(&p->Opcode, AnscUshortFromHToN(c))

/*
 * RRQ and WRQ packets (opcodes 1 and 2 respectively) have the format shown below. The file name
 * is a sequence of bytes in netascii terminated by a zero byte. The mode field contains the string
 * "netascii", "octet", or "mail" (or any combination of upper and lower case, such as "NETASCII",
 * NetAscii", etc.) in netascii indicating the three modes defined in the protocol. A host which
 * receives netascii mode data must translate the data to its own format. Octet mode is used to
 * transfer a file that is in the 8-bit format of the machine from which the file is being trans-
 * ferred. It is assumed that each type of machine has a single 8-bit format that is more common,
 * and that that format is chosen. For example, on a DEC-20, a 36 bit machine, this is four 8-bit
 * bytes to a word with four bits of breakage. If a host receives a octet file and then returns it,
 * the returned file must be identical to the original. Mail mode uses the name of a mail recipient
 * in place of a file and must begin with a WRQ. Otherwise it is identical to netascii mode. The
 * mail recipient string should be of the form "username" or "username@hostname". If the second
 * form is used, it allows the option of mail forwarding by a relay computer.
 *
 *       2 bytes     string    1 byte     string   1 byte
 *       ------------------------------------------------
 *      | Opcode |  Filename  |   0  |    Mode    |   0  |
 *       ------------------------------------------------
 */
#define  TFTP_MODE_NETASCII                         1
#define  TFTP_MODE_OCTET                            2
#define  TFTP_MODE_MAIL                             3

typedef  struct
_TFTP_RRQ_PACKET
{
    USHORT                          Opcode;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TFTP_RRQ_PACKET,  *PTFTP_RRQ_PACKET;

typedef  struct _TFTP_RRQ_PACKET  TFTP_WRQ_PACKET,  *PTFTP_WRQ_PACKET;

#define  AnscTftpRrqGetOpcode                       AnscTftpGetOpcode
#define  AnscTftpRrqSetOpcode                       AnscTftpSetOpcode

#define  AnscTftpRrqVerifyPacket(p, p_size, v_result)                                       \
         {                                                                                  \
            if ( p_size <= (sizeof(TFTP_RRQ_PACKET) + 2) )                                  \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Data[p_size - sizeof(TFTP_RRQ_PACKET) - 1] != 0 )                  \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( AnscTftpRrqGetFilenameLength(p) >=                                    \
                      (p_size - sizeof(TFTP_RRQ_PACKET) - 2) )                              \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( AnscTftpRrqGetModeLength(p) == 0 )                                    \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }
#define  AnscTftpRrqGetPacketSize(p)                                                        \
         ( sizeof(TFTP_RRQ_PACKET) + AnscTftpRrqGetFilenameLength(p) + 1 + AnscTftpRrqGetModeLength(p) + 1 )
#define  AnscTftpRrqGetFilename(p)                  ( (char*)p->Data                                                )
#define  AnscTftpRrqGetFilenameLength(p)            ( AnscSizeOfString(AnscTftpRrqGetFilename(p))                   )
#define  AnscTftpRrqGetMode(p)                      ( (char*)((ULONG)p->Data + AnscTftpRrqGetFilenameLength(p) + 1) )
#define  AnscTftpRrqGetModeLength(p)                ( AnscSizeOfString(AnscTftpRrqGetMode(p))                       )
#define  AnscTftpRrqGetModecode(p, c)                                                       \
         {                                                                                  \
            char*                   tftp_mode = AnscTftpRrqGetMode(p);                      \
                                                                                            \
            if ( AnscEqualString(tftp_mode, "netascii", FALSE) )                            \
            {                                                                               \
                c = TFTP_MODE_NETASCII;                                                     \
            }                                                                               \
            else if ( AnscEqualString(tftp_mode, "octet", FALSE) )                          \
            {                                                                               \
                c = TFTP_MODE_OCTET;                                                        \
            }                                                                               \
            else if ( AnscEqualString(tftp_mode, "mail", FALSE) )                           \
            {                                                                               \
                c = TFTP_MODE_MAIL;                                                         \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                c = TFTP_MODE_OCTET;                                                        \
            }                                                                               \
         }

#define  AnscTftpRrqSetFilename(p, f)                                                       \
         AnscCopyString(AnscTftpRrqGetFilename(p), f); *(AnscTftpRrqGetFilename(p) + AnscSizeOfString(f)) = 0
#define  AnscTftpRrqSetMode(p, m)                                                           \
         AnscCopyString(AnscTftpRrqGetMode(p), m); *(AnscTftpRrqGetMode(p) + AnscSizeOfString(m)) = 0
#define  AnscTftpRrqSetModecode(p, c)                                                       \
         {                                                                                  \
            switch ( c )                                                                    \
            {                                                                               \
                case    TFTP_MODE_NETASCII :                                                \
                                                                                            \
                        AnscTftpRrqSetMode(p, "netascii");                                  \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    TFTP_MODE_OCTET :                                                   \
                                                                                            \
                        AnscTftpRrqSetMode(p, "octet");                                     \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    TFTP_MODE_MAIL :                                                    \
                                                                                            \
                        AnscTftpRrqSetMode(p, "mail");                                      \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        AnscTftpRrqSetMode(p, "octet");                                     \
                                                                                            \
                        break;                                                              \
            }                                                                               \
         }

#define  AnscTftpWrqGetOpcode                       AnscTftpRrqGetOpcode
#define  AnscTftpWrqSetOpcode                       AnscTftpRrqSetOpcode

#define  AnscTftpWrqVerifyPacket                    AnscTftpRrqVerifyPacket
#define  AnscTftpWrqGetPacketSize                   AnscTftpRrqGetPacketSize
#define  AnscTftpWrqGetFilename                     AnscTftpRrqGetFilename
#define  AnscTftpWrqGetFilenameLength               AnscTftpRrqGetFilenameLength
#define  AnscTftpWrqGetMode                         AnscTftpRrqGetMode
#define  AnscTftpWrqGetModeLength                   AnscTftpRrqGetModeLength
#define  AnscTftpWrqGetModecode                     AnscTftpRrqGetModecode

#define  AnscTftpWrqSetFilename                     AnscTftpRrqSetFilename
#define  AnscTftpWrqSetMode                         AnscTftpRrqSetMode
#define  AnscTftpWrqSetModecode                     AnscTftpRrqSetModecode

/*
 * Data is actually transferred in DATA packets depicted below. DATA packets (opcode = 3) have a
 * block number and data field. The block numbers on data packets begin with one and increase by
 * one for each new block of data. This restriction allows the program to use a single number to
 * discriminate between new packets and duplicates. The data field is from zero to 512 bytes long.
 * If it is 512 bytes long, the block is not the last block of data; if it is from zero to 511
 * bytes long, it signals the end of the transfer.
 *
 *       2 bytes     2 bytes      n bytes
 *       ----------------------------------
 *      | Opcode |   Block #  |   Data     |
 *       ----------------------------------
 */
#define  TFTP_STD_DATA_BLOCK_SIZE                   512

typedef  struct
_TFTP_DATA_PACKET
{
    USHORT                          Opcode;
    USHORT                          BlockNum;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TFTP_DATA_PACKET,  *PTFTP_DATA_PACKET;

#define  AnscTftpDataGetOpcode                      AnscTftpGetOpcode
#define  AnscTftpDataSetOpcode                      AnscTftpSetOpcode

#define  AnscTftpDataVerifyPacket(p, p_size, v_result)                                      \
         {                                                                                  \
            if ( p_size < sizeof(TFTP_DATA_PACKET) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p_size > (sizeof(TFTP_DATA_PACKET) + TFTP_STD_DATA_BLOCK_SIZE) )      \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }
#define  AnscTftpDataGetBlockNum(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->BlockNum)) )
#define  AnscTftpDataSetBlockNum(p, b)              AnscWriteUshort(&p->BlockNum, AnscUshortFromHToN(b))

#define  AnscTftpDataGetData(p)                     ( p->Data )
#define  AnscTftpDataSetData(p, d, s)               AnscCopyMemory(p->Data, d, s)

/*
 * All packets other than duplicate ACK's and those used for termination are acknowledged unless a
 * timeout occurs. Sending a DATA packet is an acknowledgment for the first ACK packet of the
 * previous DATA packet. The WRQ and DATA packets are acknowledged by ACK or ERROR packets, while
 * RRQ and ACK packets are acknowledged by DATA  or ERROR packets. The Figure below depicts an ACK
 * packet; the opcode is 4. The block number in an  ACK echoes the block number of the DATA packet
 * being acknowledged. A WRQ is acknowledged with an ACK packet having a block number of zero.
 *
 *       2 bytes     2 bytes
 *       ---------------------
 *      | Opcode |   Block #  |
 *       ---------------------
 */
typedef  struct
_TFTP_ACK_PACKET
{
    USHORT                          Opcode;
    USHORT                          BlockNum;
}_struct_pack_
TFTP_ACK_PACKET,  *PTFTP_ACK_PACKET;

#define  AnscTftpAckGetOpcode                       AnscTftpGetOpcode
#define  AnscTftpAckSetOpcode                       AnscTftpSetOpcode

#define  AnscTftpAckVerifyPacket(p, p_size, v_result)                                       \
         {                                                                                  \
            if ( p_size != sizeof(TFTP_ACK_PACKET) )                                        \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }
#define  AnscTftpAckGetBlockNum(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->BlockNum)) )
#define  AnscTftpAckSetBlockNum(p, b)               AnscWriteUshort(&p->BlockNum, AnscUshortFromHToN(b))

/*
 * An ERROR packet (opcode 5) takes the form depicted below. An ERROR packet can be the acknowledg-
 * ment of any other type of packet. The error code is an integer indicating the nature of the
 * error. The error message is intended for human consumption, and should be in netascii. Like all
 * other strings, it is terminated with a zero byte.
 *
 *       2 bytes     2 bytes      string    1 byte
 *       -----------------------------------------
 *      | Opcode |  ErrorCode |   ErrMsg   |   0  |
 *       -----------------------------------------
 */
#define  TFTP_ERROR_NOT_DEFINED                     0
#define  TFTP_ERROR_FILE_NOT_FOUND                  1
#define  TFTP_ERROR_ACCESS_VIOLATION                2
#define  TFTP_ERROR_DISK_FULL                       3
#define  TFTP_ERROR_ILLEGAL_OPERATION               4
#define  TFTP_ERROR_UNKNOWN_TID                     5
#define  TFTP_ERROR_FILE_ALREADY_EXISTS             6
#define  TFTP_ERROR_NO_SUCH_USER                    7

typedef  struct
_TFTP_ERROR_PACKET
{
    USHORT                          Opcode;
    USHORT                          ErrorCode;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TFTP_ERROR_PACKET,  *PTFTP_ERROR_PACKET;

#define  AnscTftpErrorGetOpcode                     AnscTftpGetOpcode
#define  AnscTftpErrorSetOpcode                     AnscTftpSetOpcode

#define  AnscTftpErrorGetErrorCode(p)               ( AnscUshortFromNToH(AnscReadUshort(&p->ErrorCode)) )
#define  AnscTftpErrorSetErrorCode(p, e)            AnscWriteUshort(&p->ErrorCode, AnscUshortFromHToN(e))

#define  AnscTftpErrorVerifyPacket(p, p_size, v_result)                                     \
         {                                                                                  \
            if ( p_size < (sizeof(TFTP_ERROR_PACKET) + 1) )                                 \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( p->Data[p_size - sizeof(TFTP_ERROR_PACKET) - 1] != 0 )                \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
         }
#define  AnscTftpErrorGetPacketSize(p)              ( sizeof(TFTP_ERROR_PACKET) + AnscTftpErrorGetErrorMsgLength(p) + 1 )
#define  AnscTftpErrorGetErrorMsg(p)                ( (char*)p->Data                                                    )
#define  AnscTftpErrorGetErrorMsgLength(p)          ( AnscSizeOfString(AnscTftpErrorGetErrorMsg(p))                     )

#define  AnscTftpErrorSetErrorMsg(p, m)                                                     \
         AnscCopyString(AnscTftpErrorGetErrorMsg(p), m); *(AnscTftpErrorGetErrorMsg(p) + AnscSizeOfString(m)) = 0


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
