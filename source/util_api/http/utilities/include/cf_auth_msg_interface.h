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


#ifndef  _CF_AUTH_MSG_CO_INTERFACE_DEC
#define  _CF_AUTH_MSG_CO_INTERFACE_DEC

#include "cf_bmco_co_interface.h"

/* =============================================================================
 *      Authentication Request object "CF_AR_MESSAGE_OBJECT"
 * ========================================================================== */

#define  CF_AR_MSG_COMPONENT_NAME                   "CfARMsgComponent"
#define  CF_AR_MSG_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_AR_MSG_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  CUR_AR_VERSION                             1
#define  CUR_AP_VERSION                             1

#define DEFAULT_FILTER_TIMEOUT                      10 

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_DEVICE_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_ACCOUNT_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_LICENSE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AR_GET_EDITION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_AR_GET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pYear,
        PULONG                      pMonth,
        PULONG                      pDay,
        PULONG                      pHour,
        PULONG                      pMinute,
        PULONG                      pSecond
    );

typedef
BOOL
(*PFN_CF_AR_IS_SG_REQUEST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AR_SET_DEVICE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 DeviceType,
        ANSC_HANDLE                 DeviceID
    );

typedef
void
(*PFN_CF_AR_SET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Password
    );

typedef
void
(*PFN_CF_AR_SET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Version
    );

typedef
void
(*PFN_CF_AR_SET_ACCOUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 AccountID,
        ANSC_HANDLE                 AccountPass
    );

typedef
void
(*PFN_CF_AR_SET_LICENSE_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 LicenseType
    );

typedef
void
(*PFN_CF_AR_SET_EDITION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Edition
    );

typedef
void
(*PFN_CF_AR_SET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Year,
        ULONG                       Month,
        ULONG                       Day,
        ULONG                       Hour,
        ULONG                       Minute,
        ULONG                       Second
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_AR_MESSAGE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    /* some strings */                                                                  \
    ANSC_HANDLE                     DeviceType;                                         \
    ANSC_HANDLE                     DeviceID;                                           \
    ANSC_HANDLE                     Password;                                           \
    ANSC_HANDLE                     ClientVersion;                                      \
    ANSC_HANDLE                     AccountID;                                          \
    ANSC_HANDLE                     AccountPassword;                                    \
    ANSC_HANDLE                     LicenseType;                                        \
    ANSC_HANDLE                     Edition;                                            \
    ANSC_HANDLE                     LastTime;                                           \
    /* methods */                                                                       \
    PFN_CF_AR_GET_DEVICE_TYPE       GetDeviceType;                                      \
    PFN_CF_AR_GET_DEVICE_ID         GetDeviceID;                                        \
    PFN_CF_AR_GET_PASSWORD          GetPassword;                                        \
    PFN_CF_AR_GET_CLIENT_VERSION    GetClientVersion;                                   \
    PFN_CF_AR_GET_ACCOUNT_ID        GetAccountID;                                       \
    PFN_CF_AR_GET_ACCOUNT_PASSWORD  GetAccountPassword;                                 \
    PFN_CF_AR_GET_LICENSE_TYPE      GetLicenseType;                                     \
    PFN_CF_AR_GET_EDITION           GetEdition;                                         \
    PFN_CF_AR_GET_LAST_TIME         GetLastTime;                                        \
    PFN_CF_AR_IS_SG_REQUEST         IsSGRequest;                                        \
    PFN_CF_AR_SET_DEVICE            SetDevice;                                          \
    PFN_CF_AR_SET_PASSWORD          SetPassword;                                        \
    PFN_CF_AR_SET_CLIENT_VERSION    SetClientVersion;                                   \
    PFN_CF_AR_SET_ACCOUNT           SetAccount;                                         \
    PFN_CF_AR_SET_LICENSE_TYPE      SetLicenseType;                                     \
    PFN_CF_AR_SET_EDITION           SetEdition;                                         \
    PFN_CF_AR_SET_LAST_TIME         SetLastTime;                                        \



/* content filtering basic message object  */
typedef  struct
_CF_AR_MESSAGE_OBJECT
{
    CF_AR_MESSAGE_CLASS_CONTENT
}
CF_AR_MESSAGE_OBJECT,    *PCF_AR_MESSAGE_OBJECT;


#define  ACCESS_CF_AR_MESSAGE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_AR_MESSAGE_OBJECT, Linkage)



/* =============================================================================
 *                  Pattern object
 * ========================================================================== */

#define  CF_PATTERN_COMPONENT_NAME                   "CfPatternComponent"
#define  CF_PATTERN_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_PATTERN_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL


typedef
ANSC_HANDLE
(*PFN_CF_PATTERN_GET_STRING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_PATTERN_SET_BLOCK)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulBlockCount
    );

