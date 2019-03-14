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

    module:	dslh_properties.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to different Dslh objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_PROPERTIES_
#define  _DSLH_PROPERTIES_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "dslh_definitions.h"


/***********************************************************
         DSLH CPE DEVICE OBJECT PROPERTY DEFINITION
***********************************************************/

/*
 * The CPE WAN Management Protocol allows an ACS to provision a CPE or collection of CPE based on a
 * variety of criteria. The provisioning mechanism includes specific provisioning parameters and a
 * general mechanism for adding vendor-specific provisioning capabilities as needed. The ACS at any
 * time request that the CPE initiate a connection to the ACS using the Connection Request notifi-
 * cation mechanism. Support for this mechanism is REQUIRED in a CPE, and is RECOMMENDED in an ACS.
 */
typedef  struct
_DSLH_CPE_CONTROLLER_PROPERTY
{
    BOOL                            bStartCwmpRightAway;
}
DSLH_CPE_CONTROLLER_PROPERTY,  *PDSLH_CPE_CONTROLLER_PROPERTY;

/*
 * The CPE WAN Management Protocol defines the following mechanisms that may be used by a CPE to
 * discover the address of its associated ACS:
 *
 *      1. The CPE may be configured locally with the URL of the ACS.
 *      2. As part of the IP layer auto-configuration, a DHCP server on the access
 *         network may be configured to include the ACS URL as a DHCP option.
 *      3. The CPE may have a default ACS URL that it may use if no other URL is provided
 *         to it.
 *
 * The ACS URL MUST be in the form of a valid HTTP or HTTPS URL. Use of an HTTPS URL indicates that
 * the ACS supports SSL. If an HTTPS URL is given, and the CPE that does not support SSL, it MAY
 * attempt to use HTTP assuming the remainder of the URL is unchanged.
 */
typedef  struct
_DSLH_ACS_BROKER_PROPERTY
{
    ULONG                           Dummy;
}
DSLH_ACS_BROKER_PROPERTY,  *PDSLH_ACS_BROKER_PROPERTY;

/*
 * SOAP messages are carried between a CPE and an ACS using HTTP 1.1, where the CPE acts as the
 * HTTP client and the ACS acts as the HTTP server. The encoding of SOAP over HTTP extends the
 * basic HTTP profile for SOAP, as follows:
 *
 *      - A SOAP request from an ACS to a CPE is sent over a HTTP response, while the
 *        CPE's SOAP response to an ACS request is sent over a subsequent HTTP post.
 *      - Each HTTP post or response may contain more than one SOAP envelope (within the
 *        negotiated limits). Each envelope may contain a SOAP request or response,
 *        independent from any other envelope.
 *      - When there is more than one envelope in a single HTTP Request, the SOAPAction
 *        header in the HTTP Request MUST have no value (with no quotes), indicating that
 *        this header provides no information as to the intent of the message.
 */
typedef  struct
_DSLH_WMP_PROCESSOR_PROPERTY
{
    BOOL                            bAcquiredFromDhcp;
    BOOL                            bInitialContact;
    char                            AcsUrl     [DSLH_MAX_URL_SIZE     ];
    char                            AcsUsername[DSLH_MAX_USERNAME_SIZE];
    char                            AcsPassword[DSLH_MAX_PASSWORD_SIZE];
    BOOL                            bPeriodicInformEnabled;
    ULONG                           PeriodicInformInterval;
    ANSC_UNIVERSAL_TIME             PeriodicInformTime;
    ULONG                           FirstTimeInterval;
    ULONG                           FollowingInterval;
    BOOL                            bEnableCWMP;
    BOOL                            bEnableTR64;
    char                            ConnReqPath[DSLH_MAX_URL_SIZE];
    USHORT                          ConnReqPort;
    USHORT                          AcsPort;
}
DSLH_WMP_PROCESSOR_PROPERTY,  *PDSLH_WMP_PROCESSOR_PROPERTY;

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */

/*
 * For the plugfest test, the 10 seconds timer might be too long,
 * we change it to 4 second instead
 */
#define  DSLH_DEF_MONITOR_TIMER_INTERVAL         15      /* in seconds */

typedef  struct
_DSLH_WMP_DATABASE_PROPERTY
{
    BOOL                            bMonitorEnabled;
    ULONG                           MonitorTimerInterval;
}
DSLH_WMP_DATABASE_PROPERTY,  *PDSLH_WMP_DATABASE_PROPERTY;

/*
 * If the CPE is instructed to perform a file transfer via the Download or Upload request from the
 * ACS, and if the file location is specified as an HTTP URL with the same host name as the ACS,
 * then the CPE MAY choose any of the following approaches in performing the transfer:
 *
 *      - The CPE MAY send the HTTP get/post over the already established connection.
 *        Once the file has been transferred, the CPE MAY then proceed in sending addi-
 *        tional messages to the ACS while continuing to maintain the connection.
 *      - The CPE MAY open a second connection over which to transfer the file, while
 *        maintaining the session to the ACS over which it may continue to send messages.
 *      - The CPE MAY terminate the session tot he ACS and then perform the transfer.
 *
 * If the file location is not an HTTP URL or is not in the same domain as the ACS, then only the
 * latter two options are available to it.
 */
typedef  struct
_DSLH_FILE_MANAGER_PROPERTY
{
    ULONG                           Dummy;
}
DSLH_FILE_MANAGER_PROPERTY,  *PDSLH_FILE_MANAGER_PROPERTY;

/*
 * UPnP technology defines an architecture for pervasive peer-to-peer network connectivity of
 * intelligent applicances, wireless devices, and PCs of all form factors. It is designed to bring
 * easy-to-use, flexible, standards-based connectivity to ad-hoc or unmanaged networks whether in
 * the home, in a small business, public spaces, or attached to the Internet. UPnP technology
 * provides a distributed, open networking architecture that leverages TCP/IP and the Web tech-
 * nologies to enable seamless proximity networking in addition to control and data transfer among
 * networked devices.
 */
typedef  struct
_DSLH_UPNP_MANAGER_PROPERTY
{
    ULONG                           Dummy;
}
DSLH_UPNP_MANAGER_PROPERTY,  *PDSLH_UPNP_MANAGER_PROPERTY;

/*
 * STUN is a lightweight protocol that allows applications to discover the presence and types of
 * NATs and firewalls between them and the public Internet. It also provides the ability for
 * applications to determine the public Internet Protocol addresses allocated to them by NAT. STUN
 * works with many existing NATs, and does not require any special behavior from them. As a result,
 * it allows a wide variety of applications to work through existing NAT infrastructure.
 */
typedef  struct
_DSLH_STUN_MANAGER_PROPERTY
{
    ANSC_IPV4_ADDRESS               ClientAddr;
    USHORT                          ClientPort;
}
DSLH_STUN_MANAGER_PROPERTY,  *PDSLH_STUN_MANAGER_PROPERTY;


#endif
