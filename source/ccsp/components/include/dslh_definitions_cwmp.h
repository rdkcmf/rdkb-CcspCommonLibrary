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

    module:	dslh_definitions_cwmp.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters and message
        formats used in SOAP-based CWMP communication.

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


#ifndef  _DSLH_DEFINITIONS_CWMP_
#define  _DSLH_DEFINITIONS_CWMP_


#include "slap_definitions.h"


/***********************************************************
       BASELINE RPC ELEMENT AND PARAMETER DEFINITIONS
***********************************************************/

/*
 * The Inform message contains an argument called MaxEnvelopes that indicates to the ACS the
 * maximum number of SOAP envelopes that may be contained in a single HTTP response. The value of
 * this parameter may be one or greater. Once the Inform message has been received, any HTTP
 * response from the ACS may include at most this number of SOAP envelopes (requests or responses).
 */
#define  DSLH_CWMP_CPE_MAX_ENVELOPES                1

/*
 * A CPE MUST call the Inform method to initiate a transaction sequence whenever a connection to an
 * ACS is established. The CPE must pass in an array of EventStruct, indicating one or more events
 * that caused the transaction session to be established. If one or more causes exist, the CPE MUST
 * list all such causes.
 */
#define  DSLH_CWMP_INFORM_EVENT_BootStrap           0
#define  DSLH_CWMP_INFORM_EVENT_Boot                1
#define  DSLH_CWMP_INFORM_EVENT_Periodic            2
#define  DSLH_CWMP_INFORM_EVENT_Scheduled           3
#define  DSLH_CWMP_INFORM_EVENT_ValueChange         4
#define  DSLH_CWMP_INFORM_EVENT_Kicked              5
#define  DSLH_CWMP_INFORM_EVENT_ConnectionRequest   6
#define  DSLH_CWMP_INFORM_EVENT_TransferComplete    7
#define  DSLH_CWMP_INFORM_EVENT_DiagnosticsComplete 8

#define  DSLH_CWMP_INFORM_EVENT_Method              100
#define  DSLH_CWMP_INFORM_EVENT_VendorSpecific      101

/*
 * A fault response MUST make use of the SOAP Fault element using the following conventions:
 *
 *      - The SOAP faultcode element MUST indicate the source of the fault, either
 *        Client or Server, as appropriate for the particular fault. In this usage,
 *        Client represents the originator of the SOAP request, and Server represents
 *        the SOAP responder.
 *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
 *      - The SOAP detail element MUST contain a Fault structure defined in the
 *        "urn:dslforum-org:cwmp-1-0" namespace.
 */
#define  DSLH_CPE_SOAP_faultcode_methodUnsupported  "Server"
#define  DSLH_CPE_SOAP_faulttext_methodUnsupported  "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_methodUnsupported  9000
#define  DSLH_CPE_CWMP_FaultText_methodUnsupported  "Method not supported"

#define  DSLH_CPE_SOAP_faultcode_requestDenied      "Server"
#define  DSLH_CPE_SOAP_faulttext_requestDenied      "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_requestDenied      9001
#define  DSLH_CPE_CWMP_FaultText_requestDenied      "Request denied"

#define  DSLH_CPE_SOAP_faultcode_internalError      "Server"
#define  DSLH_CPE_SOAP_faulttext_internalError      "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_internalError      9002
#define  DSLH_CPE_CWMP_FaultText_internalError      "Internal error"

#define  DSLH_CPE_SOAP_faultcode_invalidArgs        "Client"
#define  DSLH_CPE_SOAP_faulttext_invalidArgs        "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_invalidArgs        9003
#define  DSLH_CPE_CWMP_FaultText_invalidArgs        "Invalid arguments"

#define  DSLH_CPE_SOAP_faultcode_resources          "Server"
#define  DSLH_CPE_SOAP_faulttext_resources          "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_resources          9004
#define  DSLH_CPE_CWMP_FaultText_resources          "Resources exceeded"

#define  DSLH_CPE_SOAP_faultcode_invalidParamName   "Client"
#define  DSLH_CPE_SOAP_faulttext_invalidParamName   "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_invalidParamName   9005
#define  DSLH_CPE_CWMP_FaultText_invalidParamName   "Invalid parameter name"

