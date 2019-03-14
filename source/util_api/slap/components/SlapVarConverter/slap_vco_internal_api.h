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

    module:	slap_vco_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Var Converter
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        07/13/2003    initial revision.
        08/09/2010    Bin added ipv6 support.
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#ifndef  _SLAP_VCO_INTERNAL_API_
#define  _SLAP_VCO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN SLAP_VCO_STATES.C
***********************************************************/

ANSC_STATUS
SlapVcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_VCO_CONVERSION.C
***********************************************************/

ANSC_STATUS
SlapVcoConvertVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              tpl_var,
        SLAP_VARIABLE*              tbc_var
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN SLAP_VCO_TOSTRING.C
***********************************************************/

char*
SlapVcoBoolToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_BOOL                   var_bool
    );

char*
SlapVcoIntToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_INT                    var_int
    );

char*
SlapVcoUcharArrayToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

char*
SlapVcoUcharArrayToBase64String
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

char*
SlapVcoUcharArrayToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

char*
SlapVcoUint32ToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

char*
SlapVcoIp4AddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    );

char*
SlapVcoIp4AddrToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    );

char*
SlapVcoIp4AddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          ip4_addr_list
    );

char*
SlapVcoIp6AddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           ip6_addr_list
    );

char*
SlapVcoIp6AddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    );

char*
SlapVcoMacAddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    );

char*
SlapVcoMacAddrToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    );

char*
SlapVcoMacAddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr_list
    );

char*
SlapVcoOidListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          oid_list
    );

char*
SlapVcoCalendarTimeToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_HANDLE                 calendar_time
    );

char*
SlapVcoUint32ToHexString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN SLAP_VCO_STRINGTO.C
***********************************************************/

SLAP_BOOL
SlapVcoStringToBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_bool
    );

SLAP_INT
SlapVcoStringToInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_int
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    );

SLAP_UCHAR_ARRAY*
SlapVcoBase64StringToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToUcharArray2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    );

SLAP_UINT32
SlapVcoStringToUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uint32
    );

SLAP_UINT32
SlapVcoStringToIp4Addr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr
    );

SLAP_UINT32_ARRAY*
SlapVcoStringToIp4AddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr_list
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToIp6AddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip6_addr_list
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToIp6Addr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToMacAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr
    );

SLAP_UCHAR_ARRAY*
SlapVcoStringToMacAddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr_list
    );

SLAP_UINT32_ARRAY*
SlapVcoStringToOidList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       oid_list
    );

SLAP_HANDLE
SlapVcoStringToCalendarTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       calendar_time
    );

SLAP_UINT32
SlapVcoHexStringToUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    );

SLAP_UINT32
SlapVcoHexStringToDecimal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_VCO_UINT32ARRAYTO.C
***********************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoUint32ArrayToMacAddr
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          uint32_array
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_VCO_TOUCHARARRAY.C
***********************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoUint32ToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

SLAP_UCHAR_ARRAY*
SlapVcoUint32ToUchar
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_VCO_UCHARARRAYTO.C
***********************************************************/

SLAP_UINT32
SlapVcoUcharArrayToUint32
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );


#endif
