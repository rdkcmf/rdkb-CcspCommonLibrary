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

    module:	pki_ifo_ext.h

        For BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Device Access Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/15/05    initial revision.

**********************************************************************/


#ifndef  _PKI_IFO_EXT_
#define  _PKI_IFO_EXT_


/***********************************************************
   PKI NETWORK CONFIGURATION SERVICE INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  PKI_EXT_INTERFACE_NAME                    "pkiExtendedIf"
#define  PKI_EXT_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_PKIEXTIF_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PKIEXTIF_ACTION2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pCertEncoding,
        ULONG                       lenOfCert,
        ANSC_HANDLE                 hKeyPairParam,
        ULONG                       uCACount,
        void*                       pCACerts,
        ULONG                       lenOfCACerts
    );

typedef  ANSC_STATUS
(*PFN_PKIEXTIF_CHANGE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uNewIP
    );

typedef ANSC_STATUS
(*PFN_PKIEXTIF_EVENT)
    (
        ANSC_HANDLE                 hThisObject,
        PSLAP_PARAMETER_LIST        pEventParamList
    );
/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  PKI_EXT_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_PKIEXTIF_ACTION            CAListUpdated;                                           \
    PFN_PKIEXTIF_ACTION            SetSelfSignedCert;                                       \
    PFN_PKIEXTIF_ACTION2           SetCertConfig;                                           \
    PFN_PKIEXTIF_CHANGE            CommonNameChanged;                                       \
    PFN_PKIEXTIF_EVENT             GeneratePkiEvent;                                        \
    /* end of object class content */                                                       \

typedef  struct
_PKI_EXT_INTERFACE
{
    PKI_EXT_INTERFACE_CLASS_CONTENT
}
PKI_EXT_INTERFACE,  *PPKI_EXT_INTERFACE;

#define  ACCESS_PKI_EXT_INTERFACE(p)               \
         ACCESS_CONTAINER(p, PKI_EXT_INTERFACE, Linkage)


#endif
