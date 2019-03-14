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

    module: cert_ifo_ipsec.h

        For IKE Certificate integration,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface of the certification service provided for IPsec.

        This interface is direct callback based, that is, the 
        interface provider is responsible for handling Inter-Process
        Communication if necessary.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        03/08/04    initial revision

**********************************************************************/


#ifndef  _CERT_IFO_IPSEC_
#define  _CERT_IFO_IPSEC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

/*
 *  to fill in the proper header files.
 */
#include "cert_status.h"
#include "cert_attr_structure.h"

/***********************************************************
                  CONSTANT DEFINITIONS
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  CERT_IPSEC_INTERFACE_NAME                  "certIpsecServiceIf"
#define  CERT_IPSEC_INTERFACE_ID                    0


/***********************************************************
            SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef ULONG
(*PFN_CISIF_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_CISIF_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       nIndex,
        char*                       pName,
        PULONG                      pulNameLength
    );


typedef  ANSC_STATUS
(*PFN_CISIF_GET_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName,
        PASN1_CERTIFICATE_ATTRIBUTE pAttribute
    );

/*
 *  This API is to get the DER encoded DN of a CA cert.
 *
 *  *pucCertEncodingType specifies which certificate encoding type is accepted by the CA. 
 *  The choices for *pucCertEncodingType are:
 *
 *      X509_SIG_ENCODING
 *      PKCS7_ENCODING
 *
 *  The values are defined in defined in cw_vpn_sakm.h, should we use a more generic header file?
 *
 *  Caller is responsible for allocating memory for pDnEncoding. Input of pulDnEncodingSize
 *  speicifies the maximum buffer size of pDnEncoding.
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_CA_DER_DN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCertName,
        PUCHAR                      pucCertEncodingType,
        PUCHAR                      pDnEncoding,
        PULONG                      pulDnEncodingSize
    );

/*
 *  This API is to get the DN in string format. This is a utility API.
 *
 *  Caller is responsible for allocating memory for pDnStr. Input of pulDnStrLength
 *  speicifies the maximum buffer size of pDnStr.
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_STR_DN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCertName,
        char*                       pDnStr,
        PULONG                      pulDnStrLength
    );

/*
 *  This API is to convert DER encoded DN to string format DN.
 *
 *  Caller is responsible for allocating memory for pDnStr. Input of pulDnStrLength
 *  speicifies the maximum buffer size of pDnStr.
 */
typedef  ANSC_STATUS
(*PFN_CISIF_CONV_DER_DN_TO_STR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDnEncoding,
        ULONG                       ulDnEncodingSize,
        char*                       pDnStr,
        PULONG                      pulDnStrLength
    );

/*
 *  This API is to verify whether the user cert is signed the specified CA DN, which is
 *  DER encoded.
 */
typedef  ANSC_STATUS
(*PFN_CISIF_IS_CERT_SIGNED_BY_CA)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserCertName,
        PUCHAR                      pDnEncoding,
        ULONG                       ulDnEncodingSize
    );

/*
 *  This API is to get key pair information of the certificate specified by the name.
 *
 *      char* 
 *          The name of the local certificate.
 *
 *      pKeyType
 *          The buffer of the cert key type.
 * 
 *      hKeyParam
 *          The key pair in "PANSC_CRYPTO_PUB_KEY_GEN_PARAMS"
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_KEYPAIR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName,
        PPKI_KEY_TYPE               pKeyType,
        ANSC_HANDLE                 hKeyParam
    );

/*
 *  This API is to get the encoded certificate chain in PKCS#7 format with local 
 *  certificate specified by the name.
 *
 *     PCHAR                       pUserCertName,
 *          The specified local certificate name;
 *
 *     ULONG                       ulCANumber,
 *          The ca count of the input ca array;
 *
 *     PULONG                      pulDnSizeArray,
 *          The dn size array of the cas;
 *
 *     PVOID*                      pDnArray,
 *          The dn array of the cas;
 *
 *     PUCHAR                      pEncoding,
 *          The output certificate chain in PKCS7 encoding;
 *
 *     PULONG                      pUlEncodingSize
 *          The output buffer of the size of above encoding
 *
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_PKCS7)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pUserCertName,
        ULONG                       ulCANumber,
        PULONG                      pulDnSizeArray,
        PVOID*                      pDnArray,
        PUCHAR                      pEncoding,
        PULONG                      pUlEncodingSize
    );

/*
 *  This API is to get the encoded certificate chain array with local certificate
 *  specified by the name.
 *
 *     PCHAR                       pUserCertName,
 *          The specified local certificate name;
 *
 *     ULONG                       ulCANumber,
 *          The ca count of the input ca array;
 *
 *     PULONG                      pulDnSizeArray,
 *          The dn size array of the cas;
 *
 *     PVOID*                      pDnArray,
 *          The dn array of the cas;
 *
 *     PULONG                      pulCertPath
 *          The output buffer of ca numbers.
 *
 *     PULONG                      pulCertSizeArray
 *          The output buffer of the size of certificates.
 *
 *     PVOID*                      pCertArray
 *          The output buffer of cert encoding array
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_X509)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pUserCertName,
        ULONG                       ulCANumber,
        PULONG                      pulDnSizeArray,
        PVOID*                      pDnArray,
        PULONG                      pulCertPath,
        PULONG                      pulCertSizeArray,
        PVOID*                      pCertArray
    );

/*
 *  This API is to get the encoded certificate chain in PKCS#7 format with 
 *  any local certificate with specified key type.
 *
 *     PKI_KEY_TYPE                ReqKeyType,
 *          The specified local certificate type;
 *
 *     ULONG                       ulCANumber,
 *          The ca count of the input ca array;
 *
 *     PULONG                      pulDnSizeArray,
 *          The dn size array of the cas;
 *
 *     PVOID*                      pDnArray,
 *          The dn array of the cas;
 *
 *     PUCHAR                      pEncoding,
 *          The output certificate chain in PKCS7 encoding;
 *
 *     PULONG                      pUlEncodingSize
 *          The output buffer of the size of above encoding
 *
 *     char*                       pName
 *          The output local certificate name;
 *
 *     PULONG                      pulNameLength
 *          The output buffer of the local cert name size
 *
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_APKCS7)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                ReqKeyType,
        ULONG                       ulCANumber,
        PULONG                      pulDnSizeArray,
        PVOID*                      pDnArray,
        PUCHAR                      pEncoding,
        PULONG                      pUlEncodingSize,
        char*                       pName,
        PULONG                      pulNameLength
    );

/*
 *  This API is to get the encoded certificate chain array with any local certificate
 *  specified by the key type.
 *
 *     PKI_KEY_TYPE                ReqKeyType,
 *          The specified local certificate type;
 *
 *     ULONG                       ulCANumber,
 *          The ca count of the input ca array;
 *
 *     PULONG                      pulDnSizeArray,
 *          The dn size array of the cas;
 *
 *     PVOID*                      pDnArray,
 *          The dn array of the cas;
 *
 *     PULONG                      pulCertPath
 *          The output buffer of ca numbers.
 *
 *     PULONG                      pulCertSizeArray
 *          The output buffer of the size of certificates.
 *
 *     PVOID*                      pCertArray
 *          The output buffer of cert encoding array
 *
 *     char*                       pName
 *          The output local certificate name;
 *
 *     PULONG                      pulNameLength
 *          The output buffer of the local cert name size
 */
