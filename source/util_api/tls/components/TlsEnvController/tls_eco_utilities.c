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

    module:	tls_eco_utilities.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the TLS Env Controller Object.

        *   TlsEcoSsl2ToSsl3ClientHello

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


#include "tls_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoSsl2ToSsl3ClientHello
            (
                ANSC_HANDLE                 hThisObject,
                void*                       pSrcMessage,
                ULONG                       ulSrcMsgSize,
                void*                       pDstMessage,
                PULONG                      pulDstMsgSize
            );

    description:

        This function is called to translate a SSLv2 message into a
        SSLv3 message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       pSrcMessage
                Specifies the SSLv2 message to be translated.

                ULONG                       ulSrcMsgSize
                Specifies the SSLv2 message size.

                void*                       pDstMessage
                Specifies the SSLv3 message to be translated to.

                PULONG                      pulDstMsgSize
                Specifies the translated SSLv3 message size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoSsl2ToSsl3ClientHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcMessage,
        ULONG                       ulSrcMsgSize,
        void*                       pDstMessage,
        PULONG                      pulDstMsgSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulSrcMsgSize);
    PSSL2_RECORD_HEADER             pSsl2RecordHeader  = (PSSL2_RECORD_HEADER         )pSrcMessage;
    PSSL2_HS_CLIENT_HELLO           pSsl2HsClientHello = (PSSL2_HS_CLIENT_HELLO       )Ssl2RecordGetData(pSsl2RecordHeader);
    PTLS_RECORD_HEADER              pTlsRecordHeader   = (PTLS_RECORD_HEADER          )pDstMessage;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER       )TlsRecordGetFragment(pTlsRecordHeader);
    PTLS_HS_CLIENT_HELLO            pTlsHsClientHello  = (PTLS_HS_CLIENT_HELLO        )TlsHandshakeGetBody (pTlsHsHeader    );
    UCHAR                           usMajorVersion     = (UCHAR                       )Ssl2HsClientHelloGetMajorVersion(pSsl2HsClientHello);
    UCHAR                           usMinorVersion     = (UCHAR                       )Ssl2HsClientHelloGetMinorVersion(pSsl2HsClientHello);
    ULONG                           ulCipherSuiteCount = (ULONG                       )0;
    USHORT                          usCipherSuite      = (USHORT                      )0;
    ULONG                           ulTlsHsBodySize    = (ULONG                       )0;
    ULONG                           i                  = 0;

    if ( TRUE )
    {
        TlsHsClientHelloSetMajorVersion(pTlsHsClientHello, usMajorVersion);
        TlsHsClientHelloSetMinorVersion(pTlsHsClientHello, usMinorVersion);

        AnscZeroMemory(TlsHsClientHelloGetRandom(pTlsHsClientHello), sizeof(TLS_HS_RANDOM));
        AnscCopyMemory
            (
                (PVOID)((ULONG)TlsHsClientHelloGetRandom(pTlsHsClientHello) + sizeof(TLS_HS_RANDOM) - Ssl2HsClientHelloGetChallengeLength(pSsl2HsClientHello)),
                Ssl2HsClientHelloGetChallengeData  (pSsl2HsClientHello),
                Ssl2HsClientHelloGetChallengeLength(pSsl2HsClientHello)
            );

        TlsHsClientHelloSetSessionId
            (
                pTlsHsClientHello,
                Ssl2HsClientHelloGetSessionIdLength(pSsl2HsClientHello),
                Ssl2HsClientHelloGetSessionIdData  (pSsl2HsClientHello)
            );

        if ( TRUE )
        {
            ulCipherSuiteCount = Ssl2HsClientHelloGetCipherSpecsCount(pSsl2HsClientHello);

            TlsHsClientHelloSetCipherSuiteCount(pTlsHsClientHello, ulCipherSuiteCount);

            for ( i = 0; i < ulCipherSuiteCount; i++ )
            {
                usCipherSuite = Ssl2HsClientHelloGetCipherSpecByIndex(pSsl2HsClientHello, i);

                TlsHsClientHelloSetCipherSuite(pTlsHsClientHello, usCipherSuite, i);
            }
        }

        if ( TRUE )
        {
            TlsHsClientHelloSetCompressionCount(pTlsHsClientHello, 1);
            TlsHsClientHelloSetCompression     (pTlsHsClientHello, TLS_COMPRESSION_METHOD_NULL, 0);
        }

        ulTlsHsBodySize = TlsHsClientHelloGetBodySize(pTlsHsClientHello);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_clientHello);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize               );
    }

    if ( TRUE )
    {
        TlsRecordSetContentType (pTlsRecordHeader, TLS_RECORD_TYPE_handshake                );
        TlsRecordSetMajorVersion(pTlsRecordHeader, usMajorVersion                           );
        TlsRecordSetMinorVersion(pTlsRecordHeader, usMinorVersion                           );
        TlsRecordSetLength      (pTlsRecordHeader, (USHORT)TlsHandshakeGetSize(pTlsHsHeader));
    }

    *pulDstMsgSize = TlsRecordGetSize(pTlsRecordHeader);

    return  ANSC_STATUS_SUCCESS;
}
