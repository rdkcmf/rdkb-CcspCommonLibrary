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

    module:	ansc_base64.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements Base64 encoding and decoding 
        functions.

        *   AnscBase64Decode
        *   AnscBase64Encode

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/24/03    initial revision.
        11/24/03    zhubin modified Base64 to decode whole data instead
                    of line by line, in order to support DingHua's 
                    requirement of decoding base64 with 75 chars per line

**********************************************************************/


#include "ansc_platform.h"

#define  ANSC_BASE64_CODES      \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


#define ANSC_BASE64_ENCODE_OVERRUN_SIZE             3
#define ANSC_BASE64_DECODE_OVERRUN_SIZE             3


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscBase64DecodeLine
            (
                const PUCHAR                pString,
                PUCHAR                      pData,
                PULONG                      pulSize
            )

    description:

        This function is called to decode a line of 
        Base64 encode message to original text. Users
        should not call this function directly. Instead
        users should call AnscBase64Decode.

    argument:   const PUCHAR                pString
                Buffer to Base64 encoded message.

                PUCHAR                      pData
                Buffer to decoded text.

                PULONG                      pulSize [OUT]
                It contains the length of decoded text after
                this functions successfully returns.

    return:     Buffer that contains decoded text, no need 
                to be free. 

**********************************************************************/

