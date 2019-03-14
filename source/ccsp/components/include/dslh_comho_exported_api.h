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

    module:	dslh_comho_exported_api.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Dslh Com Helper
        Object.

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


#ifndef  _DSLH_COMHO_EXPORTED_API_
#define  _DSLH_COMHO_EXPORTED_API_


/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_COMHO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
DslhCreateComHelper
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN DSLH_COMHO_BASE.C
***********************************************************/

ANSC_HANDLE
DslhComhoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
DslhComhoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhComhoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
DslhComhoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN DSLH_COMHO_DSL.C
***********************************************************/

void
DslhComhoInitWanDslIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanDslIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanDslLinkConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanDslLinkConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_COMHO_ETHERNET.C
***********************************************************/

void
DslhComhoInitLanEthIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLanEthIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLanEthIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanEthIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanEthIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN DSLH_COMHO_FORWARDING.C
***********************************************************/

void
DslhComhoInitForwardingEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanForwardingEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitForwardingTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanForwardingTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_COMHO_GENERAL.C
***********************************************************/

void
DslhComhoInitDeviceInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanDeviceInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVendorConfigFile
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanVendorConfigFile
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitDeviceConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanDeviceConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitManagementServerInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanManagementServerInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitTimeInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneTimeInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanTimeInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitUserInterfaceInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneUserInterfaceInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanUserInterfaceInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLanSecurityInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLanSecurityInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_COMHO_LANDEVICE.C
***********************************************************/

void
DslhComhoInitLanHostEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLanHostEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLanHostTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLanHostTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLanDeviceConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLanDeviceConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLanDeviceConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLanDhcpServingPool
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLanDhcpServingPool
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLanDhcpServingPool
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLanDeviceTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLanDeviceTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN DSLH_COMHO_USB.C
***********************************************************/

void
DslhComhoInitLanUsbIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_COMHO_WANDEVICE.C
***********************************************************/

void
DslhComhoInitWanCommIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanCommIfConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanConnEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanConnEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanPortMapping
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanPortMapping
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanIpConnInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanIpConnInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanPppConnInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanPppConnInfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanVlanBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneWanVlanBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanWanVlanBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanConnDevice
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanConnDevice
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanDeviceEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanDeviceEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitWanDeviceTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWanDeviceTable
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN DSLH_COMHO_WLAN.C
***********************************************************/

void
DslhComhoInitWlanConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneWlanConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext,
        BOOLEAN                     bCloneChildNodes
    );

void
DslhComhoCleanWlanConfig
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext,
        BOOLEAN                     bCleanChildNodes
    );

void
DslhComhoCleanAssociatedDevice
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanWepKey
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanPreSharedKey
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );



/***********************************************************
       FUNCTIONS IMPLEMENTED IN DSLH_COMHO_BRIDGING.C
***********************************************************/

void
DslhComhoInitLayer2BridgingPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLayer2BridgingPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLayer2Bridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanLayer2Bridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLayer2BridgeFilter
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLayer2BridgeFilter
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLayer2BridgeFilter
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLayer2BridgeMarking
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLayer2BridgeMarking
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLayer2BridgeMarking
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitLayer2BridgeInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneLayer2BridgeInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanLayer2BridgeInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_COMHO_QUEUING.C
***********************************************************/

void
DslhComhoInitQmPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmClassification
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmClassification
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmClassification
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmApp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCleanQmApp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmFlow
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmFlow
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmFlow
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmPolicer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmPolicer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmPolicer
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmQueue
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmQueue
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmQueue
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitQmRule
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneQmRule
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanQmRule
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN DSLH_COMHO_IGMP.C
***********************************************************/

void
DslhComhoInitIgmpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpWanForwarding
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpWanForwarding
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpWanForwarding
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpWanFwdEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpWanFwdEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpWanFwdEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpExcludedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpExcludedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpExcludedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpGroupIf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpGroupIf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpGroupIf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpGroupIfHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpGroupIfHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpGroupIfHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitIgmpGroupIfSource
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneIgmpGroupIfSource
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanIgmpGroupIfSource
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN DSLH_COMHO_DATAPATH.C
***********************************************************/

void
DslhComhoInitDpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneDpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanDpPolicy
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitDpBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneDpBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanDpBridge
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitDpBridgedPort
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneDpBridgedPort
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanDpBridgedPort
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitDpLearnedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneDpLearnedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanDpLearnedHost
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitDpIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneDpIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanDpIgmpGroup
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN DSLH_COMHO_VOICE.C
***********************************************************/

void
DslhComhoInitVsCapabilities
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsCapabilities
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsCapabilities
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsCapabilitiesSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsCapabilitiesSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsCapabilitiesSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsCapabilitiesMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsCapabilitiesMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsCapabilitiesMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsCapabilitiesH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsCapabilitiesH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsCapabilitiesH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfile
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfile
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfile
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileSpinfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileSpinfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileSpinfo
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileSipRm
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileSipRm
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileSipRm
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRtcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRtcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRtcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileSrtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileSrtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileSrtp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRtpRd
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRtpRd
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRtpRd
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileNump
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileNump
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileNump
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileNumpPi
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileNumpPi
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileNumpPi
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileToneEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileToneEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileToneEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileToneDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileToneDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileToneDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileTonePattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileTonePattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileTonePattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileButton
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileButton
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileButton
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileFaxT38
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileFaxT38
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileFaxT38
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLine
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLine
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLine
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineSip
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineSipEs
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineMgcp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineH323
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRingerEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRingerEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRingerEvent
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRingerDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRingerDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRingerDescr
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileRingerPattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileRingerPattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileRingerPattern
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineCf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineCf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineCf
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineVp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineVp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineVp
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineCodec
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineCodecEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineCodecEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineCodecEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsProfileLineSessionEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsProfileLineSessionEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsProfileLineSessionEntry
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsPhyInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsPhyInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsPhyInterface
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoInitVsPhyIfTests
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );

void
DslhComhoCloneVsPhyIfTests
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcContext,
        void*                       pDstContext
    );

void
DslhComhoCleanVsPhyIfTests
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pContext
    );


#endif
