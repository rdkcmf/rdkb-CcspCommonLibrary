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

    MODULE: adv_pki_entity_export_api.h

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    DESCRIPTION:

        This interface is for the entity of PKI implementation.

        The entity is responsible for generating key pair, pkcs10
        certificate request, self-signed certificate.

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   08/13/2002  initial revision

 **********************************************************************/


#ifndef  _ADV_PKI_EXPORT_API_H
#define  _ADV_PKI_EXPORT_API_H


/*************************************************************************
 *
 * This is the basic api to generate a PKI_ENTITY which is responsible to 
 * generate the key pair, PKCS10 request and selfsigned certificate.
 *
 * The handle "hCertAttr" could not be NULL here.
 *
 *************************************************************************/
ANSC_HANDLE 
AnscCreateAdvancedPKIEntity
    (
        ANSC_HANDLE                 hContext,
        ANSC_HANDLE                 hCertAttr,
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE 
AnscCreateAdvancedPKIEntityWithCert
    (
        ANSC_HANDLE                 hContext,
        ANSC_HANDLE                 hReserved,
        ANSC_HANDLE                 hCertAttr,
        PUCHAR                      pEncodingCert,
        ULONG                       lenOfCert,
        PUCHAR                      pPriKeyInfoEncode,
        ULONG                       lenOfKey
    );


#endif  /*_ADV_PKI_EXPORT_API_H*/