PUCHAR
AnscBase64DecodeLine
    (
        const PUCHAR                pString,
        PUCHAR                      pData,
        PULONG                      pulSize
    )
{
    ULONG                           ulSize  = 0;
    int                             length  = *pulSize;
   
    if (pString)
    {      
        /* do a format verification first */
        if (length > 0)
        {
            int                     count   = 0, rem = 0;
            const char*             tmp     = pString;

            while (length > 0)
            {
                int                 skip;
                
                skip    = _ansc_strspn((PUCHAR)tmp, (PUCHAR)ANSC_BASE64_CODES);
                count   += skip;
                length  -= skip;
                tmp     += skip;

                if (length > 0)
                {
                    int             i, vrfy;
                    
                    vrfy    = _ansc_strcspn((PUCHAR)tmp, (PUCHAR)ANSC_BASE64_CODES);

                    for (i = 0; i < vrfy; i++)
                    {
                        if (tmp[i] == ' ' || tmp[i] == 0x0D || tmp[i] == 0x0A)
                        {
                            continue;
                        }

                        if (tmp[i] == '=')
                        {
                            /* we should check if we're close to the end of the string */
                            rem = count % 4;

                            /* rem must be either 2 or 3, otherwise no '=' should be here */
                            if (rem < 2)
                            {
                                return NULL;
                            }

                            /* end-of-message recognized */
                            goto NEXT;
                        }
                        else
                        {
                            /* Transmission error; RFC tells us to ignore this, but:
                             *  - the rest of the message is going to even more corrupt since we're sliding bits out of place
                             * If a message is corrupt, it should be dropped. Period.
                             */

                            return NULL;
                        }
                    }

                    length -= vrfy;
                    tmp += vrfy;
                }
            }


NEXT:

            ulSize  = (count / 4) * 3 + (rem ? (rem - 1) : 0) + ANSC_BASE64_DECODE_OVERRUN_SIZE;

            if (count > 0)
            {
                int                 i, qw = 0, tw = 0;

                tmp     = pString;
                length  = *pulSize;

                for (i = 0; i < length; i++)
                {
                    char        ch = pString[i];
                    UCHAR       bits;

                    if (ch == ' ' || ch == 0x0D || ch == 0x0A)
                    {
                        continue;
                    }

                    bits = 0;
                    if ((ch >= 'A') && (ch <= 'Z'))
                    {
                        bits = (UCHAR) (ch - 'A');
                    }
                    else if ((ch >= 'a') && (ch <= 'z'))
                    {
                        bits = (UCHAR) (ch - 'a' + 26);
                    }
                    else if ((ch >= '0') && (ch <= '9'))
                    {
                        bits = (UCHAR) (ch - '0' + 52);
                    }
                    else if (ch == '+')
                    {
                        bits = (UCHAR)62;
                    }
                    else if (ch == '/')
                    {
                        bits = (UCHAR)63;
                    }
                    else if (ch == '=')
                    {
                        break;
                    }

                    switch (qw++)
                    {
                        case    0:

                                pData[tw+0] = (bits << 2)   & 0xFC;

                                break;

                        case    1:

                                pData[tw+0] |= (bits >> 4)  & 0x03;
                                pData[tw+1] = (bits << 4)   & 0xF0;

                                break;

                        case    2:

                                pData[tw+1] |= (bits >> 2)  & 0x0F;
                                pData[tw+2] = (bits << 6)   & 0xC0;

                                break;

                        case    3:

                                pData[tw+2] |= bits         & 0x3F;

                                break;
                    }

                    if (qw == 4)
                    {
                        qw = 0;
                        tw += 3;
                    }
                }
            }
        }
    }

    if (pulSize)
    {
        *pulSize    = ulSize - ANSC_BASE64_DECODE_OVERRUN_SIZE;
    }

    return pData;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscBase64Decode
            (
                PUCHAR                      pEncode,
                PULONG                      pulSize
            );

    description:

        This function is called to decode Base64 encode
        message to original text.

    argument:   PUCHAR                      pEncode
                Buffer to Base64 encoded message.

                PULONG                      pulSize [OUT]
                It contains the length of decoded text after
                this functions successfully returns.

    return:     Buffer that contains decoded text, needs to
                be free after use.

**********************************************************************/

PUCHAR
AnscBase64Decode
    (
        PUCHAR                      pEncode,
        PULONG                      pulSize
    )
{
    PUCHAR                          pDecode, pBuf;
    ULONG                           ulEncodedSize;

    pBuf            = pEncode;

    /* allocate big enough memory to avoid memory reallocation */
    ulEncodedSize   = AnscSizeOfString(pEncode);
    pDecode         = (PUCHAR)AnscAllocateMemory(ulEncodedSize);
    AnscZeroMemory(pDecode, ulEncodedSize);

    if( AnscBase64DecodeLine(pBuf, pDecode, &ulEncodedSize) == NULL)
    {
        AnscTrace("Failed to decode the Base64 data.\n");

        AnscFreeMemory(pDecode);

        return NULL;
    }
    
    if (pulSize)
    {
        *pulSize    = ulEncodedSize;
    }

    return pDecode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscBase64Encode
            (
                const PUCHAR                pBuf,
                ULONG                       ulSize
            );

    description:

        This function is called to encode given text in Base64.

    argument:   const PUCHAR                pBuf
                Buffer to text needs to be encoded.

                ULONG                       ulSize
                Size of buffer.

    return:     Buffer to encoded message, should be free
                by caller after use.

**********************************************************************/

PUCHAR
AnscBase64Encode
    (
        const PUCHAR                pBuf,
        ULONG                       ulSize
    )
{
    int                             div         = ulSize / 3;
    int                             rem         = ulSize % 3;
    int                             chars       = div * 4 + rem + 1;
    PUCHAR                          pString;
    PUCHAR                          pData       = pBuf;
    PUCHAR                          pBase64Codes;

    pBase64Codes    = (PUCHAR)ANSC_BASE64_CODES;

    pString         = 
        (PUCHAR)AnscAllocateMemory(chars + ANSC_BASE64_ENCODE_OVERRUN_SIZE);

    if (pString)
    {
        PUCHAR                      buf = pString;

        while (div > 0)
        {
            buf[0]  = pBase64Codes[ (pData[0] >> 2) & 0x3F];
            buf[1]  = pBase64Codes[((pData[0] << 4) & 0x30) + ((pData[1] >> 4) & 0xF)];
            buf[2]  = pBase64Codes[((pData[1] << 2) & 0x3C) + ((pData[2] >> 6) & 0x3)];
            buf[3]  = pBase64Codes[  pData[2] & 0x3F];
            pData   += 3;
            buf     += 4;
            div --;
        }

        switch (rem)
        {
            case    2:

                    buf[0]  = pBase64Codes[ (pData[0] >> 2) & 0x3F];
                    buf[1]  = pBase64Codes[((pData[0] << 4) & 0x30) + ((pData[1] >> 4) & 0xF)];
                    buf[2]  = pBase64Codes[ (pData[1] << 2) & 0x3C];
                    buf[3]  = '=';
                    buf     += 4;

                break;

            case    1:

                    buf[0]  = pBase64Codes[ (pData[0] >> 2) & 0x3F];
                    buf[1]  = pBase64Codes[ (pData[0] << 4) & 0x30];
                    buf[2]  = '=';
                    buf[3]  = '=';
                    buf     += 4;

                break;
        }

        *buf = '\0';
    }

    return pString;
}

