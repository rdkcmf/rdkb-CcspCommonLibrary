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

    module: ansc_obj_asn1_external_api.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the external functions for all the
        basic ASN.1 object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/18/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_OBJ_ASN1_EXTERNAL_API_
#define  _ANSC_OBJ_ASN1_EXTERNAL_API_

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_ASN1_EXTERNAL_API.C
********************************************************************/
ANSC_HANDLE
AnscAsn1CreateAny
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateInteger
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateBOOL
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateReal
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateEnumerate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateNULL
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateUniversalTime
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateGeneralizedTime
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateOID
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateOctetString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateUTF8String
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateUniversalString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateTeletexString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateIA5String
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreatePrintableString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateVideotexString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateVisibleString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateNumericString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateGeneralString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateGraphicString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateBMPString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateMSString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateBitString
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateChoice
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateSet
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateSequence
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateSetOf
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateSequenceOf
    (
        ANSC_HANDLE                 hReserved
    );

#endif /*_ANSC_ASN1_EXTERNAL_API_*/
