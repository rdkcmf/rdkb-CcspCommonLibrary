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

    module: dslh_varro_utility.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the utility functions
        of the Dslh Var Record Object.

        *   DslhVarroIsValidMacAddress
        *   DslhVarroIsValidIpAddList
        *   DslhVarroIsValidMacAddList
        *   DslhVarroIsValidHexString
        *   DslhVarroIsValidIp6AddList
        *   DslhVarroIsValidCalendarString
        *   DslhVarroIsValidBase64String
        *   DslhVarroIsValidUrlString
        *   DslhVarroIsValidMaskString
        *   DslhVarroMacAddrListToString
        *   DslhVarroDelSpaceInCommaSeparator

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        12/08/06    initial revision.

**********************************************************************/


#include "dslh_varro_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidMacAddress
            (
                PCHAR                       pAddress
            );

    description:

        This function is called check if it's a valid MacAddress.

    argument:   PCHAR                       pAddress
                The Mac Address;

    return:     TRUE or FALSE

**********************************************************************/

BOOL
DslhVarroIsValidMacAddress
    (
        PCHAR                       pAddress
    )
{
    ULONG                           length   = 0;
    ULONG                           i        = 0;
    char                            c        = 0;

    if( pAddress == NULL)
    {
        return TRUE; /* empty string is fine */
    }

    length = AnscSizeOfString(pAddress);

    if( length == 0)
    {
        return TRUE; /* empty string is fine */
    }

    /*
     *  Mac address such as "12:BB:AA:99:34:89" is fine, and mac adress
     *  with Mask is also OK, such as "12:BB:AA:99:34:89/FF:FF:FF:FF:FF:00".
     */
    if( length != 17 && length != 35)
    {
        return FALSE;
    }

    if( length > 17 && pAddress[17] != '/')
    {
        return FALSE;
    }

    for( i = 0; i < length ; i ++)
    {
        c = pAddress[i];

        if( i % 3 == 2)
        {
            if( i != 17 && c != ':')
            {
                return FALSE;
            }
        }
        else
        {
            if ( AnscIsAlphaOrDigit(c) )
            {
                continue;
            }

            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidIpAddList
            (
                PCHAR                       pAddList
            );

    description:

        This function is called check if it's a valid IpAddress List;

    argument:   PCHAR                       pAddList
                The IpAddress List string seperated by ',';

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidIpAddList
    (
        PCHAR                       pAddList
    )
{
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    BOOL                            bIsValid            = TRUE;

    if( !pAddList || AnscSizeOfString(pAddList) == 0)
    {
        return bIsValid;
    }

    if ( DslhVarroDelSpaceInCommaSeparator((PCHAR)pAddList) )
    {
        if (( pAddrListTokenChain = AnscTcAllocate( pAddList, "," ) ))
        {
            while (( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) ))
            {
                if( ! AnscIsValidIpString((PCHAR)pAddrStringToken->Name))
                {
                    bIsValid = FALSE;

                    AnscFreeMemory(pAddrStringToken);

                    goto EXIT;
                }

                AnscFreeMemory(pAddrStringToken);
            }

        }
        else
        {
            return bIsValid = FALSE;
        }
    }
    else
    {
        return bIsValid = FALSE;
    }

EXIT:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return bIsValid;
}


/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidIpAddRangeList
            (
                PCHAR                       pAddRangeList
            );

    description:

        This function is called check if it's a valid IpAddress 
        range List;

    argument:   PCHAR                       pAddList
                The IpAddress List string seperated by ',';

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidIpAddRangeList
    (
        PCHAR                       pAddRangeList
    )
{
    PANSC_TOKEN_CHAIN               pAddrRangeTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_TOKEN_CHAIN               pAddrTokenChain      = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken     = (PANSC_STRING_TOKEN        )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken1    = (PANSC_STRING_TOKEN        )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken2    = (PANSC_STRING_TOKEN        )NULL;
    BOOL                            bIsValid             = TRUE;

    if( !pAddRangeList || AnscSizeOfString(pAddRangeList) == 0)
    {
        return bIsValid;
    }

    if ( DslhVarroDelSpaceInCommaSeparator((PCHAR)pAddRangeList) )
    {
        if ( (AnscSizeOfString(pAddRangeList) != 0) && (pAddrRangeTokenChain = AnscTcAllocate( pAddRangeList, "," )) )
        {
            while (( pAddrStringToken = AnscTcUnlinkToken(pAddrRangeTokenChain) ))
            {
                if (( pAddrTokenChain = AnscTcAllocate(pAddrStringToken->Name, "-") ))
                {

                    pAddrStringToken1 = AnscTcUnlinkToken(pAddrTokenChain);

                    if( ! AnscIsValidIpString((PCHAR)pAddrStringToken1->Name))
                    {
                        bIsValid = FALSE;

                        goto EXIT2;
                    }

                    pAddrStringToken2 = AnscTcUnlinkToken(pAddrTokenChain);

                    if( !pAddrStringToken2 || ! AnscIsValidIpString((PCHAR)pAddrStringToken2->Name))
                    {
                        bIsValid = FALSE;

                        goto EXIT3;
                    }

                    if ( _ansc_inet_addr(pAddrStringToken1->Name) > _ansc_inet_addr(pAddrStringToken2->Name) )
                    {
                        bIsValid = FALSE;

                        goto EXIT3;
                    }

                    AnscFreeMemory(pAddrStringToken2);
                    AnscFreeMemory(pAddrStringToken1);
                    AnscFreeMemory(pAddrStringToken);
                    AnscTcFree((ANSC_HANDLE)pAddrTokenChain);
                }
                else
                {
                    bIsValid = FALSE;

                    goto EXIT1;
                }
            }

            goto EXIT;
        }
        else
        {
            return bIsValid = FALSE;
        }
    }
    else
    {
        return bIsValid = FALSE;
    }

EXIT3:

    if ( pAddrStringToken2 )
    {
        AnscFreeMemory(pAddrStringToken2);
    }

EXIT2:
    
    if ( pAddrStringToken1 )
    {
        AnscFreeMemory(pAddrStringToken1);
    }

    if ( pAddrStringToken )
    {
        AnscFreeMemory(pAddrStringToken);
    }

EXIT1:

    if ( pAddrTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrTokenChain);
    }

EXIT:

    if ( pAddrRangeTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrRangeTokenChain);
    }

    return bIsValid;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidMacAddList
            (
                PCHAR                       pAddList
            );

    description:

        This function is called check if it's a valid MacAddress List;

    argument:   PCHAR                       pAddList
                The MacAddress List string seperated by ',';

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidMacAddList
    (
        PCHAR                       pAddList
    )
{
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    BOOL                            bIsValid            = TRUE;

    if( !pAddList || AnscSizeOfString(pAddList) == 0)
    {
        return bIsValid;
    }

    if ( DslhVarroDelSpaceInCommaSeparator((PCHAR)pAddList) )
    {
        if (( pAddrListTokenChain = AnscTcAllocate( pAddList, "," ) ))
        {
            while (( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) ))
            {
                if( ! DslhVarroIsValidMacAddress((PCHAR)pAddrStringToken->Name))
                {
                    bIsValid = FALSE;

                    AnscFreeMemory(pAddrStringToken);

                    goto EXIT;
                }

                AnscFreeMemory(pAddrStringToken);
            }
        }
        else
        {

            return bIsValid = FALSE;
        }
    }
    else
    {
        return bIsValid = FALSE;
    }

EXIT:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return bIsValid;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidHexString
            (
                PCHAR                       pValue
            );

    description:

        This function is called check if it's a valid Hex String Value;

    argument:   PCHAR                       pValue
                The HEX string value;

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidHexString
    (
        PCHAR                       pValue
    )
{
    ULONG                           length   = 0;
    ULONG                           i        = 0;
    char                            c        = 0;

    if( pValue == NULL)
    {
        return TRUE; /* empty string is fine */
    }

    length = AnscSizeOfString(pValue);

    if( length % 2 != 0)
    {
        return FALSE;
    }

    for( i = 0; i < length ; i ++)
    {
        c = pValue[i];

            if ( AnscIsHexAlphaOrDigit(c) )
        {
            continue;
        }

        return FALSE;
    }

    return TRUE;
}


/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidCalendarString
            (
                PCHAR                       pValue
            );

    description:

        This function is called check if it's a valid Calendar String Value;

    argument:
                ANSC_HANDLE                 hSlapConverter,
                The SlapConverter Handle;

                PCHAR                       pValue
                The Calendar string value;

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidCalendarString
    (
        PCHAR                       pValue
    )
{
    PANSC_UNIVERSAL_TIME            pUniversalTime    = (PANSC_UNIVERSAL_TIME      )NULL;
    ULONG                           length            = 0;
    char                            c                 = 0;
    int                             i                 = 0;

    if( pValue == NULL)
    {
        return FALSE;
    }

    /*
     * The ISO 8601 notation is today the commonly recommended format of representing date and time
     * as human-readable strings in new plain-text communication protocols and file formats.
     * Several standards and profiles have been derived from ISO 8601, including RFC 3339 and a W3C
     * note on date and time formats.
     *
     * Three type of times supported
     * 1. Unknown or unapplicable time - 0001-01-01T00:00:00Z  (20 Bytes)
     * 2. Absolute time - YYYY-MM-DDThh:mm:ssZ                 (20 Bytes)
     * 3. Relative time - YYYY-MM-DDThh:mm:ss                  (19 Bytes)
     *
     */

    length = AnscSizeOfString(pValue);

    if( length != 19 && length != 20)
    {
        return FALSE;
    }

    if( pValue[4] != '-' || pValue[7] != '-' || pValue[10] != 'T' ||
        pValue[13]!= ':' || pValue[16]!= ':')
    {
        return FALSE;
    }

    if( length == 20 && pValue[19] != 'Z')
    {
        return FALSE;
    }

    for( i = 0; i < 19; i ++)
    {
        if( i != 4 && i != 7 && i != 10 && i != 13 && i != 16)
        {
            c = pValue[i];

            if( c < '0' || c > '9')
            {
                return FALSE;
            }
        }
    }

    pUniversalTime =
        (PANSC_UNIVERSAL_TIME)SlapVcoStringToCalendarTime
            (
                NULL,
                pValue
            );

    if( pUniversalTime == NULL)
    {
        return FALSE;
    }

    if( pUniversalTime->Year  == 1      &&
        pUniversalTime->Month == 1      &&
        pUniversalTime->DayOfMonth == 1 &&
        pUniversalTime->Hour   == 0     &&
        pUniversalTime->Minute == 0     &&
        pUniversalTime->Second == 0    )
    {
        /* unknown time */
        AnscFreeMemory(pUniversalTime);

        return TRUE;

    }
    else if( length == 20)
    {
        /* absolute time */

        if( pUniversalTime->Year  < 1000    ||
            pUniversalTime->Month == 0      || pUniversalTime->Month > 12      ||
            pUniversalTime->DayOfMonth == 0 || pUniversalTime->DayOfMonth > 31 ||
            pUniversalTime->Hour   >= 60    ||
            pUniversalTime->Minute >= 60    ||
            pUniversalTime->Second >= 60    )
        {
            AnscFreeMemory(pUniversalTime);

            return FALSE;
        }
    }
    else
    {
        /* relative time */
        if( pUniversalTime->Year  >=  1000  || pUniversalTime->Year  ==  0     ||
            pUniversalTime->Month == 0      || pUniversalTime->Month > 12      ||
            pUniversalTime->DayOfMonth == 0 || pUniversalTime->DayOfMonth > 31 ||
            pUniversalTime->Hour   >= 60    ||
            pUniversalTime->Minute >= 60    ||
            pUniversalTime->Second >= 60    )
        {
            AnscFreeMemory(pUniversalTime);

            return FALSE;
        }
    }

    AnscFreeMemory(pUniversalTime);

    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidBase64String
            (
                PCHAR                       pString
            );

    description:

        This function is called check if it's a valid base64 string;

    argument:   PCHAR                       pString
                The string;

    return:     TRUE or FALSE

**********************************************************************/

BOOL
DslhVarroIsValidBase64String
    (
        PCHAR                       pString
    )
{
    ULONG                           length   = 0;
    PUCHAR                          pNewBuf  = NULL;

    if( pString == NULL)
    {
        return TRUE; /* empty string is fine */
    }

    length = AnscSizeOfString(pString);

    if( length == 0)
    {
        return TRUE; /* empty string is fine */
    }

    pNewBuf =
        AnscBase64Decode
            (
                (PUCHAR)pString,
                &length
            );

    if( pNewBuf == NULL)
    {
        return FALSE;
    }

    AnscFreeMemory(pNewBuf);

    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidUrlString
        (
            PCHAR                       pListString
        )

    description:

        This function is called check if a string is an valid url;.

    argument:   PCHAR                       pStringValue
                The input string;

    return:     TRUE or FALSE;

**********************************************************************/

BOOL
DslhVarroIsValidUrlString
    (
        PCHAR                           pStringValue
    )
{
    PCHAR                               pTmpSubString       = NULL;
    PCHAR                               pHostString         = NULL;
    PCHAR                               pPortString         = NULL;
    char                                port[6]             = {0};
    ULONG                               i                   = 0;
    ULONG                               j                   = 0;
    ULONG                               ulPortNum           = 0;

    pTmpSubString = _ansc_strstr(pStringValue, "http");

    if ( pTmpSubString && (pTmpSubString == pStringValue) )
    {

        pTmpSubString +=  AnscSizeOfString("http");

        if ( *pTmpSubString == 's')
        {
            pTmpSubString += 1;
        }

        pHostString = _ansc_strstr(pTmpSubString, "://");

        if ( pHostString && (pHostString == pTmpSubString) )
        {
            pHostString += AnscSizeOfString("://");

            if (( pPortString = _ansc_strstr(pHostString, ":") ))
            {
                pPortString = pPortString + 1;

                while ( AnscIsDigit(*(pPortString+i)) && i < AnscSizeOfString(pPortString) )
                {
                    i++;
                }

                if ( (AnscSizeOfString(pPortString) == i) || ((AnscSizeOfString(pPortString) >i) && (*(pPortString+i) == '/')) )
                {
                    for (j = 0; j < i; j++)
                    {
                        port[j] = *(pPortString+j);
                    }

                    ulPortNum = AnscGetStringUlong(port);

                    if ( (ulPortNum > 0) && (ulPortNum <= 65535) )
                    {
                        return TRUE;
                    }
                }
            }
            else
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidMaskString
        (
            PCHAR                       pListString
        )

    description:

        This function is called check if a string is an valid subnet mask;.

    argument:   PCHAR                       pStringValue
                The input string;

    return:     TRUE or FALSE;

**********************************************************************/

BOOL
DslhVarroIsValidMaskString
    (
        PCHAR                           pStringValue
    )
{
    PANSC_TOKEN_CHAIN                   pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN                  pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
    ULONG                               ulMaskValue         = 0;
    ULONG                               i                   = 0;
    BOOL                                bIsValid            = FALSE;

    if ( !pStringValue || (AnscSizeOfString(pStringValue) == 0) )
    {
        return TRUE;
    }

    if (( pAddrListTokenChain = AnscTcAllocate(pStringValue, ".") ))
    {
        if (pAddrListTokenChain->TokensQueue.Depth != 0x4)
        {
            goto EXIT;
        }

        while (( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) ))
        {
            ulMaskValue = (ulMaskValue<<0x8) + AnscGetStringUlong(pAddrStringToken->Name);

            AnscFreeMemory(pAddrStringToken);
        }

        if ( ! ulMaskValue )
        {
            bIsValid = TRUE;
            goto EXIT;
        }

        for ( i = 0; !(ulMaskValue & 0x1); i++)
        {
            ulMaskValue >>= 0x1;
        }

        /* Subnet mask shorter than 8 bits is rejected */
        if ( i > 24)
        {
            goto EXIT;
        }

	/*CID: 71787 Bad bit shift operation*/
        else if ( (!i && !(~(0x0 ^ ulMaskValue)))           /* 255.255.255.255 */
                  ||
                  ! (~( ( (((ULONG)~0x0)<<(31-i))) ^ ulMaskValue ))  /* Other subnet masks */
                )
        {
            bIsValid = TRUE;
            goto EXIT;
        }
    }

EXIT:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

    return bIsValid;
}

/**********************************************************************

    prototype:

        char*
        DslhVarroMacAddrListToString
            (
                SLAP_UCHAR_ARRAY*           mac_addr_list
            );

    description:

        This function is called to export MAC address list to a string.

    argument:   SLAP_UCHAR_ARRAY*           mac_addr_list
                The input mac address list;

    return:     The exported string;

**********************************************************************/
char*
DslhVarroMacAddrListToString
    (
        SLAP_UCHAR_ARRAY*           mac_addr_list
    )
{
    char*                           var_string   = (char*                     )(mac_addr_list? AnscAllocateMemory(mac_addr_list->VarCount * 24 / 6 + 1) : NULL);
    ULONG                           i            = 0;
    errno_t                         rc           = -1;

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
        if( i != 0 )
        {
            if ( (i % 6) == 0)
            {
                rc = sprintf_s
                    (
                        &var_string[AnscSizeOfString(var_string)],
                        ((mac_addr_list->VarCount * 24 / 6 + 1) - AnscSizeOfString(var_string)),
                        ","
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                   return NULL;
                }
            }
            else if( i % 6 )
            {
                rc = sprintf_s
                    (
                        &var_string[AnscSizeOfString(var_string)],
                        ((mac_addr_list->VarCount * 24 / 6 + 1) - AnscSizeOfString(var_string)),
                        ":"
                    );
                if(rc < EOK)
                {
                   ERR_CHK(rc);
                   return NULL;
                }
            }
        }

        rc = sprintf_s
            (
                &var_string[AnscSizeOfString(var_string)],
                ((mac_addr_list->VarCount * 24 / 6 + 1) - AnscSizeOfString(var_string)),
                "%02X",
                mac_addr_list->Array.arrayUchar[i]
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return NULL;
        }
    }

    return  var_string;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroDelSpaceInCommaSeparator
        (
            PCHAR                       pListString
        )

    description:

        This function is called to delete spaces in a comma-separator.

    argument:   PCHAR                       pListString
                The input comma-separated IP address string;

    return:     The exported list string;

**********************************************************************/
BOOL
DslhVarroDelSpaceInCommaSeparator
    (
        PCHAR                       pListString
    )
{
    ULONG                           ulPosition              = 0;
    ULONG                           ulListStringLen         = 0;
    ULONG                           i                       = 0;
    ULONG                           j                       = 0;

    if( ! pListString || (AnscSizeOfString(pListString) == 0) || ! AnscCharInString(pListString, ANSC_SPACE))
    {
        return TRUE;
    }

    for ( i = 0; i < AnscSizeOfString(pListString); i++)
    {
        ulPosition = 0;

        ulListStringLen = AnscSizeOfString(pListString);

            for ( ulPosition = 0; ulPosition < ulListStringLen-i; ulPosition++)
        {
            if ( *(pListString+i+ulPosition) != ANSC_SPACE )
            {
                break;
            }
        }

        if ( ulPosition )
        {
            for ( j = 0; j < ulListStringLen-i-ulPosition+1; j++)
                {
                    *(pListString+i+j) = *(pListString+i+j+ulPosition);
                }
        }
    }

    if ( ! AnscCharInString(pListString, ANSC_SPACE))
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    prototype:

        BOOL
        DslhVarroIsValidIp6AddList
            (
                PCHAR						pAddList
            );

    description:

        This function is called check if it's a valid Ip6Address List;

    argument:   PCHAR						pAddList
                The MacAddress List string seperated by ',';

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhVarroIsValidIp6AddList
    (
        PCHAR						pAddList
    )
{
    PANSC_TOKEN_CHAIN               pAddrListTokenChain = (PANSC_TOKEN_CHAIN         )NULL;
    PANSC_STRING_TOKEN              pAddrStringToken    = (PANSC_STRING_TOKEN        )NULL;
	BOOL							bIsValid			= TRUE;
    ULONG                           uLength             = 0;

	if( !pAddList || AnscSizeOfString(pAddList) == 0)
	{
		return bIsValid;
	}

    uLength = AnscSizeOfString(pAddList);

    /* cannot start or end with ',' */
    if( pAddList[0] == ',' || pAddList[uLength-1] == ',' )
    {
        return FALSE;
    }

    /* cannot have empty address in the middle */
    if( _ansc_strstr(pAddList, ",,") != NULL)
    {
        return FALSE;
    }

	if ( DslhVarroDelSpaceInCommaSeparator((PCHAR)pAddList) )
	{
		if (( pAddrListTokenChain = AnscTcAllocate( pAddList, "," ) ))
		{
			while (( pAddrStringToken = AnscTcUnlinkToken(pAddrListTokenChain) ))
			{
				if( ! AnscIsValidIp6String((PCHAR)pAddrStringToken->Name))
				{
					bIsValid = FALSE;

					AnscFreeMemory(pAddrStringToken);

					goto EXIT;
				}

                AnscFreeMemory(pAddrStringToken);
			}            
		}
		else
		{
			
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

EXIT:

    if ( pAddrListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pAddrListTokenChain);
    }

	return bIsValid;
}
