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

    Module:	ansc_crypto_pub_ssleay_dh.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    Functional Description:

        Implementation of public key request

    ---------------------------------------------------------------

    Environment:

        System Independent

    ---------------------------------------------------------------

    Author:

        Kang Quan

    ---------------------------------------------------------------

    Revision history:

        *   05/13/02    initial revision

**********************************************************************/

#include "ansc_crypto_pub_ssleay_global.h"


ANSC_STATUS
AnscCryptoPubSSLeayGenDHKeyPairRandomly
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    )
{
	return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    AL_CRYPT API FUNCTION

    ANSC_STATUS
    AnscCryptoPubSSLeayAgreeDHSharedSecret
        (
            PANSC_CRYPTO_DH_PARAMETERS  pDH
        )

    This API function will be called to agree on a shared DH secret
    value based the previous DH instance and peer's public value.

    argument:   AL_HANDLE               hDHGroupContext
                Returned from ALCryptGenerateDHSecret.

                PANSC_CRYPTO_INTEGER    pPeerDHPublicValue
                Pointer to the DH public value generated by peer.

                PANSC_CRYPTO_INTEGER    pDHSharedSecret
                Pointer to the final DH shared secret value.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubSSLeayAgreeDHSharedSecret
    (
        PANSC_CRYPTO_DH_PARAMETERS  pDH
    )
{
    return  ANSC_STATUS_SUCCESS;
}


