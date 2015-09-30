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

    module:	ansc_packet_binary_stun.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2008
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of the STUN Protocol - Simple Traversal
        of User Datagram Protocol (UDP) Through Network Address
        Translators (NATs).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/08    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_STUN_
#define  _ANSC_PACKET_BINARY_STUN_


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
          DATA STRUCTURES USED BY STUN MESSAGE HEADER
***********************************************************/

/*
 * STUN messages are TLV (type-length-value) encoded using big endian (network ordered) binary. All
 * STUN messages start with a STUN header, followed by a STUN payload. The payload is a series of
 * STUN attributes, the set of which depends on the message type. The STUN header contains a STUN
 * message type, transaction ID, and length. The message type can be Binding Request, Binding
 * Response, Binding Error Response, Shared Secret Request, Shared Secret Response, or Shared
 * Secret Error Response. The transaction ID is used to correlate requests and responses. The
 * length indicates the total length of the STUN payload, not including the header. This allows
 * STUN to run over TCP. Shared Secret Requests are always sent over TCP (indeed, using TLS over
 * TCP).
 */
#define  STUN_MAX_MSG_SIZE                          256
#define  STUN_MIN_MSG_SIZE                          sizeof(STUN_HEADER)

/*
 * STUN is a request-response protocol. Clients send a request, and the server sends a response.
 * There are two requests, Binding Request, and Shared Secret Request. The response to a Binding
 * Request can either be the Binding Response or Binding Error Response. The response to a Shared
 * Secret Request can either be a Shared Secret Response or a Shared Secret Error Response.
 */
#define  STUN_MSG_TYPE_BindingRequest               0x0001
#define  STUN_MSG_TYPE_BindingResponse              0x0101
#define  STUN_MSG_TYPE_BindingErrorResponse         0x0111
#define  STUN_MSG_TYPE_SharedSecretRequest          0x0002
#define  STUN_MSG_TYPE_SharedSecretResponse         0x0102
#define  STUN_MSG_TYPE_SharedSecretErrorResponse    0x0112

/*
 * The following response codes, along with their recommended reason phrases (in brackets) are
 * defined at this time:
 * 
 *    400 (Bad Request): The request was malformed.  The client should not
 *         retry the request without modification from the previous
 *         attempt.
 * 
 *    401 (Unauthorized): The Binding Request did not contain a MESSAGE-
 *         INTEGRITY attribute.
 * 
 *    420 (Unknown Attribute): The server did not understand a mandatory
 *         attribute in the request.
 * 
 *    430 (Stale Credentials): The Binding Request did contain a MESSAGE-
 *         INTEGRITY attribute, but it used a shared secret that has
 *         expired.  The client should obtain a new shared secret and try
 *         again.
 * 
 *    431 (Integrity Check Failure): The Binding Request contained a
 *         MESSAGE-INTEGRITY attribute, but the HMAC failed verification.
 *         This could be a sign of a potential attack, or client
 *         implementation error.
 * 
 *    432 (Missing Username): The Binding Request contained a MESSAGE-
 *         INTEGRITY attribute, but not a USERNAME attribute.  Both must be
 *         present for integrity checks.
 * 
 *    433 (Use TLS): The Shared Secret request has to be sent over TLS, but
 *         was not received over TLS.
 * 
 *    500 (Server Error): The server has suffered a temporary error. The
 *         client should try again.
 * 
 *    600 (Global Failure:) The server is refusing to fulfill the request.
 *         The client should not retry.
 */
#define  STUN_ERROR_CODE_BadRequest                 400
#define  STUN_ERROR_CODE_Unauthorized               401
#define  STUN_ERROR_CODE_UnknownAttribute           420
#define  STUN_ERROR_CODE_StaleCredentials           430
#define  STUN_ERROR_CODE_IntegrityCheckFailure      431
#define  STUN_ERROR_CODE_MissingUsername            432
#define  STUN_ERROR_CODE_UseTLS                     433
#define  STUN_ERROR_CODE_ServerError                500
#define  STUN_ERROR_CODE_GlobalFailure              600

