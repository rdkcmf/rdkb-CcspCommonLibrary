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

    module:	tls_cpo_cipher.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced cipher suite handling
        functions of the TLS Crypto Provider Object.

        *   TlsCpoGetCipherInfo
        *   TlsCpoGetCertConfigBit
        *   TlsCpoGetClientCertType
        *   TlsCpoGetKeAlgorithm
        *   TlsCpoGetKeyBlockSize
        *   TlsCpoGetBulkEncryptionAlgorithm
        *   TlsCpoGetCipherType
        *   TlsCpoGetCipherKeyNumber
        *   TlsCpoGetCipherKeySize
        *   TlsCpoGetCipherKeyMaterialSize
        *   TlsCpoGetCipherIVSize
        *   TlsCpoGetCipherBlockSize
        *   TlsCpoGetMacAlgorithm
        *   TlsCpoGetMacHashSize
        *   TlsCpoGetExportable
        *   TlsCpoGetStrongSecurity

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
#include "tls_ciphersuite_list.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGetCipherInfo
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite,
                ANSC_HANDLE                 hSecurityParams
            );

    description:

        This function is called to retrieve all the security settings
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                settings to be retrieved.

                ANSC_HANDLE                 hSecurityParams
                Specifies the security parameters data structure to be
                filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoGetCipherInfo
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite,
        ANSC_HANDLE                 hSecurityParams
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PTLS_SECURITY_PARAMS            pSecurityParams  = (PTLS_SECURITY_PARAMS       )hSecurityParams;
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    pSecurityParams->KeyBlockSize            = pCipherSuiteDesp->KeyBlockSize;
    pSecurityParams->BulkEncryptionAlgorithm = pCipherSuiteDesp->BulkEncryptionAlgorithm;
    pSecurityParams->CipherType              = pCipherSuiteDesp->CipherType;
    pSecurityParams->CipherKeyNumber         = pCipherSuiteDesp->CipherKeyNumber;
    pSecurityParams->CipherKeySize           = pCipherSuiteDesp->CipherKeySize;
    pSecurityParams->CipherKeyMaterialSize   = pCipherSuiteDesp->CipherKeyMaterialSize;
    pSecurityParams->CipherIVSize            = pCipherSuiteDesp->CipherIVSize;
    pSecurityParams->CipherBlockSize         = pCipherSuiteDesp->CipherBlockSize;
    pSecurityParams->MacAlgorithm            = pCipherSuiteDesp->MacAlgorithm;
    pSecurityParams->MacHashSize             = pCipherSuiteDesp->MacHashSize;
    pSecurityParams->CompressionAlgorithm    = pCipherSuiteDesp->CompressionAlgorithm;
    pSecurityParams->bExportable             = pCipherSuiteDesp->bExportable;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCertConfigBit
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCertConfigBit
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }


    return  pCipherSuiteDesp->CertConfigBit;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetClientCertType
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetClientCertType
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }


    return  pCipherSuiteDesp->ClientCertType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetKeAlgorithm
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetKeAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }


    return  pCipherSuiteDesp->KeAlgorithm;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetKeyBlockSize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetKeyBlockSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->KeyBlockSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetBulkEncryptionAlgorithm
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetBulkEncryptionAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->BulkEncryptionAlgorithm;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherType
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherType
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherKeyNumber
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherKeyNumber
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherKeyNumber;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherKeySize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherKeySize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherKeySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherKeyMaterialSize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherKeyMaterialSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherKeyMaterialSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherIVSize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherIVSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherIVSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetCipherBlockSize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetCipherBlockSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->CipherBlockSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetMacAlgorithm
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetMacAlgorithm
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->MacAlgorithm;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        TlsCpoGetMacHashSize
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ULONG
TlsCpoGetMacHashSize
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->MacHashSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGetExportable
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ANSC_STATUS
TlsCpoGetExportable
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->bExportable;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoGetStrongSecurity
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      cipher_suite
            );

    description:

        This function is called to retrieve a security setting.
        all together.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      cipher_suite
                Specifies the cipher suite for which the security
                setting to be retrieved.

    return:     security setting.

**********************************************************************/

ANSC_STATUS
TlsCpoGetStrongSecurity
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      cipher_suite
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           ulSuiteIndex     = (ULONG                      )cipher_suite;
    PTLS_CIPHERSUITE_DESCRIPTOR     pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)NULL;

    if( cipher_suite > TLS_DH_anon_WITH_3DES_EDE_CBC_SHA && cipher_suite < TLS_RSA_WITH_AES_128_CBC_SHA)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }

    if ( cipher_suite > TLS_MAX_ALGORITHM_NUMBER)
    {
        return ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        if( cipher_suite >= TLS_RSA_WITH_AES_128_CBC_SHA)
        {
            ulSuiteIndex = cipher_suite - TLS_ALGORITHM_GAP;
        }

        pCipherSuiteDesp = (PTLS_CIPHERSUITE_DESCRIPTOR)&g_tlsCipherSuiteList[ulSuiteIndex];
    }

    return  pCipherSuiteDesp->bStrongSecurity;
}
