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

    module: ansc_asn1_taginfo_extern_api.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the external functions for all the
        TagInfo (TCL) of ANS.1 encoding.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ASN1_TAGINFO_EXTERN_API_
#define  _ANSC_ASN1_TAGINFO_EXTERN_API_

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_ASN1_TAGINFO_INTERFACE.C
********************************************************************/
ANSC_HANDLE
AnscTagInfoCreate
    (
    );

ANSC_STATUS
AnscTagInfoFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscTagInfoParsingData
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData
    );

LONG
AnscTagInfoGetSizeOfTag
    (
        ANSC_HANDLE                 hThisObject
    );

LONG
AnscTagInfoGetSizeOfLength
    (
        ANSC_HANDLE                 hThisObject
    );

#endif /*_ANSC_ASN1_TAGINFO_EXTERN_API_*/