/*
 * All STUN messages consist of a 20 byte header:
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |      STUN Message Type        |         Message Length        |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                             Transaction ID
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                                                    |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_STUN_HEADER
{
    USHORT                          MessageType;
    USHORT                          MessageLength;
    UCHAR                           TransactionID[16];
    UCHAR                           Attributes[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
STUN_HEADER,  *PSTUN_HEADER;

#define  AnscStunGetMessageType(p)                  ( AnscUshortFromNToH(AnscReadUshort(&p->MessageType  )) )
#define  AnscStunGetMessageLength(p)                ( AnscUshortFromNToH(AnscReadUshort(&p->MessageLength)) )
#define  AnscStunGetMessageSize(p)                  ( AnscStunGetMessageLength(p) + sizeof(STUN_HEADER)     )
#define  AnscStunGetTransactionID(p)                ( p->TransactionID                                      )

#define  AnscStunSetMessageType(p, t)               AnscWriteUshort(&p->MessageType,   AnscUshortFromHToN(t))
#define  AnscStunSetMessageLength(p, l)             AnscWriteUshort(&p->MessageLength, AnscUshortFromHToN(l))
#define  AnscStunSetTransactionID(p, id)            AnscCopyMemory(p->TransactionID, id, 16)

#define  AnscStunHasAttributes(p)                   ( AnscStunGetMessageLength(p) > 0 )

#define  AnscStunGetFirstAttribute(stun_header, stun_attrib)                                \
         {                                                                                  \
            if ( AnscStunHasAttributes(stun_header) )                                       \
            {                                                                               \
                stun_attrib = (PSTUN_ATTRIBUTE)(stun_header->Attributes);                   \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                stun_attrib = NULL;                                                         \
            }                                                                               \
         }

#define  AnscStunGetNextAttribute(stun_header, this_attrib, next_attrib)                    \
         {                                                                                  \
            if ( !this_attrib )                                                             \
            {                                                                               \
                if ( AnscStunHasAttributes(stun_header) )                                   \
                {                                                                           \
                    next_attrib = (PSTUN_ATTRIBUTE)stun_header->Attributes;                 \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    next_attrib = NULL;                                                     \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                next_attrib = AnscStunAttribGetNextAttrib(this_attrib);                     \
            }                                                                               \
                                                                                            \
            if ( ((ULONG)next_attrib - (ULONG)stun_header) >=                               \
                 AnscStunGetMessageSize(stun_header) )                                      \
            {                                                                               \
                next_attrib = NULL;                                                         \
            }                                                                               \
         }

#define  AnscStunGetAttributeByType(stun_header1, type1, stun_attrib1)                      \
         {                                                                                  \
            PSTUN_ATTRIBUTE         cur_attrib1 = NULL;                                     \
            PSTUN_ATTRIBUTE         cur_attrib2 = NULL;                                     \
                                                                                            \
            AnscStunGetFirstAttribute(stun_header1, cur_attrib1);                           \
                                                                                            \
            while ( cur_attrib1 )                                                           \
            {                                                                               \
                if ( AnscStunAttribGetType(cur_attrib1) == type1 )                          \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                AnscStunGetNextAttribute(stun_header1, cur_attrib1, cur_attrib2);           \
                                                                                            \
                cur_attrib1 = cur_attrib2;                                                  \
            }                                                                               \
                                                                                            \
            stun_attrib1 = cur_attrib1;                                                     \
         }


/***********************************************************
       DATA STRUCTURES USED BY STUN MESSAGE ATTRIBUTES
***********************************************************/

