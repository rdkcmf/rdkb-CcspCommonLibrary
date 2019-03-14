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

    module:	slap_vco_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Var Converter Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        07/11/2003    initial revision.
        08/09/2010    Bin added ipv6 support in SLAP
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#ifndef  _SLAP_VCO_INTERFACE_
#define  _SLAP_VCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"


/***********************************************************
        SLAP STANDARD VAR CONVERTER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_VCO_UOA_NAME                          "System.Utility.VarConverter"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPVCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPVCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPVCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPVCO_CONVERT)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              tpl_var,
        SLAP_VARIABLE*              tbc_var
    );

typedef  char*
(*PFN_SLAPVCO_BOOL1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_BOOL                   var_bool
    );

typedef  char*
(*PFN_SLAPVCO_INT1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_INT                    var_int
    );

typedef  char*
(*PFN_SLAPVCO_UCHARA1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

typedef  char*
(*PFN_SLAPVCO_UCHARA12)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

typedef  char*
(*PFN_SLAPVCO_UINT321)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

typedef  char*
(*PFN_SLAPVCO_IP4_ADDR1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    );

typedef  char*
(*PFN_SLAPVCO_IP4_ADDR12)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    );

typedef  char*
(*PFN_SLAPVCO_IP4_ADDR_LIST1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          ip4_addr_list
    );

typedef  char*
(*PFN_SLAPVCO_IP6_ADDR_LIST1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           ip6_addr_list
    );

typedef  char*
(*PFN_SLAPVCO_IP6_ADDR1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    );

typedef  char*
(*PFN_SLAPVCO_MAC_ADDR1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    );

typedef  char*
(*PFN_SLAPVCO_MAC_ADDR_LIST1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr_list
    );

typedef  char*
(*PFN_SLAPVCO_OID_LIST1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          oid_list
    );

typedef  char*
(*PFN_SLAPVCO_TIME1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_HANDLE                 calendar_time
    );

typedef  char*
(*PFN_SLAPVCO_HEX_STR1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

typedef  SLAP_BOOL
(*PFN_SLAPVCO_BOOL2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_bool
    );

typedef  SLAP_INT
(*PFN_SLAPVCO_INT2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_int
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_UCHARA2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_UCHARA22)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    );

typedef  SLAP_UINT32
(*PFN_SLAPVCO_UINT322)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uint32
    );

typedef  SLAP_UINT32
(*PFN_SLAPVCO_IP4_ADDR2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr
    );

typedef  SLAP_UINT32_ARRAY*
(*PFN_SLAPVCO_IP4_ADDR_LIST2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr_list
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_IP6_ADDR_LIST2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip6_addr_list
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_IP6_ADDR2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_MAC_ADDR2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_MAC_ADDR_LIST2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr_list
    );

typedef  SLAP_UINT32_ARRAY*
(*PFN_SLAPVCO_OID_LIST2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       oid_list
    );

typedef  SLAP_HANDLE
(*PFN_SLAPVCO_TIME2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       calendar_time
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_MAC_ADDR3)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          uint32_array
    );

typedef  SLAP_UINT32
(*PFN_SLAPVCO_HEX_STR2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    );

typedef  SLAP_UINT32
(*PFN_SLAPVCO_HEX_STR3)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_UINT323)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_SLAPVCO_UINT324)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    );

typedef  SLAP_UINT32
(*PFN_SLAPVCO_UINT325)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    );

/*
 * The SLAP Var Converter Object is derived from the base Sco Standard Object. This object will be
 * created by SLAP internally inside the 'System' container. This object is responsible for data
 * type conversion based on the data syntaxes and content types (if any). For example: an IPv4
 * address is usually rendered as a dotted decimal string, such as "192.168.0.1"; while internally
 * it's stored as either a four-bytes ucharArray or a uint32.
 */