typedef
void
(*PFN_CF_PATTERN_SET_TOTAL)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTotalCount
    );

typedef
ULONG
(*PFN_CF_PATTERN_GET_BLOCK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_PATTERN_GET_TOTAL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_PATTERN_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    /* some strings */                                                                  \
    ULONG                           Block;                                              \
    ULONG                           Total;                                              \
    /* methods */                                                                       \
    PFN_CF_PATTERN_GET_STRING       GetString;                                          \
    PFN_CF_PATTERN_SET_BLOCK        SetBlockCount;                                      \
    PFN_CF_PATTERN_SET_TOTAL        SetTotalCount;                                      \
    PFN_CF_PATTERN_GET_BLOCK        GetBlockCount;                                      \
    PFN_CF_PATTERN_GET_TOTAL        GetTotalCount;                                      \



/* content filtering basic message object  */
typedef  struct
_CF_PATTERN_OBJECT
{
    CF_PATTERN_CLASS_CONTENT
}
CF_PATTERN_OBJECT,    *PCF_PATTERN_OBJECT;


#define  ACCESS_CF_PATTERN_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_PATTERN_OBJECT, Linkage)


/* =============================================================================
 *          PC Content Filtering object
 * ========================================================================== */

#define  CF_PC_FILTER_COMPONENT_NAME                   "CfPCFilterComponent"
#define  CF_PC_FILTER_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_PC_FILTER_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

typedef
void
(*PFN_CF_PCF_ADD_P2P_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPort
    );

typedef
void
(*PFN_CF_PCF_SET_UNBLOCKED_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 UnblockList
    );

typedef
void
(*PFN_CF_PCF_ADD_PATTERN)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulBlockCount,
        ULONG                       ulTotalCount
    );

typedef
void
(*PFN_CF_PCF_SET_REPORTING)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReporting
    );

typedef
void
(*PFN_CF_PCF_SET_DNS_CHECKING)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bDnsChecking
    );

typedef
void
(*PFN_CF_PCF_SET_TIMEOUT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimeout
    );

typedef
void
(*PFN_CF_PCF_SET_BLOCKING_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBlockingPage
    );

typedef
void
(*PFN_CF_PCF_SET_WHITE_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWhiteList
    );

typedef
void
(*PFN_CF_PCF_SET_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hToken
    );

typedef
void
(*PFN_CF_PCF_SET_PASSWORDOVERRIDE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bPasswordOverride
    );

typedef
void
(*PFN_CF_PCF_SET_UPGRADE_URL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUpgradeUrl
    );

typedef
void
(*PFN_CF_PCF_SET_FAMILY_FILTERING)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFamilyFiltering
    );

typedef
void
(*PFN_CF_PCF_SET_DOMAIN_WHITELIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWhiteList
    );

typedef
void
(*PFN_CF_PCF_SET_DOMAIN_BLACKLIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBlackList
    );

