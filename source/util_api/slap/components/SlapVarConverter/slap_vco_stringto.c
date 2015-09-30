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

    module:	slap_vco_stringto.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced conversion functions
        of the Slap Var Converter Object.

        *   SlapVcoStringToBool
        *   SlapVcoStringToInt
        *   SlapVcoStringToUcharArray
        *   SlapVcoBase64StringToUcharArray
        *   SlapVcoStringToUcharArray2
        *   SlapVcoStringToUint32
        *   SlapVcoStringToIp4Addr
        *   SlapVcoStringToIp4AddrList
        *   SlapVcoStringToIp6Addr
        *   SlapVcoStringToIp6AddrList
        *   SlapVcoStringToMacAddr
        *   SlapVcoStringToMacAddrList
        *   SlapVcoStringToOidList
        *   SlapVcoStringToCalendarTime
        *   SlapVcoHexStringToUint32
        *   SlapVcoHexStringToDecimal

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        07/13/2003    initial revision.
        08/09/2010    Bin added ipv6 support
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_BOOL
        SlapVcoStringToBool
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_bool
            );

    description:

        This function is called to convert string to varBool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_bool
                Specifies the slap variable string to be converted.

    return:     varBool.

**********************************************************************/

SLAP_BOOL
SlapVcoStringToBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_bool
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    if ( AnscEqualString(var_bool, "yes",  FALSE) ||
         AnscEqualString(var_bool, "true", FALSE) ||
         AnscEqualString(var_bool, "1", FALSE))
    {
        return  0xFFFFFFFF;
    }

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_INT
        SlapVcoStringToInt
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_int
            );

    description:

        This function is called to convert string to varInt.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_int
                Specifies the slap variable string to be converted.

    return:     varInt.

**********************************************************************/

SLAP_INT
SlapVcoStringToInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_int
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    return  _ansc_atol(var_int);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToUcharArray
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_uchar_array
            );

    description:

        This function is called to convert string to varUcharArray.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_uchar_array
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulUcharCount   = AnscSizeOfString(var_uchar_array) / 2;
    ULONG                           ulTmpValue     = 0;
    ULONG                           i              = 0;
    char                            temp_char[3];

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount;
        var_ucharArray->VarCount = ulUcharCount;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;
    }

    for ( i = 0; i < ulUcharCount; i++ )
    {
        temp_char[0] = *var_uchar_array; var_uchar_array++;
        temp_char[1] = *var_uchar_array; var_uchar_array++;
        temp_char[2] = 0;

        if ( TRUE )
        {
            ulTmpValue = AnscGetStringUlongHex(temp_char);
        }
        else
        {
            /*
             * This runtime library function doesn't work well in Linux kernel...
             */
            _ansc_sscanf
                (
                    temp_char,
                    "%02X",
                    &ulTmpValue
                );
        }

        var_ucharArray->Array.arrayUchar[i] = (UCHAR)ulTmpValue;
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoBase64StringToUcharArray
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_uchar_array
            );

    description:

        This function is called to convert string to Base64 encoded varUcharArray.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_uchar_array
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoBase64StringToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;


    if ( !var_uchar_array )
    {
        return  NULL;
    }
    else
    {
		ULONG                       count          = AnscSizeOfString(var_uchar_array);
		char*                       result         = NULL;

		result = 
		AnscBase64Decode
			(
				var_uchar_array,
				&count
			);

		var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + count);
		
		var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + count;
		var_ucharArray->VarCount = count;
		var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;
        
		if ( result )
		{
			AnscCopyMemory
				(
					var_ucharArray->Array.arrayUchar,
					result,
					count
				);
			
			AnscFreeMemory(result);
		}


    }

    return  var_ucharArray;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToUcharArray2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_uchar_array
            );

    description:

        This function is called to convert string to varUcharArray.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_uchar_array
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToUcharArray2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulUcharCount   = AnscSizeOfString(var_uchar_array);
    ULONG                           i              = 0;

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount;
        var_ucharArray->VarCount = ulUcharCount;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        AnscCopyMemory
            (
                var_ucharArray->Array.arrayUchar,
                var_uchar_array,
                ulUcharCount
            );
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapVcoStringToUint32
            (
                ANSC_HANDLE                 hThisObject,
                char*                       var_uint32
            );

    description:

        This function is called to convert string to varUint.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       var_uint32
                Specifies the slap variable string to be converted.

    return:     varUint32.

**********************************************************************/

