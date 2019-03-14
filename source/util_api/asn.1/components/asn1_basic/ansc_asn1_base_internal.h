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

    module: ansc_asn1_base_internal.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the internal functions for
        the ASN.1 base class

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.
        04/24/02    AnscAsn1GetTagValue() is added as an API.
        05/23/02    AnscAsn1GetEncodedData() is added as an API.
        05/28/02    AnscAsn1GetFingerPrint() is added as an API.

**********************************************************************/


#ifndef  _ANSC_ASN1_BASE_INTERNAL_
#define  _ANSC_ASN1_BASE_INTERNAL_

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_ASN1_BASE_INTERFACE.C
********************************************************************/
ANSC_HANDLE
AnscAsn1Create
    (
        ANSC_HANDLE                 hContainerContext
    );

ANSC_STATUS
AnscAsn1Free
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1Initialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1Clone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1CopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1EqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

ANSC_STATUS
AnscAsn1SetName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pChar
    );

ANSC_STATUS
AnscAsn1SetClassName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pChar
    );

BOOLEAN
AnscAsn1IsConstructive
    (
        ANSC_HANDLE                 hThisObject
    );

UCHAR
AnscAsn1GetFirstOctet
    (
        ANSC_HANDLE                 hThisObject
    );

void
AnscAsn1AddAttribute
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_ATTR_OBJECT           pAttrObject,
        BOOLEAN                     bAppend
    );

void
AnscAsn1GetTagValue
    (
        ANSC_HANDLE                 hThisObject,
        PASN_OBJECT_FORM_TYPE       pAttr,
        PULONG                      pTagValue
    );

BOOLEAN
AnscAsn1ReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

LONG
AnscAsn1GetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1DecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1EncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

PUCHAR
AnscAsn1GetEncodedData
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pLength
    );

ANSC_HANDLE
AnscAsn1GetBAOHandle
    (
        ANSC_HANDLE                 hThisObject
    );

#endif /*_ANSC_ASN1_BASE_INTERNAL_*/