#define  SLAP_VAR_CONVERTER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_SLAPVCO_CONVERT             ConvertVariable;                                        \
                                                                                            \
    PFN_SLAPVCO_BOOL1               BoolToString;                                           \
    PFN_SLAPVCO_INT1                IntToString;                                            \
    PFN_SLAPVCO_UCHARA1             UcharArrayToString;                                     \
    PFN_SLAPVCO_UCHARA1             UcharArrayToBase64String;                               \
    PFN_SLAPVCO_UCHARA12            UcharArrayToString2;                                    \
    PFN_SLAPVCO_UINT321             Uint32ToString;                                         \
    PFN_SLAPVCO_IP4_ADDR1           Ip4AddrToString;                                        \
    PFN_SLAPVCO_IP4_ADDR12          Ip4AddrToString2;                                       \
    PFN_SLAPVCO_IP4_ADDR_LIST1      Ip4AddrListToString;                                    \
    PFN_SLAPVCO_IP6_ADDR1           Ip6AddrToString;                                        \
    PFN_SLAPVCO_IP6_ADDR_LIST1      Ip6AddrListToString;                                    \
    PFN_SLAPVCO_MAC_ADDR1           MacAddrToString;                                        \
    PFN_SLAPVCO_MAC_ADDR1           MacAddrToString2;                                       \
    PFN_SLAPVCO_MAC_ADDR_LIST1      MacAddrListToString;                                    \
    PFN_SLAPVCO_OID_LIST1           OidListToString;                                        \
    PFN_SLAPVCO_TIME1               CalendarTimeToString;                                   \
    PFN_SLAPVCO_HEX_STR1            Uint32ToHexString;                                      \
                                                                                            \
    PFN_SLAPVCO_BOOL2               StringToBool;                                           \
    PFN_SLAPVCO_INT2                StringToInt;                                            \
    PFN_SLAPVCO_UCHARA2             StringToUcharArray;                                     \
    PFN_SLAPVCO_UCHARA2             Base64StringToUcharArray;                               \
    PFN_SLAPVCO_UCHARA22            StringToUcharArray2;                                    \
    PFN_SLAPVCO_UINT322             StringToUint32;                                         \
    PFN_SLAPVCO_IP4_ADDR2           StringToIp4Addr;                                        \
    PFN_SLAPVCO_IP4_ADDR_LIST2      StringToIp4AddrList;                                    \
    PFN_SLAPVCO_IP6_ADDR_LIST2      StringToIp6AddrList;                                    \
    PFN_SLAPVCO_IP6_ADDR2           StringToIp6Addr;                                        \
    PFN_SLAPVCO_MAC_ADDR2           StringToMacAddr;                                        \
    PFN_SLAPVCO_MAC_ADDR_LIST2      StringToMacAddrList;                                    \
    PFN_SLAPVCO_OID_LIST2           StringToOidList;                                        \
    PFN_SLAPVCO_TIME2               StringToCalendarTime;                                   \
    PFN_SLAPVCO_HEX_STR2            HexStringToUint32;                                      \
    PFN_SLAPVCO_HEX_STR3            HexStringToDecimal;                                     \
                                                                                            \
    PFN_SLAPVCO_MAC_ADDR3           Uint32ArrayToMacAddr;                                   \
                                                                                            \
    PFN_SLAPVCO_UINT323             Uint32ToUcharArray;                                     \
    PFN_SLAPVCO_UINT324             Uint32ToUchar;                                          \
    PFN_SLAPVCO_UINT325             UcharArrayToUint32;                                     \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_VAR_CONVERTER_OBJECT
{
    SLAP_VAR_CONVERTER_CLASS_CONTENT
}
SLAP_VAR_CONVERTER_OBJECT,  *PSLAP_VAR_CONVERTER_OBJECT;

#define  ACCESS_SLAP_VAR_CONVERTER_OBJECT(p)        \
         ACCESS_CONTAINER(p, SLAP_VAR_CONVERTER_OBJECT, Linkage)
#define  ACCESS_SLAP_VAR_CONVERTER_FIELD(f)         \
         ACCESS_FIELD_OFFSET(SLAP_VAR_CONVERTER_OBJECT, f)


#endif
