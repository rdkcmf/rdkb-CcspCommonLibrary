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

    module: ansc_asn1_attr_extern_api.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains exported apis for Attribute of 
        ASN.1 objects

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ASN1_ATTR_EXTERN_API_
#define  _ANSC_ASN1_ATTR_EXTERN_API_

/********************************************************************

   These functions will be implemented in "ANSC_ASN1_ATTR_INTERFACE.C"

*********************************************************************/
ANSC_HANDLE
AnscAsn1AttrCreate
    (
        ASN_OBJECT_FORM_TYPE        Type,
        ULONG                       ulNumber,
        BOOLEAN                     bImplicit
    );

ANSC_HANDLE
AnscAsn1AttrParsingData
    (
        PUCHAR                      pData
    );

#endif /*_ANSC_ASN1_BASE_EXTERN_API_*/