#define  DSLH_CPE_SOAP_faultcode_invalidParamType   "Client"
#define  DSLH_CPE_SOAP_faulttext_invalidParamType   "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_invalidParamType   9006
#define  DSLH_CPE_CWMP_FaultText_invalidParamType   "Invalid parameter type"

#define  DSLH_CPE_SOAP_faultcode_invalidParamValue  "Client"
#define  DSLH_CPE_SOAP_faulttext_invalidParamValue  "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_invalidParamValue  9007
#define  DSLH_CPE_CWMP_FaultText_invalidParamValue  "Invalid parameter value"

#define  DSLH_CPE_SOAP_faultcode_notWritable        "Client"
#define  DSLH_CPE_SOAP_faulttext_notWritable        "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_notWritable        9008
#define  DSLH_CPE_CWMP_FaultText_notWritable        "Attempt to set a non-writable parameter"

#define  DSLH_CPE_SOAP_faultcode_notifyRejected     "Server"
#define  DSLH_CPE_SOAP_faulttext_notifyRejected     "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_notifyRejected     9009
#define  DSLH_CPE_CWMP_FaultText_notifyRejected     "Notification request rejected"

#define  DSLH_CPE_SOAP_faultcode_downloadFailure    "Server"
#define  DSLH_CPE_SOAP_faulttext_downloadFailure    "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_downloadFailure    9010
#define  DSLH_CPE_CWMP_FaultText_downloadFailure    "Download failure"

#define  DSLH_CPE_SOAP_faultcode_uploadFailure      "Server"
#define  DSLH_CPE_SOAP_faulttext_uploadFailure      "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_uploadFailure      9011
#define  DSLH_CPE_CWMP_FaultText_uploadFailure      "Upload failure"

#define  DSLH_CPE_SOAP_faultcode_authFailure        "Server"
#define  DSLH_CPE_SOAP_faulttext_authFailure        "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_authFailure        9012
#define  DSLH_CPE_CWMP_FaultText_authFailure        "File transfer server authentication failure"

#define  DSLH_CPE_SOAP_faultcode_protUnsupported    "Server"
#define  DSLH_CPE_SOAP_faulttext_protUnsupported    "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_protUnsupported    9013
#define  DSLH_CPE_CWMP_FaultText_protUnsupported    "Unsupported protocol for file transfer"

#define  DSLH_CPE_SOAP_faultcode_maxEnvExceeded     "Client"
#define  DSLH_CPE_SOAP_faulttext_maxEnvExceeded     "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_maxEnvExceeded     9014
#define  DSLH_CPE_CWMP_FaultText_maxEnvExceeded     "Max Envelopes exceeded"

#define  DSLH_CPE_SOAP_faultcode_mcastJoinFailed    "Server"
#define  DSLH_CPE_SOAP_faulttext_mcastJoinFailed    "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_mcastJoinFailed    9014
#define  DSLH_CPE_CWMP_FaultText_mcastJoinFailed    "Max Envelopes exceeded"

#define  DSLH_CPE_SOAP_faultcode_notReachFileServer "Server"
#define  DSLH_CPE_SOAP_faulttext_notReachFileServer "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_notReachFileServer 9015
#define  DSLH_CPE_CWMP_FaultText_notReachFileServer "Download failure: unable to contact file server"

#define  DSLH_CPE_SOAP_faultcode_noAccessToFile     "Server"
#define  DSLH_CPE_SOAP_faulttext_noAccessToFile     "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_noAccessToFile     9016
#define  DSLH_CPE_CWMP_FaultText_noAccessToFile     "Download failure: unable to access file"

#define  DSLH_CPE_SOAP_faultcode_downloadIncomplete "Server"
#define  DSLH_CPE_SOAP_faulttext_downloadIncomplete "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_downloadIncomplete 9017
#define  DSLH_CPE_CWMP_FaultText_downloadIncomplete "Download failure: unable to complete download"

#define  DSLH_CPE_SOAP_faultcode_dlfileCorrputed    "Server"
#define  DSLH_CPE_SOAP_faulttext_dlfileCorrputed    "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_dlfileCorrputed    9018
#define  DSLH_CPE_CWMP_FaultText_dlfileCorrputed    "Download failure: file corrupted"

