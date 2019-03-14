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

    module:	tls_cpo_random.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced random number generate
        functions of the TLS Crypto Provider Object.

        *   TlsCpoGetRandomBytes

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGetRandomBytes
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       random_seed,
                PUCHAR                      random_bytes,
                ULONG                       random_len
            );

    description:

        This function is called to generate a number of bytes randomly.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       random_seed
                Specifies the seed for the random number generator.

                PUCHAR                      random_bytes
                Specifies the outupt buffer for the randomly generated
                numbers.

                ULONG                       random_len
                Specifies the number of bytes to be generated.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoGetRandomBytes
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       random_seed,
        PUCHAR                      random_bytes,
        ULONG                       random_len
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;

    return
        pAnscCrypto->GetRandomOctets
            (
                (ANSC_HANDLE)pAnscCrypto,
                random_seed,
                random_bytes,
                random_len
            );
}
