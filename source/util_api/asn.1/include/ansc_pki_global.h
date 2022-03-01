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

    module: ansc_pki_global.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files for AnscPKI
        project

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        08/13/02    initial revision.

**********************************************************************/


#ifndef  _ASN1_PKI_GLOBAL_H
#define  _ASN1_PKI_GLOBAL_H

#include "asn1_advanced_global.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "ansc_crypto_pub_ssleay_interface.h"
#include "ansc_crypto_pub_ssleay_external_api.h"

#include "al_pkcs12_interface.h"
#include "al_pkcs12_export_api.h"

#include "pki_entity_interface.h"
#include "ca_entity_interface.h"
#include "alcert_interface.h"

#include "ansc_pki_export_api.h"
#include "ca_export_api.h"
#include "alcert_export_api.h"

#include "pki_utility_api.h"

#include "smartcard_sample_interface.h"

#endif  /*_ASN1_PKI_GLOBAL_H*/