#define  DSLH_CPE_SOAP_faultcode_fileAuthFailure    "Server"
#define  DSLH_CPE_SOAP_faulttext_fileAuthFailure    "CWMP Fault"
#define  DSLH_CPE_CWMP_FaultCode_fileAuthFailure    9019
#define  DSLH_CPE_CWMP_FaultText_fileAuthFailure    "Download failure: file authentication failure"

/*
 * A fault response MUST make use of the SOAP Fault element using the following conventions:
 *
 *      - The SOAP faultcode element MUST indicate the source of the fault, either
 *        Client or Server, as appropriate for the particular fault. In this usage,
 *        Client represents the originator of the SOAP request, and Server represents
 *        the SOAP responder.
 *      - The SOAP faultstring sub-element MUST contain the string "CWMP fault".
 *      - The SOAP detail element MUST contain a Fault structure defined in the
 *        "urn:dslforum-org:cwmp-1-0" namespace.
 */
#define  DSLH_ACS_SOAP_faultcode_methodUnsupported  "Server"
#define  DSLH_ACS_SOAP_faulttext_methodUnsupported  "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_methodUnsupported  8000
#define  DSLH_ACS_CWMP_FaultText_methodUnsupported  "Method not supported"

#define  DSLH_ACS_SOAP_faultcode_requestDenied      "Server"
#define  DSLH_ACS_SOAP_faulttext_requestDenied      "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_requestDenied      8001
#define  DSLH_ACS_CWMP_FaultText_requestDenied      "Request denied"

#define  DSLH_ACS_SOAP_faultcode_internalError      "Server"
#define  DSLH_ACS_SOAP_faulttext_internalError      "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_internalError      8002
#define  DSLH_ACS_CWMP_FaultText_internalError      "Internal error"

#define  DSLH_ACS_SOAP_faultcode_invalidArgs        "Client"
#define  DSLH_ACS_SOAP_faulttext_invalidArgs        "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_invalidArgs        8003
#define  DSLH_ACS_CWMP_FaultText_invalidArgs        "Invalid arguments"

#define  DSLH_ACS_SOAP_faultcode_resources          "Server"
#define  DSLH_ACS_SOAP_faulttext_resources          "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_resources          8004
#define  DSLH_ACS_CWMP_FaultText_resources          "Resources exceeded"

#define  DSLH_ACS_SOAP_faultcode_retryRequest       "Server"
#define  DSLH_ACS_SOAP_faulttext_retryRequest       "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_retryRequest       8005
#define  DSLH_ACS_CWMP_FaultText_retryRequest       "Retry request"

#define  DSLH_ACS_SOAP_faultcode_maxEnvExceeded     "Client"
#define  DSLH_ACS_SOAP_faulttext_maxEnvExceeded     "CWMP Fault"
#define  DSLH_ACS_CWMP_FaultCode_maxEnvExceeded     8006
#define  DSLH_ACS_CWMP_FaultText_maxEnvExceeded     "Max Envelopes exceeded"

/*
 * The Notification field in the SetParameterAttributeStruct indicates whether the CPE should
 * include changed values of the specified parameter(s) in the Inform message, and whether the CPE
 * must initiate a session to the ACS when the specified parameter(s) in value.
 */
#define  DSLH_CWMP_NOTIFICATION_off                 0
#define  DSLH_CWMP_NOTIFICATION_passive             1
#define  DSLH_CWMP_NOTIFICATION_active              2

/*
 * If a Parameter name argument is given as a partial path name, the request is to be inter-
 * preted as a request to return all the Parameters in the branch of the naming hierarchy that
 * shares the same prefix as the argument.
 */
#define  DslhCwmpIsPartialName(name)                ( name[AnscSizeOfString(name) - 1] == '.' )


/***********************************************************
       BASELINE RPC ARGUMENT AND STRUCTURE DEFINITIONS
***********************************************************/

/*
 * On successful receipt of a SetParameterValues RPC, the CPE MUST apply the changes to each of the
 * specified Parameters immediately and atomically. The order of Parameters listed in the Parameter-
 * List has no significance - the application of value changes to the CPE MUST be independent from
 * the order in which they are listed.
 */
