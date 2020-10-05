#ifndef _WAN_MANAGER_IPC_MSG_H_
#define _WAN_MANAGER_IPC_MSG_H_
#include <stdbool.h> 
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#define WAN_MANAGER_ADDR               "tcp://127.0.0.1:50321"
#define PPP_MANAGER_ADDR               "tcp://127.0.0.1:50322"
#define IHC_IPC_ADDR               "tcp://127.0.0.1:50323"

#define BUFLEN_4                         4           //!< buffer length 4
#define BUFLEN_8                         8           //!< buffer length 8
#define BUFLEN_16                        16          //!< buffer length 16
#define BUFLEN_18                        18          //!< buffer length 18 
#define BUFLEN_24                        24          //!< buffer length 24 
#define BUFLEN_32                        32          //!< buffer length 32
#define BUFLEN_40                        40          //!< buffer length 40
#define BUFLEN_48                        48          //!< buffer length 48
#define BUFLEN_64                        64          //!< buffer length 64
#define BUFLEN_80                        80          //!< buffer length 80
#define BUFLEN_128                       128         //!< buffer length 128
#define BUFLEN_256                       256         //!< buffer length 256
#define BUFLEN_264                       264         //!< buffer length 264
#define BUFLEN_512                       512         //!< buffer length 512
#define BUFLEN_1024                      1024        //!< buffer length 1024
#define IP_ADDR_LENGTH                   46          //!< IP address length
#define IFNAME_LENGTH                    BUFLEN_32
#define MAX_FULLPATH_LENGTH              1024
#define AFTR_NAME_LENGTH                 256

//LCP States
typedef enum _PPP_STATES
{
    PPP_INTERFACE_UP = 1,
    PPP_INTERFACE_DOWN,
    PPP_INTERFACE_UNCONFIGURED,
    PPP_INTERFACE_CONNECTING,
    PPP_INTERFACE_AUTHENTICATING,
    PPP_INTERFACE_PENDING_DISCONNET,
    PPP_INTERFACE_DISCONNECTING,
    PPP_INTERFACE_DISCONNECTED,
    PPP_INTERFACE_LCP_ECHO_FAILED,
    PPP_INTERFACE_AUTH_FAILED,
    PPP_IPCP_COMPLETED,
    PPP_IPCP_FAILED,
    PPP_IPV6CP_COMPLETED,
    PPP_IPV6CP_FAILED,
    PPP_MAX_STATE
}PPP_STATES;

typedef enum
{
    DHCPC_STATE_CHANGED = 1,
    DHCP6C_STATE_CHANGED,
    IPC_MSG_PPP_STATE_CHANGE,
    IHC_STATE_CHANGE,
}ipc_msg_type_t;

typedef struct _ipc_ppp_ipcp_msg_t
{
    char ip[BUFLEN_32];
    char mask[BUFLEN_32];
    char gateway[BUFLEN_32];
    char nameserver[BUFLEN_64];

}ipc_ppp_ipcp_msg_t;

typedef struct _ipc_ppp_ipv6cp_msg_t
{
    char localIntfId[IP_ADDR_LENGTH];
    char remoteIntfId[IP_ADDR_LENGTH];

}ipc_ppp_ipv6cp_msg_t;

typedef struct _ipc_ppp_event_msg_t
{
    pid_t pid;
    PPP_STATES pppState; //lcp state
    union
    {
        ipc_ppp_ipcp_msg_t pppIpcpMsg; //ncp state
        ipc_ppp_ipv6cp_msg_t pppIpv6cpMsg; // ncp v6 state
    }event;

}ipc_ppp_event_msg_t;

typedef struct _ipc_dhcpv4_data_t
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
} ipc_dhcpv4_data_t;

typedef struct _ipc_dhcpv6_data_t
{
   bool prefixAssigned;  /**< Have we been assigned a site prefix ? */
   bool addrAssigned;    /**< Have we been assigned an IPv6 address ? */
   bool dnsAssigned;     /**< Have we been assigned dns server addresses ? */
   bool domainNameAssigned;     /**< Have we been assigned dns server addresses ? */
   bool aftrAssigned;     /**< Have we been assigned aftr name ? */
   bool mapeAssigned;     /**< Have we been assigned mape config ? */
   bool maptAssigned;     /**< Have we been assigned mapt config ? */
   bool isExpired;        /**< Is the lease time expired ? */
   char sitePrefix[BUFLEN_48];   /**< New site prefix, if prefixAssigned==TRUE */
   uint32_t prefixPltime;
   uint32_t prefixVltime;
   char sitePrefixOld[BUFLEN_48]; /**< add support for RFC7084 requirement L-13 */
   uint32_t prefixVltimeOld;
   uint32_t prefixCmd;
   char ifname[BUFLEN_32];
   char address[BUFLEN_48];      /**< New IPv6 address, if addrAssigned==TRUE */
   uint32_t addressPltime;
   uint32_t addressVltime;
   char pdIfAddress[BUFLEN_48];      /**< New IPv6 address of PD interface */
   uint32_t addrCmd;
   char nameserver[BUFLEN_128];  /**< New nameserver, if addressAssigned==TRUE */
   char nameserver1[BUFLEN_128];  /**< New nameserver, if addressAssigned==TRUE */
   char domainName[BUFLEN_64];  /**< New domain Name, if addressAssigned==TRUE */
   char ntpserver[BUFLEN_128];  /**< New ntp server(s), dhcp server may provide this */
   char aftr[AFTR_NAME_LENGTH];      /**< dhcp server may provide this */
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
} ipc_dhcpv6_data_t;

/* IPoE definitions */

typedef enum
{
    IPOE_MSG_WAN_CONNECTION_UP = 1,
    IPOE_MSG_WAN_CONNECTION_DOWN,
    IPOE_MSG_WAN_CONNECTION_IPV6_UP,
    IPOE_MSG_WAN_CONNECTION_IPV6_DOWN,
    IPOE_MSG_IHC_ECHO_FAIL_IPV4,
    IPOE_MSG_IHC_ECHO_FAIL_IPV6,
    IPOE_MSG_IHC_ECHO_RENEW_IPV4,
    IPOE_MSG_IHC_ECHO_RENEW_IPV6,
    IPOE_MSG_IHC_ECHO_IPV4_UP,
    IPOE_MSG_IHC_ECHO_IPV6_UP,
}ipoe_msg_type_t;

typedef struct
{
   ipoe_msg_type_t msgType;
   char ifName[IFNAME_LENGTH];
   char ipv4Address[IP_ADDR_LENGTH];
   char ipv6Address[BUFLEN_48];
} ipc_ihc_data_t;

typedef struct _ipc_msg_payload_t
{
    ipc_msg_type_t msg_type;
    union {
        ipc_dhcpv4_data_t dhcpv4;
        ipc_dhcpv6_data_t dhcpv6;
        ipc_ppp_event_msg_t pppEventMsg;
	ipc_ihc_data_t ihcData;
    }data;
} ipc_msg_payload_t;
#endif
