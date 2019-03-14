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

    module: ansc_asn1_attr_internal.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the internal functions for
        the attribute of ASN.1 object

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/21/02    initial revision.
        04/01/02    Function "AnscAsn1AttrDump" was added;

**********************************************************************/


#ifndef  _ANSC_ASN1_ATTR_INTERNAL_
#define  _ANSC_ASN1_ATTR_INTERNAL_

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_ASN1_ATTR_INTERFACE.C
********************************************************************/
ANSC_STATUS
AnscAsn1AttrFree
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAsn1AttrGetSizeOfEncodedTag
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1AttrClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1AttrEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject
    );

ANSC_STATUS
AnscAsn1AttrCopyValueFrom
    (
        ANSC_HANDLE                 hDestObject,
        ANSC_HANDLE                 hSourceObject
    );

ANSC_STATUS
AnscAsn1AttrEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding,
        BOOLEAN                     bIsConstructive
    );

BOOLEAN
AnscAsn1AttrDump
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength
    );

#endif /*_ANSC_ASN1_ATTR_INTERNAL_*/