/*
 * The following types are defined:
 * 
 *      0x0001: MAPPED-ADDRESS
 *      0x0002: RESPONSE-ADDRESS
 *      0x0003: CHANGE-REQUEST
 *      0x0004: SOURCE-ADDRESS
 *      0x0005: CHANGED-ADDRESS
 *      0x0006: USERNAME
 *      0x0007: PASSWORD
 *      0x0008: MESSAGE-INTEGRITY
 *      0x0009: ERROR-CODE
 *      0x000a: UNKNOWN-ATTRIBUTES
 *      0x000b: REFLECTED-FROM
 * 
 * To allow future revisions of this specification to add new attributes if needed, the attribute
 * space is divided into optional and mandatory ones. Attributes with values greater than 0x7fff
 * are optional, which means that the message can be processed by the client or server even though
 * the attribute is not understood. Attributes with values less than or equal to 0x7fff are
 * mandatory to understand, which means that the client or server cannot process the message unless
 * it understands the attribute.
 */
#define  STUN_ATTRIB_TYPE_MAPPED_ADDRESS            0x0001
#define  STUN_ATTRIB_TYPE_RESPONSE_ADDRESS          0x0002
#define  STUN_ATTRIB_TYPE_CHANGE_REQUEST            0x0003
#define  STUN_ATTRIB_TYPE_SOURCE_ADDRESS            0x0004
#define  STUN_ATTRIB_TYPE_CHANGED_ADDRESS           0x0005
#define  STUN_ATTRIB_TYPE_USERNAME                  0x0006
#define  STUN_ATTRIB_TYPE_PASSWORD                  0x0007
#define  STUN_ATTRIB_TYPE_MSG_INTEGRITY             0x0008
#define  STUN_ATTRIB_TYPE_ERROR_CODE                0x0009
#define  STUN_ATTRIB_TYPE_UNKNOWN_ATTRIBUTES        0x000A
#define  STUN_ATTRIB_TYPE_REFLECTED_FROM            0x000B

#define  STUN_ATTRIB_TYPE_XOR_MAPPED_ADDRESS        0x8020

#define  STUN_ATTRIB_TYPE_CONN_REQUEST_BINDING      0xC001
#define  STUN_ATTRIB_TYPE_BINDING_CHANGE            0xC002

/*
 * After the header are 0 or more attributes.  Each attribute is TLV encoded, with a 16 bit type,
 * 16 bit length, and variable value:
 * 
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |         Type                  |            Length             |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                             Value                             ....
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_STUN_ATTRIBUTE
{
    USHORT                          Type;
    USHORT                          Length;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
STUN_ATTRIBUTE,  *PSTUN_ATTRIBUTE;

#define  AnscStunAttribGetType(p)                   ( AnscUshortFromNToH(AnscReadUshort(&p->Type  )) )
#define  AnscStunAttribGetLength(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  AnscStunAttribGetSize(p)                   ( AnscStunAttribGetLength(p) + sizeof(STUN_ATTRIBUTE) )
#define  AnscStunAttribGetValue(p)                  ( p->Value  )

#define  AnscStunAttribSetType(p, t)                AnscWriteUshort(&p->Type,   AnscUshortFromHToN(t))
#define  AnscStunAttribSetLength(p, l)              AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  AnscStunAttribSetValue(p, v)               AnscCopyMemory(p->Value, v, AnscStunAttribGetLength(p))

#define  AnscStunAttribGetNextAttrib(p)             ( (PSTUN_ATTRIBUTE)((ULONG)p + AnscStunAttribGetSize(p)) )

/*
 * The MAPPED-ADDRESS attribute indicates the mapped IP address and port. It consists of an eight
 * bit address family, and a sixteen bit port, followed by a fixed length value representing the IP
 * address.
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |x x x x x x x x|    Family     |           Port                |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                             Address                           |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * The port is a network byte ordered representation of the mapped port. The address family is
 * always 0x01, corresponding to IPv4. The first 8 bits of the MAPPED-ADDRESS are ignored, for the
 * purposes of aligning parameters on natural boundaries. The IPv4 address is 32 bits.
 */
typedef  struct
_STUN_ATTRIB_MAPPED_ADDRESS
{
    USHORT                          Type;
    USHORT                          Length;
    UCHAR                           Reserved;
    UCHAR                           Family;
    USHORT                          Port;
    ANSC_IPV4_ADDRESS               Address;
}_struct_pack_
STUN_ATTRIB_MAPPED_ADDRESS,  *PSTUN_ATTRIB_MAPPED_ADDRESS;

