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

#ifndef  _CERT_ATTR_STRUCT_H
#define  _CERT_ATTR_STRUCT_H

#define  ATTRIBUTE_BUFFER_SIZE                      64
#define  SUBJECT_ALT_NAME_SIZE                      128

typedef  enum
_AL_HASH_TYPE
{
    HASH_RESERVE        = 0,
    HASH_MD5,
    HASH_SHA1
}
HASH_TYPE,  *PHASH_TYPE;

/*
 *  Definitions of PKI Signature
 */
typedef  enum
_AL_SIGNATURE_TYPE
{
    RSA_MD5_SIGNATURE   = 0,
    RSA_MD2_SIGNATURE,
    RSA_SHA1_SIGNATURE,
    DSA_SHA1_SIGNATURE,
    RSA_MD4_SIGNATURE,
    SIGNATURE_RESERVED
}
SIGNATURE_TYPE,  *PSIGNATURE_TYPE;

typedef  enum
_AL_PKI_KEY_TYPE
{
    PKI_RSA_KEY         = 0,
    PKI_DSA_KEY,
    PKI_DH_KEY,
    PKI_KEY_RESERVED,
}
PKI_KEY_TYPE,  *PPKI_KEY_TYPE;

typedef  struct
_ASN1_CERTIFICATE_ATTRIBUTE
{
    char                            pCountryName[ATTRIBUTE_BUFFER_SIZE];                    /*C*/
    char                            pOrganizationName[ATTRIBUTE_BUFFER_SIZE];               /*O*/
    char                            pOrganizationUnitName[ATTRIBUTE_BUFFER_SIZE];           /*OU*/
    char                            pCommonName[ATTRIBUTE_BUFFER_SIZE];                     /*CN*/
    char                            pStateOrProvinceName[ATTRIBUTE_BUFFER_SIZE];
    char                            pGenerationQualifier[ATTRIBUTE_BUFFER_SIZE];
    char                            pDNQualifier[ATTRIBUTE_BUFFER_SIZE];
    char                            pLocalityName[ATTRIBUTE_BUFFER_SIZE];
    char                            pName[ATTRIBUTE_BUFFER_SIZE];
    char                            pSurName[ATTRIBUTE_BUFFER_SIZE];
    char                            pGivenName[ATTRIBUTE_BUFFER_SIZE];
    char                            pInitials[ATTRIBUTE_BUFFER_SIZE];
    char                            pTitle[ATTRIBUTE_BUFFER_SIZE];
    char                            pPKCS9Email[ATTRIBUTE_BUFFER_SIZE];
    char                            pChallengePassword[ATTRIBUTE_BUFFER_SIZE];
    char                            pDescription[ATTRIBUTE_BUFFER_SIZE];
    /* For Cisco Router */
    char                            pSerialNumber[ATTRIBUTE_BUFFER_SIZE];
    char                            pUnstructuredName[ATTRIBUTE_BUFFER_SIZE];
    char                            pUnstructuredAddress[ATTRIBUTE_BUFFER_SIZE];

    /*
     *  The format for entering ipsec naming information is "type=value;", 
     *  the ';' are literal and must encapsulate each item, 
     *  type is one of ip, dns, email, or uri
     *
     *  Examples:  
     *  ip=127.0.0.1;dns=spice.vpcom.com;email=geek@osgroup.com   
     *  ip=127.0.0.1;dns=gonzo.vpcom.com;ip=47.97.226.20;dns=blah.aol.com
     */
    char                            pSubjectAltName[SUBJECT_ALT_NAME_SIZE];    
    ULONG                           KeyType;        /*Default:  RSA*/
    ULONG                           KeyLength;      /*Default:  64*/
    SIGNATURE_TYPE                  SignAlgor;      /*Default:  RSA_MD5_SIGNATURE*/
}
ASN1_CERTIFICATE_ATTRIBUTE, *PASN1_CERTIFICATE_ATTRIBUTE;

typedef  enum
_AL_ENCODING_TYPE
{
    ENCODING_BINARY = 0,
    ENCODING_BASE64_FILE,
    ENCODING_BASE64_MSG,
}
ENCODING_TYPE,  *PENCODING_TYPE;

#endif
