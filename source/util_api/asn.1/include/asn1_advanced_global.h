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

    module: asn1_advanced_global.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the ASN.1 object implementation.

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


#ifndef  _ASN1_ADVANCED_GLOBAL_H
#define  _ASN1_ADVANCED_GLOBAL_H

#include "ansc_asn1_global.h"
#include "asn1_advanced_interface.h"
#include "asn1_advanced_export_api.h"

#define  CERT_VALID                         0
#define  CERT_EXPIRED                       1
#define  CERT_IN_FUTURE                     2
#define  CERT_NO_CA                         3
#define  CERT_REVOKED                       4
#define  CERT_VERIFY_FAILURE                5

#define  MAXI_ENTITY_NAME_BUFFER            64

#endif  /*_ASN1_ADVANCED_GLOBAL_H*/