#define  AnscStunAttribMaGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribMaGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribMaGetReserved(p)             ( p->Reserved )
#define  AnscStunAttribMaGetFamily(p)               ( p->Family   )
#define  AnscStunAttribMaGetPort(p)                 ( AnscUshortFromNToH(AnscReadUshort(&p->Port)) )
#define  AnscStunAttribMaGetAddressValue(p)         ( AnscReadUlong(&p->Address.Value) )

#define  AnscStunAttribMaSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribMaSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribMaSetReserved(p, r)          ( p->Reserved = r )
#define  AnscStunAttribMaSetFamily(p, f)            ( p->Family   = f )
#define  AnscStunAttribMaSetPort(p, o)              AnscWriteUshort(&p->Port, AnscUshortFromHToN(o))
#define  AnscStunAttribMaSetAddressValue(p, addr)   AnscWriteUlong (&p->Address.Value, addr)

/*
 * The RESPONSE-ADDRESS attribute indicates where the response to a Binding Request should be sent.
 * Its syntax is identical to MAPPED-ADDRESS.
 */
typedef  struct
_STUN_ATTRIB_MAPPED_ADDRESS  STUN_ATTRIB_RESPONSE_ADDRESS,  *PSTUN_ATTRIB_RESPONSE_ADDRESS;

#define  AnscStunAttribRaGetType(p)                 AnscStunAttribMaGetType(p)
#define  AnscStunAttribRaGetLength(p)               AnscStunAttribMaGetLength(p)
#define  AnscStunAttribRaGetReserved(p)             AnscStunAttribMaGetReserved(p)
#define  AnscStunAttribRaGetFamily(p)               AnscStunAttribMaGetFamily(p)
#define  AnscStunAttribRaGetPort(p)                 AnscStunAttribMaGetPort(p)
#define  AnscStunAttribRaGetAddressValue(p)         AnscStunAttribMaGetAddressValue(p)

#define  AnscStunAttribRaSetType(p, t)              AnscStunAttribMaSetType(p, t)
#define  AnscStunAttribRaSetLength(p, l)            AnscStunAttribMaSetLength(p, l)
#define  AnscStunAttribRaSetReserved(p, r)          AnscStunAttribMaSetReserved(p, r)
#define  AnscStunAttribRaSetFamily(p, f)            AnscStunAttribMaSetFamily(p, f)
#define  AnscStunAttribRaSetPort(p, o)              AnscStunAttribMaSetPort(p, o)
#define  AnscStunAttribRaSetAddressValue(p, addr)   AnscStunAttribMaSetAddressValue(p, addr)

/*
 * The CHANGED-ADDRESS attribute indicates the IP address and port where responses would have been
 * sent from if the "change IP" and "change port" flags had been set in the CHANGE-REQUEST
 * attribute of the Binding Request. The attribute is always present in a Binding Response,
 * independent of the value of the flags. Its syntax is identical to MAPPED-ADDRESS.
 */
typedef  struct
_STUN_ATTRIB_MAPPED_ADDRESS  STUN_ATTRIB_CHANGED_ADDRESS,  *PSTUN_ATTRIB_CHANGED_ADDRESS;

#define  AnscStunAttribCaGetType(p)                 AnscStunAttribMaGetType(p)
#define  AnscStunAttribCaGetLength(p)               AnscStunAttribMaGetLength(p)
#define  AnscStunAttribCaGetReserved(p)             AnscStunAttribMaGetReserved(p)
#define  AnscStunAttribCaGetFamily(p)               AnscStunAttribMaGetFamily(p)
#define  AnscStunAttribCaGetPort(p)                 AnscStunAttribMaGetPort(p)
#define  AnscStunAttribCaGetAddressValue(p)         AnscStunAttribMaGetAddressValue(p)

