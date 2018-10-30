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

#ifndef _XML_NODE_NAME_H
#define _XML_NODE_NAME_H

#ifndef  BASED_CODE
#define  BASED_CODE
#endif

/* 
 * Definitions for XML  format
 */
static char BASED_CODE szXML_HEADER[]        = "<?xml version=\"1.0\"  encoding=\"UTF-8\" ?>";
static char BASED_CODE szXML_NAMESPACE[]     = "http://www.w3.org/XML/1998/namespace";

/*
 * default values
 */
#define    DEVICE_PC                            "PC"
#define    DEVICE_SG                            "SG"
#define    DEVICE_PDA                           "PDA"

#define    EDITION_WIN95                        "Win95"
#define    EDITION_WIN98                        "Win98"
#define    EDITION_WIN2000                      "Win2000"
#define    EDITION_WINNT                        "WinNT"
#define    EDITION_MAC                          "Mac"
#define    EDITION_LINUX                        "Linux"
#define    EDITION_UNIX                         "Unix"
#define    EDITION_VXWORKS                      "Vxworks"

#define    LICENSE_BUSINESS                     "Business"
#define    LICENSE_RESIDENTIAL                  "Residential"

#define    STATUS_INVALID_PASS                  "Wrong password"
#define    STATUS_NO_SUCH_ACCOUNT               "No such account"
#define    STATUS_NO_SUCH_USER                  "No such user"
#define    STATUS_UNSUPPORT_VERSION             "Unsupported version"
#define    STATUS_FAILURE                       "Failure"

/*
 *  Normal Node Definitions
 */
static char BASED_CODE szALMessage[]           ="ALMessage";
static char BASED_CODE szMessageVersion[]      ="version";
static char BASED_CODE szProtocolVersion[]     ="version";
static char BASED_CODE szAccountName[]         ="account";
static char BASED_CODE szAccountPass[]         ="accountKey";
static char BASED_CODE szUserName[]            ="user";
static char BASED_CODE szUserPass[]            ="userKey";
static char BASED_CODE szDomain[]              ="domain";
static char BASED_CODE szPage[]                ="page";
static char BASED_CODE szClientVersion[]       ="clientVersion";
static char BASED_CODE szServerVersion[]       ="serverVersion";
static char BASED_CODE szDeviceType[]          ="deviceType";
static char BASED_CODE szDeviceID[]            ="deviceID";
static char BASED_CODE szLicenseType[]         ="licenseType";
static char BASED_CODE szEdition[]             ="edition";
static char BASED_CODE szTimeout[]             ="timeout";
static char BASED_CODE szLastTime[]            ="lasttime";


/*
 *  Node Definitions for AR message
 */
static char BASED_CODE szAR[]                  ="ar";

/*
 *  Sample AR message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *  <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *    <ar version="1">
 *        <account>sample</account>
 *        <accountKey>556676349838492750948497393847</accountKey>
 *        <user>Jim Hush</user>
 *        <userKey>556676349838492750948497393847</userKey>
 *        <clientVersion>1.0</clientVersion>
 *        <deviceType>PC</deviceType>
 *        <deviceID>pc2</deviceID>
 *        <licenseType>Business</licenseType>
 *        <edition>Win98</edition>
 *        <lasttime>2001/07/02 12:34:19</lasttime>  
 *    </ar> 
 *  </ALMessage>
 */

/*
 *  Node Definitions for AP message
 */
static char BASED_CODE szAP[]                  ="ap";
static char BASED_CODE szAPStatus[]            ="status";
static char BASED_CODE szAPFailInfo[]          ="failureInfo";
static char BASED_CODE szAPDNSChecking[]       ="dnsChecking";
static char BASED_CODE szAPPtoPPorts[]         ="peerToPeerPorts";
static char BASED_CODE szAPTCPPorts[]          ="tcpPorts";
static char BASED_CODE szAPUDPPorts[]          ="udpPorts";
static char BASED_CODE szAPPort[]              ="port";
static char BASED_CODE szAPFirewall[]          ="firewall";
static char BASED_CODE szAPAppControl[]        ="appControl";
static char BASED_CODE szAPContentFilter[]     ="contentFilter";
static char BASED_CODE szAPWhiteList[]         ="whiteList";
static char BASED_CODE szAPDefaultWhiteList[]  ="defaultWhiteList";
static char BASED_CODE szAPToken[]             ="token";
static char BASED_CODE szAPPassOverride[]      ="passwordOverride";
static char BASED_CODE szAPReporting[]         ="reporting";
static char BASED_CODE szAPEmail[]             ="email";
static char BASED_CODE szAPPattern[]           ="pattern";
static char BASED_CODE szAPPatternBlock[]      ="block";
static char BASED_CODE szAPPatternTotal[]      ="total";
static char BASED_CODE szAPPersonalized[]      ="isPersonal";

