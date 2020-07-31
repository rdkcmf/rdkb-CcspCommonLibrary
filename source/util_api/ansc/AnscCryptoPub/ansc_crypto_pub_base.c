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

    module:	ansc_crypto_pub_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Crypto Object.

        *   AnscCryptoPubCreate
        *   AnscCryptoPubRemove

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

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscCryptoPubCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the Command Translator Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscCryptoPubCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_CRYPTO_PUB_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_CRYPTO_PUB_OBJECT)AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object
     */
    pMyObject->hOwnerContext          = hOwnerContext;
    pMyObject->Create                 = AnscCryptoPubCreate;
    pMyObject->Remove                 = AnscCryptoPubRemove;
    pMyObject->Initialize             = AnscCryptoPubInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoPubRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CRYPTO_PUB_OBJECT         pMyObject    = (PANSC_CRYPTO_PUB_OBJECT)hThisObject;

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoPubInitialize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoPubInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CRYPTO_PUB_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_CRYPTO_PUB_OBJECT)hThisObject;

    /*
     * Initialize the common variables and functions for a container object
     */
    pMyObject->Create                 = AnscCryptoPubCreate;
    pMyObject->Remove                 = AnscCryptoPubRemove;

    /*
     * Initialize the specific variables and functions for this object
     */
    pMyObject->Request                = AnscCryptoPubRequest;
    pMyObject->KeyPairMatch           = AnscCryptoPubKeyPairMatch;
    pMyObject->RSAHashAndSign         = AnscCryptoPubKeyRSASign;
    pMyObject->RSAHashAndVerify       = AnscCryptoPubKeyRSAVerify;
    pMyObject->DSAHashAndSign         = AnscCryptoPubKeyDSASign;
    pMyObject->DSAHashAndVerify       = AnscCryptoPubKeyDSAVerify;

    return ANSC_STATUS_SUCCESS;
}