typedef  struct
_DSLH_CWMP_PARAM_VALUE
{
    char*                           Name;
    SLAP_VARIABLE*                  Value;
}
DSLH_CWMP_PARAM_VALUE,  *PDSLH_CWMP_PARAM_VALUE;

#define  DslhCwmpCleanParamValue(param_value)                                               \
         {                                                                                  \
            if ( param_value->Name )                                                        \
            {                                                                               \
                AnscFreeMemory(param_value->Name);                                          \
                                                                                            \
                param_value->Name = NULL;                                                   \
            }                                                                               \
                                                                                            \
            if ( param_value->Value )                                                       \
            {                                                                               \
                SlapFreeVariable(param_value->Value);                                       \
                                                                                            \
                param_value->Value = NULL;                                                  \
            }                                                                               \
         }

#define  DslhCwmpFreeParamValue(param_value)                                                \
         {                                                                                  \
            DslhCwmpCleanParamValue(param_value);                                           \
            AnscFreeMemory         (param_value);                                           \
         }

typedef  struct
_DSLH_CWMP_PARAM_INFO
{
    char*                           Name;
    BOOL                            bWritable;
}
DSLH_CWMP_PARAM_INFO,  *PDSLH_CWMP_PARAM_INFO;

#define  DslhCwmpCleanParamInfo(param_info)                                                 \
         {                                                                                  \
            if ( param_info->Name )                                                         \
            {                                                                               \
                AnscFreeMemory(param_info->Name);                                           \
                                                                                            \
                param_info->Name = NULL;                                                    \
            }                                                                               \
         }

#define  DslhCwmpFreeParamInfo(param_info)                                                  \
         {                                                                                  \
            DslhCwmpCleanParamInfo(param_info);                                             \
            AnscFreeMemory        (param_info);                                             \
         }

typedef  struct
_DSLH_CWMP_SET_PARAM_ATTRIB
{
    char*                           Name;
    BOOL                            bNotificationChange;
    int                             Notification;
    BOOL                            bAccessListChange;
    char*                           AccessList;     /* comma separated list */
    int                             RequesterID;
}
DSLH_CWMP_SET_PARAM_ATTRIB,  *PDSLH_CWMP_SET_PARAM_ATTRIB;

#define  DslhCwmpCleanSetParamAttrib(set_param_attrib)                                      \
         {                                                                                  \
            if ( set_param_attrib->Name )                                                   \
            {                                                                               \
                AnscFreeMemory(set_param_attrib->Name);                                     \
                                                                                            \
                set_param_attrib->Name = NULL;                                              \
            }                                                                               \
                                                                                            \
            if ( set_param_attrib->AccessList )                                             \
            {                                                                               \
                AnscFreeMemory(set_param_attrib->AccessList);                               \
                                                                                            \
                set_param_attrib->AccessList = NULL;                                        \
            }                                                                               \
         }

#define  DslhCwmpFreeSetParamAttrib(set_param_attrib)                                       \
         {                                                                                  \
            DslhCwmpCleanSetParamAttrib(set_param_attrib);                                  \
            AnscFreeMemory             (set_param_attrib);                                  \
         }

typedef  struct
_DSLH_CWMP_PARAM_ATTRIB
{
    char*                           Name;
    int                             Notification;
    char*                           AccessList;     /* comma separated list */
}
DSLH_CWMP_PARAM_ATTRIB,  *PDSLH_CWMP_PARAM_ATTRIB;

#define  DslhCwmpCleanParamAttrib(param_attrib)                                             \
         {                                                                                  \
            if ( param_attrib->Name )                                                       \
            {                                                                               \
                AnscFreeMemory(param_attrib->Name);                                         \
                                                                                            \
                param_attrib->Name = NULL;                                                  \
            }                                                                               \
                                                                                            \
            if ( param_attrib->AccessList )                                                 \
            {                                                                               \
                AnscFreeMemory(param_attrib->AccessList);                                   \
                                                                                            \
                param_attrib->AccessList = NULL;                                            \
            }                                                                               \
         }

