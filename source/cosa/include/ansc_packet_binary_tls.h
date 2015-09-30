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

    module:	ansc_packet_binary_tls.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of Transport Layer Security Protocol
        (TLS) - RFC 2246.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/12/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_PACKET_BINARY_TLS_
#define  _ANSC_PACKET_BINARY_TLS_


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
          DATA STRUCTURES USED BY TLS RECORD PROTOCOL
***********************************************************/

/*
 * Four record protocol clients are described in RFC 2246: the handshake protocol, the alter
 * protocol, the change cipher spec protocol, and the application data protocol. In order to allow
 * extension of the TLS protocol, additional record types can be supported by the record protocol.
 */
#define  TLS_RECORD_TYPE_changeCipherSpec           20
#define  TLS_RECORD_TYPE_alert                      21
#define  TLS_RECORD_TYPE_handshake                  22
#define  TLS_RECORD_TYPE_applicationData            23

/*
 * The record layer fragments information blocks into TLSPlaintext records carrying data in chunks
 * of 2^14 bytes or less. Client message boundaries are not preserved in the record layer (i.e.,
 * multiple client messages of the same ContentType may be coalesced into a single TLSPlaintext
 * record, or a single message may be fragmented across several records).
 *
 *      struct {
 *          ContentType         type;
 *          ProtocolVersion     version;
 *          uint16              length;
 *          opaque              fragment[TLSPlaintext.length];
 *      } TLSPlaintext;
 *
 * The encryption and MAC functions translate a TLSCompressed structure into a TLSCiphertext. The
 * decryption functions reverse the process. The MAC of the record also includes a sequence number
 * so that missing, extra or repeated message are detectable.
 *
 *      struct {
 *          ContentType         type;
 *          ProtocolVersion     version;
 *          uint16              length;
 *          select (CipherSpec.cipher_type) {
 *              case stream: GenericStreamCipher;
 *              case block : GenericBlockCipher;
 *          } fragment;
 *      } TLSCipherText;
 */
#define  TLS_RECORD_MAX_FRAGMENT_SIZE_PLAIN         16384           /* 2^14        bytes */
#define  TLS_RECORD_MAX_FRAGMENT_SIZE_COMPRESSED    16384 + 1024    /* 2^14 + 1024 bytes */
#define  TLS_RECORD_MAX_FRAGMENT_SIZE_CIPHER        16384 + 2048    /* 2^14 + 2048 bytes */