typedef
BOOL
(*PFN_CF_PCF_HAS_REPORTING)
    (   
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCF_HAS_DNS_CHECKING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_PATTERN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_PCF_GET_TIMEOUT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_BLOCKING_PAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_P2P_PORT_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_WHITE_LIST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCF_GET_PASSWORDOVERRIDE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_UPGRADE_URL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_DOMAIN_WHITELIST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_GET_DOMAIN_BLACKLIST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCF_IS_PERSONALIZED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCF_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCF_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlDomNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_PC_FILTER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    /* some strings */                                                                  \
    BOOL                            bDnsChecking;                                       \
    BOOL                            bReporting;                                         \
    /* Actually a pointer with type "PCF_PATTERN_OBJECT" */                             \
    ANSC_HANDLE                     hPattern;                                           \
    ULONG                           Timeout;                                            \
    ANSC_HANDLE                     BlockPage;                                          \
    ANSC_HANDLE                     WhiteList;                                          \
    ANSC_HANDLE                     Token;                                              \
    BOOL                            bPasswordOverride;                                  \
    ANSC_HANDLE                     UpgradeURL;                                         \
    ANSC_HANDLE                     DomainWhiteList;                                    \
    ANSC_HANDLE                     DomainBlackList;                                    \
    BOOL                            bPersonalized;                                      \
    ANSC_HANDLE                     P2pPortArray;                                       \
    ANSC_HANDLE                     UnblockedPortArray;                                 \
    /* methods */                                                                       \
    PFN_CF_PCF_ADD_P2P_PORT         AddP2pPort;                                         \
    PFN_CF_PCF_SET_UNBLOCKED_PORT   SetUnblockedPort;                                   \
    PFN_CF_PCF_ADD_PATTERN          AddPattern;                                         \
    PFN_CF_PCF_SET_REPORTING        SetReporting;                                       \
    PFN_CF_PCF_SET_DNS_CHECKING     SetDnsChecking;                                     \
    PFN_CF_PCF_SET_TIMEOUT          SetTimeout;                                         \
    PFN_CF_PCF_SET_BLOCKING_PAGE    SetBlockingPage;                                    \
    PFN_CF_PCF_SET_WHITE_LIST       SetWhiteList;                                       \
    PFN_CF_PCF_SET_TOKEN            SetToken;                                           \
    PFN_CF_PCF_SET_PASSWORDOVERRIDE SetPasswordOverride;                                \
    PFN_CF_PCF_SET_UPGRADE_URL      SetUpgradeURL;                                      \
    PFN_CF_PCF_SET_FAMILY_FILTERING SetFamilyFiltering;                                 \
    PFN_CF_PCF_SET_DOMAIN_WHITELIST SetDomainWhiteList;                                 \
    PFN_CF_PCF_SET_DOMAIN_BLACKLIST SetDomainBlackList;                                 \
    PFN_CF_PCF_HAS_REPORTING        HasReporting;                                       \
    PFN_CF_PCF_HAS_DNS_CHECKING     HasDnsChecking;                                     \
    PFN_CF_PCF_GET_PATTERN          GetPattern;                                         \
    PFN_CF_PCF_GET_TIMEOUT          GetTimeout;                                         \
    PFN_CF_PCF_GET_BLOCKING_PAGE    GetBlockingPage;                                    \
    PFN_CF_PCF_GET_P2P_PORT_ARRAY   GetP2pPortArray;                                    \
    PFN_CF_PCF_GET_WHITE_LIST       GetWhiteList;                                       \
    PFN_CF_PCF_GET_TOKEN            GetToken;                                           \
    PFN_CF_PCF_GET_PASSWORDOVERRIDE GetPasswordOverride;                                \
    PFN_CF_PCF_GET_UPGRADE_URL      GetUpgradeURL;                                      \
    PFN_CF_PCF_GET_DOMAIN_WHITELIST GetDomainWhiteList;                                 \
    PFN_CF_PCF_GET_DOMAIN_BLACKLIST GetDomainBlackList;                                 \
    PFN_CF_PCF_IS_PERSONALIZED      IsPersonalized;                                     \
    PFN_CF_PCF_CREATE_XML_HANDLE    CreateXmlHandle;                                    \
    PFN_CF_PCF_PARSE_XML_NODE       ParseXmlNode;                                       \



/* content filtering basic message object  */
typedef  struct
_CF_PC_FILTER_OBJECT
{
    CF_PC_FILTER_CLASS_CONTENT
}
CF_PC_FILTER_OBJECT,    *PCF_PC_FILTER_OBJECT;


#define  ACCESS_CF_PC_FILTER_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_PC_FILTER_OBJECT, Linkage)


/* =============================================================================
 *          PC Firewall object
 * ========================================================================== */

#define  CF_PC_FIREWALL_COMPONENT_NAME              "CfPCFirewallComponent"
#define  CF_PC_FIREWALL_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  CF_PC_FIREWALL_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL


typedef
BOOL
(*PFN_CF_PCFW_HAS_APP_CONTROL)
    (   
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_PCFW_ADD_TCP_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPort
    );

typedef
void
(*PFN_CF_PCFW_ADD_UDP_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPort
    );

typedef
void
(*PFN_CF_PCFW_SET_APP_CONTROL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAppControl
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCFW_GET_TCP_PORT_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCFW_GET_UDP_PORT_ARRAY)
    (   
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCFW_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCFW_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlDomNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_PC_FIREWALL_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    BOOL                            bAppControl;                                        \
    ANSC_HANDLE                     TcpPortArray;                                       \
    ANSC_HANDLE                     UdpPortArray;                                       \
    /* methods */                                                                       \
    PFN_CF_PCFW_HAS_APP_CONTROL     HasAppControl;                                      \
    PFN_CF_PCFW_ADD_TCP_PORT        AddTcpPort;                                         \
    PFN_CF_PCFW_ADD_UDP_PORT        AddUdpPort;                                         \
    PFN_CF_PCFW_SET_APP_CONTROL     SetAppControl;                                      \
    PFN_CF_PCFW_GET_TCP_PORT_ARRAY  GetTcpPortArray;                                    \
    PFN_CF_PCFW_GET_UDP_PORT_ARRAY  GetUdpPortArray;                                    \
    PFN_CF_PCFW_CREATE_XML_HANDLE   CreateXmlHandle;                                    \
    PFN_CF_PCFW_PARSE_XML_NODE      ParseXmlNode;                                       \



/* content filtering basic message object  */
typedef  struct
_CF_PC_FIREWALL_OBJECT
{
    CF_PC_FIREWALL_CLASS_CONTENT
}
CF_PC_FIREWALL_OBJECT,    *PCF_PC_FIREWALL_OBJECT;


#define  ACCESS_CF_PC_FIREWALL_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_PC_FIREWALL_OBJECT, Linkage)


/* =============================================================================
 *          Changing Password Object
 * ========================================================================== */

#define  CF_CHANGE_PASS_COMPONENT_NAME              "CfChangePassComponent"
#define  CF_CHANGE_PASS_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  CF_CHANGE_PASS_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL


typedef
ANSC_HANDLE
(*PFN_CF_PCCP_GET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCCP_GET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_PCCP_SET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUserPass
    );

typedef
void
(*PFN_CF_PCCP_SET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAccountPass
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCCP_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCCP_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlDomNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_PC_CHANGE_PASS_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     UserPassword;                                       \
    ANSC_HANDLE                     AccountPassword;                                    \
    /* methods */                                                                       \
    PFN_CF_PCCP_GET_USER_PASS       GetUserPassword;                                    \
    PFN_CF_PCCP_GET_ACCOUNT_PASS    GetAccountPassword;                                 \
    PFN_CF_PCCP_SET_USER_PASS       SetUserPassword;                                    \
    PFN_CF_PCCP_SET_ACCOUNT_PASS    SetAccountPassword;                                 \
    PFN_CF_PCCP_CREATE_XML_HANDLE   CreateXmlHandle;                                    \
    PFN_CF_PCCP_PARSE_XML_NODE      ParseXmlNode;                                       \



/* content filtering basic message object  */
typedef  struct
_CF_PC_CHANGE_PASS_OBJECT
{
    CF_PC_CHANGE_PASS_CLASS_CONTENT
}
CF_PC_CHANGE_PASS_OBJECT,    *PCF_PC_CHANGE_PASS_OBJECT;


#define  ACCESS_CF_PC_CHANGE_PASS_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_PC_CHANGE_PASS_OBJECT, Linkage)



/* =============================================================================
 *          Upgrade Message Object
 * ========================================================================== */

#define  CF_UPGRADE_MSG_COMPONENT_NAME              "CfUpgradeMsgComponent"
#define  CF_UPGRADE_MSG_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  CF_UPGRADE_MSG_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL


typedef
ANSC_HANDLE
(*PFN_CF_PCUM_GET_URL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_PCUM_GET_OPTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_PCUM_SET_URL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUrl
    );

typedef
void
(*PFN_CF_PCUM_SET_OPTION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Option
    );

typedef
ANSC_HANDLE
(*PFN_CF_PCUM_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_PCUM_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlDomNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_PC_UPGRADE_MSG_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     Url;                                                \
    ULONG                           Option;                                             \
    /* methods */                                                                       \
    PFN_CF_PCUM_GET_URL             GetUrl;                                             \
    PFN_CF_PCUM_GET_OPTION          GetOption;                                          \
    PFN_CF_PCUM_SET_URL             SetUrl;                                             \
    PFN_CF_PCUM_SET_OPTION          SetOption;                                          \
    PFN_CF_PCUM_CREATE_XML_HANDLE   CreateXmlHandle;                                    \
    PFN_CF_PCUM_PARSE_XML_NODE      ParseXmlNode;                                      \



/* content filtering basic message object  */
typedef  struct
_CF_PC_UPGRADE_MSG_OBJECT
{
    CF_PC_UPGRADE_MSG_CLASS_CONTENT
}
CF_PC_UPGRADE_MSG_OBJECT,    *PCF_PC_UPGRADE_MSG_OBJECT;


#define  ACCESS_CF_PC_UPGRADE_MSG_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_PC_UPGRADE_MSG_OBJECT, Linkage)


/* =============================================================================
 *      Authentication Response object "CF_AP_MESSAGE_OBJECT"
 * ========================================================================== */


#define  CF_AP_MSG_COMPONENT_NAME                   "CfAPMsgComponent"
#define  CF_AP_MSG_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_AP_MSG_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_SERVER_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_AP_IS_OK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_FAILURE_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_PC_FIREWALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_PC_FILTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_SG_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_UPGRADE_MSG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_INFO_PAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_AP_GET_CHANGE_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_AP_PROFILE_NOT_CHANGED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AP_SET_SERVER_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerVersion
    );

typedef
void
(*PFN_CF_AP_SET_SUCCESS_STATUS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AP_SET_FAILURE_STATUS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFailureInfo
    );

typedef
void
(*PFN_CF_AP_SET_PC_FILTER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPCFilter
    );

typedef
void
(*PFN_CF_AP_SET_PC_FIREWALL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPCFirewall
    );

typedef
void
(*PFN_CF_AP_SET_CHANGE_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChangePass
    );

typedef
void
(*PFN_CF_AP_SET_PROFILE_UNCHANGED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AP_SET_SG_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSGHandle
    );

typedef
void
(*PFN_CF_AP_SET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Year,
        ULONG                       Month,
        ULONG                       Day,
        ULONG                       Hour,
        ULONG                       Minute,
        ULONG                       Second
    );

typedef
void
(*PFN_CF_AP_SET_UPGRADE_MSG)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUpgradeMsg
    );

typedef
void
(*PFN_CF_AP_SET_INFO_PAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfoPage
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_AP_MESSAGE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     ServerVersion;                                      \
    BOOL                            bStatus;                                            \
    ANSC_HANDLE                     FailureInfo;                                        \
    ANSC_HANDLE                     hPCFirewall;                                        \
    ANSC_HANDLE                     hPCFilter;                                          \
    ANSC_HANDLE                     hUpgradeMsg;                                        \
    BOOL                            bNotChanged;                                        \
    ANSC_HANDLE                     hSGHandle;                                          \
    ANSC_HANDLE                     LastTime;                                           \
    ANSC_HANDLE                     InfoPage;                                           \
    ANSC_HANDLE                     hChangePass;                                        \
    /* methods */                                                                       \
    PFN_CF_AP_GET_SERVER_VERSION    GetServerVersion;                                   \
    PFN_CF_AP_IS_OK                 IsOK;                                               \
    PFN_CF_AP_GET_FAILURE_INFO      GetFailureInfo;                                     \
    PFN_CF_AP_GET_PC_FIREWALL       GetPCFirewall;                                      \
    PFN_CF_AP_GET_PC_FILTER         GetPCFilter;                                        \
    PFN_CF_AP_GET_SG_HANDLE         GetSGHandle;                                        \
    PFN_CF_AP_GET_UPGRADE_MSG       GetUpgradeMsg;                                      \
    PFN_CF_AP_GET_INFO_PAGE         GetInfoPage;                                        \
    PFN_CF_AP_GET_CHANGE_PASS       GetChangePass;                                      \
    PFN_CF_AP_PROFILE_NOT_CHANGED   IsProfileNotChanged;                                \
    PFN_CF_AP_SET_SERVER_VERSION    SetServerVersion;                                   \
    PFN_CF_AP_SET_SUCCESS_STATUS    SetSuccessStatus;                                   \
    PFN_CF_AP_SET_FAILURE_STATUS    SetFailureStatus;                                   \
    PFN_CF_AP_SET_PC_FILTER         SetPCFilter;                                        \
    PFN_CF_AP_SET_PC_FIREWALL       SetPCFirewall;                                      \
    PFN_CF_AP_SET_CHANGE_PASS       SetChangePass;                                      \
    PFN_CF_AP_SET_PROFILE_UNCHANGED SetProfileUnchanged;                                \
    PFN_CF_AP_SET_SG_HANDLE         SetSGHandle;                                        \
    PFN_CF_AP_SET_LAST_TIME         SetLastTime;                                        \
    PFN_CF_AP_SET_UPGRADE_MSG       SetUpgradeMsg;                                      \
    PFN_CF_AP_SET_INFO_PAGE         SetInfoPage;                                        \



/* content filtering basic message object  */
typedef  struct
_CF_AP_MESSAGE_OBJECT
{
    CF_AP_MESSAGE_CLASS_CONTENT
}
CF_AP_MESSAGE_OBJECT,    *PCF_AP_MESSAGE_OBJECT;


#define  ACCESS_CF_AP_MESSAGE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_AP_MESSAGE_OBJECT, Linkage)



#endif

