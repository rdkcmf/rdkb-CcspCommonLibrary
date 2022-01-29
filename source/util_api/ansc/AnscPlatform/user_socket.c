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

#include "user_base.h"
#include "user_socket.h"
#include <sys/ioctl.h>
#include <linux/if.h>
#include "safec_lib_common.h"


int
_ansc_select
    (
        int                     width,
        pansc_fd_set            pReadFds,
        pansc_fd_set            pWriteFds,
        pansc_fd_set            pExceptFds,
        struct timeval*         pTimeout
    )
{
    fd_set*                 rSet         = pReadFds   ? &(pReadFds->set)   : NULL;
    fd_set*                 wSet         = pWriteFds  ? &(pWriteFds->set)  : NULL;
    fd_set*                 eSet         = pExceptFds ? &(pExceptFds->set) : NULL;

    /*UserTrace("_ansc_select is called.\n");*/

    return  select(width, rSet, wSet, eSet, pTimeout);
}

void
_ansc_socket_fd_get
    (
        pansc_fd_set            pSet,
        int*                    pSocket,
        int                     i
    )
{
#ifdef __USE_XOPEN
   int                 iArraySize          = sizeof(pSet->set.fds_bits) / sizeof(long);
#else
   int                 iArraySize          = sizeof(pSet->set.__fds_bits) / sizeof(fd_mask);
#endif
    int                 iArrayIndex         = 0;
    int                 iBitIndex           = 0;
    int                 iSetCount           = 0;
    int                 bFound              = 0;

    /*UserTrace("iArraySize: %d, MFDBITS: %d\n", iArraySize, NFDBITS);*/

    for ( iArrayIndex = 0; iArrayIndex < iArraySize; iArrayIndex ++ )
    {
	    /*UserTrace("%X\n", pSet->set.fds_bits[iArrayIndex]);*/

        for ( iBitIndex = 0; (unsigned int)iBitIndex < NFDBITS; iBitIndex ++ )
        {
#ifdef __USE_XOPEN
            if( pSet->set.fds_bits[iArrayIndex] & (1 << iBitIndex) )
#else
            if( pSet->set.__fds_bits[iArrayIndex] & (1 << iBitIndex) )
#endif
            {
                if( iSetCount == i )
                {
                    bFound = 1;
                    break;
                }

                iSetCount ++;
            }
        }

        if ( bFound == 1 )
        {
			break;
	    }
    }

    /*UserTrace("iSetCount: %d, iBitIndex: %d\n", iSetCount, iBitIndex);*/

    if ( bFound == 1)
    {
        *pSocket = iArrayIndex * NFDBITS + iBitIndex;
    }
    else
    {
        *pSocket = ANSC_SOCKET_INVALID_SOCKET;
    }

    return;
}

int
send_all (int fd, char *buf, int bufsize, int c)
{
  int res =0;
  while (bufsize > 0)
  {
	  do
	    res = send (fd, buf, bufsize, c);
	  while (res == -1 && errno == EINTR);

	  if (res <= 0)
	  	break;
	  buf += res;
	  bufsize -= res;
  }

  return res;
}

int
recv_all (int fd, char *buf, int bufsize, int c)
{
  int res;
  do
    res = recv (fd, buf, bufsize, c);
  while (res == -1 && errno == EINTR);

  return res;
}

/*for int=64bit platform, this code may fail.*/
int
_ansc_get_ipv4_addr
    (
        char*          netdev,
        unsigned int * p_n_addr,
        char *         p_addr,
        int            p_addr_len
    )
{

    struct ifreq            ifr;
    int                     fd = 0;
    errno_t                 rc = -1;

    if (!netdev)
        return -1;

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), netdev);
    ERR_CHK(rc);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        if (!ioctl(fd, SIOCGIFADDR, &ifr))
           memcpy(p_n_addr, ifr.ifr_ifru.ifru_addr.sa_data + 2,4);
        else
           perror("_ansc_get_ipv4_addr IOCTL failure.");

        close(fd);
    }
    else
    {
        perror("_ansc_get_ipv4_addr failed to open socket.");
        return -1;
    }

    if (p_addr)
    {
        struct in_addr addr1 = {0}; 
        addr1.s_addr = *p_n_addr;
        snprintf(p_addr, p_addr_len, inet_ntoa(addr1));
    }
    
    return 0;
}


int
_ansc_get_hw_addr
    (
        char*          netdev,
        char *         p_addr,
        int            p_addr_len
    )
{

    struct ifreq            ifr;
    int                     fd = 0, j, k;
    errno_t                 rc = -1;

    if (!netdev)
        return -1;

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), netdev);
    ERR_CHK(rc);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        if (!ioctl(fd, SIOCGIFHWADDR, &ifr))
        {
            if ( p_addr && p_addr_len > 17 )
            {
                for (j=0, k=0; j<6; j++)
                {
                    k+=snprintf(p_addr+k, p_addr_len-k-1, j ? ":%2X" : "%02X",
                            (int)(unsigned int)(unsigned char)ifr.ifr_hwaddr.sa_data[j]);
                }
                p_addr[k+1] = '\0';
            }
        }
        else
           perror("_ansc_get_hw_addr IOCTL failure.");

        close(fd);
    }
    else
    {
        perror("_ansc_get_hw_addr failed to open socket.");
        return -1;
    }

    return 0;
}

