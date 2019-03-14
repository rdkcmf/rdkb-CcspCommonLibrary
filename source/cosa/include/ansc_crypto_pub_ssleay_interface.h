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

    module:	ansc_crypto_pub_ssleay_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object interface for the
        Crypto Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_CRYPTO_PUB_SSLEAY_INTERFACE_
#define  _ANSC_CRYPTO_PUB_SSLEAY_INTERFACE_

#include "ansc_crypto_pub_interface.h"

/***********************************************************
              GENERAL CRYPTO OBJECT DEFINITION
***********************************************************/

#define  RSA_SIGN_WITH_SHA1                         0
#define  RSA_SIGN_WITH_MD5                          1
#define  RSA_SIGN_WITH_MD2                          2
#define  RSA_SIGN_WITH_MD4                          3

#define  RSA_VERIFY_WITH_SHA1                       0
#define  RSA_VERIFY_WITH_MD5                        1
#define  RSA_VERIFY_WITH_MD2                        2
#define  RSA_VERIFY_WITH_MD4                        3

/*
 * The Crypto Object provides a set of crypto wrapper functions for other container objects to
 * perform symmetric encryption/decryption, hmac-based authentication, one-way hash function and
 * asymmetric encryption/decryption, and signature verification.
 */
typedef  struct
_ANSC_CRYPTO_PUB_SSLEAY_OBJECT
{
    _ANSC_CRYPTO_PUB_BASE
}
ANSC_CRYPTO_PUB_SSLEAY_OBJECT,  *PANSC_CRYPTO_PUB_SSLEAY_OBJECT;


#endif
