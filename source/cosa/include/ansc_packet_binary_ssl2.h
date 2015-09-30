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

    module:	ansc_packet_binary_ssl2.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of Secure Socket Layer Protocol (SSL) v2.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/18/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_SSL2_
#define  _ANSC_PACKET_BINARY_SSL2_


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
         DATA STRUCTURES USED BY SSL2 RECORD PROTOCOL
***********************************************************/

/*
 * In SSL, all data sent is encapsulated in a record, an object which is composed of a header and
 * some non-zero amount of data. Each record header contains a two or three byte length code. If
 * the most significant bit is set in the first byte of the record length code then the record has
 * no padding and the total header length will be 2 bytes, otherwise the record has padding and the
 * total header length will be 3 bytes. The record header is transmitted before the data portion of
 * the record.
 */
typedef  struct
_SSL2_RECORD_HEADER
{
    UCHAR                           HeaderBit1;
    UCHAR                           HeaderBit2;
    UCHAR                           PadAndData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
SSL2_RECORD_HEADER,  *PSSL2_RECORD_HEADER;

#define  Ssl2RecordValidate(ssl2_record, b_size, v_result)                                  \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size < Ssl2RecordGetHeaderSize(ssl2_record) )                        \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != Ssl2RecordGetSize(ssl2_record) )                             \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  Ssl2RecordGetHeaderSize(p)                 ( (p->HeaderBit1 & 0x80)? 2 : 3                         )
#define  Ssl2RecordGetLength(p)                     ( ((USHORT)(p->HeaderBit1 & 0x7F) << 8) | p->HeaderBit2 )
#define  Ssl2RecordGetSize(p)                       ( Ssl2RecordGetHeaderSize(p) + Ssl2RecordGetLength(p)   )
#define  Ssl2RecordGetData(p)                       ( (PVOID)((ULONG)p + Ssl2RecordGetHeaderSize(p))        )
#define  Ssl2RecordGetMsgType(p)                    ( *(PUCHAR)Ssl2RecordGetData(p)                         )


/***********************************************************
       DATA STRUCTURES USED BY SSL2 HANDSHAKE MESSAGES
***********************************************************/

/*
 * The SSL Handshake Protocol messages are encapsulated in the SSL Record Protocol and are composed
 * of two parts: a single byte message type code, and some data. The client and server exchange
 * messages until both ends have sent their "finished" message, indicating that they are satisfied
 * with the SSL Handshake Protocol conversation. While one end may be finished, the other may not,
 * therefore the finished end must continue to receive SSL Handshake Protocol messages until it too
 * receives a "finished" message. 
 */
#define  SSL2_MSG_TYPE_ERROR                        0
#define  SSL2_MSG_TYPE_CLIENT_HELLO                 1
#define  SSL2_MSG_TYPE_CLIENT_MASTER_KEY            2
#define  SSL2_MSG_TYPE_CLIENT_FINISHED              3
#define  SSL2_MSG_TYPE_SERVER_HELLO                 4
#define  SSL2_MSG_TYPE_SERVER_VERIFY                5
#define  SSL2_MSG_TYPE_SERVER_FINISHED              6
#define  SSL2_MSG_TYPE_REQUEST_CERTIFICATE          7
#define  SSL2_MSG_TYPE_CLIENT_CERTIFICATE           8


/***********************************************************
     DATA STRUCTURES USED BY SSL2 HANDSHAKE CLIENT HELLO
***********************************************************/

/*
 * There are several messages that are only generated by clients. These messages are never
 * generated by correctly functioning servers. A client receiving such a message closes the connec-
 * tion to the server and returns an error status to the application through some unspecified
 * mechanism. 
 *
 *      CLIENT-HELLO (Phase 1; Sent in the clear)
 *
 *          char MSG-CLIENT-HELLO
 *          char CLIENT-VERSION-MSB
 *          char CLIENT-VERSION-LSB
 *          char CIPHER-SPECS-LENGTH-MSB
 *          char CIPHER-SPECS-LENGTH-LSB
 *          char SESSION-ID-LENGTH-MSB
 *          char SESSION-ID-LENGTH-LSB
 *          char CHALLENGE-LENGTH-MSB
 *          char CHALLENGE-LENGTH-LSB
 *          char CIPHER-SPECS-DATA[(MSB<<8)|LSB]
 *          char SESSION-ID-DATA[(MSB<<8)|LSB]
 *          char CHALLENGE-DATA[(MSB<<8)|LSB]
 */
typedef  struct
_SSL2_HS_CLIENT_HELLO
{
    UCHAR                           MsgType;
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    USHORT                          CipherSpecsLength;
    USHORT                          SessionIdLength;
    USHORT                          ChallengeLength;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
SSL2_HS_CLIENT_HELLO,  *PSSL2_HS_CLIENT_HELLO;

#define  Ssl2HsClientHelloValidate(client_hello, b_size, v_result)                          \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size <= sizeof(SSL2_HS_CLIENT_HELLO) )                               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != Ssl2HsClientHelloGetBodySize(client_hello) )                 \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( Ssl2HsClientHelloGetCipherSpecsCount(client_hello) < 1 )               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  Ssl2HsClientHelloGetMsgType(p)             ( p->MsgType      )
#define  Ssl2HsClientHelloGetMajorVersion(p)        ( p->MajorVersion )
#define  Ssl2HsClientHelloGetMinorVersion(p)        ( p->MinorVersion )
#define  Ssl2HsClientHelloGetCipherSpecsLength(p)   ( AnscUshortFromNToH(AnscReadUshort(&p->CipherSpecsLength)) )
#define  Ssl2HsClientHelloGetSessionIdLength(p)     ( AnscUshortFromNToH(AnscReadUshort(&p->SessionIdLength  )) )
#define  Ssl2HsClientHelloGetChallengeLength(p)     ( AnscUshortFromNToH(AnscReadUshort(&p->ChallengeLength  )) )
#define  Ssl2HsClientHelloGetCipherSpecsData(p)     ( (PVOID)((ULONG)p->Data + 0                                       ) )
#define  Ssl2HsClientHelloGetSessionIdData(p)       ( (PVOID)((ULONG)p->Data + Ssl2HsClientHelloGetCipherSpecsLength(p)) )
#define  Ssl2HsClientHelloGetChallengeData(p)       ( (PVOID)((ULONG)p->Data + Ssl2HsClientHelloGetCipherSpecsLength(p) + Ssl2HsClientHelloGetSessionIdLength(p)) )

#define  Ssl2HsClientHelloGetCipherSpecsCount(p)    ( Ssl2HsClientHelloGetCipherSpecsLength(p) / 3 )
#define  Ssl2HsClientHelloGetCipherSpecByIndex(p, index)                                    \
         ( AnscUshortFromNToH(AnscReadUshort((PVOID)((ULONG)Ssl2HsClientHelloGetCipherSpecsData(p) + index * 3 + 1))) )


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
