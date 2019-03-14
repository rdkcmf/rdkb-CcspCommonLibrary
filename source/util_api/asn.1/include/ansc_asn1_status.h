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

    module: ansc_asn1_status.h

        For Status Information about ASN.1 for 
        Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file defines the status for the general ASN.1 
        object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#ifndef ANSC_ASN1_STATUS_H
#define ANSC_ASN1_STATUS_H


/***********************************************************
          DEFINITION OF ASN.1 STATUS
***********************************************************/

#define  ANSC_ASN1_STATUS                           0xFFFF8000
#define  ANSC_ASN1_NULL_OBJCET                      ANSC_ASN1_STATUS        + 1
#define  ANSC_ASN1_BAD_PARAMETER                    ANSC_ASN1_STATUS        + 2
#define  ANSC_ASN1_VALUE_NOT_SET                    ANSC_ASN1_STATUS        + 3
#define  ANSC_ASN1_BAD_OID_VALUE                    ANSC_ASN1_STATUS        + 4
#define  ANSC_ASN1_NULL_PARAMETER                   ANSC_ASN1_STATUS        + 5
#define  ANSC_ASN1_NO_SELECTION_IN_CHOICE           ANSC_ASN1_STATUS        + 6
#define  ANSC_ASN1_UNKOWN_OBJECT_TYPE               ANSC_ASN1_STATUS        + 7
#define  ANSC_ASN1_INVALID_UNUSED_BIT               ANSC_ASN1_STATUS        + 8
#define  ANSC_ASN1_INVALID_TYPE_IN_SEQOF_OR_SETOF   ANSC_ASN1_STATUS        + 9
#define  ANSC_ASN1_ALREADY_EXIST                    ANSC_ASN1_STATUS        + 10
#define  ANSC_ASN1_NOT_EXIST                        ANSC_ASN1_STATUS        + 11 
#define  ANSC_ASN1_FORBIDDEN_PLACE                  ANSC_ASN1_STATUS        + 12 
#define  ANSC_ASN1_UNAVAILABLE                      ANSC_ASN1_STATUS        + 13 
#define  ANSC_ASN1_UNEXPECTED_TAG                   ANSC_ASN1_STATUS        + 14 
#define  ANSC_ASN1_INVALID_ENCODE_LENGTH            ANSC_ASN1_STATUS        + 15 
#define  ANSC_ASN1_INFINITE_END_MISSED              ANSC_ASN1_STATUS        + 16 
#define  ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE          ANSC_ASN1_STATUS        + 17 
#define  ANSC_ASN1_INVALID_TIME_VALUE               ANSC_ASN1_STATUS        + 18
#define  ANSC_ASN1_NO_ENOUGH_SIZE                   ANSC_ASN1_STATUS        + 19
#define  ANSC_ASN1_INVALID_VALUE                    ANSC_ASN1_STATUS        + 20
#define  ANSC_ASN1_NOT_READY_TO_ENCODE              ANSC_ASN1_STATUS        + 21
#define  ANSC_ASN1_INVALID_SIGNATURE                ANSC_ASN1_STATUS        + 22
#define  ANSC_ASN1_NOT_SUPPORTED                    ANSC_ASN1_STATUS        + 23
#define  ANSC_ASN1_NO_RESOURCES                     ANSC_ASN1_STATUS        + 24
#define  ANSC_ASN1_INVALID_TIME                     ANSC_ASN1_STATUS        + 25
#define  ANSC_ASN1_FAILED_TO_VERIFY                 ANSC_ASN1_STATUS        + 26
#define  ANSC_ASN1_ERROR_FIND_ISSUER                ANSC_ASN1_STATUS        + 27
#define  ANSC_ASN1_DIFFERENT_NONCE                  ANSC_ASN1_STATUS        + 28
#define  ANSC_ASN1_CERT_REVOKED                     ANSC_ASN1_STATUS        + 29

#endif /*ANSC_ASN1_STATUS_H*/
