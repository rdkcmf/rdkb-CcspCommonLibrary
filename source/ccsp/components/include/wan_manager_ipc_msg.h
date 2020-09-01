/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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
#ifndef _WAN_MANAGER_IPC_MSG_H_
#define _WAN_MANAGER_IPC_MSG_H_
#include <stdbool.h> 
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#define WAN_MANAGER_ADDR               "tcp://127.0.0.1:50321"

#define BUFLEN_4                         4
#define BUFLEN_8                         8
#define BUFLEN_16                        16
#define BUFLEN_18                        18
#define BUFLEN_24                        24
#define BUFLEN_32                        32
#define BUFLEN_40                        40
#define BUFLEN_48                        48
#define BUFLEN_64                        64
#define BUFLEN_80                        80
#define BUFLEN_128                       128
#define BUFLEN_256                       256
#define BUFLEN_264                       264
#define BUFLEN_512                       512
#define BUFLEN_1024                      1024
#define IP_ADDR_LENGTH                   46
#define IFNAME_LENGTH                    BUFLEN_32
#define MAX_FULLPATH_LENGTH              1024
#define AFTR_NAME_LENGTH                 256

typedef enum
{
    DHCPC_STATE_CHANGED = 1,
    DHCP6C_STATE_CHANGED,
}msg_type_t;

typedef struct _dhcpv4_data_t
{
    bool addressAssigned;              /** Have we been assigned an IP address ? */
    bool isExpired;                    /** Is the lease time expired ? */
    char ip[BUFLEN_32];                /** New IP address, if addressAssigned==TRUE */
    char mask[BUFLEN_32];              /** New netmask, if addressAssigned==TRUE */
    char gateway[BUFLEN_32];           /** New gateway, if addressAssigned==TRUE */
    char dnsServer[BUFLEN_64];         /** New dns Server, if addressAssigned==TRUE */
    char dnsServer1[BUFLEN_64];        /** New dns Server, if addressAssigned==TRUE */
    char dhcpcInterface[BUFLEN_64];    /** Dhcp interface name */
    uint32_t leaseTime;                /** Lease time, , if addressAssigned==TRUE */
    uint32_t rebindingTime;            /** Rebinding time, if addressAssigned==TRUE */
    uint32_t renewalTime;              /** Renewal Time, if addressAssigned==TRUE */
    int32_t timeOffset;                /** New time offset, if addressAssigned==TRUE */
    bool isTimeOffsetAssigned;         /** Is the time offset assigned ? */
    char timeZone[BUFLEN_64];          /** New time zone, if addressAssigned==TRUE */
    uint32_t upstreamCurrRate;         /** Upstream rate */
    uint32_t downstreamCurrRate;       /** Downstream rate */
    char dhcpServerId[BUFLEN_64];      /** Dhcp server id */
    char dhcpState[BUFLEN_64];         /** Dhcp state. */
} dhcpv4_data_t;

typedef struct _dhcpv6_data_t
{
   bool prefixAssigned;  /** Have we been assigned a site prefix ? */
   bool addrAssigned;    /** Have we been assigned an IPv6 address ? */
   bool dnsAssigned;     /** Have we been assigned dns server addresses ? */
   bool domainNameAssigned;     /** Have we been assigned dns server addresses ? */
   bool aftrAssigned;     /** Have we been assigned aftr name ? */
   bool mapeAssigned;     /** Have we been assigned mape config ? */
   bool maptAssigned;     /** Have we been assigned mapt config ? */
   bool isExpired;        /** Is the lease time expired ? */
   char sitePrefix[BUFLEN_48];   /** New site prefix, if prefixAssigned==TRUE */
   uint32_t prefixPltime;
   uint32_t prefixVltime;
   char sitePrefixOld[BUFLEN_48]; /** add support for RFC7084 requirement L-13 */
   uint32_t prefixVltimeOld;
   uint32_t prefixCmd;
   char ifname[BUFLEN_32];
   char address[BUFLEN_48];      /** New IPv6 address, if addrAssigned==TRUE */
   uint32_t addressPltime;
   uint32_t addressVltime;
   char pdIfAddress[BUFLEN_48];      /** New IPv6 address of PD interface */
   uint32_t addrCmd;
   char nameserver[BUFLEN_128];  /** New nameserver, if addressAssigned==TRUE */
   char nameserver1[BUFLEN_128];  /** New nameserver, if addressAssigned==TRUE */
   char domainName[BUFLEN_64];  /** New domain Name, if addressAssigned==TRUE */
   char ntpserver[BUFLEN_128];  /** New ntp server(s), dhcp server may provide this */
   char aftr[AFTR_NAME_LENGTH];      /** dhcp server may provide this */
#ifdef FEATURE_MAPT
   char brIPv6Prefix[IP_ADDR_LENGTH];
   char ruleIPv4Prefix[IP_ADDR_LENGTH];
   char ruleIPv6Prefix[IP_ADDR_LENGTH];
   uint32_t eaLen;
   uint32_t psidOffset;
   uint32_t psidLen;
   uint32_t psid;
   bool isFMR;
#endif
} dhcpv6_data_t;

typedef struct _msg_payload_t
{
    msg_type_t msg_type;
    union {
        dhcpv4_data_t dhcpv4;
        dhcpv6_data_t dhcpv6;
    }data;
} msg_payload_t;
#endif
