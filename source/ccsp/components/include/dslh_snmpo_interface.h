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

    module:	dslh_snmpo_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Snmp Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        07/24/08    initial revision.

**********************************************************************/


#ifndef  _DSLH_SNMPO_INTERFACE_
#define  _DSLH_SNMPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "dslh_ifo_snmpmib.h"


/***********************************************************
        DSLH SNMP MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHSNMPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_ADD_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pTableName,
        ULONG                       uCount
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_REG_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pMibName,
        char*                       pTableName
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHSNMPO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );


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
#define  DSLH_SNMP_MANAGER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    ANSC_HANDLE                     hDslhSnmpMibIf;                                         \
    ANSC_HANDLE                     hDslhMibGetIf;                                          \
    ANSC_HANDLE                     hAtoMibs;                                               \
    ANSC_HANDLE                     hAtoTableCount;                                         \
    ANSC_HANDLE                     hAtoTableCountParam;                                    \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_DSLHSNMPO_GET_IF           GetDslhSnmpMibIf;                                        \
    PFN_DSLHSNMPO_SET_IF           SetDslhMibGetIf;                                         \
                                                                                            \
    PFN_DSLHSNMPO_ADD_COUNT        SaveTableCount;                                          \
    PFN_DSLHSNMPO_REG_PARAM        RegTableCountParam;                                      \
                                                                                            \
    PFN_DSLHSNMPO_ENGAGE           Engage;                                                  \
    PFN_DSLHSNMPO_CANCEL           Cancel;                                                  \
    PFN_DSLHSNMPO_SETUP_ENV        SetupEnv;                                                \
    PFN_DSLHSNMPO_CLOSE_ENV        CloseEnv;                                                \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_SNMP_MANAGER_OBJECT
{
    DSLH_SNMP_MANAGER_CLASS_CONTENT
}
DSLH_SNMP_MANAGER_OBJECT,  *PDSLH_SNMP_MANAGER_OBJECT;

#define  ACCESS_DSLH_SNMP_MANAGER_OBJECT(p)         \
         ACCESS_CONTAINER(p, DSLH_SNMP_MANAGER_OBJECT, Linkage)



#endif