SLAP_UINT32
SlapVcoStringToUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uint32
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    if ( !var_uint32 )
    {
        return  0;
    }

    return  _ansc_atol(var_uint32);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapVcoStringToIp4Addr
            (
                ANSC_HANDLE                 hThisObject,
                char*                       ip4_addr
            );

    description:

        This function is called to convert string to varUint32/ip4_addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       ip4_addr
                Specifies the slap variable string to be converted.

    return:     varUint32.

**********************************************************************/

SLAP_UINT32
SlapVcoStringToIp4Addr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    if ( !ip4_addr )
    {
        return  0;
    }

    return  _ansc_inet_addr(ip4_addr);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32_ARRAY*
        SlapVcoStringToIp4AddrList
            (
                ANSC_HANDLE                 hThisObject,
                char*                       ip4_addr_list
            );

    description:

        This function is called to convert string to varUint32Array/
        ip4_addr_list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       ip4_addr_list
                Specifies the slap variable string to be converted.

    return:     varUint32Array.

**********************************************************************/

SLAP_UINT32_ARRAY*
SlapVcoStringToIp4AddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip4_addr_list
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject           = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    SLAP_UINT32_ARRAY*              var_uint32Array     = (SLAP_UINT32_ARRAY*        )NULL;
    ULONG                           ulAddrCount         = 0;

    if ( !ip4_addr_list )
    {
        return  NULL;
    }
    else
    {
        pAddrListTokenChain =
            AnscTcAllocate
                (
                    ip4_addr_list,
                    ",;"
                );
    }

    if ( !pAddrListTokenChain )
    {
        return  NULL;
    }
    else
    {
        SlapAllocUint32Array(NULL, AnscTcGetTokenCount(pAddrListTokenChain) * sizeof(ULONG), var_uint32Array);
    }

    if ( !var_uint32Array )
    {
        goto  EXIT1;
    }

    while ( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) )
    {
        var_uint32Array->Array.arrayUint32[ulAddrCount++] = _ansc_inet_addr(pAddrStringToken->Name);

        AnscFreeMemory(pAddrStringToken);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return  var_uint32Array;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToIp6Addr
            (
                ANSC_HANDLE                 hThisObject,
                char*                       ip6_addr
            );

    description:

        This function is called to convert string to varUcharArray/
        mac_addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       ip6_addr
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToIp6Addr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip6_addr
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    UCHAR                           ulTmpValue[16] = { 0 };

    if(! AnscParseIp6Address(ip6_addr, ulTmpValue))
    {
        return NULL;
    }

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + 16);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + 16;
        var_ucharArray->VarCount = 16;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;
    }

    AnscCopyMemory(var_ucharArray->Array.arrayUchar, ulTmpValue, 16);

    return  var_ucharArray;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToIp6AddrList
            (
                ANSC_HANDLE                 hThisObject,
                char*                       ip6_addr_list
            );

    description:

        This function is called to convert string to varUcharArray/
        ip6_addr_list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       ip6_addr_list
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToIp6AddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       ip6_addr_list
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject           = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    SLAP_UCHAR_ARRAY*               var_ucharArray      = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulAddrCount         = 0;
    ULONG                           i                   = 0;

    if ( !ip6_addr_list )
    {
        return  NULL;
    }
    else
    {
        pAddrListTokenChain =
            AnscTcAllocate
                (
                    ip6_addr_list,
                    ",;"
                );
    }

    if ( !pAddrListTokenChain )
    {
        return  NULL;
    }
    else
    {
        SlapAllocUcharArray(NULL, AnscTcGetTokenCount(pAddrListTokenChain) * 16, var_ucharArray);
    }

    if ( !var_ucharArray )
    {
        goto  EXIT1;
    }

    while ( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) )
    {
        AnscParseIp6Address(pAddrStringToken->Name, &var_ucharArray->Array.arrayUchar[ulAddrCount * 16]);

        ulAddrCount++;

        AnscFreeMemory(pAddrStringToken);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToMacAddr
            (
                ANSC_HANDLE                 hThisObject,
                char*                       mac_addr
            );

    description:

        This function is called to convert string to varUcharArray/
        mac_addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       mac_addr
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToMacAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           i              = 0;
    ULONG                           ulTmpValue[6];
    char                            temp_char [3];

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + 6);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + 6;
        var_ucharArray->VarCount = 6;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;
    }

    for ( i = 0; i < 6; i++ )
    {
        temp_char[0] = mac_addr[i * 3    ];
        temp_char[1] = mac_addr[i * 3 + 1];
        temp_char[2] = 0;

        ulTmpValue[i] = AnscGetStringUlongHex(temp_char);
    }

    /*
     * This runtime library function doesn't work well in Linux kernel...
     */
    /*
    _ansc_sscanf
        (
            mac_addr,
            "%02X:%02X:%02X:%02X:%02X:%02X",
            &ulTmpValue[0],
            &ulTmpValue[1],
            &ulTmpValue[2],
            &ulTmpValue[3],
            &ulTmpValue[4],
            &ulTmpValue[5]
        );
    */

    for ( i = 0; i < 6; i++ )
    {
        var_ucharArray->Array.arrayUchar[i] = (UCHAR)ulTmpValue[i];
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoStringToMacAddrList
            (
                ANSC_HANDLE                 hThisObject,
                char*                       mac_addr_list
            );

    description:

        This function is called to convert string to varUcharArray/
        mac_addr_list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       mac_addr_list
                Specifies the slap variable string to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoStringToMacAddrList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       mac_addr_list
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject           = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    SLAP_UCHAR_ARRAY*               var_ucharArray      = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulAddrCount         = 0;
    ULONG                           i                   = 0;
    ULONG                           ulTmpValue[6];
    char                            temp_char [3];

    if ( !mac_addr_list )
    {
        return  NULL;
    }
    else
    {
        pAddrListTokenChain =
            AnscTcAllocate
                (
                    mac_addr_list,
                    ",;"
                );
    }

    if ( !pAddrListTokenChain )
    {
        return  NULL;
    }
    else
    {
        SlapAllocUcharArray(NULL, AnscTcGetTokenCount(pAddrListTokenChain) * 6, var_ucharArray);
    }

    if ( !var_ucharArray )
    {
        goto  EXIT1;
    }

    while ( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) )
    {
        for ( i = 0; i < 6; i++ )
        {
            temp_char[0] = pAddrStringToken->Name[i * 3    ];
            temp_char[1] = pAddrStringToken->Name[i * 3 + 1];
            temp_char[2] = 0;

            ulTmpValue[i] = AnscGetStringUlongHex(temp_char);
        }

        /*
         * This runtime library function doesn't work well in Linux kernel...
         */
        /*
        _ansc_sscanf
            (
                mac_addr,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                &ulTmpValue[0],
                &ulTmpValue[1],
                &ulTmpValue[2],
                &ulTmpValue[3],
                &ulTmpValue[4],
                &ulTmpValue[5]
            );
        */

        for ( i = 0; i < 6; i++ )
        {
            var_ucharArray->Array.arrayUchar[ulAddrCount * 6 + i] = (UCHAR)ulTmpValue[i];
        }

        ulAddrCount++;

        AnscFreeMemory(pAddrStringToken);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32_ARRAY*
        SlapVcoStringToOidList
            (
                ANSC_HANDLE                 hThisObject,
                char*                       oid_list
            );

    description:

        This function is called to convert string to varUint32Array/
        oid_list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       oid_list
                Specifies the slap variable string to be converted.

    return:     varUint32Array.

**********************************************************************/

SLAP_UINT32_ARRAY*
SlapVcoStringToOidList
    (
        ANSC_HANDLE                 hThisObject,
        char*                       oid_list
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject       = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    SLAP_UINT32_ARRAY*              var_uint32Array = (SLAP_UINT32_ARRAY*        )NULL;
    PANSC_ASN1_OID                  pAsn1Oid        = (PANSC_ASN1_OID            )AnscAsn1ConvOid(oid_list);

    if ( !pAsn1Oid )
    {
        return  NULL;
    }
    else
    {
        var_uint32Array = (SLAP_UINT32_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UINT32_ARRAY) + pAsn1Oid->ArraySize);
    }

    if ( !var_uint32Array )
    {
        return  NULL;
    }
    else
    {
        var_uint32Array->Size     = sizeof(SLAP_UINT32_ARRAY) + pAsn1Oid->ArraySize;
        var_uint32Array->VarCount = pAsn1Oid->Length;
        var_uint32Array->Syntax   = SLAP_VAR_SYNTAX_uint32Array;
    }

    AnscCopyMemory
        (
            var_uint32Array->Array.arrayUint32,
            pAsn1Oid->Identifiers,
            pAsn1Oid->ArraySize
        );

    return  var_uint32Array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_HANDLE
        SlapVcoStringToCalendarTime
            (
                ANSC_HANDLE                 hThisObject,
                char*                       calendar_time
            );

    description:

        This function is called to convert string to varHandle/
        calendar_time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       calendar_time
                Specifies the slap variable string to be converted.

    return:     varHandle.

**********************************************************************/

SLAP_HANDLE
SlapVcoStringToCalendarTime
    (
        ANSC_HANDLE                 hThisObject,
        char*                       calendar_time
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    PANSC_UNIVERSAL_TIME            pUniversalTime = (PANSC_UNIVERSAL_TIME      )NULL;
    ULONG                           ulFieldIndex   = 0;
    char                            temp_char [5];

    pUniversalTime = (PANSC_UNIVERSAL_TIME)AnscAllocateMemory(sizeof(ANSC_UNIVERSAL_TIME));

    if ( !pUniversalTime )
    {
        return  (SLAP_HANDLE)NULL;
    }

    /*
     * The ISO 8601 notation is today the commonly recommended format of representing date and time
     * as human-readable strings in new plain-text communication protocols and file formats.
     * Several standards and profiles have been derived from ISO 8601, including RFC 3339 and a W3C
     * note on date and time formats.
     *
     *      - e.g. 2001-01-01
     *      - e.g. 2005-09-28T18:20:41
     */

    /*
     * Convert 'Year'...
     */
    ulFieldIndex = 0;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];
        temp_char[2] = calendar_time[ulFieldIndex++];
        temp_char[3] = calendar_time[ulFieldIndex++];

        pUniversalTime->Year = (USHORT)AnscGetStringUlong(temp_char);
    }

    /*
     * Convert 'Month'...
     */
    ulFieldIndex++;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];

        pUniversalTime->Month = (USHORT)AnscGetStringUlong(temp_char);
    }

    /*
     * Convert 'Day'...
     */
    ulFieldIndex++;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];

        pUniversalTime->DayOfMonth = (USHORT)AnscGetStringUlong(temp_char);
    }

    /*
     * Convert 'Hour'...
     */
    ulFieldIndex++;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];

        pUniversalTime->Hour = (USHORT)AnscGetStringUlong(temp_char);
    }

    /*
     * Convert 'Minute'...
     */
    ulFieldIndex++;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];

        pUniversalTime->Minute = (USHORT)AnscGetStringUlong(temp_char);
    }

    /*
     * Convert 'Second'...
     */
    ulFieldIndex++;

    if ( TRUE )
    {
        AnscZeroMemory(temp_char, 5);

        temp_char[0] = calendar_time[ulFieldIndex++];
        temp_char[1] = calendar_time[ulFieldIndex++];

        pUniversalTime->Second = (USHORT)AnscGetStringUlong(temp_char);
    }

    return  (SLAP_HANDLE)pUniversalTime;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapVcoHexStringToUint32
            (
                ANSC_HANDLE                 hThisObject,
                char*                       hex_string
            );

    description:

        This function is called to convert hex string to varUint.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       hex_string
                Specifies the slap variable string to be converted.

    return:     varUint32.