/* the next 2 are the same, but different places */
static char BASED_CODE szAPRedirect[]          ="redirect";
static char BASED_CODE szAPBlockURL[]          ="blockURL";

static char BASED_CODE szAPAdvanced[]          ="advanced";

static char BASED_CODE szAPServices[]          ="pcservices";
static char BASED_CODE szAPGateway[]           ="sgServices";
/*
 *  zhubin modified here on 08/06/2001
 *
 *  Add new field "accountpass" and "userpass" in the AP message when user 
 *  changed it's password.
 */
static char BASED_CODE szAPPassChanged[]          ="passchanged";
static char BASED_CODE szAPAccountPass[]          ="account";
static char BASED_CODE szAPUserPass[]             ="user";

/*
 *  zhubin added here on 08/27/2001
 */
static char BASED_CODE szAPTimeStamp[]            ="time";
static char BASED_CODE szAPUnchanged[]            ="unchanged";
static char BASED_CODE szAPInfoPage[]             ="infoPage";

/*
 *  zhubin added here on 08/30/2001
 */
static char BASED_CODE szAPUpURL[]                ="upgradeURL";
static char BASED_CODE szAPUpgrade[]              ="upgrade";
static char BASED_CODE szAPUpgradeURL[]           ="url";
static char BASED_CODE szAPUpgradeOpt[]           ="option";

/*
 * New fields are added on 09/26/2001 for white/black domain lists
 */
static char BASED_CODE szAPDomainWhiteList[]      ="domainWhiteList";
static char BASED_CODE szAPDomainBlackList[]      ="domainBlackList";
static char BASED_CODE szAPUnblockedPorts[]       ="unblockedPorts";
/*
 *  Failed AP message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *       <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>false</status>
 *         <failureInfo>No such user</failureInfo>
 *       </ap> 
 *   </ALMessage> 
 *
 *  Succeeded AP message for password change 
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *          <passchanged>
 *             <user>9234238402384023849023843</user>
 *             <account>9234238402384023849023843</account>
 *          </passchanged> 
 *       </ap>   
 *   </ALMessage>
 *
 *  Succeeded AP message with unchanged profile
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *         <unchanged>true</unchanged>
 *       </ap>   
 *   </ALMessage>
 *
 *  Succeeded AP message with upgrade information
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *         <upgrade>
 *             <url>http://www.upgrade.com/upgrade.exe</url>
 *             <option>0</option>
 *         </upgrade>
 *       </ap>   
 *   </ALMessage>

 *  Succeeded AP message for PC client 
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <infoPage>http://www.bsafeonline.com/update</infoPage> 
 *         <status>true</status>
 *         <pcservices> 
 *             <time>2001/07/02 12:34:19</time> 
 *             <contentFilter>
 *                 <token>234234235642534534</token>
 *                 <whiteList>Game,Sex,Sports</whiteList>
 *                 <dnsChecking>yes</dnsChecking>
 *                 <peerToPeerPorts>
 *                   <port>33</port>
 *                   <port>66</port>
 *                 </peerToPeerPorts>
 *                 <timeout>10</timeout>   
 *                 <pattern>
 *                    <block>15</block>
 *                    <total>20</total>
 *                 </pattern> 
 *                 <redirect>www.bsafeonline.com</redirect> 
 *                 <advanced>
 *                     <reporting>yes</reporting>
 *                 </advanced>
 *             </contentFilter>
 *             <firewall>
 *                 <tcpPorts>
 *                   <port>43</port>
 *                   <port>56</port>
 *                 </tcpPorts>
 *                 <udpPorts>
 *                   <port>77</port>
 *                   <port>99</port>
 *                 </udpPorts>
 *                 <advanced>
 *                   <appControl>yes</appControl>
 *                 </advanced>  
 *             </firewall>
 *         </pcservices>
 *       </ap> 
 *  </ALMessage>
 *
 *  Succeeded AP message for gateway client
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <ap version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *         <sgservices>
 *            <time>2001/07/02 12:34:19</time>   
 *            ********  Ding hua will add more here.
 *         </sgservices>
 *       </ap> 
 *   </ALMessage>
 */


