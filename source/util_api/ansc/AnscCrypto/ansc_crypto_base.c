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

    module:	ansc_crypto_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Crypto Object.

        *   AnscCryptoCreate
        *   AnscCryptoRemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        06/02/04    zhubin added 3 rc2 related apis
        03/09/05    zhubin added uuid generation apis
        08/03/05    zhubin added zlib and zip support

**********************************************************************/


#include "ansc_crypto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscCryptoCreate
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
AnscCryptoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_CRYPTO_OBJECT             pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_CRYPTO_OBJECT)AnscAllocateMemory(sizeof(ANSC_CRYPTO_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object
     */
    pMyObject->hOwnerContext          = hOwnerContext;
    pMyObject->Create                 = AnscCryptoCreate;
    pMyObject->Remove                 = AnscCryptoRemove;

    /*
     * Initialize the specific variables and functions for this object
     */
    pMyObject->Digest                 = AnscCryptoDigest;
    pMyObject->HmacMd5Digest          = AnscCryptoHmacMd5Digest;
    pMyObject->HmacSha1Digest         = AnscCryptoHmacSha1Digest;
    pMyObject->DesMacDigest           = AnscCryptoDesMacDigest;
    pMyObject->KpdkDigest             = AnscCryptoKpdkDigest;
    pMyObject->Md5Digest              = AnscCryptoMd5Digest;
    pMyObject->Sha1Digest             = AnscCryptoSha1Digest;
    pMyObject->Md4Digest              = AnscCryptoMd4Digest;
    pMyObject->Md2Digest              = AnscCryptoMd2Digest;

    pMyObject->Encrypt                = AnscCryptoEncrypt;
    pMyObject->DesIv64Encrypt         = AnscCryptoDesIv64Encrypt;
    pMyObject->DesEncrypt             = AnscCryptoDesEncrypt;
    pMyObject->TripleDesEncrypt       = AnscCryptoTripleDesEncrypt;
    pMyObject->Rc5Encrypt             = AnscCryptoRc5Encrypt;
    pMyObject->IdeaEncrypt            = AnscCryptoIdeaEncrypt;
    pMyObject->CastEncrypt            = AnscCryptoCastEncrypt;
    pMyObject->BlowfishEncrypt        = AnscCryptoBlowfishEncrypt;
    pMyObject->TripleIdeaEncrypt      = AnscCryptoTripleIdeaEncrypt;
    pMyObject->DesIv32Encrypt         = AnscCryptoDesIv32Encrypt;

#ifndef _ANSC_CRYPTO_NO_RC4_
    pMyObject->Rc4Encrypt             = AnscCryptoRc4Encrypt;
    pMyObject->TLSRc4Encrypt          = AnscCryptoTLSRc4Encrypt;
#endif

    pMyObject->AesEncrypt             = AnscCryptoAesEncrypt;

    pMyObject->Decrypt                = AnscCryptoDecrypt;
    pMyObject->DesIv64Decrypt         = AnscCryptoDesIv64Decrypt;
    pMyObject->DesDecrypt             = AnscCryptoDesDecrypt;
    pMyObject->TripleDesDecrypt       = AnscCryptoTripleDesDecrypt;
    pMyObject->Rc5Decrypt             = AnscCryptoRc5Decrypt;
    pMyObject->IdeaDecrypt            = AnscCryptoIdeaDecrypt;
    pMyObject->CastDecrypt            = AnscCryptoCastDecrypt;
    pMyObject->BlowfishDecrypt        = AnscCryptoBlowfishDecrypt;
    pMyObject->TripleIdeaDecrypt      = AnscCryptoTripleIdeaDecrypt;
    pMyObject->DesIv32Decrypt         = AnscCryptoDesIv32Decrypt;

#ifndef _ANSC_CRYPTO_NO_RC4_
    pMyObject->Rc4Decrypt             = AnscCryptoRc4Decrypt;
    pMyObject->TLSRc4Decrypt          = AnscCryptoTLSRc4Decrypt;
#endif

    pMyObject->AesDecrypt             = AnscCryptoAesDecrypt;

    pMyObject->Compress               = AnscCryptoCompress;
    pMyObject->OutCompress            = AnscCryptoOutCompress;
    pMyObject->DeflateCompress        = AnscCryptoDeflateCompress;
    /* remove LZS related APIs, two reasons:
     * LZS is NOT free software
     * LzsCompress/Decompress, as a IPSec compression option, is not used by vendors
     */

# ifdef _ANSC_LZS_USED_
    pMyObject->LzsCompress            = AnscCryptoLzsCompress;
    pMyObject->LzsDecompress          = AnscCryptoLzsDecompress;
# endif
    pMyObject->V42bisCompress         = AnscCryptoV42bisCompress;
    pMyObject->ZlibCompress           = AnscCryptoZlibCompress;
#ifdef _ANSC_GZIP_USED_
    pMyObject->GzipCompress           = AnscCryptoGzipCompress;
    pMyObject->GzipDecompress         = AnscCryptoGzipDecompress;
#else
    pMyObject->GzipCompress           = AnscCryptoZlibCompress;
    pMyObject->GzipDecompress         = AnscCryptoZlibDecompress;
#endif
    pMyObject->Decompress             = AnscCryptoDecompress;
    pMyObject->OutDecompress          = AnscCryptoOutDecompress;
    pMyObject->DeflateDecompress      = AnscCryptoDeflateDecompress;
    pMyObject->V42bisDecompress       = AnscCryptoV42bisDecompress;
    pMyObject->ZlibDecompress         = AnscCryptoZlibDecompress;

    pMyObject->DesPrepareKey          = AnscCryptoDesPrepareKey;
    pMyObject->MsChap1LmHash          = AnscCryptoMsChap1LmHash;
    pMyObject->MsChap1DeriveKey1      = AnscCryptoMsChap1DeriveKey1;
    pMyObject->MsChap1DeriveKey2      = AnscCryptoMsChap1DeriveKey2;
    pMyObject->MsChap2GetMKey         = AnscCryptoMsChap2GetMKey;
    pMyObject->MsChap2GetAKey         = AnscCryptoMsChap2GetAKey;
    pMyObject->MsChap2GetNKey         = AnscCryptoMsChap2GetNKey;
    pMyObject->MsChap2GetAuthResponse = AnscCryptoMsChap2GetAuthResponse;

    pMyObject->HashMd5                = AnscCryptoHashMd5;
    pMyObject->HashSha1               = AnscCryptoHashSha1;
    pMyObject->HashMd4                = AnscCryptoHashMd4;

    pMyObject->GetRandomUlong         = AnscCryptoGetRandomUlong;
    pMyObject->GetRandomUshort        = AnscCryptoGetRandomUshort;
    pMyObject->GetRandomOctets        = AnscCryptoGetRandomOctets;

    pMyObject->DesMacKDigest          = AnscCryptoDesMacKDigest;
    pMyObject->IsDesWeakKey           = AnscCryptoIsDesWeakKey;
    pMyObject->FixParity              = AnscCryptoFixParity;
    pMyObject->CheckParity            = AnscCryptoCheckParity;
    pMyObject->CorrectDesKey          = AnscCryptoCorrectDesKey;
    pMyObject->ReverseByte            = AnscCryptoReverseByte;
    pMyObject->XorByte                = AnscCryptoXorByte;
    pMyObject->MitDesStrToKey         = AnscCryptoMitDesStrToKey;

    pMyObject->NFold                  = AnscCryptoNFold;
    pMyObject->RandomToKey            = AnscCryptoRandomToKey;
    pMyObject->MapEncTypeToKeySize    = AnscCryptoMapEncTypeToKeySize;
    pMyObject->DesRandomToKey         = AnscCryptoDesRandomToKey;
    pMyObject->Des3RandomToKey        = AnscCryptoDes3RandomToKey;
    pMyObject->DeriveKey              = AnscCryptoDeriveKey;
    pMyObject->Des3StringToKey        = AnscCryptoDes3StringToKey;

#ifndef _ANSC_CRYPTO_NO_RC2_
    pMyObject->Rc2Encrypt             = AnscCryptoRc2Encrypt;
    pMyObject->Rc2Decrypt             = AnscCryptoRc2Decrypt;
    pMyObject->Crypt_rc2_keyschedule  = AnscCryptoCrypt_rc2_keyschedule;
    pMyObject->Crypt_rc2_encrypt      = AnscCryptoCrypt_rc2_encrypt;
    pMyObject->Crypt_rc2_decrypt      = AnscCryptoCrypt_rc2_decrypt;
#endif

#ifndef _ANSC_CRYPTO_NO_UUID
    pMyObject->GenerateUuid           = AnscCryptoGenerateUuid;
    pMyObject->GenerateUuid2          = AnscCryptoGenerateUuid2;
#endif

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCryptoRemove
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
AnscCryptoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_CRYPTO_OBJECT             pMyObject    = (PANSC_CRYPTO_OBJECT)hThisObject;

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