**********************************************************************/

SLAP_UINT32
SlapVcoHexStringToUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    ULONG                           ulUcharCount = AnscGetMin2(AnscSizeOfString(hex_string) / 2, sizeof(ULONG));
    ULONG                           ulTmpValue   = 0;
    ULONG                           i            = 0;
    UCHAR                           tempValue[4];
    char                            temp_char[3];

    if ( !hex_string )
    {
        return  0;
    }
    else
    {
        *(PULONG)tempValue = 0;
    }

    for ( i = 0; i < ulUcharCount; i++ )
    {
        temp_char[0] = hex_string[i * 2    ];
        temp_char[1] = hex_string[i * 2 + 1];
        temp_char[2] = 0;

        if ( TRUE )
        {
            ulTmpValue = AnscGetStringUlongHex(temp_char);
        }
        else
        {
            /*
             * This runtime library function doesn't work well in Linux kernel...
             */
            _ansc_sscanf
                (
                    &temp_char[0],
                    "%02X",
                    &ulTmpValue
                );
        }

        tempValue[i] = (UCHAR)ulTmpValue;
    }

    return  AnscUlongFromNToH(*(PULONG)tempValue);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapVcoHexStringToDecimal
            (
                ANSC_HANDLE                 hThisObject,
                char*                       hex_string
            );

    description:

        This function is called to convert hex string to varUint.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       hex_string
                Specifies the slap variable string to be converted.

    return:     varUint32.

**********************************************************************/

SLAP_UINT32
SlapVcoHexStringToDecimal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       hex_string
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    ULONG                           ulTmpValue   = 0;

    if ( !hex_string )
    {
        return  0;
    }

    if ( TRUE )
    {
        ulTmpValue = AnscGetStringUlongHex(hex_string);
    }
    else
    {
        /*
         * This runtime library function doesn't work well in Linux kernel...
         */
        _ansc_sscanf
            (
                hex_string,
                "%X",
                &ulTmpValue
            );
    }

    return  ulTmpValue;
}