typedef  struct
_TLS_RECORD_HEADER
{
    UCHAR                           ContentType;
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    USHORT                          Length;
    UCHAR                           Fragment[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_RECORD_HEADER,  *PTLS_RECORD_HEADER;

#define  TlsRecordIsTypeValid(tls_record)                                                   \
         ( (tls_record->ContentType == TLS_RECORD_TYPE_changeCipherSpec) ||                 \
           (tls_record->ContentType == TLS_RECORD_TYPE_alert           ) ||                 \
           (tls_record->ContentType == TLS_RECORD_TYPE_handshake       ) ||                 \
           (tls_record->ContentType == TLS_RECORD_TYPE_applicationData ) )

#define  TlsRecordValidate(tls_record, b_size, v_result)                                    \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size < sizeof(TLS_RECORD_HEADER) )                                   \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != TlsRecordGetSize(tls_record) )                               \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( TlsRecordGetLength(tls_record) > TLS_RECORD_MAX_FRAGMENT_SIZE_CIPHER ) \
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

#define  TlsRecordIsVersionTls10(p)                 \
         ( (TlsRecordGetMajorVersion(p) == 3) && (TlsRecordGetMinorVersion(p) == 1) )
#define  TlsRecordIsVersionSsl30(p)                 \
         ( (TlsRecordGetMajorVersion(p) == 3) && (TlsRecordGetMinorVersion(p) == 0) )
#define  TlsRecordIsVersionSsl20(p)                 \
         ( (TlsRecordGetMajorVersion(p) == 2) && (TlsRecordGetMinorVersion(p) == 0) )

#define  TlsRecordSetVersionTls10(p)                { p->MajorVersion = 3;     p->MinorVersion = 1;     }
#define  TlsRecordSetVersionSsl30(p)                { p->MajorVersion = 3;     p->MinorVersion = 0;     }
#define  TlsRecordSetVersionSsl20(p)                { p->MajorVersion = 2;     p->MinorVersion = 0;     }
#define  TlsRecordSetVersion(p, major, minor)       { p->MajorVersion = major; p->MinorVersion = minor; }

#define  TlsRecordGetContentType(p)                 ( p->ContentType                                    )
#define  TlsRecordGetMajorVersion(p)                ( p->MajorVersion                                   )
#define  TlsRecordGetMinorVersion(p)                ( p->MinorVersion                                   )
#define  TlsRecordGetLength(p)                      ( AnscUshortFromNToH(AnscReadUshort(&p->Length))    )
#define  TlsRecordGetSize(p)                        ( sizeof(TLS_RECORD_HEADER) + TlsRecordGetLength(p) )
#define  TlsRecordGetFragment(p)                    ( p->Fragment                                       )
#define  TlsRecordGetEndOfFragment(p)               ( (PVOID)((ULONG)p + TlsRecordGetSize(p))           )

#define  TlsRecordSetContentType(p, t)              ( p->ContentType  = t                               )
#define  TlsRecordSetMajorVersion(p, v)             ( p->MajorVersion = v                               )
#define  TlsRecordSetMinorVersion(p, v)             ( p->MinorVersion = v                               )
#define  TlsRecordSetLength(p, l)                   AnscWriteUshort(&p->Length, AnscUshortFromHToN(l)   )
#define  TlsRecordSetFragment(p, f)                 \
         { if ( TlsRecordGetLength(p) > 0 ) AnscCopyMemory(p->Fragment, f, TlsRecordGetLength(p)); }

#define  TlsRecordAppendData(p, d, s)                                                       \
         {                                                                                  \
            USHORT                  frag_len = TlsRecordGetLength       (p);                \
            void*                   frag_end = TlsRecordGetEndOfFragment(p);                \
                                                                                            \
            AnscCopyMemory(frag_end, d, s);                                                 \
                                                                                            \
            frag_len += (USHORT)s;                                                          \
                                                                                            \
            TlsRecordSetLength(p, frag_len);                                                \
         }


/***********************************************************
   DATA STRUCTURES USED BY TLS CHANGE-CIPHER-SPEC PROTOCOL
***********************************************************/

/*
 * The change cipher spec protocol exists to signal transitions in ciphering strategies. The
 * protocol consists of a single message, which is encrypted and compressed under the current (not
 * the pending) connection state. The message consists of a single byte of value 1.
 *
 *      struct {
 *          enum { change_cipher_spec(1), (255) } type;
 *      } ChangeCipherSpec;
 */
#define  TLS_CCS_TYPE_changeCipherSpec              1

typedef  struct
_TLS_CCS_HEADER
{
    UCHAR                           Type;
}_struct_pack_
TLS_CCS_HEADER,  *PTLS_CCS_HEADER;

#define  TlsCcsGetType(p)                           ( p->Type     )
#define  TlsCcsSetType(p, t)                        ( p->Type = t )


/***********************************************************
         DATA STRUCTURES USED BY TLS ALERT PROTOCOL
***********************************************************/

/*
 * One of the content types supported by the TLS Record layer is the alert type. Alert messages
 * convey the severity of the message and a description of the alert. Alert messages with a level
 * of fatal result in the immediate termination of the connection. In this case, other connections
 * corresponding to the session may continue, but the session identifier must be invalidated,
 * preventing the failed session from being used to establish new connections. Like other messages,
 * alert messages are encrypted and compressed, as specified by the current connection state.
 *
 *      enum { warning(1), fatal(2), (255) } AlertLevel;
 *
 *      enum {
 *          close_notify(0),
 *          unexpected_message(10),
 *          bad_record_mac(20),
 *          decryption_failed(21),
 *          record_overflow(22),
 *          decompression_failure(30),
 *          handshake_failure(40),
 *          bad_certificate(42),
 *          unsupported_certificate(43),
 *          certificate_revoked(44),
 *          certificate_expired(45),
 *          certificate_unknown(46),
 *          illegal_parameter(47),
 *          unknown_ca(48),
 *          access_denied(49),
 *          decode_error(50),
 *          decrypt_error(51),
 *          export_restriction(60),
 *          protocol_version(70),
 *          insufficient_security(71),
 *          internal_error(80),
 *          user_canceled(90),
 *          no_renegotiation(100),
 *          (255)
 *      } AlertDescription;
 *
 *      struct {
 *          AlertLevel          level;
 *          AlertDescription    description;
 *      } Alert;
 */
#define  TLS_ALERT_LEVEL_warning                    1
#define  TLS_ALERT_LEVEL_fatal                      2

#define  TLS_ALERT_DESCR_closeNotify                0
#define  TLS_ALERT_DESCR_unexpectedMessage          10
#define  TLS_ALERT_DESCR_badRecordMac               20
#define  TLS_ALERT_DESCR_decryptionFailed           21
#define  TLS_ALERT_DESCR_recordOverflow             22
#define  TLS_ALERT_DESCR_decompressionFailure       30
#define  TLS_ALERT_DESCR_handshakeFailure           40
#define  TLS_ALERT_DESCR_badCertificate             42
#define  TLS_ALERT_DESCR_unsupportedCertificate     43
#define  TLS_ALERT_DESCR_certificateRevoked         44
#define  TLS_ALERT_DESCR_certificateExpired         45
#define  TLS_ALERT_DESCR_certificateUnknown         46
#define  TLS_ALERT_DESCR_illegalParameter           47
#define  TLS_ALERT_DESCR_unknownCa                  48
#define  TLS_ALERT_DESCR_accessDenied               49
#define  TLS_ALERT_DESCR_decodeError                50
#define  TLS_ALERT_DESCR_decryptError               51
#define  TLS_ALERT_DESCR_exportRestriction          60
#define  TLS_ALERT_DESCR_protocolVersion            70
#define  TLS_ALERT_DESCR_insufficientSecurity       71
#define  TLS_ALERT_DESCR_internalError              80
#define  TLS_ALERT_DESCR_userCanceled               90
#define  TLS_ALERT_DESCR_noRenegotiation            100

#define  TLS_ALERT_DESCR_timeout                    200
#define  TLS_ALERT_DESCR_socketError                201

typedef  struct
_TLS_ALERT_HEADER
{
    UCHAR                           Level;
    UCHAR                           Description;
}_struct_pack_
TLS_ALERT_HEADER,  *PTLS_ALERT_HEADER;

#define  TlsAlertGetLevel(p)                        ( p->Level           )
#define  TlsAlertGetDescription(p)                  ( p->Description     )

#define  TlsAlertSetLevel(p, l)                     ( p->Level       = l )
#define  TlsAlertSetDescription(p, d)               ( p->Description = d )


/***********************************************************
        DATA STRUCTURES USED BY TLS HANDSHAKE PROTOCOL
***********************************************************/

/*
 * The TLS Handshake Protocol is one of the defined higher level clients of the TLS Record Protocol.
 * This protocol is used to negotiate the secure attributes of a session. Handshake messages are
 * supplied to the TLS Record Layer, where they are encapsulated within one or more TLSPlaintext
 * structures, which are processed and transmitted as specified by the current active session state.
 *
 *      enum {
 *          hello_request(0), client_hello(1), server_hello(2),
 *          certificate(11), server_key_exchange (12),
 *          certificate_request(13), server_hello_done(14),
 *          certificate_verify(15), client_key_exchange(16),
 *          finished(20), (255)
 *      } HandshakeType;
 *
 *      struct {
 *          HandshakeType   msg_type;
 *          uint24          length;
 *          select (HandshakeType) {
 *              case hello_request:         HelloRequest;
 *              case client_hello:          ClientHello;
 *              case server_hello:          ServerHello;
 *              case certificate:           Certificate;
 *              case server_key_exchange:   ServerKeyExchange;
 *              case certificate_request:   CertificateRequest;
 *              case server_hello_done:     ServerHelloDone;
 *              case certificate_verify:    CertificateVerify;
 *              case client_key_exchange:   ClientKeyExchange;
 *              case finished:              Finished;
 *          } body;
 *      } Handshake;
 */
#define  TLS_HANDSHAKE_TYPE_helloRequest            0
#define  TLS_HANDSHAKE_TYPE_clientHello             1
#define  TLS_HANDSHAKE_TYPE_serverHello             2
#define  TLS_HANDSHAKE_TYPE_certificate             11
#define  TLS_HANDSHAKE_TYPE_serverKeyExchange       12
#define  TLS_HANDSHAKE_TYPE_certificateRequest      13
#define  TLS_HANDSHAKE_TYPE_serverHelloDone         14
#define  TLS_HANDSHAKE_TYPE_certificateVerify       15
#define  TLS_HANDSHAKE_TYPE_clientKeyExchange       16
#define  TLS_HANDSHAKE_TYPE_finished                20

typedef  struct
_TLS_HANDSHAKE_HEADER
{
    UCHAR                           MsgType;
    UCHAR                           Length[3];
    UCHAR                           Body[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HANDSHAKE_HEADER,  *PTLS_HANDSHAKE_HEADER;

#define  TlsHandshakeValidate(tls_handshake, b_size, v_result)                              \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size < sizeof(TLS_HANDSHAKE_HEADER) )                                \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != TlsHandshakeGetSize(tls_handshake) )                         \
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

#define  TlsHandshakeGetMsgType(p)                  ( p->MsgType )
#define  TlsHandshakeGetLength(p)                   \
         ( (((ULONG)p->Length[0]) << 16) + (((ULONG)p->Length[1]) << 8) + (ULONG)p->Length[2] )
#define  TlsHandshakeGetBody(p)                     ( p->Body    )
#define  TlsHandshakeGetSize(p)                     ( sizeof(TLS_HANDSHAKE_HEADER) + TlsHandshakeGetLength(p) )

#define  TlsHandshakeSetMsgType(p, t)               ( p->MsgType = t )
#define  TlsHandshakeSetLength(p, l)                                                        \
         {                                                                                  \
            p->Length[0] = (UCHAR)((l & 0x00FF0000) >> 16);                                 \
            p->Length[1] = (UCHAR)((l & 0x0000FF00) >> 8 );                                 \
            p->Length[2] = (UCHAR)((l & 0x000000FF)      );                                 \
         }
#define  TlsHandshakeSetBody(p, b)                  AnscCopyMemory(p->Body, b, TlsHandshakeGetLength(p))


/***********************************************************
    COMMON DATA STRUCTURES USED BY TLS HANDSHAKE PROTOCOL
***********************************************************/

/*
 * In digital signing, one-way hash functions are used as input for a signing algorithm. A
 * digitally-signed element is encoded as an opaque vector <0..2^16-1>, where the length is
 * specified by the signing algorithm and key.
 *
 * In RSA signing, a 36-byte structure of two hashes (one SHA and one MD5) is signed (encrypted
 * with the private key). It is encoded with PKCS #1 block type 0 or type 1 as described in [PKCS1].
 *
 * In DSS, the 20 bytes of the SHA hash are run directly through the Digital Signing Algorithm with
 * no additional hashing. This produces two values, r and s. The DSS signature is an opaque vector,
 * as above, the contents of which are the DER encoding of:
 *
 *      Dss-Sig-Value  ::=  SEQUENCE  {
 *          r       INTEGER,
 *          s       INTEGER
 *      }
 */
typedef  struct
_TLS_HS_SIGNATURE
{
    USHORT                          Length;
    UCHAR                           Value[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_SIGNATURE,  *PTLS_HS_SIGNATURE;

#define  TlsHsSigGetLength(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Length)) )
#define  TlsHsSigGetValue(p)                        ( p->Value                                       )

#define  TlsHsSigSetLength(p, l)                    AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  TlsHsSigSetValue(p, v)                     AnscCopyMemory(p->Value, v, TlsHsSigGetLength(p) )

/*
 * The client hello message includes a random structure, which is used later in the protocol.
 *
 *      struct {
 *          uint32  gmt_unix_time;
 *          opaque  random_bytes[28];
 *      } Random;
 *
 *  gmt_unix_time
 *      The current time and date in standard UNIX 32-bit format
 *      (seconds since the midnight starting Jan 1, 1970, GMT)
 *      according to the sender's internal clock. Clocks are not
 *      required to be set correctly by the basic TLS Protocol;
 *      higher level or application protocols may define additional
 *      requirements.
 *
 *  random_bytes
 *      28 bytes generated by a secure random number generator.
 */
#define  TLS_HANDSHAKE_DEF_RANDOM_BYTES             28

typedef  struct
_TLS_HS_RANDOM
{
    ULONG                           GmtUnixTime;
    UCHAR                           RandomBytes[28];
}_struct_pack_
TLS_HS_RANDOM,  *PTLS_HS_RANDOM;

#define  TlsHsRandomGetGmtUnixTime(p)               ( AnscUlongFromNToH(AnscReadUlong(&p->GmtUnixTime)) )
#define  TlsHsRandomGetRandomBytes(p)               ( p->RandomBytes                                    )

#define  TlsHsRandomSetGmtUnixTime(p, t)            AnscWriteUlong(&p->GmtUnixTime, AnscUlongFromHToN(t))
#define  TlsHsRandomSetRandomBytes(p, r)            AnscCopyMemory(p->RandomBytes, r, 28                )

/*
 * The client hello message includes a variable length session identifier. If not empty, the value
 * identifies a session between the same client and server whose security parameters the client
 * wishes to reuse. The session identifier may be from an earlier connection, this connection, or
 * another currently active connection. The second option is useful if the client only wishes to
 * update the random structures and derived values of a connection, while the third option makes it
 * possible to establish several independent secure connections without repeating the full
 * handshake protocol. These independent connections may occur sequentially or simultaneously; a
 * SessionID becomes valid when the handshake negotiating it completes with the exchange of
 * Finished messages and persists until removed due to aging or because a fatal error was
 * encountered on a connection associated with the session. The actual contents of the SessionID
 * are defined by the server.
 *
 *      opaque SessionID<0..32>;
 *
 * Warning:
 *      Because the SessionID is transmitted without encryption or immediate MAC protection,
 *      servers must not place confidential information in session identifiers or let the contents
 *      of fake session identifiers cause any breach of security. (Note that the content of the
 *      handshake as a whole, including the SessionID, is protected by the Finished messages
 *      exchanged at the end of the handshake.)
 */
typedef  struct
_TLS_HS_SESSION_ID
{
    UCHAR                           Length;
    UCHAR                           Identifier[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_SESSION_ID,  *PTLS_HS_SESSION_ID;

#define  TlsHsSessionIdGetLength(p)                 ( p->Length     )
#define  TlsHsSessionIdGetIdentifier(p)             ( p->Identifier )

#define  TlsHsSessionIdSetLength(p, l)              ( p->Length = l )
#define  TlsHsSessionIdSetIdentifier(p, id)                                                 \
         { if ( p->Length > 0 ) AnscCopyMemory(p->Identifier, id, TlsHsSessionIdGetLength(p)); }

/*
 * The CipherSuite list, passed from the client to the server in the client hello message, contains
 * the combinations of cryptographic algorithms supported by the client in order of the client's
 * preference (favorite choice first). Each CipherSuite defines a key exchange algorithm, a bulk
 * encryption algorithm (including secret key length) and a MAC algorithm. The server will select a
 * cipher suite or, if no acceptable choices are presented, return a handshake failure alert and
 * close the connection.
 *
 *      uint8 CipherSuite[2];
 */
typedef  struct
_TLS_HS_CIPHER_SUITE_LIST
{
    USHORT                          Length;
    USHORT                          CipherSuiteList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_CIPHER_SUITE_LIST,  *PTLS_HS_CIPHER_SUITE_LIST;

#define  TlsHsCslGetLength(p)                       ( AnscUshortFromNToH(AnscReadUshort(&p->Length            )) )
#define  TlsHsCslGetCipherSuiteCount(p)             ( TlsHsCslGetLength(p) / sizeof(USHORT)                      )
#define  TlsHsCslGetCipherSuite(p, i)               ( AnscUshortFromNToH(AnscReadUshort(&p->CipherSuiteList[i])) )

#define  TlsHsCslSetLength(p, l)                    AnscWriteUshort(&p->Length, AnscUshortFromHToN(l))
#define  TlsHsCslSetCipherSuiteCount(p, c)          TlsHsCslSetLength(p, (USHORT)(c * sizeof(USHORT)))
#define  TlsHsCslSetCipherSuite(p, s, i)            AnscWriteUshort(&p->CipherSuiteList[i], AnscUshortFromHToN(s))

/*
 * The client hello includes a list of compression algorithms supported by the client, ordered
 * according to the client's preference.
 *
 *      enum { null(0), (255) } CompressionMethod;
 */
typedef  struct
_TLS_HS_COMPRESSION_LIST
{
    UCHAR                           Length;
    UCHAR                           CompressionList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_COMPRESSION_LIST,  *PTLS_HS_COMPRESSION_LIST;

#define  TlsHsCmlGetLength(p)                       ( p->Length                 )
#define  TlsHsCmlGetMethodCount(p)                  ( TlsHsCmlGetLength(p)      )
#define  TlsHsCmlGetMethod(p, i)                    ( p->CompressionList[i]     )

#define  TlsHsCmlSetLength(p, l)                    ( p->Length = l              )
#define  TlsHsCmlSetMethodCount(p, c)               TlsHsCmlSetLength(p, (UCHAR)c)
#define  TlsHsCmlSetMethod(p, m, i)                 ( p->CompressionList[i] = m  )

/*
 * certificate_list is a sequence (chain) of X.509v3 certificates. The sender's certificate must
 * come first in the list. Each following certificate must directly certify the one preceding it.
 * Because certificate validation requires that root keys be distributed independently, the self-
 * signed certificate which specifies the root certificate authority may optionally be omitted from
 * the chain, under the assumption that the remote end must already possess it in order to validate
 * it in any case.
 */
typedef  struct
_TLS_HS_ASN1_CERT
{
    UCHAR                           Length[3];
    UCHAR                           Certificate[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_ASN1_CERT,  *PTLS_HS_ASN1_CERT;

#define  TlsHsAsn1CertGetLength(p)                  \
         ( (((ULONG)p->Length[0]) << 16) + (((ULONG)p->Length[1]) << 8) + (ULONG)p->Length[2] )
#define  TlsHsAsn1CertGetCertificate(p)             ( p->Certificate )
#define  TlsHsAsn1CertGetNext(p)                    \
         ( (PTLS_HS_ASN1_CERT)((ULONG)p->Certificate + TlsHsAsn1CertGetLength(p)) )
#define  TlsHsAsn1CertGetSize(p)                    ( sizeof(TLS_HS_ASN1_CERT) + TlsHsAsn1CertGetLength(p) )

#define  TlsHsAsn1CertSetLength(p, l)                                                       \
         {                                                                                  \
            p->Length[0] = (UCHAR)((l & 0x00FF0000) >> 16);                                 \
            p->Length[1] = (UCHAR)((l & 0x0000FF00) >> 8 );                                 \
            p->Length[2] = (UCHAR)((l & 0x000000FF)      );                                 \
         }
#define  TlsHsAsn1CertSetCertificate(p, c)          AnscCopyMemory(p->Certificate, c, TlsHsAsn1CertGetLength(p))

/*
 * There're several TLS messages contain cryptographic parameters, which are usually encapsulated
 * in a vary-sized array. For example:
 *
 *      struct {
 *          opaque      rsa_modulus <1..2^16-1>;
 *          opaque      rsa_exponent<1..2^16-1>;
 *      } ServerRSAParams;
 *
 *      struct {
 *          opaque      dh_p <1..2^16-1>;
 *          opaque      dh_g <1..2^16-1>;
 *          opaque      dh_Ys<1..2^16-1>;
 *      } ServerDHParams;
 *
 */
typedef  struct
_TLS_HS_PARAMETER
{
    USHORT                          Length;
    UCHAR                           Opaque[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_PARAMETER,  *PTLS_HS_PARAMETER;

#define  TlsHsParamGetLength(p)                     ( AnscUshortFromNToH(AnscReadUshort(&p->Length))    )
#define  TlsHsParamGetOpaque(p)                     ( p->Opaque                                         )
#define  TlsHsParamGetSize(p)                       ( sizeof(TLS_HS_PARAMETER) + TlsHsParamGetLength(p) )
#define  TlsHsParamGetNext(p)                       \
         ( (PTLS_HS_PARAMETER)((ULONG)p + TlsHsParamGetSize((p))) )

#define  TlsHsParamSetLength(p, l)                  AnscWriteUshort(&p->Length, AnscUshortFromHToN((USHORT)l))
#define  TlsHsParamSetOpaque(p, o)                  AnscCopyMemory(p->Opaque, o, TlsHsParamGetLength(p) )

/*
 * If RSA is being used for key agreement and authentication, the client generates a 48-byte pre-
 * master secret, encrypts it using the public key from the server's certificate or the temporary
 * RSA key provided in a server key exchange message, and sends the result in an encrypted pre-
 * master secret message. This structure is a variant of the client key exchange message, not a
 * message in itself.
 *
 *      struct {
 *          ProtocolVersion client_version;
 *          opaque          random[46];
 *      } PreMasterSecret;
 *
 *  client_version
 *      The latest (newest) version supported by the client. This is
 *      used to detect version roll-back attacks. Upon receiving the
 *      premaster secret, the server should check that this value
 *      matches the value transmitted by the client in the client
 *      hello message.
 *
 *  random
 *      46 securely-generated random bytes.
 */
typedef  struct
_TLS_HS_PRE_MASTER_SECRET
{
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    UCHAR                           Random[46];
}_struct_pack_
TLS_HS_PRE_MASTER_SECRET,  *PTLS_HS_PRE_MASTER_SECRET;

#define  TlsHsPmsSetVersionTls10                    TlsRecordSetVersionTls10
#define  TlsHsPmsSetVersionSsl30                    TlsRecordSetVersionSsl30
#define  TlsHsPmsSetVersionSsl20                    TlsRecordSetVersionSsl20
#define  TlsHsPmsSetVersion                         TlsRecordSetVersion

#define  TlsHsPmsGetMajorVersion(p)                 ( p->MajorVersion )
#define  TlsHsPmsGetMinorVersion(p)                 ( p->MinorVersion )
#define  TlsHsPmsGetRandom(p)                       ( p->Random       )

#define  TlsHsPmsSetMajorVersion(p, v)              ( p->MajorVersion = v )
#define  TlsHsPmsSetMinorVersion(p, v)              ( p->MinorVersion = v )
#define  TlsHsPmsSetRandom(p, r)                    AnscCopyMemory(p->Random, r, 46)


/***********************************************************
     DATA STRUCTURES USED BY TLS HANDSHAKE HELLO REQUEST
***********************************************************/

/*
 * Hello request is a simple notification that the client should begin the negotiation process anew
 * by sending a client hello message when convenient. This message will be ignored by the client if
 * the client is currently negotiating a session. This message may be ignored by the client if it
 * does not wish to renegotiate a session, or the client may, if it wishes, respond with a
 * no_renegotiation alert. Since handshake messages are intended to have transmission precedence
 * over application data, it is expected that the negotiation will begin before no more than a few
 * records are received from the client. If the server sends a hello request but does not receive a
 * client hello in response, it may close the connection with a fatal alert.
 *
 *      struct { } HelloRequest;
 */


/***********************************************************
     DATA STRUCTURES USED BY TLS HANDSHAKE CLIENT HELLO
***********************************************************/

/*
 * When a client first connects to a server it is required to send the client hello as its first
 * message. The client can also send a client hello in response to a hello request or on its own
 * initiative in order to renegotiate the security parameters in an existing connection.
 *
 *      struct {
 *          ProtocolVersion     client_version;
 *          Random              random;
 *          SessionID           session_id;
 *          CipherSuite         cipher_suites<2..2^16-1>;
 *          CompressionMethod   compression_methods<1..2^8-1>;
 *      } ClientHello;
 *
 *  client_version
 *      The version of the TLS protocol by which the client wishes to
 *      communicate during this session. This should be the latest
 *      (highest valued) version supported by the client. For this
 *      version of the specification, the version will be 3.1 (See
 *      Appendix E for details about backward compatibility).
 *
 *  random
 *      A client-generated random structure.
 *
 *  session_id
 *      The ID of a session the client wishes to use for this
 *      connection. This field should be empty if no session_id is
 *      available or the client wishes to generate new security
 *      parameters.
 *
 *  cipher_suites
 *      This is a list of the cryptographic options supported by the
 *      client, with the client's first preference first. If the
 *      session_id field is not empty (implying a session resumption
 *      request) this vector must include at least the cipher_suite
 *      from that session. Values are defined in Appendix A.5.
 *
 *  compression_methods
 *      This is a list of the compression methods supported by the
 *      client, sorted by client preference. If the session_id field
 *      is not empty (implying a session resumption request) it must
 *      include the compression_method from that session. This vector
 *      must contain, and all implementations must support,
 *      CompressionMethod.null. Thus, a client and server will always
 *      be able to agree on a compression method.
 */
typedef  struct
_TLS_HS_CLIENT_HELLO
{
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    TLS_HS_RANDOM                   Random;
    TLS_HS_SESSION_ID               SessionId;
}_struct_pack_
TLS_HS_CLIENT_HELLO,  *PTLS_HS_CLIENT_HELLO;

#define  TlsHsClientHelloValidate(client_hello, b_size, v_result)                           \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size <= sizeof(TLS_HS_CLIENT_HELLO) )                                \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size < TlsHsClientHelloGetBodySize(client_hello) )                   \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( TlsHsClientHelloGetCipherSuiteCount(client_hello) < 1 )                \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( TlsHsClientHelloGetCompressionCount(client_hello) < 1 )                \
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

#define  TlsHsClientHelloSetVersionTls10            TlsRecordSetVersionTls10
#define  TlsHsClientHelloSetVersionSsl30            TlsRecordSetVersionSsl30
#define  TlsHsClientHelloSetVersionSsl20            TlsRecordSetVersionSsl20
#define  TlsHsClientHelloSetVersion                 TlsRecordSetVersion

#define  TlsHsClientHelloGetMajorVersion(p)         ( p->MajorVersion                   )
#define  TlsHsClientHelloGetMinorVersion(p)         ( p->MinorVersion                   )
#define  TlsHsClientHelloGetRandom(p)               ( (PUCHAR            )&p->Random    )
#define  TlsHsClientHelloGetRandom2(p)              ( (PTLS_HS_RANDOM    )&p->Random    )
#define  TlsHsClientHelloGetSessionId(p)            ( (PTLS_HS_SESSION_ID)&p->SessionId )
#define  TlsHsClientHelloGetCsl(p)                                                          \
         ( (PTLS_HS_CIPHER_SUITE_LIST)((ULONG)TlsHsClientHelloGetSessionId(p) + sizeof(TLS_HS_SESSION_ID) + TlsHsSessionIdGetLength((&p->SessionId))) )
#define  TlsHsClientHelloGetCml(p)                                                          \
         ( (PTLS_HS_COMPRESSION_LIST)((ULONG)TlsHsClientHelloGetCsl(p) + sizeof(TLS_HS_CIPHER_SUITE_LIST) + TlsHsCslGetLength(TlsHsClientHelloGetCsl(p))) )
#define  TlsHsClientHelloGetCipherSuiteCount(p)                                             \
         ( TlsHsCslGetCipherSuiteCount(TlsHsClientHelloGetCsl(p)) )
#define  TlsHsClientHelloGetCipherSuite(p, index)                                           \
         ( TlsHsCslGetCipherSuite(TlsHsClientHelloGetCsl(p), index) )
#define  TlsHsClientHelloGetCompressionCount(p)                                             \
         ( TlsHsCmlGetMethodCount(TlsHsClientHelloGetCml(p)) )
#define  TlsHsClientHelloGetCompression(p, index)                                           \
         ( TlsHsCmlGetMethod(TlsHsClientHelloGetCml(p), index) )

#define  TlsHsClientHelloMatchCipherSuite(p, cipher_suite, m_result)                        \
         {                                                                                  \
            ULONG                   cipher_suite_count = 0;                                 \
            ULONG                   ii                 = 0;                                 \
                                                                                            \
            m_result = FALSE;                                                               \
                                                                                            \
            cipher_suite_count = TlsHsClientHelloGetCipherSuiteCount(p);                    \
                                                                                            \
            for ( ii = 0; ii < cipher_suite_count; ii++ )                                   \
            {                                                                               \
                if ( TlsHsClientHelloGetCipherSuite(p, ii) == cipher_suite )                \
                {                                                                           \
                    m_result = TRUE;                                                        \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
         }
#define  TlsHsClientHelloMatchCompression(p, comp_method, m_result)                         \
         {                                                                                  \
            ULONG                   comp_method_count = 0;                                  \
            ULONG                   ii                = 0;                                  \
                                                                                            \
            m_result = FALSE;                                                               \
                                                                                            \
            comp_method_count = TlsHsClientHelloGetCompressionCount(p);                     \
                                                                                            \
            for ( ii = 0; ii < comp_method_count; ii++ )                                    \
            {                                                                               \
                if ( TlsHsClientHelloGetCompression(p, ii) == comp_method )                 \
                {                                                                           \
                    m_result = TRUE;                                                        \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
         }

#define  TlsHsClientHelloSetMajorVersion(p, v)      ( p->MajorVersion = v )
#define  TlsHsClientHelloSetMinorVersion(p, v)      ( p->MinorVersion = v )
#define  TlsHsClientHelloSetRandom(p, r)            AnscCopyMemory(&p->Random, r, sizeof(TLS_HS_RANDOM))
#define  TlsHsClientHelloSetRandom2(p, r)           ( p->Random       = r )
#define  TlsHsClientHelloSetSessionId(p, l, id)                                             \
         {                                                                                  \
            TlsHsSessionIdSetLength    ((&p->SessionId), (UCHAR)l);                         \
            TlsHsSessionIdSetIdentifier((&p->SessionId), id      );                         \
         }
#define  TlsHsClientHelloSetCipherSuiteCount(p, count)                                      \
         {                                                                                  \
            PTLS_HS_CIPHER_SUITE_LIST           cipher_suite_list = NULL;                   \
                                                                                            \
            cipher_suite_list = TlsHsClientHelloGetCsl(p);                                  \
                                                                                            \
            TlsHsCslSetCipherSuiteCount(cipher_suite_list, count);                          \
         }
#define  TlsHsClientHelloSetCipherSuite(p, suite, index)                                    \
         {                                                                                  \
            PTLS_HS_CIPHER_SUITE_LIST           cipher_suite_list = NULL;                   \
                                                                                            \
            cipher_suite_list = TlsHsClientHelloGetCsl(p);                                  \
                                                                                            \
            TlsHsCslSetCipherSuite(cipher_suite_list, suite, index);                        \
         }
#define  TlsHsClientHelloSetCompressionCount(p, count)                                      \
         {                                                                                  \
            PTLS_HS_COMPRESSION_LIST            compression_list = NULL;                    \
                                                                                            \
            compression_list = TlsHsClientHelloGetCml(p);                                   \
                                                                                            \
            TlsHsCmlSetMethodCount(compression_list, count);                                \
         }
#define  TlsHsClientHelloSetCompression(p, method, index)                                   \
         {                                                                                  \
            PTLS_HS_COMPRESSION_LIST            compression_list = NULL;                    \
                                                                                            \
            compression_list = TlsHsClientHelloGetCml(p);                                   \
                                                                                            \
            TlsHsCmlSetMethod(compression_list, method, index);                             \
         }

#define  TlsHsClientHelloGetBodySize(p)                                                     \
         ( sizeof(TLS_HS_CLIENT_HELLO)                              +                       \
           TlsHsSessionIdGetLength(TlsHsClientHelloGetSessionId(p)) +                       \
           sizeof(TLS_HS_CIPHER_SUITE_LIST)                         +                       \
           TlsHsCslGetLength      (TlsHsClientHelloGetCsl      (p)) +                       \
           sizeof(TLS_HS_COMPRESSION_LIST )                         +                       \
           TlsHsCmlGetLength      (TlsHsClientHelloGetCml      (p)) )


/***********************************************************
     DATA STRUCTURES USED BY TLS HANDSHAKE SERVER HELLO
***********************************************************/

/*
 * The server will send this message in response to a client hello message when it was able to find
 * an acceptable set of algorithms. If it cannot find such a match, it will respond with a
 * handshake failure alert.
 *
 *      struct {
 *          ProtocolVersion     server_version;
 *          Random              random;
 *          SessionID           session_id;
 *          CipherSuite         cipher_suite;
 *          CompressionMethod   compression_method;
 *      } ServerHello;
 */
typedef  struct
_TLS_HS_SERVER_HELLO
{
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    TLS_HS_RANDOM                   Random;
    TLS_HS_SESSION_ID               SessionId;
}_struct_pack_
TLS_HS_SERVER_HELLO,  *PTLS_HS_SERVER_HELLO;

#define  TlsHsServerHelloValidate(server_hello, b_size, v_result)                           \
         {                                                                                  \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size <= sizeof(TLS_HS_SERVER_HELLO) )                                \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != TlsHsServerHelloGetBodySize(server_hello) )                  \
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

#define  TlsHsServerHelloSetVersionTls10            TlsRecordSetVersionTls10
#define  TlsHsServerHelloSetVersionSsl30            TlsRecordSetVersionSsl30
#define  TlsHsServerHelloSetVersionSsl20            TlsRecordSetVersionSsl20
#define  TlsHsServerHelloSetVersion                 TlsRecordSetVersion

#define  TlsHsServerHelloGetMajorVersion(p)         ( p->MajorVersion                   )
#define  TlsHsServerHelloGetMinorVersion(p)         ( p->MinorVersion                   )
#define  TlsHsServerHelloGetRandom(p)               ( (PUCHAR            )&p->Random    )
#define  TlsHsServerHelloGetRandom2(p)              ( (PTLS_HS_RANDOM    )&p->Random    )
#define  TlsHsServerHelloGetSessionId(p)            ( (PTLS_HS_SESSION_ID)&p->SessionId )
#define  TlsHsServerHelloGetCs(p)                                                           \
         ( (PUSHORT)((ULONG)TlsHsServerHelloGetSessionId(p) + sizeof(TLS_HS_SESSION_ID) + TlsHsSessionIdGetLength((&p->SessionId))) )
#define  TlsHsServerHelloGetCm(p)                                                           \
         ( (PUCHAR )((ULONG)TlsHsServerHelloGetCs(p) + sizeof(USHORT)) )
#define  TlsHsServerHelloGetCipherSuite(p)                                                  \
         ( AnscUshortFromNToH(AnscReadUshort(TlsHsServerHelloGetCs(p))) )
#define  TlsHsServerHelloGetCompression(p)                                                  \
         ( *(PUCHAR)TlsHsServerHelloGetCm(p) )

#define  TlsHsServerHelloMatchCipherSuite(p, cipher_suite, m_result)                        \
         {                                                                                  \
            if ( TlsHsServerHelloGetCipherSuite(p) == cipher_suite )                        \
            {                                                                               \
                m_result = TRUE;                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                m_result = FALSE;                                                           \
            }                                                                               \
         }
#define  TlsHsServerHelloMatchCompression(p, comp_method, m_result)                         \
         {                                                                                  \
            if ( TlsHsServerHelloGetCompression(p) == comp_method )                         \
            {                                                                               \
                m_result = TRUE;                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                m_result = FALSE;                                                           \
            }                                                                               \
         }

#define  TlsHsServerHelloSetMajorVersion(p, v)      ( p->MajorVersion = v )
#define  TlsHsServerHelloSetMinorVersion(p, v)      ( p->MinorVersion = v )
#define  TlsHsServerHelloSetRandom(p, r)            AnscCopyMemory(&p->Random, r, sizeof(TLS_HS_RANDOM))
#define  TlsHsServerHelloSetRandom2(p, r)           ( p->Random       = r )
#define  TlsHsServerHelloSetSessionId(p, l, id)                                             \
         {                                                                                  \
            TlsHsSessionIdSetLength    ((&p->SessionId), (UCHAR)l);                         \
            TlsHsSessionIdSetIdentifier((&p->SessionId), id      );                         \
         }
#define  TlsHsServerHelloSetCipherSuite(p, suite)                                           \
         AnscWriteUshort(TlsHsServerHelloGetCs(p), AnscUshortFromHToN(suite))
#define  TlsHsServerHelloSetCompression(p, method)                                          \
         ( *(PUCHAR)TlsHsServerHelloGetCm(p) = method )

#define  TlsHsServerHelloGetBodySize(p)                                                     \
         ( sizeof(TLS_HS_SERVER_HELLO)                              +                       \
           TlsHsSessionIdGetLength(TlsHsServerHelloGetSessionId(p)) +                       \
           sizeof(USHORT)                                           +                       \
           sizeof(UCHAR ) )


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE SERVER CERTIFICATE
***********************************************************/

/*
 * The server must send a certificate whenever the agreed-upon key exchange method is not an
 * anonymous one. This message will always immediately follow the server hello message. The certifi-
 * cate type must be appropriate for the selected cipher suite's key exchange algorithm, and is
 * generally an X.509v3 certificate. It must contain a key which matches the key exchange method,
 * as follows. Unless otherwise specified, the signing algorithm for the certificate must be the
 * same as the algorithm for the certificate key. Unless otherwise specified, the public key may be
 * of any length.
 *
 *      opaque ASN.1Cert<1..2^24-1>;
 *
 *      struct {
 *          ASN.1Cert certificate_list<0..2^24-1>;
 *      } Certificate;
 */
typedef  struct
_TLS_HS_CERTIFICATE
{
    UCHAR                           Length[3];
    UCHAR                           CertificateList[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_CERTIFICATE,  *PTLS_HS_CERTIFICATE;

#define  TlsHsCertValidate(certificate, b_size, v_result)                                   \
         {                                                                                  \
            ULONG                   cert_list_length = TlsHsCertGetLength(certificate);     \
            PTLS_HS_ASN1_CERT       asn1_cert        = NULL;                                \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size < sizeof(TLS_HS_CERTIFICATE) )                                  \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != (cert_list_length + sizeof(TLS_HS_CERTIFICATE)) )            \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( cert_list_length == 0 )                                                \
                {                                                                           \
                    v_result = TRUE;                                                        \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                asn1_cert = (PTLS_HS_ASN1_CERT)&certificate->CertificateList[0];            \
                                                                                            \
                while ( asn1_cert )                                                         \
                {                                                                           \
                    if ( cert_list_length < TlsHsAsn1CertGetSize(asn1_cert) )               \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        cert_list_length -= TlsHsAsn1CertGetSize(asn1_cert);                \
                    }                                                                       \
                                                                                            \
                    if ( cert_list_length > 0 )                                             \
                    {                                                                       \
                        asn1_cert = TlsHsAsn1CertGetNext(asn1_cert);                        \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        asn1_cert = NULL;                                                   \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  TlsHsCertGetLength(p)                      \
         ( (((ULONG)p->Length[0]) << 16) + (((ULONG)p->Length[1]) << 8) + (ULONG)p->Length[2] )
#define  TlsHsCertGetEndOfList(p)                   \
         ( (PVOID)((ULONG)p + sizeof(TLS_HS_CERTIFICATE) + TlsHsCertGetLength(p)) )
#define  TlsHsCertGetCertCount(p, c_count)                                                  \
         {                                                                                  \
            ULONG                   cert_list_length = TlsHsCertGetLength(p);               \
            PTLS_HS_ASN1_CERT       asn1_cert        = NULL;                                \
            ULONG                   asn1_cert_count  = 0;                                   \
                                                                                            \
            c_count = 0;                                                                    \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( cert_list_length == 0 )                                                \
                {                                                                           \
                    c_count = 0;                                                            \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                asn1_cert = (PTLS_HS_ASN1_CERT)&p->CertificateList[0];                      \
                                                                                            \
                while ( asn1_cert )                                                         \
                {                                                                           \
                    asn1_cert_count++;                                                      \
                                                                                            \
                    cert_list_length -= TlsHsAsn1CertGetSize(asn1_cert);                    \
                                                                                            \
                    if ( cert_list_length > 0 )                                             \
                    {                                                                       \
                        asn1_cert = TlsHsAsn1CertGetNext(asn1_cert);                        \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        asn1_cert = NULL;                                                   \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                c_count = asn1_cert_count;                                                  \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }
#define  TlsHsCertGetCert1(p, r_cert, index)                                                \
         {                                                                                  \
            ULONG                   cert_list_length = TlsHsCertGetLength(p);               \
            PTLS_HS_ASN1_CERT       asn1_cert        = NULL;                                \
            ULONG                   asn1_cert_count  = 0;                                   \
                                                                                            \
            r_cert = NULL;                                                                  \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( cert_list_length == 0 )                                                \
                {                                                                           \
                    r_cert = NULL;                                                          \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                asn1_cert = (PTLS_HS_ASN1_CERT)&p->CertificateList[0];                      \
                                                                                            \
                while ( asn1_cert )                                                         \
                {                                                                           \
                    if ( asn1_cert_count == index )                                         \
                    {                                                                       \
                        r_cert = asn1_cert;                                                 \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                                                                                            \
                    asn1_cert_count++;                                                      \
                                                                                            \
                    cert_list_length -= TlsHsAsn1CertGetSize(asn1_cert);                    \
                                                                                            \
                    if ( cert_list_length > 0 )                                             \
                    {                                                                       \
                        asn1_cert = TlsHsAsn1CertGetNext(asn1_cert);                        \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        asn1_cert = NULL;                                                   \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }
#define  TlsHsCertGetCert2(p, cert_data, cert_size, index)                                  \
         {                                                                                  \
            PTLS_HS_ASN1_CERT       dst_cert = NULL;                                        \
                                                                                            \
            TlsHsCertGetCert1(p, dst_cert, index);                                          \
                                                                                            \
            if ( dst_cert )                                                                 \
            {                                                                               \
                cert_data = TlsHsAsn1CertGetCertificate(dst_cert);                          \
                cert_size = TlsHsAsn1CertGetLength     (dst_cert);                          \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                cert_data = NULL;                                                           \
                cert_size = 0;                                                              \
            }                                                                               \
         }

#define  TlsHsCertSetLength(p, l)                                                           \
         {                                                                                  \
            p->Length[0] = (UCHAR)((l & 0x00FF0000) >> 16);                                 \
            p->Length[1] = (UCHAR)((l & 0x0000FF00) >> 8 );                                 \
            p->Length[2] = (UCHAR)((l & 0x000000FF)      );                                 \
         }
#define  TlsHsCertAddCert1(p, n_cert)                                                       \
         {                                                                                  \
            ULONG                   cert_list_length = TlsHsCertGetLength(p);               \
            PTLS_HS_ASN1_CERT       asn1_cert        = NULL;                                \
                                                                                            \
            asn1_cert = (PTLS_HS_ASN1_CERT)((ULONG)TlsHsCertGetEndOfList(p));               \
                                                                                            \
            AnscCopyMemory                                                                  \
                (                                                                           \
                    (PVOID)asn1_cert,                                                       \
                    (PVOID)n_cert,                                                          \
                    TlsHsAsn1CertGetSize(n_cert)                                            \
                );                                                                          \
                                                                                            \
            cert_list_length += TlsHsAsn1CertGetSize(asn1_cert);                            \
                                                                                            \
            TlsHsCertSetLength(p, cert_list_length);                                        \
         }
#define  TlsHsCertAddCert2(p, cert_data, cert_size)                                         \
         {                                                                                  \
            ULONG                   cert_list_length = TlsHsCertGetLength(p);               \
            PTLS_HS_ASN1_CERT       asn1_cert        = NULL;                                \
                                                                                            \
            asn1_cert = (PTLS_HS_ASN1_CERT)((ULONG)TlsHsCertGetEndOfList(p));               \
                                                                                            \
            TlsHsAsn1CertSetLength     (asn1_cert, cert_size);                              \
            TlsHsAsn1CertSetCertificate(asn1_cert, cert_data);                              \
                                                                                            \
            cert_list_length += TlsHsAsn1CertGetSize(asn1_cert);                            \
                                                                                            \
            TlsHsCertSetLength(p, cert_list_length);                                        \
         }

#define  TlsHsCertGetBodySize(p)                                                            \
         ( sizeof(TLS_HS_CERTIFICATE) + TlsHsCertGetLength(p) )


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE SERVER KEY EXCHANGE
***********************************************************/

/*
 * The server key exchange message is sent by the server only when the server certificate message
 * (if sent) does not contain enough data to allow the client to exchange a premaster secret. This
 * message conveys cryptographic information to allow the client to communicate the premaster
 * secret: either an RSA public key to encrypt the premaster secret with, or a Diffie-Hellman
 * public key with which the client can complete a key exchange (with the result being the
 * premaster secret.)
 *
 *      enum { rsa, diffie_hellman } KeyExchangeAlgorithm;
 *
 *      struct {
 *          opaque      rsa_modulus <1..2^16-1>;
 *          opaque      rsa_exponent<1..2^16-1>;
 *      } ServerRSAParams;
 *
 *      struct {
 *          opaque      dh_p <1..2^16-1>;
 *          opaque      dh_g <1..2^16-1>;
 *          opaque      dh_Ys<1..2^16-1>;
 *      } ServerDHParams;
 *
 *      struct {
 *          select (KeyExchangeAlgorithm) {
 *              case diffie_hellman:
 *                  ServerDHParams  params;
 *                  Signature       signed_params;
 *              case rsa:
 *                  ServerRSAParams params;
 *                  Signature       signed_params;
 *          };
 *      } ServerKeyExchange;
 */
#define  TLS_HS_KE_ALGORITHM_diffieHellman          1
#define  TLS_HS_KE_ALGORITHM_rsa                    2

#define  TLS_HS_SIG_ALGORITHM_anonymous             1
#define  TLS_HS_SIG_ALGORITHM_rsa                   2
#define  TLS_HS_SIG_ALGORITHM_dsa                   3

typedef  struct
_TLS_HS_SERVER_KEY_EXCHANGE
{
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_SERVER_KEY_EXCHANGE,  *PTLS_HS_SERVER_KEY_EXCHANGE;

#define  TlsHsServerKeValidate(key_exchange, b_size, ke_algm, sig_algm, v_result)           \
         {                                                                                  \
            ULONG                   param_list_size = b_size;                               \
            PTLS_HS_PARAMETER       param_dh_p      = NULL;                                 \
            PTLS_HS_PARAMETER       param_dh_g      = NULL;                                 \
            PTLS_HS_PARAMETER       param_dh_y      = NULL;                                 \
            PTLS_HS_PARAMETER       param_rsa_m     = NULL;                                 \
            PTLS_HS_PARAMETER       param_rsa_e     = NULL;                                 \
            PTLS_HS_PARAMETER       param_last      = NULL;                                 \
            PTLS_HS_PARAMETER       param_signature = NULL;                                 \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            if ( ke_algm == TLS_HS_KE_ALGORITHM_rsa )                                       \
            {                                                                               \
                do                                                                          \
                {                                                                           \
                    if ( TRUE )     /* rsa_modulus <1..2^16-1> */                           \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_rsa_m = (PTLS_HS_PARAMETER)key_exchange->Data;                \
                                                                                            \
                        if ( TlsHsParamGetLength(param_rsa_m) < 1 )                         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size < TlsHsParamGetSize(param_rsa_m) )        \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            param_list_size -= TlsHsParamGetSize(param_rsa_m);              \
                        }                                                                   \
                    }                                                                       \
                                                                                            \
                    if ( TRUE )     /* rsa_exponent <1..2^16-1> */                          \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_rsa_e = TlsHsParamGetNext(param_rsa_m);                       \
                                                                                            \
                        if ( TlsHsParamGetLength(param_rsa_e) < 1 )                         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size < TlsHsParamGetSize(param_rsa_e) )        \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            param_list_size -= TlsHsParamGetSize(param_rsa_e);              \
                        }                                                                   \
                    }                                                                       \
                                                                                            \
                    param_last = param_rsa_e;                                               \
                }                                                                           \
                while ( FALSE );                                                            \
            }                                                                               \
            else if ( ke_algm == TLS_HS_KE_ALGORITHM_diffieHellman )                        \
            {                                                                               \
                do                                                                          \
                {                                                                           \
                    if ( TRUE )     /* dh_p <1..2^16-1> */                                  \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_dh_p = (PTLS_HS_PARAMETER)key_exchange->Data;                 \
                                                                                            \
                        if ( TlsHsParamGetLength(param_dh_p) < 1 )                          \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size < TlsHsParamGetSize(param_dh_p) )         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            param_list_size -= TlsHsParamGetSize(param_dh_p);               \
                        }                                                                   \
                    }                                                                       \
                                                                                            \
                    if ( TRUE )     /* dh_g <1..2^16-1> */                                  \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_dh_g = TlsHsParamGetNext(param_dh_p);                         \
                                                                                            \
                        if ( TlsHsParamGetLength(param_dh_g) < 1 )                          \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size < TlsHsParamGetSize(param_dh_g) )         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            param_list_size -= TlsHsParamGetSize(param_dh_g);               \
                        }                                                                   \
                    }                                                                       \
                                                                                            \
                    if ( TRUE )     /* dh_Ys <1..2^16-1> */                                 \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_dh_y = TlsHsParamGetNext(param_dh_g);                         \
                                                                                            \
                        if ( TlsHsParamGetLength(param_dh_y) < 1 )                          \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size < TlsHsParamGetSize(param_dh_y) )         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else                                                                \
                        {                                                                   \
                            param_list_size -= TlsHsParamGetSize(param_dh_y);               \
                        }                                                                   \
                    }                                                                       \
                                                                                            \
                    param_last = param_dh_y;                                                \
                }                                                                           \
                while ( FALSE );                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
                                                                                            \
            while ( v_result )                                                              \
            {                                                                               \
                if ( sig_algm == TLS_HS_SIG_ALGORITHM_anonymous )                           \
                {                                                                           \
                    if ( param_list_size != 0 )                                             \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    if ( param_list_size < sizeof(USHORT) )                                 \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                                                                                            \
                    param_signature = TlsHsParamGetNext(param_last);                        \
                                                                                            \
                    if ( TlsHsParamGetLength(param_signature) < 1 )                         \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else if ( param_list_size != TlsHsParamGetSize(param_signature) )       \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
         }

#define  TlsHsServerKeGetRSAModulus1(p, rsa_param)  \
         ( rsa_param = (PTLS_HS_PARAMETER)p->Data )
#define  TlsHsServerKeGetRSAModulus2(p, rsa_modulus_data, rsa_modulus_size)                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_modulus_param = NULL;                               \
                                                                                            \
            TlsHsServerKeGetRSAModulus1(p, rsa_modulus_param);                              \
                                                                                            \
            rsa_modulus_data = TlsHsParamGetOpaque(rsa_modulus_param);                      \
            rsa_modulus_size = TlsHsParamGetLength(rsa_modulus_param);                      \
         }

#define  TlsHsServerKeGetRSAExponent1(p, rsa_param) \
         ( rsa_param = TlsHsParamGetNext((PTLS_HS_PARAMETER)p->Data) )
#define  TlsHsServerKeGetRSAExponent2(p, rsa_exponent_data, rsa_exponent_size)              \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_exponent_param = NULL;                              \
                                                                                            \
            TlsHsServerKeGetRSAExponent1(p, rsa_exponent_param);                            \
                                                                                            \
            rsa_exponent_data = TlsHsParamGetOpaque(rsa_exponent_param);                    \
            rsa_exponent_size = TlsHsParamGetLength(rsa_exponent_param);                    \
         }

#define  TlsHsServerKeGetDHPrime1(p, dh_param)      \
         ( dh_param = (PTLS_HS_PARAMETER)p->Data )
#define  TlsHsServerKeGetDHPrime2(p, dh_p_data, dh_p_size)                                  \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_p_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPrime1(p, dh_p_param);                                        \
                                                                                            \
            dh_p_data = TlsHsParamGetOpaque(dh_p_param);                                    \
            dh_p_size = TlsHsParamGetLength(dh_p_param);                                    \
         }

#define  TlsHsServerKeGetDHGenerator1(p, dh_param)  \
         ( dh_param = TlsHsParamGetNext((PTLS_HS_PARAMETER)p->Data) )
#define  TlsHsServerKeGetDHGenerator2(p, dh_g_data, dh_g_size)                              \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_g_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHGenerator1(p, dh_g_param);                                    \
                                                                                            \
            dh_g_data = TlsHsParamGetOpaque(dh_g_param);                                    \
            dh_g_size = TlsHsParamGetLength(dh_g_param);                                    \
         }

#define  TlsHsServerKeGetDHPublic1(p, dh_param)                                             \
         {                                                                                  \
            dh_param = TlsHsParamGetNext(TlsHsParamGetNext((PTLS_HS_PARAMETER)p->Data));    \
         }
#define  TlsHsServerKeGetDHPublic2(p, dh_y_data, dh_y_size)                                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_y_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPublic1(p, dh_y_param);                                       \
                                                                                            \
            dh_y_data = TlsHsParamGetOpaque(dh_y_param);                                    \
            dh_y_size = TlsHsParamGetLength(dh_y_param);                                    \
         }

#define  TlsHsServerKeGetSignature1(p, ke_algm, sig_param)                                  \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_exponent_param = NULL;                              \
            PTLS_HS_PARAMETER       dh_y_param         = NULL;                              \
                                                                                            \
            if ( ke_algm == TLS_HS_KE_ALGORITHM_rsa )                                       \
            {                                                                               \
                TlsHsServerKeGetRSAExponent1(p, rsa_exponent_param);                        \
                                                                                            \
                sig_param = TlsHsParamGetNext(rsa_exponent_param);                          \
            }                                                                               \
            else if ( ke_algm == TLS_HS_KE_ALGORITHM_diffieHellman )                        \
            {                                                                               \
                TlsHsServerKeGetDHPublic1(p, dh_y_param);                                   \
                                                                                            \
                sig_param = TlsHsParamGetNext(dh_y_param);                                  \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                sig_param = NULL;                                                           \
            }                                                                               \
         }
#define  TlsHsServerKeGetSignature2(p, ke_algm, sig_data, sig_size)                         \
         {                                                                                  \
            PTLS_HS_PARAMETER       sig_param = NULL;                                       \
                                                                                            \
            TlsHsServerKeGetSignature1(p, ke_algm, sig_param);                              \
                                                                                            \
            sig_data = TlsHsParamGetOpaque(sig_param);                                      \
            sig_size = TlsHsParamGetLength(sig_param);                                      \
         }

#define  TlsHsServerKeSetRSAModulus1(p, rsa_param)                                          \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_modulus_param = NULL;                               \
                                                                                            \
            TlsHsServerKeGetRSAModulus1(p, rsa_modulus_param);                              \
                                                                                            \
            TlsHsParamSetLength(rsa_modulus_param, TlsHsParamGetLength(rsa_param));         \
            TlsHsParamSetOpaque(rsa_modulus_param, TlsHsParamGetOpaque(rsa_param));         \
         }
#define  TlsHsServerKeSetRSAModulus2(p, rsa_modulus_data, rsa_modulus_size)                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_modulus_param = NULL;                               \
                                                                                            \
            TlsHsServerKeGetRSAModulus1(p, rsa_modulus_param);                              \
                                                                                            \
            TlsHsParamSetLength(rsa_modulus_param, rsa_modulus_size);                       \
            TlsHsParamSetOpaque(rsa_modulus_param, rsa_modulus_data);                       \
         }

#define  TlsHsServerKeSetRSAExponent1(p, rsa_param)                                         \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_exponent_param = NULL;                              \
                                                                                            \
            TlsHsServerKeGetRSAExponent1(p, rsa_exponent_param);                            \
                                                                                            \
            TlsHsParamSetLength(rsa_exponent_param, TlsHsParamGetLength(rsa_param));        \
            TlsHsParamSetOpaque(rsa_exponent_param, TlsHsParamGetOpaque(rsa_param));        \
         }
#define  TlsHsServerKeSetRSAExponent2(p, rsa_exponent_data, rsa_exponent_size)              \
         {                                                                                  \
            PTLS_HS_PARAMETER       rsa_exponent_param = NULL;                              \
                                                                                            \
            TlsHsServerKeGetRSAExponent1(p, rsa_exponent_param);                            \
                                                                                            \
            TlsHsParamSetLength(rsa_exponent_param, rsa_exponent_size);                     \
            TlsHsParamSetOpaque(rsa_exponent_param, rsa_exponent_data);                     \
         }

#define  TlsHsServerKeSetDHPrime1(p, dh_param)                                              \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_p_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPrime1(p, dh_p_param);                                        \
                                                                                            \
            TlsHsParamSetLength(dh_p_param, TlsHsParamGetLength(dh_param));                 \
            TlsHsParamSetOpaque(dh_p_param, TlsHsParamGetOpaque(dh_param));                 \
         }
#define  TlsHsServerKeSetDHPrime2(p, dh_p_data, dh_p_size)                                  \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_p_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPrime1(p, dh_p_param);                                        \
                                                                                            \
            TlsHsParamSetLength(dh_p_param, dh_p_size);                                     \
            TlsHsParamSetOpaque(dh_p_param, dh_p_data);                                     \
         }

#define  TlsHsServerKeSetDHGenerator1(p, dh_param)                                          \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_g_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHGenerator1(p, dh_g_param);                                    \
                                                                                            \
            TlsHsParamSetLength(dh_g_param, TlsHsParamGetLength(dh_param));                 \
            TlsHsParamSetOpaque(dh_g_param, TlsHsParamGetOpaque(dh_param));                 \
         }
#define  TlsHsServerKeSetDHGenerator2(p, dh_g_data, dh_g_size)                              \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_g_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHGenerator1(p, dh_g_param);                                    \
                                                                                            \
            TlsHsParamSetLength(dh_g_param, dh_g_size);                                     \
            TlsHsParamSetOpaque(dh_g_param, dh_g_data);                                     \
         }

#define  TlsHsServerKeSetDHPublic1(p, dh_param)                                             \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_y_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPublic1(p, dh_y_param);                                       \
                                                                                            \
            TlsHsParamSetLength(dh_y_param, TlsHsParamGetLength(dh_param));                 \
            TlsHsParamSetOpaque(dh_y_param, TlsHsParamGetOpaque(dh_param));                 \
         }
#define  TlsHsServerKeSetDHPublic2(p, dh_y_data, dh_y_size)                                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_y_param = NULL;                                      \
                                                                                            \
            TlsHsServerKeGetDHPublic1(p, dh_y_param);                                       \
                                                                                            \
            TlsHsParamSetLength(dh_y_param, dh_y_size);                                     \
            TlsHsParamSetOpaque(dh_y_param, dh_y_data);                                     \
         }

#define  TlsHsServerKeSetSignature1(p, ke_algm, sig_param)                                  \
         {                                                                                  \
            PTLS_HS_PARAMETER       ke_sig_param = NULL;                                    \
                                                                                            \
            TlsHsServerKeGetSignature1(p, ke_algm, ke_sig_param);                           \
                                                                                            \
            TlsHsParamSetLength(ke_sig_param, TlsHsParamGetLength(sig_param));              \
            TlsHsParamSetOpaque(ke_sig_param, TlsHsParamGetOpaque(sig_param));              \
         }
#define  TlsHsServerKeSetSignature2(p, ke_algm, sig_data, sig_size)                         \
         {                                                                                  \
            PTLS_HS_PARAMETER       ke_sig_param = NULL;                                    \
                                                                                            \
            TlsHsServerKeGetSignature1(p, ke_algm, ke_sig_param);                           \
                                                                                            \
            TlsHsParamSetLength(ke_sig_param, sig_size);                                    \
            TlsHsParamSetOpaque(ke_sig_param, sig_data);                                    \
         }

#define  TlsHsServerKeGetBodySize(key_exchange, ke_algm, sig_algm, b_size)                  \
         {                                                                                  \
            PTLS_HS_PARAMETER       sig_param = NULL;                                       \
            ULONG                   body_size = 0;                                          \
                                                                                            \
            TlsHsServerKeGetSignature1(key_exchange, ke_algm, sig_param);                   \
                                                                                            \
            if ( sig_algm == TLS_HS_SIG_ALGORITHM_anonymous )                               \
            {                                                                               \
                body_size = (ULONG)sig_param - (ULONG)key_exchange;                         \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                body_size  = (ULONG)sig_param - (ULONG)key_exchange;                        \
                body_size += TlsHsParamGetSize(sig_param);                                  \
            }                                                                               \
                                                                                            \
            b_size = body_size;                                                             \
         }


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE CERTIFICATE REQUEST
***********************************************************/

/*
 * A non-anonymous server can optionally request a certificate from the client, if appropriate for
 * the selected cipher suite. This message, if sent, will immediately follow the Server Key
 * Exchange message (if it is sent; otherwise, the Server Certificate message).
 *
 *      enum {
 *          rsa_sign(1), dss_sign(2), rsa_fixed_dh(3), dss_fixed_dh(4), (255)
 *      } ClientCertificateType;
 *
 *      opaque DistinguishedName<1..2^16-1>;
 *
 *      struct {
 *          ClientCertificateType   certificate_types<1..2^8-1>;
 *          DistinguishedName       certificate_authorities<3..2^16-1>;
 *      } CertificateRequest;
 */
#define  TLS_HS_CERT_TYPE_rsaSign                   1
#define  TLS_HS_CERT_TYPE_dssSign                   2
#define  TLS_HS_CERT_TYPE_rsaFixedDH                3
#define  TLS_HS_CERT_TYPE_dssFixedDH                4

typedef  struct
_TLS_HS_CERT_REQUEST
{
    UCHAR                           TypeCount;
    UCHAR                           TypeArray[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_CERT_REQUEST,  *PTLS_HS_CERT_REQUEST;

#define  TlsHsCertReqValidate(cert_req, b_size, v_result)                                   \
         {                                                                                  \
            ULONG                   dn_list_length = TlsHsCertReqGetDnListLength (cert_req);\
            void*                   dn_list_start  = TlsHsCertReqGetStartOfDnList(cert_req);\
            PTLS_HS_PARAMETER       dn_param       = NULL;                                  \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( b_size < sizeof(TLS_HS_CERT_REQUEST) )                                 \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( TlsHsCertReqGetTypeCount(cert_req) == 0 )                              \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( b_size != ((ULONG)dn_list_start - (ULONG)cert_req + dn_list_length) )  \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                if ( dn_list_length == 0 )                                                  \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                dn_param = (PTLS_HS_PARAMETER)dn_list_start;                                \
                                                                                            \
                while ( dn_param )                                                          \
                {                                                                           \
                    if ( dn_list_length < TlsHsParamGetSize(dn_param) )                     \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        dn_list_length -= TlsHsParamGetSize(dn_param);                      \
                    }                                                                       \
                                                                                            \
                    if ( dn_list_length > 0 )                                               \
                    {                                                                       \
                        dn_param = TlsHsParamGetNext(dn_param);                             \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        dn_param = NULL;                                                    \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  TlsHsCertReqGetTypeCount(p)                ( p->TypeCount        )
#define  TlsHsCertReqGetTypeArray(p)                ( p->TypeArray        )
#define  TlsHsCertReqGetType(p, index)              ( p->TypeArray[index] )
#define  TlsHsCertReqGetEndOfTypeArray(p)           \
         ( (PVOID)((ULONG)p->TypeArray + p->TypeCount) )
#define  TlsHsCertReqGetDnListParam(p)              \
         ( (PTLS_HS_PARAMETER)TlsHsCertReqGetEndOfTypeArray(p) )
#define  TlsHsCertReqGetDnListLength(p)             \
         ( TlsHsParamGetLength(TlsHsCertReqGetDnListParam(p)) )
#define  TlsHsCertReqGetStartOfDnList(p)            \
         ( TlsHsParamGetOpaque(TlsHsCertReqGetDnListParam(p)) )
#define  TlsHsCertReqGetEndOfDnList(p)              \
         ( (PVOID)((ULONG)TlsHsParamGetOpaque(TlsHsCertReqGetDnListParam(p)) + TlsHsParamGetLength(TlsHsCertReqGetDnListParam(p))) )
#define  TlsHsCertReqGetDnCount(p, dn_count)                                                \
         {                                                                                  \
            ULONG                   dn_list_length = TlsHsCertReqGetDnListLength(p);        \
            PTLS_HS_PARAMETER       dn_param       = NULL;                                  \
            ULONG                   dn_param_count = 0;                                     \
                                                                                            \
            dn_count = 0;                                                                   \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( dn_list_length == 0 )                                                  \
                {                                                                           \
                    dn_count = 0;                                                           \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                dn_param = (PTLS_HS_PARAMETER)TlsHsCertReqGetStartOfDnList(p);              \
                                                                                            \
                while ( dn_param )                                                          \
                {                                                                           \
                    dn_param_count++;                                                       \
                                                                                            \
                    dn_list_length -= TlsHsParamGetSize(dn_param);                          \
                                                                                            \
                    if ( dn_list_length > 0 )                                               \
                    {                                                                       \
                        dn_param = TlsHsParamGetNext(dn_param);                             \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        dn_param = NULL;                                                    \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                dn_count = dn_param_count;                                                  \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }
#define  TlsHsCertReqGetDn1(p, dst_dn_param, index)                                         \
         {                                                                                  \
            ULONG                   dn_list_length = TlsHsCertReqGetDnListLength(p);        \
            PTLS_HS_PARAMETER       dn_param       = NULL;                                  \
            ULONG                   dn_param_count = 0;                                     \
                                                                                            \
            dst_dn_param = NULL;                                                            \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( dn_list_length == 0 )                                                  \
                {                                                                           \
                    dst_dn_param = NULL;                                                    \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                dn_param = (PTLS_HS_PARAMETER)TlsHsCertReqGetStartOfDnList(p);              \
                                                                                            \
                while ( dn_param )                                                          \
                {                                                                           \
                    if ( dn_param_count == index )                                          \
                    {                                                                       \
                        dst_dn_param = dn_param;                                            \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                                                                                            \
                    dn_param_count++;                                                       \
                                                                                            \
                    dn_list_length -= TlsHsParamGetSize(dn_param);                          \
                                                                                            \
                    if ( dn_list_length > 0 )                                               \
                    {                                                                       \
                        dn_param = TlsHsParamGetNext(dn_param);                             \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        dn_param = NULL;                                                    \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                break;                                                                      \
            }                                                                               \
            while ( FALSE );                                                                \
         }
#define  TlsHsCertReqGetDn2(p, dn_data, dn_size, index)                                     \
         {                                                                                  \
            PTLS_HS_PARAMETER       dst_dn_param = NULL;                                    \
                                                                                            \
            TlsHsCertReqGetDn1(p, dst_dn_param, index);                                     \
                                                                                            \
            if ( dst_dn_param )                                                             \
            {                                                                               \
                dn_data = TlsHsParamGetOpaque(dst_dn_param);                                \
                dn_size = TlsHsParamGetLength(dst_dn_param);                                \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                dn_data = NULL;                                                             \
                dn_size = 0;                                                                \
            }                                                                               \
         }

#define  TlsHsCertReqSetTypeCount(p, tc)            ( p->TypeCount = tc )
#define  TlsHsCertReqAddType(p, type)                                                       \
         {                                                                                  \
            ULONG                   type_count = TlsHsCertReqGetTypeCount(p);               \
                                                                                            \
            p->TypeArray[type_count++] = type;                                              \
                                                                                            \
            TlsHsCertReqSetTypeCount(p, (UCHAR)type_count);                                 \
         }
#define  TlsHsCertReqSetDnListLength(p, l)          \
         TlsHsParamSetLength(TlsHsCertReqGetDnListParam(p), l)

#define  TlsHsCertReqAddDn1(p, src_dn_param)                                                \
         {                                                                                  \
            ULONG                   dn_list_length = TlsHsCertReqGetDnListLength(p);        \
            PTLS_HS_PARAMETER       dn_param       = NULL;                                  \
                                                                                            \
            dn_param = (PTLS_HS_PARAMETER)TlsHsCertReqGetEndOfDnList(p);                    \
                                                                                            \
            AnscCopyMemory                                                                  \
                (                                                                           \
                    (PVOID)dn_param,                                                        \
                    (PVOID)src_dn_param,                                                    \
                    TlsHsParamGetSize(src_dn_param)                                         \
                );                                                                          \
                                                                                            \
            dn_list_length += TlsHsParamGetSize(dn_param);                                  \
                                                                                            \
            TlsHsCertReqSetDnListLength(p, dn_list_length);                                 \
         }
#define  TlsHsCertReqAddDn2(p, dn_data, dn_size)                                            \
         {                                                                                  \
            ULONG                   dn_list_length = TlsHsCertReqGetDnListLength(p);        \
            PTLS_HS_PARAMETER       dn_param       = NULL;                                  \
                                                                                            \
            dn_param = (PTLS_HS_PARAMETER)TlsHsCertReqGetEndOfDnList(p);                    \
                                                                                            \
            TlsHsParamSetLength(dn_param, dn_size);                                         \
            TlsHsParamSetOpaque(dn_param, dn_data);                                         \
                                                                                            \
            dn_list_length += TlsHsParamGetSize(dn_param);                                  \
                                                                                            \
            TlsHsCertReqSetDnListLength(p, dn_list_length);                                 \
         }

#define  TlsHsCertReqGetBodySize(p)                                                         \
         ( (ULONG)TlsHsCertReqGetEndOfDnList(p) - (ULONG)p )


/***********************************************************
   DATA STRUCTURES USED BY TLS HANDSHAKE SERVER HELLO DONE
***********************************************************/

/*
 * The server hello done message is sent by the server to indicate the end of the server hello and
 * associated messages. After sending this message the server will wait for a client response. This
 * message means that the server is done sending messages to support the key exchange, and the
 * client can proceed with its phase of the key exchange. Upon receipt of the server hello done
 * message the client should verify that the server provided a valid certificate if required and
 * check that the server hello parameters are acceptable.
 *
 *      struct { } ServerHelloDone;
 */


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE CLIENT CERTIFICATE
***********************************************************/

/*
 * This is the first message the client can send after receiving a server hello done message. This
 * message is only sent if the server requests a certificate. If no suitable certificate is
 * available, the client should send a certificate message containing no certificates. If client
 * authentication is required by the server for the handshake to continue, it may respond with a
 * fatal handshake failure alert. Client certificates are sent using the Certificate structure
 * defined in Section 7.4.2.
 */


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE CLIENT KEY EXCHANGE
***********************************************************/

/*
 * This message is always sent by the client. It will immediately follow the client certificate
 * message, if it is sent. Otherwise it will be the first message sent by the client after it
 * receives the server hello done message. With this message, the premaster secret is set, either
 * though direct transmission of the RSA-encrypted secret, or by the transmission of Diffie-
 * Hellman parameters which will allow each side to agree upon the same premaster secret. When the
 * key exchange method is DH_RSA or DH_DSS, client certification has been requested, and the client
 * was able to respond with a certificate which contained a Diffie-Hellman public key whose
 * parameters (group and generator) matched those specified by the server in its certificate, this
 * message will not contain any data.
 *
 *      struct {
 *          select (KeyExchangeAlgorithm) {
 *              case rsa:               EncryptedPreMasterSecret;
 *              case diffie_hellman:    ClientDiffieHellmanPublic;
 *          } exchange_keys;
 *      } ClientKeyExchange;
 *
 *      struct {
 *          ProtocolVersion client_version;
 *          opaque          random[46];
 *      } PreMasterSecret;
 *
 *      struct {
 *          public-key-encrypted PreMasterSecret pre_master_secret;
 *      } EncryptedPreMasterSecret;
 *
 *      enum { implicit, explicit } PublicValueEncoding;
 *
 *      struct {
 *          select (PublicValueEncoding) {
 *              case implicit: struct { };
 *              case explicit: opaque dh_Yc<1..2^16-1>;
 *          } dh_public;
 *      } ClientDiffieHellmanPublic;
 */
typedef  struct
_TLS_HS_CLIENT_KEY_EXCHANGE
{
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_CLIENT_KEY_EXCHANGE,  *PTLS_HS_CLIENT_KEY_EXCHANGE;

#define  TlsHsClientKeValidate(key_exchange, b_size, ssl_ver, ke_algm, b_implicit, v_result)\
         {                                                                                  \
            ULONG                   param_list_size = b_size;                               \
            PTLS_HS_PARAMETER       param_dh_y      = NULL;                                 \
            PTLS_HS_PARAMETER       param_epms      = NULL;                                 \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            if ( ke_algm == TLS_HS_KE_ALGORITHM_rsa )                                       \
            {                                                                               \
                do                                                                          \
                {                                                                           \
                    if ( ssl_ver == 0 )                                                     \
                    {                                                                       \
                        if ( b_size < 1 )                                                   \
                        {                                                                   \
                            v_result = FALSE;                                               \
                        }                                                                   \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_epms = (PTLS_HS_PARAMETER)key_exchange->Data;                 \
                                                                                            \
                        if ( TlsHsParamGetLength(param_epms) < 1 )                          \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size != TlsHsParamGetSize(param_epms) )        \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                while ( FALSE );                                                            \
            }                                                                               \
            else if ( ke_algm == TLS_HS_KE_ALGORITHM_diffieHellman )                        \
            {                                                                               \
                do                                                                          \
                {                                                                           \
                    if ( !b_implicit )     /* dh_Yc <1..2^16-1> */                          \
                    {                                                                       \
                        if ( param_list_size < sizeof(USHORT) )                             \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                                                                                            \
                        param_dh_y = (PTLS_HS_PARAMETER)key_exchange->Data;                 \
                                                                                            \
                        if ( TlsHsParamGetLength(param_dh_y) < 1 )                          \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                        else if ( param_list_size != TlsHsParamGetSize(param_dh_y) )        \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        if ( param_list_size != 0 )                                         \
                        {                                                                   \
                            v_result = FALSE;                                               \
                                                                                            \
                            break;                                                          \
                        }                                                                   \
                    }                                                                       \
                }                                                                           \
                while ( FALSE );                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
         }

#define  TlsHsClientKeGetEpms1(p, epms_param)                                               \
         {                                                                                  \
            epms_param = (PTLS_HS_PARAMETER)p->Data;                                        \
         }
#define  TlsHsClientKeGetEpms2(p, b_size, ssl_ver, epms_data, epms_size)                    \
         {                                                                                  \
            PTLS_HS_PARAMETER       epms_param = NULL;                                      \
                                                                                            \
            if ( ssl_ver == 0 )                                                             \
            {                                                                               \
                epms_size = b_size;                                                         \
                epms_data = p->Data;                                                        \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                TlsHsClientKeGetEpms1(p, epms_param);                                       \
                                                                                            \
                epms_size = TlsHsParamGetLength(epms_param);                                \
                epms_data = TlsHsParamGetOpaque(epms_param);                                \
            }                                                                               \
         }

#define  TlsHsClientKeGetDHPublic1(p, dh_param)     \
         ( dh_param = (PTLS_HS_PARAMETER)p->Data )
#define  TlsHsClientKeGetDHPublic2(p, dh_y_data, dh_y_size)                                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_param = NULL;                                        \
                                                                                            \
            TlsHsClientKeGetDHPublic1(p, dh_param);                                         \
                                                                                            \
            dh_y_size = TlsHsParamGetLength(dh_param);                                      \
            dh_y_data = TlsHsParamGetOpaque(dh_param);                                      \
         }

#define  TlsHsClientKeSetEpms1(p, src_epms_param)                                           \
         {                                                                                  \
            PTLS_HS_PARAMETER       epms_param = NULL;                                      \
                                                                                            \
            TlsHsClientKeGetEpms1(p, epms_param);                                           \
                                                                                            \
            TlsHsParamSetLength(epms_param, TlsHsParamGetLength(src_epms_param));           \
            TlsHsParamSetOpaque(epms_param, TlsHsParamGetOpaque(src_epms_param));           \
         }
#define  TlsHsClientKeSetEpms2(p, ssl_ver, epms_data, epms_size)                            \
         {                                                                                  \
            PTLS_HS_PARAMETER       epms_param = NULL;                                      \
                                                                                            \
            if ( ssl_ver == 0 )                                                             \
            {                                                                               \
                AnscCopyMemory(p->Data, epms_data, epms_size);                              \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                TlsHsClientKeGetEpms1(p, epms_param);                                       \
                                                                                            \
                TlsHsParamSetLength(epms_param, epms_size);                                 \
                TlsHsParamSetOpaque(epms_param, epms_data);                                 \
            }                                                                               \
         }

#define  TlsHsClientKeSetDHPublic1(p, src_dh_param)                                         \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_param = NULL;                                        \
                                                                                            \
            TlsHsClientKeGetDHPublic1(p, dh_param);                                         \
                                                                                            \
            TlsHsParamSetLength(dh_param, TlsHsParamGetLength(src_dh_param));               \
            TlsHsParamSetOpaque(dh_param, TlsHsParamGetOpaque(src_dh_param));               \
         }
#define  TlsHsClientKeSetDHPublic2(p, dh_y_data, dh_y_size)                                 \
         {                                                                                  \
            PTLS_HS_PARAMETER       dh_param = NULL;                                        \
                                                                                            \
            TlsHsClientKeGetDHPublic1(p, dh_param);                                         \
                                                                                            \
            TlsHsParamSetLength(dh_param, dh_y_size);                                       \
            TlsHsParamSetOpaque(dh_param, dh_y_data);                                       \
         }

#define  TlsHsClientKeGetBodySize(p, b_implicit)                                            \
         ( b_implicit? 0 : TlsHsParamGetSize(((PTLS_HS_PARAMETER)p->Data)) )


/***********************************************************
  DATA STRUCTURES USED BY TLS HANDSHAKE CERTIFICATE VERIFY
***********************************************************/

/*
 * This message is used to provide explicit verification of a client certificate. This message is
 * only sent following a client certificate that has signing capability (i.e. all certificates
 * except those containing fixed Diffie-Hellman parameters). When sent, it will immediately follow
 * the client key exchange message.
 *
 *      struct {
 *          Signature   signature;
 *      } CertificateVerify;
 */
typedef  struct
_TLS_HS_CERT_VERIFY
{
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_CERT_VERIFY,  *PTLS_HS_CERT_VERIFY;

#define  TlsHsCertVerifyValidate(cert_verify, b_size, v_result)                             \
         {                                                                                  \
            ULONG                   param_list_size = b_size;                               \
            PTLS_HS_PARAMETER       param_sig       = NULL;                                 \
                                                                                            \
            v_result = TRUE;                                                                \
                                                                                            \
            do                                                                              \
            {                                                                               \
                if ( param_list_size < sizeof(USHORT) )                                     \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                                                                                            \
                param_sig = (PTLS_HS_PARAMETER)cert_verify->Data;                           \
                                                                                            \
                if ( TlsHsParamGetLength(param_sig) < 1 )                                   \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
                else if ( param_list_size != TlsHsParamGetSize(param_sig) )                 \
                {                                                                           \
                    v_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
            while ( FALSE );                                                                \
         }

#define  TlsHsCertVerifyGetSignature1(p, sig_param) \
         ( sig_param = (PTLS_HS_PARAMETER)p->Data )
#define  TlsHsCertVerifyGetSignature2(p, sig_data, sig_size)                                \
         {                                                                                  \
            PTLS_HS_PARAMETER       sig_param = NULL;                                       \
                                                                                            \
            TlsHsCertVerifyGetSignature1(p, sig_param);                                     \
                                                                                            \
            sig_size = TlsHsParamGetLength(sig_param);                                      \
            sig_data = TlsHsParamGetOpaque(sig_param);                                      \
         }

#define  TlsHsCertVerifySetSignature1(p, src_sig_param)                                     \
         {                                                                                  \
            PTLS_HS_PARAMETER       sig_param = NULL;                                       \
                                                                                            \
            TlsHsCertVerifyGetSignature1(p, sig_param);                                     \
                                                                                            \
            TlsHsParamSetLength(sig_param, TlsHsParamGetLength(src_sig_param));             \
            TlsHsParamSetOpaque(sig_param, TlsHsParamGetOpaque(src_sig_param));             \
         }
#define  TlsHsCertVerifySetSignature2(p, sig_data, sig_size)                                \
         {                                                                                  \
            PTLS_HS_PARAMETER       sig_param = NULL;                                       \
                                                                                            \
            TlsHsCertVerifyGetSignature1(p, sig_param);                                     \
                                                                                            \
            TlsHsParamSetLength(sig_param, sig_size);                                       \
            TlsHsParamSetOpaque(sig_param, sig_data);                                       \
         }

#define  TlsHsCertVerifyGetBodySize(p)                                                      \
         ( TlsHsParamGetSize(((PTLS_HS_PARAMETER)p->Data)) )


/***********************************************************
       DATA STRUCTURES USED BY TLS HANDSHAKE FINISHED
***********************************************************/

/*
 * A finished message is always sent immediately after a change cipher spec message to verify that
 * the key exchange and authentication processes were successful. It is essential that a change
 * cipher spec message be received between the other handshake messages and the Finished message.
 * The finished message is the first protected with the just-negotiated algorithms, keys, and
 * secrets. Recipients of finished messages must verify that the contents are correct. Once a side
 * has sent its Finished message and received and validated the Finished message from its peer, it
 * may begin to send and receive application data over the connection.
 *
 *      struct {
 *          opaque verify_data[12];
 *      } Finished;
 */
typedef  struct
_TLS_HS_FINISHED
{
    UCHAR                           VerifyData[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
TLS_HS_FINISHED,  *PTLS_HS_FINISHED;

#define  TlsHsFinishedValidate(finished, b_size, minor_ver, v_result)                       \
         {                                                                                  \
            if ( b_size == 12 && minor_ver == 1 )                                           \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
            else if ( b_size == 36 && minor_ver == 0 )                                      \
            {                                                                               \
                v_result = TRUE;                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
         }

#define  TlsHsFinishedGetVerifyData(p)              ( p->VerifyData )
#define  TlsHsFinishedSetVerifyData(p, d, s)        AnscCopyMemory(p->VerifyData, d, s)


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
