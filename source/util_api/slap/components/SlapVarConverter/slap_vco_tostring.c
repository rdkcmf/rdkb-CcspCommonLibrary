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

    module:	slap_vco_tostring.c

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

        *   SlapVcoBoolToString
        *   SlapVcoIntToString
        *   SlapVcoUcharArrayToString
        *   SlapVcoUcharArrayToBase64String
        *   SlapVcoUcharArrayToString2
        *   SlapVcoUint32ToString
        *   SlapVcoIp4AddrToString
        *   SlapVcoIp4AddrToString2
        *   SlapVcoIp4AddrListToString
        *   SlapVcoIp6AddrToString
        *   SlapVcoIp6AddrListToString
        *   SlapVcoMacAddrToString
        *   SlapVcoMacAddrToString2
        *   SlapVcoMacAddrListToString
        *   SlapVcoOidListToString
        *   SlapVcoCalendarTimeToString
        *   SlapVcoUint32ToHexString

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
        08/09/2010    Bin added ipv6 support
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoBoolToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_BOOL                   var_bool
            );

    description:

        This function is called to convert varBool to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_BOOL                   var_bool
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoBoolToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_BOOL                   var_bool
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    if ( var_bool == 0 )
    {
        return  AnscCloneString("false");
    }
    else
    {
        return  AnscCloneString("true");
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIntToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_INT                    var_int
            );

    description:

        This function is called to convert varInt to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_INT                    var_int
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIntToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_INT                    var_int
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else
    {
        _ansc_itoa(var_int, var_string, 10);
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoUcharArrayToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           var_uchar_array
            );

    description:

        This function is called to convert varUcharArray to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           var_uchar_array
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoUcharArrayToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(var_uchar_array->VarCount * 2 + 1);
    ULONG                           i            = 0;

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( !var_uchar_array || (var_uchar_array->VarCount == 0) )
    {
        return  var_string;
    }
    else
    {
        for ( i = 0; i < var_uchar_array->VarCount; i++ )
        {
            _ansc_sprintf
                (
                    &var_string[i * 2],
                    "%02X",
                    var_uchar_array->Array.arrayUchar[i]
                );
        }
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoUcharArrayToBase64String
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           var_uchar_array
            );

    description:

        This function is called to convert varUcharArray to Base64 encoded string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           var_uchar_array
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoUcharArrayToBase64String
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    ULONG                           i            = var_uchar_array->VarCount;

    
    if ( !var_uchar_array || (var_uchar_array->VarCount == 0) )
    {
        return  NULL;
    }
    else
	{
        return 
        AnscBase64Encode
            (
                (const PUCHAR)var_uchar_array->Array.arrayUchar,
                i
            );
	}

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoUcharArrayToString2
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           var_uchar_array
            );

    description:

        This function is called to convert varUcharArray to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           var_uchar_array
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoUcharArrayToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(var_uchar_array->VarCount + 1);
    ULONG                           i            = 0;

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( !var_uchar_array || (var_uchar_array->VarCount == 0) )
    {
        return  var_string;
    }
    else
    {
        AnscCopyMemory
            (
                var_string,
                var_uchar_array->Array.arrayUchar,
                var_uchar_array->VarCount
            );
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoUint32ToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 var_uint32
            );

    description:

        This function is called to convert varUint32 to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 var_uint32
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoUint32ToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else
    {
        _ansc_ultoa(var_uint32, var_string, 10);
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIp4AddrToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ip4_addr
            );

    description:

        This function is called to convert varUint32/ip4_addr to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ip4_addr
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIp4AddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else
    {
        AnscGetIpAddrString(((PUCHAR)&ip4_addr), var_string);
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIp4AddrToString2
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ip4_addr
            );

    description:

        This function is called to convert varUint32/ip4_addr to string.
        The differece between this function and Ip4AddrToString() is
        that this function returns an empty string if the address is
        all zeros.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ip4_addr
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIp4AddrToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ip4_addr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( ip4_addr == 0 )
    {
        return  var_string;
    }
    else
    {
        AnscGetIpAddrString(((PUCHAR)&ip4_addr), var_string);
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIp4AddrListToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32_ARRAY*          ip4_addr_list
            );

    description:

        This function is called to convert varUint32Array/ip4_addr_list
        to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32_ARRAY*          ip4_addr_list
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIp4AddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          ip4_addr_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )(ip4_addr_list? AnscAllocateMemory(32 * ip4_addr_list->VarCount + 1) : NULL);
    ULONG                           i            = 0;
    ANSC_IPV4_ADDRESS               temp_addr;

    if ( !var_string )
    {
        return  NULL;
    }

    if ( !ip4_addr_list || (ip4_addr_list->VarCount == 0) )
    {
        return  var_string;
    }
    else
    {
        for ( i = 0; i < ip4_addr_list->VarCount; i++ )
        {
            temp_addr.Value = ip4_addr_list->Array.arrayUint32[i];

            if ( i == (ip4_addr_list->VarCount - 1) )
            {
                _ansc_sprintf
                    (
                        &var_string[AnscSizeOfString(var_string)],
                        "%d.%d.%d.%d",
                        temp_addr.Dot[0],
                        temp_addr.Dot[1],
                        temp_addr.Dot[2],
                        temp_addr.Dot[3]
                    );
            }
            else
            {
                _ansc_sprintf
                    (
                        &var_string[AnscSizeOfString(var_string)],
                        "%d.%d.%d.%d,",
                        temp_addr.Dot[0],
                        temp_addr.Dot[1],
                        temp_addr.Dot[2],
                        temp_addr.Dot[3]
                    );
            }
        }
    }

    return  var_string;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIp6AddrToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           ip6_addr
            );

    description:

        This function is called to convert varUcharArray/ip6_addr to
        string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           ip6_addr
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIp6AddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           ip6_addr
    )
{
    if( ip6_addr == NULL)
    {
        return AnscCloneString("::");
    }

    return AnscIp6AddressToString(ip6_addr->Array.arrayUchar);

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoIp6AddrListToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           ip6_addr_list
            );

    description:

        This function is called to convert varUcharArray/ip6_addr_list
        to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           ip6_addr_list
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoIp6AddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           ip6_addr_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )(ip6_addr_list? AnscAllocateMemory(ip6_addr_list->VarCount * 4 + 1) : NULL);
    ULONG                           i            = 0;
    char*                           pTempString  = NULL;

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( !ip6_addr_list || (ip6_addr_list->VarCount == 0) )
    {
        return  var_string;
    }

    for ( i = 0; i < ip6_addr_list->VarCount; i++ )
    {
        if ( ((i % 16) == 0) && (i != 0) )
        {
            _ansc_sprintf
                (
                    &var_string[AnscSizeOfString(var_string)],
                    ", "
                );
        }

        pTempString = AnscIp6AddressToString((unsigned char*)&ip6_addr_list->Array.arrayUchar[i]);

        if( pTempString != NULL)
        {
            _ansc_strcat
                (
                    var_string,
                    pTempString
                );

            AnscFreeMemory(pTempString);
        }
    }

    return  var_string;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoMacAddrToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           mac_addr
            );

    description:

        This function is called to convert varUcharArray/mac_addr to
        string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           mac_addr
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoMacAddrToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else
    {
        _ansc_sprintf
            (
                var_string,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                (mac_addr->VarCount > 0)? mac_addr->Array.arrayUchar[0] : 0,
                (mac_addr->VarCount > 1)? mac_addr->Array.arrayUchar[1] : 0,
                (mac_addr->VarCount > 2)? mac_addr->Array.arrayUchar[2] : 0,
                (mac_addr->VarCount > 3)? mac_addr->Array.arrayUchar[3] : 0,
                (mac_addr->VarCount > 4)? mac_addr->Array.arrayUchar[4] : 0,
                (mac_addr->VarCount > 5)? mac_addr->Array.arrayUchar[5] : 0
            );
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoMacAddrToString2
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           mac_addr
            );

    description:

        This function is called to convert varUcharArray/mac_addr to
        string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           mac_addr
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoMacAddrToString2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
	else if ( !mac_addr->Array.arrayUchar[0] &&
		!mac_addr->Array.arrayUchar[1] &&
		!mac_addr->Array.arrayUchar[2] &&
		!mac_addr->Array.arrayUchar[3] &&
		!mac_addr->Array.arrayUchar[4] &&
		!mac_addr->Array.arrayUchar[5] )
	{
		var_string[0] = '\0';
	}
    else
    {
        _ansc_sprintf
            (
                var_string,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                (mac_addr->VarCount > 0)? mac_addr->Array.arrayUchar[0] : 0,
                (mac_addr->VarCount > 1)? mac_addr->Array.arrayUchar[1] : 0,
                (mac_addr->VarCount > 2)? mac_addr->Array.arrayUchar[2] : 0,
                (mac_addr->VarCount > 3)? mac_addr->Array.arrayUchar[3] : 0,
                (mac_addr->VarCount > 4)? mac_addr->Array.arrayUchar[4] : 0,
                (mac_addr->VarCount > 5)? mac_addr->Array.arrayUchar[5] : 0
            );
    }

    return  var_string;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoMacAddrListToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           mac_addr_list
            );

    description:

        This function is called to convert varUcharArray/mac_addr_list
        to string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           mac_addr_list
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoMacAddrListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           mac_addr_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )(mac_addr_list? AnscAllocateMemory(mac_addr_list->VarCount * 24 / 6 + 1) : NULL);
    ULONG                           i            = 0;

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( !mac_addr_list || (mac_addr_list->VarCount == 0) )
    {
        return  var_string;
    }

    for ( i = 0; i < mac_addr_list->VarCount; i++ )
    {
        if ( ((i % 6) == 0) && (i != 0) )
        {
            _ansc_sprintf
                (
                    &var_string[AnscSizeOfString(var_string)],
                    ", "
                );
        }

        _ansc_sprintf
            (
                &var_string[AnscSizeOfString(var_string)],
                "%02X",
                mac_addr_list->Array.arrayUchar[i]
            );
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoOidListToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32_ARRAY*          oid_list
            );

    description:

        This function is called to convert varUint32Array/oid_list to
        string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32_ARRAY*          oid_list
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoOidListToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          oid_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )(oid_list? AnscAllocateMemory(oid_list->VarCount * 16 + 1) : NULL);
    ULONG                           i            = 0;

    if ( !var_string )
    {
        return  NULL;
    }
    else if ( !oid_list || (oid_list->VarCount == 0) )
    {
        return  var_string;
    }
    else
    {
        for ( i = 0; i < oid_list->VarCount; i++ )
        {
            _ansc_sprintf
                (
                    &var_string[AnscSizeOfString(var_string)],
                    "%d.",
                    oid_list->Array.arrayUint32[i]
                );
        }

        var_string[AnscSizeOfString(var_string) - 1] = 0;
    }

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoCalendarTimeToString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_HANDLE                 calendar_time
            );

    description:

        This function is called to convert varHandle/calendar_time to
        string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_HANDLE                 calendar_time
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoCalendarTimeToString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_HANDLE                 calendar_time
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject      = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    PANSC_UNIVERSAL_TIME            pUniversalTime = (PANSC_UNIVERSAL_TIME      )calendar_time;
    char*                           var_string     = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }

    _ansc_sprintf
        (
            var_string,
            "%04d-%02d-%02dT%02d:%02d:%02dZ",
            pUniversalTime->Year,
            pUniversalTime->Month,
            pUniversalTime->DayOfMonth,
            pUniversalTime->Hour,
            pUniversalTime->Minute,
            pUniversalTime->Second
        );

    return  var_string;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapVcoUint32ToHexString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 var_uint32
            );

    description:

        This function is called to convert varUint32 to hex string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 var_uint32
                Specifies the slap variable to be converted.

    return:     varString.

**********************************************************************/

char*
SlapVcoUint32ToHexString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    char*                           var_string   = (char*                     )AnscAllocateMemory(32);

    if ( !var_string )
    {
        return  NULL;
    }
    else
    {
        /*_ansc_ultoa(var_uint32, var_string, 16);*/
        _ansc_sprintf
            (
                var_string,
                "%X",
                var_uint32
            );
    }

    return  var_string;
}
