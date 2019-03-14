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

    module: ansc_asn1_advanced_local.h

        For Abstract Syatax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        local ASN.1 object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/25/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ASN1_ADVANCED_LOCAL_
#define  _ANSC_ASN1_ADVANCED_LOCAL_

#include "asn1_advanced_global.h"

#include "ansc_asn1_certificate_internal.h"
#include "ansc_asn1_extensions_internal.h"
#include "ansc_asn1_algorithmidentifier_internal.h"
#include "ansc_asn1_name_internal.h"
#include "ansc_asn1_crl_internal.h"
#include "ansc_asn1_publickey_internal.h"
#include "ansc_asn1_privatekeyinfo_internal.h"
#include "ansc_asn1_rsadigestinfo_internal.h"
#include "ansc_asn1_othername_internal.h"
#include "ansc_asn1_contentinfo_internal.h"
#include "ansc_asn1_certificaterequest_internal.h"

#ifndef _NO_PKI_KB5_SUPPORT
#include "kb5_global.h"
#endif

#include "pki_internal.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "ansc_crypto_pub_ssleay_interface.h"
#include "ansc_crypto_pub_ssleay_external_api.h"

#endif  /*_ANSC_ASN1_ADVANCED_LOCAL_*/