#define  AnscStunAttribCaSetType(p, t)              AnscStunAttribMaSetType(p, t)
#define  AnscStunAttribCaSetLength(p, l)            AnscStunAttribMaSetLength(p, l)
#define  AnscStunAttribCaSetReserved(p, r)          AnscStunAttribMaSetReserved(p, r)
#define  AnscStunAttribCaSetFamily(p, f)            AnscStunAttribMaSetFamily(p, f)
#define  AnscStunAttribCaSetPort(p, o)              AnscStunAttribMaSetPort(p, o)
#define  AnscStunAttribCaSetAddressValue(p, addr)   AnscStunAttribMaSetAddressValue(p, addr)

/*
 * The CHANGE-REQUEST attribute is used by the client to request that the server use a different
 * address and/or port when sending the response. The attribute is 32 bits long, although only two
 * bits (A and B) are used:
 * 
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 A B 0|
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * The meaning of the flags is:
 * 
 *    A: This is the "change IP" flag.  If true, it requests the server
 *       to send the Binding Response with a different IP address than the
 *       one the Binding Request was received on.
 * 
 *    B: This is the "change port" flag.  If true, it requests the
 *       server to send the Binding Response with a different port than the
 *       one the Binding Request was received on.
 */
#define  STUN_CHANGE_REQUEST_FLAG_changeIP          0x04
#define  STUN_CHANGE_REQUEST_FLAG_changePort        0x02

typedef  struct
_STUN_ATTRIB_CHANGE_REQUEST
{
    USHORT                          Type;
    USHORT                          Length;
    UCHAR                           Reserved1;
    UCHAR                           Reserved2;
    UCHAR                           Reserved3;
    UCHAR                           Flags;
}_struct_pack_
STUN_ATTRIB_CHANGE_REQUEST,  *PSTUN_ATTRIB_CHANGE_REQUEST;

#define  AnscStunAttribCrGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribCrGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribCrGetReserved1(p)            ( p->Reserved1 )
#define  AnscStunAttribCrGetReserved2(p)            ( p->Reserved2 )
#define  AnscStunAttribCrGetReserved3(p)            ( p->Reserved3 )
#define  AnscStunAttribCrGetFlags(p)                ( p->Flags     )

#define  AnscStunAttribCrSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribCrSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribCrSetReserved1(p, r)         ( p->Reserved1 = r )
#define  AnscStunAttribCrSetReserved2(p, r)         ( p->Reserved2 = r )
#define  AnscStunAttribCrSetReserved3(p, r)         ( p->Reserved3 = r )
#define  AnscStunAttribCrSetFlags(p, f)             ( p->Flags     = f )

/*
 * The SOURCE-ADDRESS attribute is present in Binding Responses. It indicates the source IP address
 * and port that the server is sending the response from. Its syntax is identical to that of MAPPED-
 * ADDRESS.
 */
typedef  struct
_STUN_ATTRIB_MAPPED_ADDRESS  STUN_ATTRIB_SOURCE_ADDRESS,  *PSTUN_ATTRIB_SOURCE_ADDRESS;

#define  AnscStunAttribSaGetType(p)                 AnscStunAttribMaGetType(p)
#define  AnscStunAttribSaGetLength(p)               AnscStunAttribMaGetLength(p)
#define  AnscStunAttribSaGetReserved(p)             AnscStunAttribMaGetReserved(p)
#define  AnscStunAttribSaGetFamily(p)               AnscStunAttribMaGetFamily(p)
#define  AnscStunAttribSaGetPort(p)                 AnscStunAttribMaGetPort(p)
#define  AnscStunAttribSaGetAddressValue(p)         AnscStunAttribMaGetAddressValue(p)