/*
 *  Node Definitions for FR message
 */
static char BASED_CODE szFR[]                  ="fr";
static char BASED_CODE szFRIndex[]             ="index";

/*
 *  Sample FR message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace">
 *     <fr version="1">
 *         <user>Jim Hush</user>
 *         <userKey>556676349838492750948497393847</userKey>
 *         <clientVersion>1.0</clientVersion>
 *         <deviceType>PC</deviceType>
 *         <index>5</index>
 *         <url>www.sprintf.com/testing.html</url>
 *     </fr> 
 *   </ALMessage> 
 */


/*
 *  Node Definitions for FP message
 */
static char BASED_CODE szFP[]                  ="fp";
static char BASED_CODE szFPBlock[]             ="block";
static char BASED_CODE szFPCategory[]          ="category";
static char BASED_CODE szFPResult[]            ="result";

/*
 *  Failed FP message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *       <fp version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>false</status>
 *         <failureInfo>Wrong password</failureInfo>
 *       </fp> 
 *   </ALMessage> 
 *
 *  Succeeded FP message as
 * 
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <fp version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *         <result index="5">
 *           <block>yes</block> 
 *           <category>sex</category>
 *         </result>
 *       </fp> 
 *  </ALMessage>
 */


/*
 *  Node Definitions for RR message
 */
static char BASED_CODE szRR[]                  ="rr";
static char BASED_CODE szRRUpdateInfo[]        ="info";
static char BASED_CODE szRRReporting[]         ="reporting";
static char BASED_CODE szRRHistory[]           ="history";
static char BASED_CODE szRRLogging[]           ="log";

/*
 *  Sample RR message for history update to reporting server
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *     <rr version="1">
 *         <account>sample</account>
 *         <accountKey>556676349838492750948497393847</accountKey>
 *         <user>Jim Hush</user>
 *         <userKey>556676349838492750948497393847</userKey>
 *         <clientVersion>1.0</clientVersion>
 *         <deviceType>PC</deviceType>
 *         <deviceID>pc3</deviceID> 
 *         <reporting>
 *             <history>
 *               <time>2001/07/02 12:39:22</time>
 *               <domain>http://www.playboy.com/</domain>
 *               <block>yes</block>
 *               <category>sex</category>
 *             </history>
 *             <history>
 *               <time>2001/07/02 12:34:19</time>
 *               <domain>www.girls.com</domain>
 *               <page>sex.html</page>        
 *               <block>yes</block>
 *               <category>sex</category>
 *             </history>
 *         </reporting>
 *     </rr> 
 *   </ALMessage> 
 * 
 *  Sample RR message for log update to reporting server
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *     <rr version="1">
 *         <user>Jim Hush</user>
 *         <userKey>556676349838492750948497393847</userKey>
 *         <clientVersion>1.0</clientVersion>
 *         <deviceType>SG</deviceType>
 *         <deviceID>sg1</deviceID>
 *         <reporting>
 *             <log>
 *             ********* Ding hua will write something here!
 *             </log>
 *             <log>
 *             </log>
 *         </reporting>
 *     </rr> 
 *   </ALMessage> 
 */


/*
 *  Node Definitions for RP message
 */
static char BASED_CODE szRP[]                  ="rp";
static char BASED_CODE szRPStatus[]            ="status";
static char BASED_CODE szRPFailInfo[]          ="failureInfo";

/*
 *  Failed RP message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *       <rp version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>false</status>
 *         <failureInfo>Wrong password</failureInfo>
 *       </rp> 
 *   </ALMessage> 
 *
 *  Succeeded RP message as
 *
 *   <?xml version="1.0"  encoding="UTF-8" ?>
 *   <ALMessage xml="http://www.w3.org/XML/1998/namespace" version="1">
 *      <rp version="1">
 *         <user>Jim Hush</user>
 *         <serverVersion>1.0</serverVersion>
 *         <status>true</status>
 *       </rp> 
 *  </ALMessage>
 */

/*
 *  Node name about history
 */
static char BASED_CODE szHistoryURL[]                  ="url";
static char BASED_CODE szHistoryTime[]                 ="time";
static char BASED_CODE szHistoryBlock[]                ="block";
static char BASED_CODE szHistoryCategory[]             ="category";

#endif /* #ifndef _XML_NODE_NAME_H */

