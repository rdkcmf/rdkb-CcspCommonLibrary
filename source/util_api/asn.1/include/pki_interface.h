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

    module: pki_interface.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for PKI
        implementation

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        *   05/09/2002  initial revision
        *   08/20/2002  PKICertAttributeInitRFC2253Encode was supported;
        *   08/20/2002  PKICertAttributeTrace was supported;
        *   08/29/2002  KB5Principal name was added.
        *   11/26/2003  Add APIs for StreetAddress 

**********************************************************************/


#ifndef  _PKI_INTERFACE_IF_H
#define  _PKI_INTERFACE_IF_H


/**********************************************************************
 *
 *  Definition of keyusage
 *
 *********************************************************************/
#define  KEY_USAGE_DIGITALSIGNATURE_MASK            0x80
#define  KEY_USAGE_NONREPUDIATION_MASK              0x40
#define  KEY_USAGE_KEYENCIPHERMENT_MASK             0x20
#define  KEY_USAGE_DATAENCIPHERMENT_MASK            0x10
#define  KEY_USAGE_KEYAGREEMENT_MASK                0x08
#define  KEY_USAGE_KEYCERTSIGN_MASK                 0x04
#define  KEY_USAGE_CRLSIGN_MASK                     0x02
#define  KEY_USAGE_ENCIPHERONLY_MASK                0x01
#define  KEY_USAGE_DECIPHERONLY_MASK                0x80


/*
 *  Callback function to enum the revoked certificates;
 */
typedef BOOLEAN 
(*EnumRevokedCertProc)
(
    void*                           pData, 
    ANSC_HANDLE                     hRevokeCertHandle, 
    ANSC_HANDLE                     hCertSerialNumber,
    ANSC_HANDLE                     hRevokedTime
);


/*-------------------------------------------------------*/

typedef  ANSC_HANDLE
(*PFN_AL_CREATE)
    (
        ANSC_HANDLE                 hReserved
    );

typedef  void
(*PFN_AL_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_AL_SET_ATTRIBUTE)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

typedef  BOOLEAN
(*PFN_AL_SET_KB5NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPrinName,
        ANSC_HANDLE                 hRealmName
    );

typedef  BOOLEAN
(*PFN_AL_SET_KEY_LENGTh)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       length
    );

