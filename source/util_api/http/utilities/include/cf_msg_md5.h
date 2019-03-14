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

    module:	cf_msg_md5.h

        For Variant MD5,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        Provides a variant MD5.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/25/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/


#ifndef  _CF_MSG_MD5
#define  _CF_MSG_MD5

#include "ansc_crypto.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"


void 
Convert2Hex
    (
        PUCHAR                      pMD5
    );


PUCHAR
ToMD5
    (
        PANSC_CRYPTO_OBJECT         pCryptoObj,
        PUCHAR                      pPassword
    );

#endif

