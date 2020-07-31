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

    module:	ansc_crypto_pub_interface.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to crypto operation.

        *   AnscCreatePubCrypto

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


#include "ansc_crypto_pub_global.h"


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscCreateCryptoPub
            (
                ANSC_HANDLE                 hOwnerContext
            )

    description:

        This function is called to create a new crypto object.

    argument:   ANSC_HANDLE                 hOwnerContext
                This context handle is transparent to the object
                descriptor wrapper, it's only meanful to the caller.

    return:     handle of the crypto object.

**********************************************************************/

ANSC_HANDLE
AnscCreateCryptoPub
    (
        ANSC_HANDLE                 hOwnerContext
    )
{

    return  AnscCryptoPubCreate(hOwnerContext);
}