typedef  ANSC_STATUS
(*PFN_CISIF_GET_AX509)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                ReqKeyType,
        ULONG                       ulCANumber,
        PULONG                      pulDnSizeArray,
        PVOID*                      pDnArray,
        PULONG                      pulCertPath,
        PULONG                      pulCertSizeArray,
        PVOID*                      pCertArray,
        char*                       pName,
        PULONG                      pulNameLength
    );

/*
 *  This API is to verify the certificate chain. The IKE ID must match the user cert's DN
 *  or alternative name.
 *
 *  arguments:
 *
 *      ulCertPathLength
 *          Specify the number of certs contained in the cert array.
 *
 *      pCertArray
 *          Array of pointers that includes every certificate which is supposed to be 
 *          on the certification path.
 *
 *      pulCertSizeArray
 *          Array of size of the DER encoding certificate buffer.
 *
 *      pIkeId
 *          Match the ID to the user cert's DN or alternative name.
 *
 *      pKeyType
 *          The returned buffer of key type 
 *
 *      hPublicKey
 *          The returned public key of this cert.
 *          Could be "ANSC_CRYPTO_RSA_PUBLIC_KEY" or "ANSC_CRYPTO_DSA_PUBLIC_KEY", depends
 *          on the key type above. This buffer should be big enough to hold either of them.
 */
typedef  ANSC_STATUS
(*PFN_CISIF_VERIFY_CERTS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCertPathLength,
        PVOID*                      pCertArray,
        PULONG                      pulCertSizeArray,
        ANSC_HANDLE                 pIkeId,
        PPKI_KEY_TYPE               pKeyType,
        ANSC_HANDLE                 hPublicKey
    );


/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  CERT_IPSEC_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_CISIF_GET_COUNT             GetCaCount;                                             \
    PFN_CISIF_GET_NAME              GetCaNameByIndex;                                       \
                                                                                            \
    PFN_CISIF_GET_ATTR              GetCaCertAttribute;                                     \
    PFN_CISIF_GET_ATTR              GetUserCertAttribute;                                   \
                                                                                            \
    PFN_CISIF_GET_CA_DER_DN         GetCaCertDerDn;                                         \
    PFN_CISIF_GET_CA_DER_DN         GetUserCertDerDn;                                       \
    PFN_CISIF_GET_STR_DN            GetCaCertStrDn;                                         \
    PFN_CISIF_GET_STR_DN            GetUserCertStrDn;                                       \
    PFN_CISIF_CONV_DER_DN_TO_STR    ConvertDerDnToStr;                                      \
    PFN_CISIF_GET_KEYPAIR           GetUserKeyPair;                                         \
                                                                                            \
    PFN_CISIF_IS_CERT_SIGNED_BY_CA  IsCertSignedByCa;                                       \
    PFN_CISIF_GET_PKCS7             GetPkcs7Certs;                                          \
    PFN_CISIF_GET_X509              GetX509Certs;                                           \
    PFN_CISIF_GET_APKCS7            GetAvailPkcs7Certs;                                     \
    PFN_CISIF_GET_AX509             GetAvailX509Certs;                                      \
    PFN_CISIF_VERIFY_CERTS          VerifyCerts;                                            \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_CERT_IPSEC_INTERFACE
{
    CERT_IPSEC_INTERFACE_CLASS_CONTENT
}
CERT_IPSEC_INTERFACE,  *PCERT_IPSEC_INTERFACE;

#define  ACCESS_CERT_IPSEC_INTERFACE(p)             ACCESS_CONTAINER(p, CERT_IPSEC_INTERFACE, Linkage)


#endif