typedef  PCHAR
(*PFN_AL_GET_ATTRIBUTE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_AL_CLONE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef VOID
(*PFN_AL_TRACE)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  ATTRIBUTE_BUFFER_SIZE                      64
#define  SUBJECT_ALT_NAME_SIZE                      128
#define  COUNTRY_NAME_SIZE                          3
#define  MAXI_FULL_NAME_SIZE                        512

/*
 *  The ALCertificateAttr object is the DN name object
 *                                                                                      
 *  For the SubjectAlternativeName:
 *  The format for entering ipsec naming information is "type=value;", 
 *  the ';' are literal and must encapsulate each item, 
 *  type is one of ip, dns, email, or uri
 *
 *  Examples:  
 *  ip=127.0.0.1;dns=spice.vpcom.com;email=geek@osgroup.com   
 *  ip=127.0.0.1;dns=gonzo.vpcom.com;ip=47.97.226.20;dns=blah.aol.com
 *
 */
#define  AL_CERT_ATTR_CLASS_CONTENT                                                         \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    CHAR                            pFullName[MAXI_FULL_NAME_SIZE];                         \
    CHAR                            pCountryName[COUNTRY_NAME_SIZE];                        \
    CHAR                            pOrganizationName[ATTRIBUTE_BUFFER_SIZE];               \
    CHAR                            pOrganizationUnitName[ATTRIBUTE_BUFFER_SIZE];           \
    CHAR                            pCommonName[ATTRIBUTE_BUFFER_SIZE];                     \
    CHAR                            pStateOrProvinceName[ATTRIBUTE_BUFFER_SIZE];            \
    CHAR                            pGenerationQualifier[ATTRIBUTE_BUFFER_SIZE];            \
    CHAR                            pDNQualifier[ATTRIBUTE_BUFFER_SIZE];                    \
    CHAR                            pLocalityName[ATTRIBUTE_BUFFER_SIZE];                   \
    CHAR                            pName[ATTRIBUTE_BUFFER_SIZE];                           \
    CHAR                            pSurName[ATTRIBUTE_BUFFER_SIZE];                        \
    CHAR                            pGivenName[ATTRIBUTE_BUFFER_SIZE];                      \
    CHAR                            pInitials[ATTRIBUTE_BUFFER_SIZE];                       \
    CHAR                            pTitle[ATTRIBUTE_BUFFER_SIZE];                          \
    CHAR                            pPKCS9Email[ATTRIBUTE_BUFFER_SIZE];                     \
    CHAR                            pChallengePassword[ATTRIBUTE_BUFFER_SIZE];              \
    CHAR                            pDescription[ATTRIBUTE_BUFFER_SIZE];                    \
    CHAR                            pSerialNumber[ATTRIBUTE_BUFFER_SIZE];                   \
    CHAR                            pUnstructedName[ATTRIBUTE_BUFFER_SIZE];                 \
    CHAR                            pUnstructedAddress[ATTRIBUTE_BUFFER_SIZE];              \
    CHAR                            pStreetAddress[ATTRIBUTE_BUFFER_SIZE];                  \
    CHAR                            pSubjectAltName[SUBJECT_ALT_NAME_SIZE];                 \
    ANSC_HANDLE                     hKB5PrinName;                                           \
    ANSC_HANDLE                     hKB5RealmName;                                          \
    PKI_KEY_TYPE                    KeyType;                                                \
    /* default key length is 128, in BYTE */                                                \
    ULONG                           KeyLength;                                              \
    SIGNATURE_TYPE                  SignAlgor;                                              \
    /* function members here */                                                             \
    PFN_AL_CREATE                   Create;                                                 \
    PFN_AL_CLONE                    Clone;                                                  \
    PFN_AL_FREE                     Remove;                                                 \
    PFN_AL_SET_KEY_LENGTh           SetKeylengthInByte;                                     \
    PFN_AL_SET_ATTRIBUTE            AddCountryName;                                         \
    PFN_AL_SET_ATTRIBUTE            AddOrganizationName;                                    \
    PFN_AL_SET_ATTRIBUTE            AddOrganUnitName;                                       \
    PFN_AL_SET_ATTRIBUTE            AddCommonName;                                          \
    PFN_AL_SET_ATTRIBUTE            AddStateOrProvinceName;                                 \
    PFN_AL_SET_ATTRIBUTE            AddGenerationQualifierName;                             \
    PFN_AL_SET_ATTRIBUTE            AddDNQualifier;                                         \
    PFN_AL_SET_ATTRIBUTE            AddLocalityName;                                        \
    PFN_AL_SET_ATTRIBUTE            AddPKCS9Email;                                          \
    PFN_AL_SET_ATTRIBUTE            AddSerialNumber;                                        \
    PFN_AL_SET_ATTRIBUTE            AddStreetAddress;                                       \
    PFN_AL_SET_ATTRIBUTE            SetName;                                                \
    PFN_AL_SET_ATTRIBUTE            SetChallengePassword;                                   \
    PFN_AL_SET_ATTRIBUTE            SetSurName;                                             \
    PFN_AL_SET_ATTRIBUTE            SetGivenName;                                           \
    PFN_AL_SET_ATTRIBUTE            SetInitials;                                            \
    PFN_AL_SET_ATTRIBUTE            SetTitle;                                               \
    PFN_AL_SET_ATTRIBUTE            SetDescription;                                         \
    PFN_AL_SET_ATTRIBUTE            SetUnstructedName;                                      \
    PFN_AL_SET_ATTRIBUTE            SetUnstructedAddress;                                   \
    PFN_AL_SET_ATTRIBUTE            SetSubjectAltName;                                      \
    PFN_AL_SET_ATTRIBUTE            AddDomainName;                                          \
    PFN_AL_SET_ATTRIBUTE            AddEmailAddress;                                        \
    PFN_AL_SET_ATTRIBUTE            AddIPAddress;                                           \
    PFN_AL_SET_KB5NAME              SetKB5PrincipalName;                                    \
    PFN_AL_GET_ATTRIBUTE            GetCountryName;                                         \
    PFN_AL_GET_ATTRIBUTE            GetOrganizationName;                                    \
    PFN_AL_GET_ATTRIBUTE            GetOrganUnitName;                                       \
    PFN_AL_GET_ATTRIBUTE            GetCommonName;                                          \
    PFN_AL_GET_ATTRIBUTE            GetStateOrProvinceName;                                 \
    PFN_AL_GET_ATTRIBUTE            GetGenerationQualifierName;                             \
    PFN_AL_GET_ATTRIBUTE            GetName;                                                \
    PFN_AL_GET_ATTRIBUTE            GetDNQualifier;                                         \
    PFN_AL_GET_ATTRIBUTE            GetLocalityName;                                        \
    PFN_AL_GET_ATTRIBUTE            GetPKCS9Email;                                          \
    PFN_AL_GET_ATTRIBUTE            GetSerialNumber;                                        \
    PFN_AL_GET_ATTRIBUTE            GetChallengePassword;                                   \
    PFN_AL_GET_ATTRIBUTE            GetSurName;                                             \
    PFN_AL_GET_ATTRIBUTE            GetGivenName;                                           \
    PFN_AL_GET_ATTRIBUTE            GetInitials;                                            \
    PFN_AL_GET_ATTRIBUTE            GetTitle;                                               \
    PFN_AL_GET_ATTRIBUTE            GetDescription;                                         \
    PFN_AL_GET_ATTRIBUTE            GetUnstructedName;                                      \
    PFN_AL_GET_ATTRIBUTE            GetUnstructedAddress;                                   \
    PFN_AL_GET_ATTRIBUTE            GetStreetAddress;                                       \
    PFN_AL_GET_ATTRIBUTE            GetSubjectAltName;                                      \
    PFN_AL_SET_ATTRIBUTE            InitRFC2253Encode;                                      \
    PFN_AL_GET_ATTRIBUTE            GetFullName;                                            \
    PFN_AL_TRACE                    Trace;                                                  \

typedef  struct
_ALCERTIFICATE_ATTRIBUTE
{
    AL_CERT_ATTR_CLASS_CONTENT
}
ALCERTIFICATE_ATTRIBUTE,  *PALCERTIFICATE_ATTRIBUTE;

#define  ACCESS_ALCERTIFICATE_ATTRIBUTE(p)            \
         ACCESS_CONTAINER(p, ALCERTIFICATE_ATTRIBUTE, Linkage)

/*
 *  Definition of AltNameParser
 */
/*
 *  The format for entering ipsec naming information is "type=value;", 
 *  the ';' are literal and must encapsulate each item, 
 *  type is one of ip, dns, email, or uri
 *
 *  Examples:  
 *  ip=127.0.0.1;dns=spice.entrust.com;email=geek@entrust.com   
 *  ip=127.0.0.1;dns=gonzo.entrust.com;ip=47.97.226.20;dns=blah.cisco.com
 */
#define  ALT_EMAIL_NAME                             "email"
#define  ALT_DNS_NAME                               "dns"
#define  ALT_URI_NAME                               "uri"
#define  ALT_IP_NAME                                "ip"

#define  ALT_NAME_BUFFER_SIZE                       64
#define  MAXI_ALT_NAME_BLOCK                        10

#define  X500_CN                                    "CN"
#define  X500_L                                     "L"
#define  X500_ST                                    "ST"
#define  X500_O                                     "O"
#define  X500_OU                                    "OU"
#define  X500_C                                     "C"
#define  X500_STREET                                "STREET"
#define  X500_SN                                    "SN"

#define  ALT_TYPE_EMAIL_NAME                        0
#define  ALT_TYPE_DNS_NAME                          1
#define  ALT_TYPE_URI_NAME                          2
#define  ALT_TYPE_IP_NAME                           3

typedef  ANSC_HANDLE
(*PFN_ALT_PARSING)
    (
        PCHAR                       pString
    );

typedef  ANSC_STATUS
(*PFN_ALT_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_ALT_GET_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

typedef  ULONG
(*PFN_ALT_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

typedef  ULONG
(*PFN_ALT_GET_TOTAL)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  ALT_NAME_PARSER_CLASS_CONTENT                                                      \
    CHAR                            pName[MAXI_ALT_NAME_BLOCK][ALT_NAME_BUFFER_SIZE];       \
    ULONG                           uType[MAXI_ALT_NAME_BLOCK];                             \
    ULONG                           uTotalBlock;                                            \
    /* function members here */                                                             \
    PFN_ALT_PARSING                 Parsing;                                                \
    PFN_ALT_FREE                    Remove;                                                 \
    PFN_ALT_GET_TOTAL               GetTotalBlock;                                          \
    PFN_ALT_GET_TYPE                GetBlockType;                                           \
    PFN_ALT_GET_STRING              GetBlockValue;                                          \

typedef  struct
_PKI_ALTNAME_PARSER
{
    ALT_NAME_PARSER_CLASS_CONTENT
}
PKI_ALTNAME_PARSER,  *PPKI_ALTNAME_PARSER;

#endif /*_PKI_INTERFACE_9988_H*/

