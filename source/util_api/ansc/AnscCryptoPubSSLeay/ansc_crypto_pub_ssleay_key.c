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

    Module:	ansc_crypto_pub_ssleay_key.c

        For Virtual Private Network Driver
        by Cisco Systems, Inc.

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


PVOID
AnscCryptoCreateSSLeayDSA
    (
        PVOID                       pKey,
        BOOL                        bPublicKey
    )
{
    UNREFERENCED_PARAMETER(pKey);
    UNREFERENCED_PARAMETER(bPublicKey);
    return NULL;
}


PVOID
AnscCryptCreateSSLeayRSA
    (
        PVOID                       pKey,
        BOOL                        bPublicKey
    )
{
    UNREFERENCED_PARAMETER(pKey);
    UNREFERENCED_PARAMETER(bPublicKey);
    return NULL;
}

