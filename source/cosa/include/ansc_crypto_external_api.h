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

    module:	ansc_crypto_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Crypto Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        09/13/01    revised to work in BroadWay 2.0

**********************************************************************/


#ifndef  _ANSC_CRYPTO_EXTERNAL_API_
#define  _ANSC_CRYPTO_EXTERNAL_API_


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_CRYPTO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateCrypto
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscCryptoRemove
    (
        ANSC_HANDLE                 hOwnerContext
    );

/**********************************************************/

ULONG
AnscCryptoMd5Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash
    );

ULONG
AnscCryptoHmacSha1Digest
    (
        PVOID                       buffer,
        ULONG                       size,
        PANSC_CRYPTO_HASH           hash,
        PANSC_CRYPTO_KEY            key
    );

ULONG
AnscCryptoGetRandomUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rand_seed
    );

#endif