#define  DslhCwmpFreeParamAttrib(param_attrib)                                              \
         {                                                                                  \
            DslhCwmpCleanParamAttrib(param_attrib);                                         \
            AnscFreeMemory          (param_attrib);                                         \
         }

/*
 * A CPE MUST call the Inform method to initiate a transaction sequence whenever a connection to an
 * ACS is established. When the Inform call results from a change to one or more parameter values
 * (due to a cause other than being set by the ACS itself) that the ACS has marked for notification
 * (either active or passive) via SetParameterAttributes, all of the changed parameters must also
 * be included in the ParameterList.
 */
typedef  struct
_DSLH_CWMP_DEVICE_ID
{
    char*                           Manufacturer;
    char*                           OUI;
    char*                           ProductClass;
    char*                           SerialNumber;
}
DSLH_CWMP_DEVICE_ID,  *PDSLH_CWMP_DEVICE_ID;

#define  DslhCwmpCleanDeviceId(device_id)                                                   \
         {                                                                                  \
            if ( device_id->Manufacturer )                                                  \
            {                                                                               \
                AnscFreeMemory(device_id->Manufacturer);                                    \
                                                                                            \
                device_id->Manufacturer = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( device_id->OUI )                                                           \
            {                                                                               \
                AnscFreeMemory(device_id->OUI);                                             \
                                                                                            \
                device_id->OUI = NULL;                                                      \
            }                                                                               \
                                                                                            \
            if ( device_id->ProductClass )                                                  \
            {                                                                               \
                AnscFreeMemory(device_id->ProductClass);                                    \
                                                                                            \
                device_id->ProductClass = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( device_id->SerialNumber )                                                  \
            {                                                                               \
                AnscFreeMemory(device_id->SerialNumber);                                    \
                                                                                            \
                device_id->SerialNumber = NULL;                                             \
            }                                                                               \
         }

#define  DslhCwmpFreeDeviceId(device_id)                                                    \
         {                                                                                  \
            DslhCwmpCleanDeviceId(device_id);                                               \
            AnscFreeMemory       (device_id);                                               \
         }

typedef  struct
_DSLH_CWMP_EVENT
{
    char*                           EventCode;
    char*                           CommandKey;
}
DSLH_CWMP_EVENT,  *PDSLH_CWMP_EVENT;

#define  DslhCwmpCleanEvent(event)                                                          \
         {                                                                                  \
            if ( event->EventCode )                                                         \
            {                                                                               \
                AnscFreeMemory(event->EventCode);                                           \
                                                                                            \
                event->EventCode = NULL;                                                    \
            }                                                                               \
                                                                                            \
            if ( event->CommandKey )                                                        \
            {                                                                               \
                AnscFreeMemory(event->CommandKey);                                          \
                                                                                            \
                event->CommandKey = NULL;                                                   \
            }                                                                               \
         }

#define  DslhCwmpFreeEvent(event)                                                           \
         {                                                                                  \
            DslhCwmpCleanEvent(event);                                                      \
            AnscFreeMemory    (event);                                                      \
         }

typedef  struct
_DSLH_CWMP_FAULT
{
    ULONG                           FaultCode;
    char*                           FaultString;
}
DSLH_CWMP_FAULT,  *PDSLH_CWMP_FAULT;

#define  DslhCwmpCleanFault(fault)                                                          \
         {                                                                                  \
            if ( fault->FaultString )                                                       \
            {                                                                               \
                AnscFreeMemory(fault->FaultString);                                         \
                                                                                            \
                fault->FaultString = NULL;                                                  \
            }                                                                               \
         }

#define  DslhCwmpFreeFault(fault)                                                           \
         {                                                                                  \
            DslhCwmpCleanFault(fault);                                                      \
            AnscFreeMemory    (fault);                                                      \
         }

typedef  struct
_DSLH_CWMP_SET_PARAM_FAULT
{
    char*                           ParameterName;
    ULONG                           FaultCode;
    char*                           FaultString;
}
DSLH_CWMP_SET_PARAM_FAULT,  *PDSLH_CWMP_SET_PARAM_FAULT;

#define  DslhCwmpCleanSetParamFault(param_fault)                                            \
         {                                                                                  \
            if ( param_fault->ParameterName )                                               \
            {                                                                               \
                AnscFreeMemory(param_fault->ParameterName);                                 \
                                                                                            \
                param_fault->ParameterName = NULL;                                          \
            }                                                                               \
                                                                                            \
            if ( param_fault->FaultString )                                                 \
            {                                                                               \
                AnscFreeMemory(param_fault->FaultString);                                   \
                                                                                            \
                param_fault->FaultString = NULL;                                            \
            }                                                                               \
         }

#define  DslhCwmpFreeSetParamFault(param_fault)                                             \
         {                                                                                  \
            DslhCwmpCleanSetParamFault(param_fault);                                        \
            AnscFreeMemory            (param_fault);                                        \
         }

/*
 * The GetQueuedTransfers method can be called by the server to determine the status of previously
 * requested downloads or uploads. The CPE device shall return an array of QueuedTransferStruct,
 * each describing the state of one transfer that the CPE has been instructed to perform, but has
 * not yet been fully completed.
 */
typedef  struct
_DSLH_CWMP_QUEUED_TRANSFER
{
    char*                           CommandKey;
    ULONG                           State;
}
DSLH_CWMP_QUEUED_TRANSFER,  *PDSLH_CWMP_QUEUED_TRANSFER;

#define  DslhCwmpCleanQueuedTransfer(queued_transfer)                                       \
         {                                                                                  \
            if ( queued_transfer->CommandKey )                                              \
            {                                                                               \
                AnscFreeMemory(queued_transfer->CommandKey);                                \
                                                                                            \
                queued_transfer->CommandKey = NULL;                                         \
            }                                                                               \
         }

#define  DslhCwmpFreeQueuedTransfer(queued_transfer)                                        \
         {                                                                                  \
            DslhCwmpCleanQueuedTransfer(queued_transfer);                                   \
            AnscFreeMemory             (queued_transfer);                                   \
         }

typedef  struct
_DSLH_CWMP_OPTION
{
    char*                           OptionName;
    ULONG                           VoucherSN;
    ULONG                           State;
    int                             Mode;
    ANSC_UNIVERSAL_TIME             StartDate;
    ANSC_UNIVERSAL_TIME             ExpirationDate;
    BOOL                            IsTransferable;
}
DSLH_CWMP_OPTION,  *PDSLH_CWMP_OPTION;

#define  DslhCwmpCleanOption(option)                                                        \
         {                                                                                  \
            if ( option->OptionName )                                                       \
            {                                                                               \
                AnscFreeMemory(option->OptionName);                                         \
                                                                                            \
                option->OptionName = NULL;                                                  \
            }                                                                               \
         }

#define  DslhCwmpFreeOption(option)                                                         \
         {                                                                                  \
            DslhCwmpCleanOption(option);                                                    \
            AnscFreeMemory     (option);                                                    \
         }

/*
 * The RequestDownload method allows the CPE to request a file download from the Server. On recep-
 * tion of this request, the Server MAY call the Download method to initiate the download.
 */
typedef  struct
_DSLH_CWMP_FILE_TYPE
{
    char*                           Name;
    char*                           Value;
}
DSLH_CWMP_FILE_TYPE,  *PDSLH_CWMP_FILE_TYPE;

#define  DslhCwmpCleanFileType(file_type)                                                   \
         {                                                                                  \
            if ( file_type->Name )                                                          \
            {                                                                               \
                AnscFreeMemory(file_type->Name);                                            \
                                                                                            \
                file_type->Name = NULL;                                                     \
            }                                                                               \
                                                                                            \
            if ( file_type->Value )                                                         \
            {                                                                               \
                AnscFreeMemory(file_type->Value);                                           \
                                                                                            \
                file_type->Value = NULL;                                                    \
            }                                                                               \
         }

#define  DslhCwmpFreeFileType(file_type)                                                    \
         {                                                                                  \
            DslhCwmpCleanFileType(file_type);                                               \
            AnscFreeMemory       (file_type);                                               \
         }


/***********************************************************
    SOAP-BASED MESSAGE ELEMENT AND PARAMETER DEFINITIONS
***********************************************************/

/*
 * In the CPE WAN Management Protocol, a remote procedure call mechanism is used for bi-directional
 * communication between a CPE device and an Auto-configuration Server (ACS). It is assumed that
 * the lower layers that transport RPC messages provide most aspects of security, including mutual
 * authentication between the CPE and ACS, confidentiality, and data integrity.
 */
#define  DSLH_CWMP_METHOD_Unknown                   0
#define  DSLH_CWMP_METHOD_GetRPCMethods             1
#define  DSLH_CWMP_METHOD_SetParameterValues        2
#define  DSLH_CWMP_METHOD_GetParameterValues        3
#define  DSLH_CWMP_METHOD_GetParameterNames         4
#define  DSLH_CWMP_METHOD_SetParameterAttributes    5
#define  DSLH_CWMP_METHOD_GetParameterAttributes    6
#define  DSLH_CWMP_METHOD_AddObject                 7
#define  DSLH_CWMP_METHOD_DeleteObject              8
#define  DSLH_CWMP_METHOD_Reboot                    9
#define  DSLH_CWMP_METHOD_Download                  10
#define  DSLH_CWMP_METHOD_Upload                    11
#define  DSLH_CWMP_METHOD_FactoryReset              12
#define  DSLH_CWMP_METHOD_GetQueuedTransfers        13
#define  DSLH_CWMP_METHOD_ScheduleInform            14
#define  DSLH_CWMP_METHOD_SetVouchers               15
#define  DSLH_CWMP_METHOD_GetOptions                16
#define  DSLH_CWMP_METHOD_ChangeDUState             17

#define  DSLH_CWMP_METHOD_Inform                            101
#define  DSLH_CWMP_METHOD_TransferComplete                  102
#define  DSLH_CWMP_METHOD_RequestDownload                   103
#define  DSLH_CWMP_METHOD_Kicked                            104
#define  DSLH_CWMP_METHOD_AutonomousTransferComplete        105
#define  DSLH_CWMP_METHOD_DUStateChangeComplete             106
#define  DSLH_CWMP_METHOD_AutonomousDUStateChangeComplete   107

/*
 * In each direction, the order of SOAP envelopes is defined independently from the number of
 * envelopes carried per HTTP post/response pair. Specifically, envelopes are ordered from first to
 * last within a single HTTP post/response and then between successive post/response pairs. That is,
 * the succession of envelopes within each HTTP post/response and then between successive posts or
 * responses can be thought of as a single ordered sequence of envelopes.
 */
#define  DSLH_CWMP_SOAP_HEADER_ID                   0x00000001
#define  DSLH_CWMP_SOAP_HEADER_HoldRequests         0x00000002
#define  DSLH_CWMP_SOAP_HEADER_NoMoreRequests       0x00000004

typedef  struct
_DSLH_CWMP_SOAP_HEADER
{
    ULONG                           ElementMask;
    char*                           ID;
    BOOL                            bHoldRequests;
    BOOL                            bNoMoreRequests;
}
DSLH_CWMP_SOAP_HEADER,  *PDSLH_CWMP_SOAP_HEADER;

#define  DslhCwmpCleanSoapHeader(soap_header)                                               \
         {                                                                                  \
            if ( soap_header->ID )                                                          \
            {                                                                               \
                AnscFreeMemory(soap_header->ID);                                            \
                                                                                            \
                soap_header->ID = NULL;                                                     \
            }                                                                               \
         }

#define  DslhCwmpFreeSoapHeader(soap_header)                                                \
         {                                                                                  \
            DslhCwmpCleanSoapHeader(soap_header);                                           \
            AnscFreeMemory         (soap_header);                                           \
         }

typedef  struct
_DSLH_CWMP_SOAP_REQUEST
{
    DSLH_CWMP_SOAP_HEADER           Header;
    ULONG                           Method;
    ANSC_HANDLE                     hReqArguments;
}
DSLH_CWMP_SOAP_REQUEST,  *PDSLH_CWMP_SOAP_REQUEST;

#define  DslhCwmpCleanSoapReq(soap_req)                                                     \
         {                                                                                  \
            DslhCwmpCleanSoapHeader((&soap_req->Header));                                   \
         }

#define  DslhCwmpFreeSoapReq(soap_req)                                                      \
         {                                                                                  \
            DslhCwmpCleanSoapReq(soap_req);                                                 \
            AnscFreeMemory      (soap_req);                                                 \
         }

typedef  struct
_DSLH_CWMP_SOAP_FAULT
{
    char*                           soap_faultcode;
    char*                           soap_faultstring;
    DSLH_CWMP_FAULT                 Fault;
    ULONG                           SetParamValuesFaultCount;
    DSLH_CWMP_SET_PARAM_FAULT       SetParamValuesFaultArray[ANSC_ZERO_ARRAY_SIZE];
}
DSLH_CWMP_SOAP_FAULT,  *PDSLH_CWMP_SOAP_FAULT;

#define  DslhCwmpCleanSoapFault(soap_fault)                                                 \
         {                                                                                  \
            ULONG                   ii = 0;                                                 \
                                                                                            \
            if ( soap_fault->soap_faultcode )                                               \
            {                                                                               \
                AnscFreeMemory(soap_fault->soap_faultcode);                                 \
                                                                                            \
                soap_fault->soap_faultcode = NULL;                                          \
            }                                                                               \
                                                                                            \
            if ( soap_fault->soap_faultstring )                                             \
            {                                                                               \
                AnscFreeMemory(soap_fault->soap_faultstring);                               \
                                                                                            \
                soap_fault->soap_faultstring = NULL;                                        \
            }                                                                               \
                                                                                            \
            DslhCwmpCleanFault((&soap_fault->Fault));                                       \
                                                                                            \
            for ( ii = 0; ii < soap_fault->SetParamValuesFaultCount; ii++ )                 \
            {                                                                               \
                DslhCwmpCleanSetParamFault((&soap_fault->SetParamValuesFaultArray[ii]));    \
            }                                                                               \
         }

#define  DslhCwmpFreeSoapFault(soap_fault)                                                  \
         {                                                                                  \
            DslhCwmpCleanSoapFault(soap_fault);                                             \
            AnscFreeMemory        (soap_fault);                                             \
         }

typedef  struct
_DSLH_CWMP_SOAP_RESPONSE
{
    DSLH_CWMP_SOAP_HEADER           Header;
    ULONG                           Method;
    ANSC_HANDLE                     hRepArguments;
    DSLH_CWMP_SOAP_FAULT*           Fault;
}
DSLH_CWMP_SOAP_RESPONSE,  *PDSLH_CWMP_SOAP_RESPONSE;

#define  DslhCwmpCleanSoapResponse(soap_rep)                                                \
         {                                                                                  \
            DslhCwmpCleanSoapHeader((&soap_rep->Header));                                   \
                                                                                            \
            if ( soap_rep->Fault )                                                          \
            {                                                                               \
                DslhCwmpFreeSoapFault(soap_rep->Fault);                                     \
                                                                                            \
                soap_rep->Fault = NULL;                                                     \
            }                                                                               \
                                                                                            \
            if ( soap_rep->hRepArguments )                                                  \
            {                                                                               \
                if ( soap_rep->Method == DSLH_CWMP_METHOD_GetRPCMethods )                   \
                {                                                                           \
                    SlapFreeVarArray(((SLAP_STRING_ARRAY*)soap_rep->hRepArguments));        \
                                                                                            \
                    soap_rep->hRepArguments = (ANSC_HANDLE)NULL;                            \
                }                                                                           \
                else if ( soap_rep->Method == DSLH_CWMP_METHOD_Kicked )                     \
                {                                                                           \
                    AnscFreeMemory((char*)soap_rep->hRepArguments);                         \
                                                                                            \
                    soap_rep->hRepArguments = (ANSC_HANDLE)NULL;                            \
                }                                                                           \
            }                                                                               \
         }

#define  DslhCwmpFreeSoapResponse(soap_rep)                                                 \
         {                                                                                  \
            DslhCwmpCleanSoapResponse(soap_rep);                                            \
            AnscFreeMemory           (soap_rep);                                            \
         }

/*
 * The RPC methods defined in TR-069 make use of a limited subset of the default SOAP data types.
 * They also make use of structures and arrays (in some cases containing mixed types). Array ele-
 * ments are indicated with square brackets after the data type. If specified, the maximum length
 * of the array would be indicated within the brackets.
 */


#endif
