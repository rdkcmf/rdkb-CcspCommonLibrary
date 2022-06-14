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


#include "ansc_platform.h"
#include "ansc_string_util.h"
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrBufGetCharNum
            (
                PUCHAR                      pString,
                ULONG                       ulStrLen,
                UCHAR                       uChar
            )

    description:

        This function is called to count the specified character
        in the given string.

    argument:   PUCHAR                      pString
                The string to be processed.

                ULONG                       ulStrLen
                The length of string.

                UCHAR                       uChar
                The character to be counted.

    return:     the number of characters in the string.

**********************************************************************/

ULONG
AnscStrBufGetCharNum
    (
        PUCHAR                      pString,
        ULONG                       ulStrLen,
        UCHAR                       uChar
    )
{
    ULONG                           i;
    ULONG                           ulCount = 0;
    PUCHAR                          pBuf;

    if (pString && ulStrLen != 0)
    {
        pBuf    = pString;
        for (i = 0; i < ulStrLen; i ++)
        {
            if (*pBuf == uChar)
                ulCount ++;

            pBuf ++;
        }
    }

    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrGetCharNum
            (
                PUCHAR                      pString,
                UCHAR                       uChar
            )

    description:

        This function is called to get specific character
        in given string.

    argument:   PUCHAR                      pString
                The string to be processed.

                UCHAR                       uChar
                The character to be counted.

    return:     the number of characters in the string.

**********************************************************************/

ULONG
AnscStrGetCharNum
    (
        PUCHAR                      pString,
        UCHAR                       uChar
    )
{
    ULONG                           ulCount = 0;

    if (pString)
    {
        ULONG                       ulStrLen;

        ulStrLen    = AnscSizeOfString((const char *)pString);

        ulCount     = AnscStrBufGetCharNum(pString, ulStrLen, uChar);
    }

    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscStrUpper
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to captilize the given string.

    argument:   PUCHAR                      pStr
                The string to be processed.

    return:     the uppercase string.

**********************************************************************/

PUCHAR
AnscStrUpper
    (
        PUCHAR                      pStr
    )
{
    ULONG                           i, ulStrLen;
    UCHAR                           ch;

    ulStrLen    = AnscSizeOfString((const char *)pStr);

    for (i = 0; i < ulStrLen; i ++)
    {
        ch  = pStr[i];

        if (ch >= 'a' && ch <= 'z')
            pStr[i] = ch - 'a' + 'A';
    }

    return pStr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStrLower
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to convert all letters to
        lower case.

    argument:   PUCHAR                      pStr
                The string to be processed.

    return:     the string with all letters in lower case.

**********************************************************************/

PUCHAR
AnscStrLower
    (
        PUCHAR                      pStr
    )
{
    ULONG                           i, ulStrLen;
    UCHAR                           ch;

    ulStrLen    = AnscSizeOfString((const char *)pStr);

    for (i = 0; i < ulStrLen; i ++)
    {
        ch  = pStr[i];

        if (ch >= 'A' && ch <= 'Z')
            pStr[i] = ch - 'A' + 'a';
    }

    return pStr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDupString
            (
                PUCHAR                      pStr
            )

    description:

        This function is called to duplicate the given string.

    argument:   PUCHAR                      pStr
                The string to be duplicated.

    return:     the copy of the given string.

**********************************************************************/

PUCHAR
AnscDupString
    (
        PUCHAR                      pStr
    )
{
    if (!pStr)
    {
        return NULL;
    }
    else
    {
        ULONG                       ulStrLen;
        PUCHAR                      pDupStr;

        ulStrLen    = AnscSizeOfString((const char *)pStr);

        pDupStr     = (PUCHAR)AnscAllocateMemory(ulStrLen + 1);
        if (ulStrLen != 0)
            AnscCopyString((char *)pDupStr, (char *)pStr);
        else
            pDupStr[0]  = 0;

        return pDupStr;
    }
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        AnscMemUtilRealloc
            (
                PUCHAR                      pMem,
                ULONG                       ulSize,
                ULONG                       ulNewSize
            )

    description:

        This function is called to reallocate memory.

    argument:   PUCHAR                      pMem
                The memory needs to be reallocated.

                ULONG                       ulSize
                The old size of memory.

                ULONG                       ulNewSize
                The new memory size.

    return:     the new memory block.

**********************************************************************/

PVOID
AnscMemUtilRealloc
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        ULONG                       ulNewSize
    )
{
    PVOID                           pNewMem;
    ULONG                           ulCopySize;

    if (ulNewSize == 0)
    {
        if (pMem)
        {
            AnscFreeMemory(pMem);
        }

        return NULL;
    }

    pNewMem = AnscAllocateMemory(ulNewSize);

    if (ulSize == 0 || !pMem)
    {
        return pNewMem;
    }

    if (pNewMem)
    {
        /* copy content from pMem to pNewMem */
        ulCopySize  = (ulNewSize > ulSize)?ulSize:ulNewSize;

        AnscCopyMemory(pNewMem, pMem, ulCopySize);

        /* free old memory block */
        AnscFreeMemory(pMem);
    }

    return pNewMem;
}

BOOL
is_IpAddress
    (
        PUCHAR                      pString
    )
{
    struct sockaddr_in sa;
    if(inet_pton(AF_INET, (const char *)pString, &(sa.sin_addr))==1)
        return TRUE;
    else
        return FALSE;
}

BOOL
is_Ipv6_address
    (
        PUCHAR                      pString
    )
{
    struct sockaddr_in6 sa;
    if(inet_pton(AF_INET6, (const char *)pString, &(sa.sin6_addr))==1)
        return TRUE;
    else
        return FALSE;
}

BOOL is_ValidPort(PUCHAR pString)
{
     int port=0,i=0;
     while(pString[i]!='\0')
     {
         if(!(isdigit(pString[i])))
             return FALSE;
         i++;
     }
     port = atoi((const char *)pString);
     if ((port <= 0) || (port > 65535))
         return FALSE;
  return TRUE;
}
BOOL is_ValidHost(PUCHAR pString)
{
      int i = 0,count = 0,alpha = 0,len = 0;
      char* tok;
      char *host = strdup((const char*)pString);
      if(!host)
          return FALSE;  
      while(host[i]!='\0')
      {
          if(isalpha(host[i]))
              alpha = 1;
          i++;
      }
      len = strlen(host);
      if(len==0 || len>253 || host[len-1]=='-' || alpha == 0)
      {
	  /* CID 252352 fix */
	  free(host);
          return FALSE;
      }
      else
      {
          i = 0;
          tok = strtok(host,":");
          while(tok!=NULL)
          {
              if(count==0)
              {   
                  while(tok[i]!='\0')
                  {
                       if((isalnum(tok[i])) || (tok[i] == '.') || (tok[i] == '-') || (tok[i] == '_'))
                           i++;
                       else
		       {
		           /* CID 252352 fix */
			   free(host);
                           return FALSE;   
		       }
                  }
              }
              else if(count==1)
              {
                  if(!(is_ValidPort((PUCHAR)tok)))
		  {
		      /* CID 252352 fix */
		      free(host);
		      return FALSE;
		  }
              }
              else
	      {
                  /* CID 252352 fix */
                  free(host);
		  return FALSE;
	      }
             tok = strtok(NULL,":");
             if(tok!=NULL)
                 count++;
          }
      }
    free(host);
    return TRUE;
}

BOOL is_ValidIpAddressv4_port(PUCHAR pString)
{  
    char *hostdup;
    BOOL result = FALSE;
    hostdup = strdup((const char*)pString);
    if(!hostdup)
        return FALSE;  
    char* str = strtok(hostdup,":");
    char* tok = strtok(NULL,":");
    if(tok){
        if((is_IpAddress((PUCHAR)str)) && is_ValidPort((PUCHAR)tok))
            result = TRUE;  
    }
    else{
        if(is_IpAddress((PUCHAR)str))
            result = TRUE;
    }
  free(hostdup);
  return result;
}

BOOL is_ValidIpAddressv6_port(PUCHAR pString)
{
    BOOL ret = FALSE;
    if((!pString) || pString[0]!='[' || ((!(isalnum(pString[1]))) && (pString[1]!=':')))
        return FALSE;
    char *hostcpy = strdup((const char*)pString);
    char *hostdup = strdup((const char*)pString);
    char* ipv6 = strtok(hostcpy,"]");
    ipv6 = strtok(ipv6,"[");
    char *port_ptr = strchr(hostdup,']');
    if(port_ptr[1])
    {
        if((port_ptr[1]==':') && (isdigit(port_ptr[2])))
        {
            port_ptr = strtok(port_ptr,":");
            port_ptr = strtok(NULL,":");
        }
        else
	{
	    /* CID 252351, 252353 fix */
	    free(hostcpy);
	    free(hostdup);
            return FALSE;
	}
    }
    if(is_Ipv6_address((PUCHAR)ipv6))
    {
        if(port_ptr[1])
        {
            if(is_ValidPort((PUCHAR)port_ptr))
                ret = TRUE;
        }
        else
            ret = TRUE;
    }
    free(hostcpy);
    free(hostdup);
  return ret;
}

/*
   Return FALSE if pString is NULL, an empty string or contains any character from invalid_chars
*/
BOOL AnscValidStringCheck2 (char *pString, char *invalid_chars)
{
    int i;
    int len;

    if ((pString == NULL) || (*pString == '\0'))
    {
        return FALSE;
    }

    len = strlen(invalid_chars);

    while (*pString != '\0')
    {
        for (i = 0; i < len; i++)
        {
            if (*pString == invalid_chars[i])
            {
                return FALSE;
            }
        }

        pString++;
    }

    return TRUE;
}

/*
   Return FALSE if pString is NULL, an empty string or contains <space> or any of <>&|'"
*/
BOOL AnscValidStringCheck (char *pString)
{
    return AnscValidStringCheck2 (pString, " <>&|'\"");
}

BOOL
is_PrivateIp(PUCHAR pString)
{
  in_addr_t ip_addr = ntohl(inet_addr((const char *)pString));
  if(((ip_addr & 0xFF000000) == 0x00000000)  /* RFC 5735 section 3. "here" network */ ||
    ((ip_addr & 0xFF000000) == 0x0A000000)  /* 10.0.0.0/8     (private)  */ ||
    ((ip_addr & 0xFFF00000) == 0xAC100000)  /* 172.16.0.0/12  (private)  */ ||
    ((ip_addr & 0xFFFF0000) == 0xC0A80000)  /* 192.168.0.0/16 (private) */ ||
    ((ip_addr & 0xFF000000) == 0x7F000000)) /* 127.0.0.0/8    (loopback) */
      return TRUE;
  else
      return FALSE;
}
