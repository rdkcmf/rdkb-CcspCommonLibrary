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

    module:	dslh_comho_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Com Helper Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/05/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_COMHO_INTERFACE_
#define  _DSLH_COMHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "dslh_properties.h"


/***********************************************************
         DSLH COM HELPER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHCOMHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCOMHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHCOMHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHCOMHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHCOMHO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  void
(*PFN_DSLHCOMHO_INIT)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

typedef  void
(*PFN_DSLHCOMHO_CLONE)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

typedef  void
(*PFN_DSLHCOMHO_CLONE2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext,
        BOOLEAN                     bCloneChildNodes
    );

typedef  void
(*PFN_DSLHCOMHO_CLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

typedef  void
(*PFN_DSLHCOMHO_CLEAN2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext,
        BOOLEAN                     bCleanChildNodes
    );

/*
 * As module grows bigger and the number of objects and data structures increase, it's no longer
 * practical to manage the allocation/release of data structures using MACROs. The Com Helper is
 * implemented to encapsulate such logic in sub-routines.
 */
#define  DSLH_COM_HELPER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    PFN_DSLHCOMHO_INIT              InitWanDslIfConfig;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanDslIfConfig;                                    \
    PFN_DSLHCOMHO_INIT              InitWanDslLinkConfig;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanWanDslLinkConfig;                                  \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitLanEthIfConfig;                                     \
    PFN_DSLHCOMHO_CLONE             CloneLanEthIfConfig;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanLanEthIfConfig;                                    \
    PFN_DSLHCOMHO_INIT              InitWanEthIfConfig;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanEthIfConfig;                                    \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitForwardingEntry;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanForwardingEntry;                                   \
    PFN_DSLHCOMHO_INIT              InitForwardingTable;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanForwardingTable;                                   \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitDeviceInfo;                                         \
    PFN_DSLHCOMHO_CLEAN             CleanDeviceInfo;                                        \
    PFN_DSLHCOMHO_INIT              InitVendorConfigFile;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVendorConfigFile;                                  \
    PFN_DSLHCOMHO_INIT              InitDeviceConfig;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanDeviceConfig;                                      \
    PFN_DSLHCOMHO_INIT              InitManagementServerInfo;                               \
    PFN_DSLHCOMHO_CLEAN             CleanManagementServerInfo;                              \
    PFN_DSLHCOMHO_INIT              InitTimeInfo;                                           \
    PFN_DSLHCOMHO_CLONE             CloneTimeInfo;                                          \
    PFN_DSLHCOMHO_CLEAN             CleanTimeInfo;                                          \
    PFN_DSLHCOMHO_INIT              InitUserInterfaceInfo;                                  \
    PFN_DSLHCOMHO_CLONE             CloneUserInterfaceInfo;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanUserInterfaceInfo;                                 \
    PFN_DSLHCOMHO_INIT              InitLanSecurityInfo;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanLanSecurityInfo;                                   \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitLanHostEntry;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanLanHostEntry;                                      \
    PFN_DSLHCOMHO_INIT              InitLanHostTable;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanLanHostTable;                                      \
    PFN_DSLHCOMHO_INIT              InitLanDeviceConfig;                                    \
    PFN_DSLHCOMHO_CLONE             CloneLanDeviceConfig;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanLanDeviceConfig;                                   \
    PFN_DSLHCOMHO_INIT              InitLanDhcpServingPool;                                 \
    PFN_DSLHCOMHO_CLONE             CloneLanDhcpServingPool;                                \
    PFN_DSLHCOMHO_CLEAN             CleanLanDhcpServingPool;                                \
    PFN_DSLHCOMHO_INIT              InitLanDeviceTable;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanLanDeviceTable;                                    \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitLanUsbIfConfig;                                     \
    PFN_DSLHCOMHO_CLONE             CloneLanUsbIfConfig;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanLanUsbIfConfig;                                    \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitWlanConfig;                                         \
    PFN_DSLHCOMHO_CLONE2            CloneWlanConfig;                                        \
    PFN_DSLHCOMHO_CLEAN2            CleanWlanConfig;                                        \
                                                                                            \
    PFN_DSLHCOMHO_CLEAN             CleanAssociatedDevice;                                  \
    PFN_DSLHCOMHO_CLEAN             CleanWepKey;                                            \
    PFN_DSLHCOMHO_CLEAN             CleanPreSharedKey;                                      \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitWanCommIfConfig;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanWanCommIfConfig;                                   \
    PFN_DSLHCOMHO_INIT              InitWanConnEntry;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanWanConnEntry;                                      \
    PFN_DSLHCOMHO_INIT              InitWanPortMapping;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanPortMapping;                                    \
    PFN_DSLHCOMHO_INIT              InitWanIpConnInfo;                                      \
    PFN_DSLHCOMHO_CLEAN             CleanWanIpConnInfo;                                     \
    PFN_DSLHCOMHO_INIT              InitWanPppConnInfo;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanPppConnInfo;                                    \
    PFN_DSLHCOMHO_INIT              InitWanVlanBridge;                                      \
    PFN_DSLHCOMHO_CLONE             CloneWanVlanBridge;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanVlanBridge;                                     \
    PFN_DSLHCOMHO_INIT              InitWanConnDevice;                                      \
    PFN_DSLHCOMHO_CLEAN             CleanWanConnDevice;                                     \
    PFN_DSLHCOMHO_INIT              InitWanDeviceEntry;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanDeviceEntry;                                    \
    PFN_DSLHCOMHO_INIT              InitWanDeviceTable;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanWanDeviceTable;                                    \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitLayer2BridgingPolicy;                               \
    PFN_DSLHCOMHO_CLEAN             CleanLayer2BridgingPolicy;                              \
    PFN_DSLHCOMHO_INIT              InitLayer2Bridge;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanLayer2Bridge;                                      \
    PFN_DSLHCOMHO_INIT              InitLayer2BridgeFilter;                                 \
    PFN_DSLHCOMHO_CLONE             CloneLayer2BridgeFilter;                                \
    PFN_DSLHCOMHO_CLEAN             CleanLayer2BridgeFilter;                                \
    PFN_DSLHCOMHO_INIT              InitLayer2BridgeMarking;                                \
    PFN_DSLHCOMHO_CLONE             CloneLayer2BridgeMarking;                               \
    PFN_DSLHCOMHO_CLEAN             CleanLayer2BridgeMarking;                               \
    PFN_DSLHCOMHO_INIT              InitLayer2BridgeInterface;                              \
    PFN_DSLHCOMHO_CLONE             CloneLayer2BridgeInterface;                             \
    PFN_DSLHCOMHO_CLEAN             CleanLayer2BridgeInterface;                             \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitQmPolicy;                                           \
    PFN_DSLHCOMHO_CLONE             CloneQmPolicy;                                          \
    PFN_DSLHCOMHO_CLEAN             CleanQmPolicy;                                          \
    PFN_DSLHCOMHO_INIT              InitQmClassification;                                   \
    PFN_DSLHCOMHO_CLONE             CloneQmClassification;                                  \
    PFN_DSLHCOMHO_CLEAN             CleanQmClassification;                                  \
    PFN_DSLHCOMHO_INIT              InitQmApp;                                              \
    PFN_DSLHCOMHO_CLEAN             CleanQmApp;                                             \
    PFN_DSLHCOMHO_INIT              InitQmFlow;                                             \
    PFN_DSLHCOMHO_CLONE             CloneQmFlow;                                            \
    PFN_DSLHCOMHO_CLEAN             CleanQmFlow;                                            \
    PFN_DSLHCOMHO_INIT              InitQmPolicer;                                          \
    PFN_DSLHCOMHO_CLONE             CloneQmPolicer;                                         \
    PFN_DSLHCOMHO_CLEAN             CleanQmPolicer;                                         \
    PFN_DSLHCOMHO_INIT              InitQmQueue;                                            \
    PFN_DSLHCOMHO_CLONE             CloneQmQueue;                                           \
    PFN_DSLHCOMHO_CLEAN             CleanQmQueue;                                           \
    PFN_DSLHCOMHO_INIT              InitQmRule;                                             \
    PFN_DSLHCOMHO_CLONE             CloneQmRule;                                            \
    PFN_DSLHCOMHO_CLEAN             CleanQmRule;                                            \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitIgmpPolicy;                                         \
    PFN_DSLHCOMHO_CLONE             CloneIgmpPolicy;                                        \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpPolicy;                                        \
    PFN_DSLHCOMHO_INIT              InitIgmpWanForwarding;                                  \
    PFN_DSLHCOMHO_CLONE             CloneIgmpWanForwarding;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpWanForwarding;                                 \
    PFN_DSLHCOMHO_INIT              InitIgmpWanFwdEntry;                                    \
    PFN_DSLHCOMHO_CLONE             CloneIgmpWanFwdEntry;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpWanFwdEntry;                                   \
    PFN_DSLHCOMHO_INIT              InitIgmpExcludedHost;                                   \
    PFN_DSLHCOMHO_CLONE             CloneIgmpExcludedHost;                                  \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpExcludedHost;                                  \
    PFN_DSLHCOMHO_INIT              InitIgmpGroup;                                          \
    PFN_DSLHCOMHO_CLONE             CloneIgmpGroup;                                         \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpGroup;                                         \
    PFN_DSLHCOMHO_INIT              InitIgmpGroupIf;                                        \
    PFN_DSLHCOMHO_CLONE             CloneIgmpGroupIf;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpGroupIf;                                       \
    PFN_DSLHCOMHO_INIT              InitIgmpGroupIfHost;                                    \
    PFN_DSLHCOMHO_CLONE             CloneIgmpGroupIfHost;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpGroupIfHost;                                   \
    PFN_DSLHCOMHO_INIT              InitIgmpGroupIfSource;                                  \
    PFN_DSLHCOMHO_CLONE             CloneIgmpGroupIfSource;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanIgmpGroupIfSource;                                 \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitDpPolicy;                                           \
    PFN_DSLHCOMHO_CLONE             CloneDpPolicy;                                          \
    PFN_DSLHCOMHO_CLEAN             CleanDpPolicy;                                          \
    PFN_DSLHCOMHO_INIT              InitDpBridge;                                           \
    PFN_DSLHCOMHO_CLONE             CloneDpBridge;                                          \
    PFN_DSLHCOMHO_CLEAN             CleanDpBridge;                                          \
    PFN_DSLHCOMHO_INIT              InitDpBridgedPort;                                      \
    PFN_DSLHCOMHO_CLONE             CloneDpBridgedPort;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanDpBridgedPort;                                     \
    PFN_DSLHCOMHO_INIT              InitDpLearnedHost;                                      \
    PFN_DSLHCOMHO_CLONE             CloneDpLearnedHost;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanDpLearnedHost;                                     \
    PFN_DSLHCOMHO_INIT              InitDpIgmpGroup;                                        \
    PFN_DSLHCOMHO_CLONE             CloneDpIgmpGroup;                                       \
    PFN_DSLHCOMHO_CLEAN             CleanDpIgmpGroup;                                       \
                                                                                            \
    PFN_DSLHCOMHO_INIT              InitVsCapabilities;                                     \
    PFN_DSLHCOMHO_CLONE             CloneVsCapabilities;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanVsCapabilities;                                    \
    PFN_DSLHCOMHO_INIT              InitVsCapabilitiesSip;                                  \
    PFN_DSLHCOMHO_CLONE             CloneVsCapabilitiesSip;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanVsCapabilitiesSip;                                 \
    PFN_DSLHCOMHO_INIT              InitVsCapabilitiesMgcp;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsCapabilitiesMgcp;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsCapabilitiesMgcp;                                \
    PFN_DSLHCOMHO_INIT              InitVsCapabilitiesH323;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsCapabilitiesH323;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsCapabilitiesH323;                                \
    PFN_DSLHCOMHO_INIT              InitVsCodec;                                            \
    PFN_DSLHCOMHO_CLONE             CloneVsCodec;                                           \
    PFN_DSLHCOMHO_CLEAN             CleanVsCodec;                                           \
    PFN_DSLHCOMHO_INIT              InitVsProfile;                                          \
    PFN_DSLHCOMHO_CLONE             CloneVsProfile;                                         \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfile;                                         \
    PFN_DSLHCOMHO_INIT              InitVsProfileSpinfo;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileSpinfo;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileSpinfo;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileSip;                                       \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileSip;                                      \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileSip;                                      \
    PFN_DSLHCOMHO_INIT              InitVsProfileSipEs;                                     \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileSipEs;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileSipEs;                                    \
    PFN_DSLHCOMHO_INIT              InitVsProfileSipRm;                                     \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileSipRm;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileSipRm;                                    \
    PFN_DSLHCOMHO_INIT              InitVsProfileMgcp;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileMgcp;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileMgcp;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileH323;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileH323;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileH323;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileRtp;                                       \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRtp;                                      \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRtp;                                      \
    PFN_DSLHCOMHO_INIT              InitVsProfileRtcp;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRtcp;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRtcp;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileSrtp;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileSrtp;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileSrtp;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileRtpRd;                                     \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRtpRd;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRtpRd;                                    \
    PFN_DSLHCOMHO_INIT              InitVsProfileNump;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileNump;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileNump;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileNumpPi;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileNumpPi;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileNumpPi;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileToneEvent;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileToneEvent;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileToneEvent;                                \
    PFN_DSLHCOMHO_INIT              InitVsProfileToneDescr;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileToneDescr;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileToneDescr;                                \
    PFN_DSLHCOMHO_INIT              InitVsProfileTonePattern;                               \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileTonePattern;                              \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileTonePattern;                              \
    PFN_DSLHCOMHO_INIT              InitVsProfileButton;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileButton;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileButton;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileFaxT38;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileFaxT38;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileFaxT38;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileLine;                                      \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLine;                                     \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLine;                                     \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineSip;                                   \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineSip;                                  \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineSip;                                  \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineSipEs;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineSipEs;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineSipEs;                                \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineMgcp;                                  \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineMgcp;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineMgcp;                                 \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineH323;                                  \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineH323;                                 \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineH323;                                 \
    PFN_DSLHCOMHO_INIT              InitVsProfileRingerEvent;                               \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRingerEvent;                              \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRingerEvent;                              \
    PFN_DSLHCOMHO_INIT              InitVsProfileRingerDescr;                               \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRingerDescr;                              \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRingerDescr;                              \
    PFN_DSLHCOMHO_INIT              InitVsProfileRingerPattern;                             \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileRingerPattern;                            \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileRingerPattern;                            \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineCf;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineCf;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineCf;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineVp;                                    \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineVp;                                   \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineVp;                                   \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineCodec;                                 \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineCodec;                                \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineCodec;                                \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineCodecEntry;                            \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineCodecEntry;                           \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineCodecEntry;                           \
    PFN_DSLHCOMHO_INIT              InitVsProfileLineSessionEntry;                          \
    PFN_DSLHCOMHO_CLONE             CloneVsProfileLineSessionEntry;                         \
    PFN_DSLHCOMHO_CLEAN             CleanVsProfileLineSessionEntry;                         \
    PFN_DSLHCOMHO_INIT              InitVsPhyInterface;                                     \
    PFN_DSLHCOMHO_CLONE             CloneVsPhyInterface;                                    \
    PFN_DSLHCOMHO_CLEAN             CleanVsPhyInterface;                                    \
    PFN_DSLHCOMHO_INIT              InitVsPhyIfTests;                                       \
    PFN_DSLHCOMHO_CLONE             CloneVsPhyIfTests;                                      \
    PFN_DSLHCOMHO_CLEAN             CleanVsPhyIfTests;                                      \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_COM_HELPER_OBJECT
{
    DSLH_COM_HELPER_CLASS_CONTENT
}
DSLH_COM_HELPER_OBJECT,  *PDSLH_COM_HELPER_OBJECT;

#define  ACCESS_DSLH_COM_HELPER_OBJECT(p)           \
         ACCESS_CONTAINER(p, DSLH_COM_HELPER_OBJECT, Linkage)


#endif