#define  AnscStunAttribSaSetType(p, t)              AnscStunAttribMaSetType(p, t)
#define  AnscStunAttribSaSetLength(p, l)            AnscStunAttribMaSetLength(p, l)
#define  AnscStunAttribSaSetReserved(p, r)          AnscStunAttribMaSetReserved(p, r)
#define  AnscStunAttribSaSetFamily(p, f)            AnscStunAttribMaSetFamily(p, f)
#define  AnscStunAttribSaSetPort(p, o)              AnscStunAttribMaSetPort(p, o)
#define  AnscStunAttribSaSetAddressValue(p, addr)   AnscStunAttribMaSetAddressValue(p, addr)

/*
 * The USERNAME attribute is used for message integrity. It serves as a means to identify the
 * shared secret used in the message integrity check. The USERNAME is always present in a Shared
 * Secret Response, along with the PASSWORD. It is optionally present in a Binding Request when
 * message integrity is used. The value of USERNAME is a variable length opaque value. Its length
 * MUST be a multiple of 4 (measured in bytes) in order to guarantee alignment of attributes on
 * word boundaries.
 */
typedef  struct
_STUN_ATTRIBUTE  STUN_ATTRIB_USERNAME,  *PSTUN_ATTRIB_USERNAME;

#define  AnscStunAttribUnGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribUnGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribUnGetValue(p)                AnscStunAttribGetValue(p)

#define  AnscStunAttribUnSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribUnSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribUnSetValue(p, v)             AnscStunAttribSetValue(p, v)

/*
 * The PASSWORD attribute is used in Shared Secret Responses. It is always present in a Shared
 * Secret Response, along with the USERNAME. The value of PASSWORD is a variable length value that
 * is to be used as a shared secret. Its length MUST be a multiple of 4 (measured in bytes) in
 * order to guarantee alignment of attributes on word boundaries.
 */
typedef  struct
_STUN_ATTRIBUTE  STUN_ATTRIB_PASSWORD,  *PSTUN_ATTRIB_PASSWORD;

#define  AnscStunAttribPwGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribPwGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribPwGetValue(p)                AnscStunAttribGetValue(p)

#define  AnscStunAttribPwSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribPwSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribPwSetValue(p, v)             AnscStunAttribSetValue(p, v)

/*
 * The MESSAGE-INTEGRITY attribute contains an HMAC-SHA1 [13] of the STUN message. It can be
 * present in Binding Requests or Binding Responses. Since it uses the SHA1 hash, the HMAC will be
 * 20 bytes. The text used as input to HMAC is the STUN message, including the header, up to and
 * including the attribute preceding the MESSAGE-INTEGRITY attribute. That text is then padded with
 * zeroes so as to be a multiple of 64 bytes. As a result, the MESSAGE-INTEGRITY attribute MUST be
 * the last attribute in any STUN message. The key used as input to HMAC depends on the context.
 */
typedef  struct
_STUN_ATTRIBUTE  STUN_ATTRIB_MSG_INTEGRITY,  *PSTUN_ATTRIB_MSG_INTEGRITY;

#define  AnscStunAttribMiGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribMiGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribMiGetValue(p)                AnscStunAttribGetValue(p)

#define  AnscStunAttribMiSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribMiSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribMiSetValue(p, v)             AnscStunAttribSetValue(p, v)

/*
 * The ERROR-CODE attribute is present in the Binding Error Response and Shared Secret Error
 * Response. It is a numeric value in the range of 100 to 699 plus a textual reason phrase encoded
 * in UTF-8, and is consistent in its code assignments and semantics with SIP [10] and HTTP [15].
 * The reason phrase is meant for user consumption, and can be anything appropriate for the
 * response code. The lengths of the reason phrases MUST be a multiple of 4 (measured in bytes).
 * This can be accomplished by added spaces to the end of the text, if necessary. Recommended
 * reason phrases for the defined response codes are presented below.
 * 
 * To facilitate processing, the class of the error code (the hundreds digit) is encoded separately
 * from the rest of the code.
 * 
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |                   0                     |Class|     Number    |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |      Reason Phrase (variable)                                ..
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef  struct
_STUN_ATTRIB_ERROR_CODE
{
    USHORT                          Type;
    USHORT                          Length;
    USHORT                          Reserved;
    UCHAR                           Class;
    UCHAR                           Number;
    UCHAR                           ReasonPhrase[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
STUN_ATTRIB_ERROR_CODE,  *PSTUN_ATTRIB_ERROR_CODE;

#define  AnscStunAttribEcGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribEcGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribEcGetReserved(p)             ( AnscUshortFromNToH(AnscReadUshort(&p->Reserved)) )
#define  AnscStunAttribEcGetClass(p)                ( p->Class >> 5 )
#define  AnscStunAttribEcGetNumber(p)               ( p->Number )
#define  AnscStunAttribEcGetReasonPhrase(p)         ( p->ReasonPhrase )

#define  AnscStunAttribEcSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribEcSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribEcSetReserved(p, r)          AnscWriteUshort(&p->Reserved, AnscUshortFromHToN(r))
#define  AnscStunAttribEcSetClass(p, c)             ( p->Class = (c << 5) )
#define  AnscStunAttribEcSetNumber(p, n)            ( p->Number = n )
#define  AnscStunAttribEcSetReasonPhrase(p, r)      AnscCopyMemory(p->ReasonPhrase, r, AnscStunAttribGetSize(p) - sizeof(STUN_ATTRIB_ERROR_CODE))

#define  AnscStunAttribEcGetErrorCode(p)            ( AnscStunAttribEcGetClass(p) * 100 + AnscStunAttribEcGetNumber(p) )

/*
 * The UNKNOWN-ATTRIBUTES attribute is present only in a Binding Error Response or Shared Secret
 * Error Response when the response code in the ERROR-CODE attribute is 420.
 * 
 * The attribute contains a list of 16 bit values, each of which represents an attribute type that
 * was not understood by the server. If the number of unknown attributes is an odd number, one of
 * the attributes MUST be repeated in the list, so that the total length of the list is a multiple
 * of 4 bytes.
 * 
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |      Attribute 1 Type           |     Attribute 2 Type        |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |      Attribute 3 Type           |     Attribute 4 Type    ...
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */
typedef  struct
_STUN_ATTRIB_UNKNOWN_ATTRIBS
{
    USHORT                          Type;
    USHORT                          Length;
    USHORT                          AttributeTypes[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
STUN_ATTRIB_UNKNOWN_ATTRIBS,  *PSTUN_ATTRIB_UNKNOWN_ATTRIBS;

#define  AnscStunAttribUaGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribUaGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribUaGetAttributeCount(p)       ( AnscStunAttribUaGetLength(p) / 2 )
#define  AnscStunAttribUaGetAttributeType(p, i)     ( AnscUshortFromNToH(AnscReadUshort(&p->AttributeTypes[i])) )

#define  AnscStunAttribUaSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribUaSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribUaSetAttributeType(p, i, a)  AnscWriteUshort(&p->AttributeTypes[i], AnscUshortFromHToN(a))

/*
 * The SOURCE-ADDRESS attribute is present in Binding Responses. It indicates the source IP address
 * and port that the server is sending the response from. Its syntax is identical to that of MAPPED-
 * ADDRESS.
 */
typedef  struct
_STUN_ATTRIB_MAPPED_ADDRESS  STUN_ATTRIB_REFLECTED_FROM,  *PSTUN_ATTRIB_REFLECTED_FROM;

#define  AnscStunAttribRfGetType(p)                 AnscStunAttribMaGetType(p)
#define  AnscStunAttribRfGetLength(p)               AnscStunAttribMaGetLength(p)
#define  AnscStunAttribRfGetReserved(p)             AnscStunAttribMaGetReserved(p)
#define  AnscStunAttribRfGetFamily(p)               AnscStunAttribMaGetFamily(p)
#define  AnscStunAttribRfGetPort(p)                 AnscStunAttribMaGetPort(p)
#define  AnscStunAttribRfGetAddressValue(p)         AnscStunAttribMaGetAddressValue(p)

#define  AnscStunAttribRfSetType(p, t)              AnscStunAttribMaSetType(p, t)
#define  AnscStunAttribRfSetLength(p, l)            AnscStunAttribMaSetLength(p, l)
#define  AnscStunAttribRfSetReserved(p, r)          AnscStunAttribMaSetReserved(p, r)
#define  AnscStunAttribRfSetFamily(p, f)            AnscStunAttribMaSetFamily(p, f)
#define  AnscStunAttribRfSetPort(p, o)              AnscStunAttribMaSetPort(p, o)
#define  AnscStunAttribRfSetAddressValue(p, addr)   AnscStunAttribMaSetAddressValue(p, addr)


/***********************************************************
  DATA STRUCTURES USED BY OPTIONAL STUN MESSAGE ATTRIBUTES
***********************************************************/

/*
 * The CONNECTION-REQUEST-BINDING attribute indicates the binding on which the CPE is listening for
 * UDP Connection Requests. The content of the Value element of this attribute MUST be the
 * following byte string:
 *
 *      0x64 0x73 0x6C 0x66
 *      0x6F 0x72 0x75 0x6D
 *      0x2E 0x6F 0x72 0x67
 *      0x2F 0x54 0x52 0x2D
 *      0x31 0x31 0x31 0x20
 *
 * This corresponds to the following text string:
 *
 *      "dslforum.org/TR-111 "
 *
 * A space character is the last character of this string so that its length is a multiple of four
 * characters. The Length element of this attribute MUST equal: 0x0014 (20 decimal)
 */
typedef  struct
_STUN_ATTRIBUTE  STUN_ATTRIB_CONN_REQ_BINDING,  *PSTUN_ATTRIB_CONN_REQ_BINDING;

#define  AnscStunAttribCrbGetType(p)                AnscStunAttribGetType(p)
#define  AnscStunAttribCrbGetLength(p)              AnscStunAttribGetLength(p)
#define  AnscStunAttribCrbGetValue(p)               AnscStunAttribGetValue(p)

#define  AnscStunAttribCrbSetType(p, t)             AnscStunAttribSetType(p, t)
#define  AnscStunAttribCrbSetLength(p, l)           AnscStunAttribSetLength(p, l)
#define  AnscStunAttribCrbSetValue(p, v)            AnscStunAttribSetValue(p, v)

#define  AnscStunAttribCrbSetAll(p)                                                         \
         {                                                                                  \
            AnscStunAttribCrbSetType  (p, STUN_ATTRIB_TYPE_CONN_REQUEST_BINDING);           \
            AnscStunAttribCrbSetLength(p, 20                                   );           \
            AnscStunAttribCrbSetValue (p, "dslforum.org/TR-111 "               );           \
         }

/*
 * The BINDING-CHANGE attribute Indicates that the binding has changed. This attribute contains no
 * value. Its Length element MUST be equal to zero. This attribute MUST only be used where the
 * CONNECTION-REQUEST-BINDING is also included.
 */
typedef  struct
_STUN_ATTRIBUTE  STUN_ATTRIB_BINDING_CHANGE,  *PSTUN_ATTRIB_BINDING_CHANGE;

#define  AnscStunAttribBcGetType(p)                 AnscStunAttribGetType(p)
#define  AnscStunAttribBcGetLength(p)               AnscStunAttribGetLength(p)
#define  AnscStunAttribBcGetValue(p)                AnscStunAttribGetValue(p)

#define  AnscStunAttribBcSetType(p, t)              AnscStunAttribSetType(p, t)
#define  AnscStunAttribBcSetLength(p, l)            AnscStunAttribSetLength(p, l)
#define  AnscStunAttribBcSetValue(p, v)             AnscStunAttribSetValue(p, v)

#define  AnscStunAttribBcSetAll(p)                                                          \
         {                                                                                  \
            AnscStunAttribBcSetType  (p, STUN_ATTRIB_TYPE_BINDING_CHANGE);                  \
            AnscStunAttribBcSetLength(p, 0                              );                  \
         }


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
