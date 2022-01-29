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

        For Dbus base library Implementation,
        Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Qiang Tu

    ---------------------------------------------------------------

    revision:

        06/17/11    initial revision.

**********************************************************************/

#ifndef CCSP_BASE_API_H
#define CCSP_BASE_API_H
#include "ccsp_custom.h"
#include "ccsp_message_bus.h"

/*
Notes:
*/
//if you don't want use SLAP_VARIABLE which require extra headers, remove this
#define PSM_SLAP_VAR

#define CCSP_DBUS_PSM    "com.cisco.spvtg.ccsp.psm"

#define CCSP_DBUS_PATH_CR          "/com/cisco/spvtg/ccsp/CR"
#define CCSP_DBUS_PATH_PSM         "/com/cisco/spvtg/ccsp/PSM"
#define CCSP_DBUS_PATH_EVENT       "/com/cisco/spvtg/ccsp/EVENT"
#define CCSP_RBUS_EVENT            "/com/cisco/spvtg/ccsp/EVENT"
#define CCSP_DBUS_INTERFACE_EVENT  "com.cisco.spvtg.ccsp.EVENT"
#define CCSP_DBUS_INTERFACE_CR     "com.cisco.spvtg.ccsp.CR"
#define CCSP_DBUS_INTERFACE_BASE   "com.cisco.spvtg.ccsp.baseInterface"

#define CCSP_SYSTEM_READY_SIGNAL            "systemReadySignal"
#define CCSP_TELEMETRY_DATA_SIGNAL           "telemetryDataSignal"
#define CCSP_CURRENT_SESSION_ID_SIGNAL      "currentSessionIDSignal"
#define CCSP_DEVICE_PROFILE_CHANGE_SIGNAL   "deviceProfileChangeSignal"
#define CCSP_DIAG_COMPLETE_SIGNAL           "diagCompleteSignal"
#define CCSP_PARAMETER_VALUE_CHANGE_SIGNAL  "parameterValueChangeSignal"
#define CCSP_SYSTEM_REBOOT_SIGNAL           "systemRebootSignal"

#define METHOD_REGISTERCAPABILITIES         "registerCapabilities"
#define METHOD_ISSYSTEMREADY                "isSystemReady"
#define METHOD_GETHEALTH                    "METHOD_GETHEALTH"

#define CCSP_DBUS_INTERFACE_PA     "com.cisco.stbservice.protocolagent"

#define STBSERVICE_CDL_INTERFACE 	"com.cisco.stbservice.codedownload"
#define STBSERVICE_CDL_PATH      	"/com/cisco/stbservice/codedownload"
#define STBSERVICE_CDL_DEST         STBSERVICE_CDL_INTERFACE
#define STBSERVICE_CDL_DLC_SIGNAL   "cdloverall-dlcomplete"

#define CCSP_USER_CHANGED_MONITOR_PARAM "EnableMonitorUserChangedParams"
#define CCSP_USER_COMPONENTS_PARAM      "UserOpComponents"

#define CCSP_SUCCESS 						        100

#define CCSP_ERR_MEMORY_ALLOC_FAIL  				101
#define CCSP_FAILURE                 				102

#define CCSP_ERR_NOT_CONNECT          				190   //can't connect to daemon
#define CCSP_ERR_TIMEOUT            				191
#define CCSP_ERR_NOT_EXIST            				192   //remote not exist 
#define CCSP_ERR_NOT_SUPPORT           				193   //remote can't support this api

/* Bin add more TR Fault Error Code */
#define CCSP_ERR_CWMP_BEGINNING                     9000
#define CCSP_ERR_METHOD_NOT_SUPPORTED               9000
#define CCSP_ERR_REQUEST_REJECTED                   9001
#define CCSP_ERR_INTERNAL_ERROR                     9002
#define CCSP_ERR_INVALID_ARGUMENTS					9003
#define CCSP_ERR_RESOURCE_EXCEEDED                  9004
#define CCSP_ERR_INVALID_PARAMETER_NAME             9005
#define CCSP_ERR_INVALID_PARAMETER_TYPE             9006
#define CCSP_ERR_INVALID_PARAMETER_VALUE            9007
#define CCSP_ERR_NOT_WRITABLE                       9008
#define CCSP_ERR_SETATTRIBUTE_REJECTED              9009
#define CCSP_ERR_FILE_TRANSFER_FAILURE              9010
#define CCSP_ERR_UPLOAD_FAILURE                     9011
#define CCSP_ERR_FILE_TRANSFER_AUTH_FAILURE         9012
#define CCSP_ERR_UNSUPPORTED_PROTOCOL               9013
#define CCSP_ERR_UNABLE_TO_JOIN_MULTICAST           9014
#define CCSP_ERR_UNABLE_TO_CONTACT_FILE_SERVER      9015
#define CCSP_ERR_UNABLE_TO_ACCESS_FILE              9016
#define CCSP_ERR_UNABLE_TO_COMPLETE_DOWNLOAD        9017
#define CCSP_ERR_FILE_CORRUPTED_OR_UNUSABLE         9018
#define CCSP_ERR_FILE_AUTH_FAILURE                  9019
#define CCSP_ERR_UNABLE_TO_COMPLETE_ONTIME          9020
#define CCSP_ERR_CANCELATION_NOT_PERMITTED          9021
#define CCSP_ERR_INVALID_UUID_FORMAT                9022
#define CCSP_ERR_UNKNOWN_EE                         9023
#define CCSP_ERR_DISABLED_EE                        9024
#define CCSP_ERR_DU_EE_MISMATCH                     9025
#define CCSP_ERR_DUPLICATE_DU                       9026
#define CCSP_ERR_SYSTEM_RES_EXCEEDED                9027
#define CCSP_ERR_UNKNOWN_DU                         9028
#define CCSP_ERR_INVALID_DU_STATE                   9029
#define CCSP_ERR_DOWNGRADE_NOT_PERMITTED            9030
#define CCSP_ERR_VERSION_NOT_SPECIFIED              9031
#define CCSP_ERR_VERSION_EXISTS                     9032
/* the maxi CWMP error code */
#define CCSP_ERR_CWMP_ENDING                        9032

#define CCSP_CR_ERR_NAMESPACE_OVERLAP 				201
#define CCSP_CR_ERR_UNKNOWN_COMPONENT				202
#define CCSP_CR_ERR_NAMESPACE_MISMATCH		        203
#define CCSP_CR_ERR_UNSUPPORTED_NAMESPACE 	        204
#define CCSP_CR_ERR_DP_COMPONENT_VERSION_MISMATCH	205
#define CCSP_CR_ERR_INVALID_PARAM					206
#define CCSP_CR_ERR_UNSUPPORTED_DATATYPE			207
#define CCSP_CR_ERR_SESSION_IN_PROGRESS             208
#define CCSP_INVALID_PSMCLI_CMD                     209

typedef unsigned int dbus_bool ; //1 is true, 0 is false
extern int rbus_enabled;

#define DBUS_MESSAGE_APPEND_STRING(iter,string) do {   \
    if(string)  \
        dbus_message_iter_append_basic (iter, DBUS_TYPE_STRING, &string); \
    else \
    {  \
        char *tmp = ""; \
        dbus_message_iter_append_basic (iter, DBUS_TYPE_STRING, &tmp); \
	} \
  } while (0)

#define DBUS_MESSAGE_ITER_RECURSE(iter,subiter,type, val, ret, freefc) do {   \
	if(dbus_message_iter_get_arg_type(iter) == type) \
	  dbus_message_iter_recurse(iter,subiter); \
	else \
	{ \
	    freefc(val); \
	    dbus_message_unref (reply); \
	    dbus_message_unref (message); \
		return ret; \
	} \
  } while (0)


#define DBUS_MESSAGE_ITER_RECURSE_SRV(iter,subiter,type, val, freefc) do {   \
	if(dbus_message_iter_get_arg_type(iter) == type) \
	  dbus_message_iter_recurse(iter,subiter); \
	else \
	{ \
	    freefc(val); \
	    dbus_message_unref (reply); \
	    reply = dbus_message_new_error (message, \
	                                    DBUS_ERROR_INVALID_ARGS, \
	                                    "InvalidArgs"); \
	    dbus_connection_send (conn, reply, NULL); \
	    dbus_message_unref (reply); \
	    return DBUS_HANDLER_RESULT_HANDLED; \
	} \
  } while (0)

  
#define DBUS_MESSAGE_ITER_RECURSE_SIG(iter,subiter,type, val, freefc) do {   \
	if(dbus_message_iter_get_arg_type(iter) == type) \
	  dbus_message_iter_recurse(iter,subiter); \
	else \
	{ \
	    freefc(val); \
	    return DBUS_HANDLER_RESULT_HANDLED; \
	} \
  } while (0)

#define RBUS_LOG(...) do {\
    if(access("/nvram/rbus_support_log_to_file", F_OK) == 0) {\
        CcspTraceInfo((__VA_ARGS__));\
    }\
} while(0)

#define RBUS_LOG_ERR(...) do {\
    CcspTraceError((__VA_ARGS__));\
} while(0)

enum dataType_e
{
    ccsp_string = 0,
    ccsp_int,
    ccsp_unsignedInt,
    ccsp_boolean,
    ccsp_dateTime,
    ccsp_base64,
    ccsp_long,
    ccsp_unsignedLong,
    ccsp_float,
    ccsp_double,
    ccsp_byte,
    ccsp_none,
} ;

enum access_e
{
    CCSP_RO,
    CCSP_RW,
    CCSP_WO
}  ;


typedef struct
{
    char *parameterName;
    char *parameterValue;
    enum dataType_e type;
} parameterValStruct_t;

typedef struct
{
    char* parameterName;
    dbus_bool notificationChanged;
    dbus_bool notification;
    enum access_e access;
    dbus_bool accessControlChanged;
    unsigned int accessControlBitmask;
    unsigned int RequesterID;
} parameterAttributeStruct_t;

typedef struct
{
    char *parameterName;
    dbus_bool writable;
} parameterInfoStruct_t;

typedef struct
{
    char * name_space;
    enum dataType_e dataType;
} name_spaceType_t;


typedef struct
{
    const char *parameterName;
    const char* oldValue;
    const char* newValue;
    enum dataType_e type;                              // data type
    const char* subsystem_prefix;   // subsystem prefix of the namespace
    unsigned int writeID;
} parameterSigStruct_t;


typedef struct
{
    char *componentName;
    char *dbusPath;
    enum dataType_e type;   
    char *remoteCR_name;
    char *remoteCR_dbus_path;
} componentStruct_t;


typedef struct
{
    char *componentName;
    char *dbusPath;
    char *subsystem_prefix;
} registeredComponent_t;


#define CCSP_BASE_INSTANCE  0
#define CCSP_BASE_PARAM     1
#define CCSP_BASE_OBJECT    2

#define  CCSP_BASE_PARAM_LENGTH              256

typedef struct
{
    union
    {
        unsigned int       InstanceNumber;
        char               Name[CCSP_BASE_PARAM_LENGTH];
    } Instance;
    int         RecordType;
} CCSP_BASE_RECORD, *PCCSP_BASE_RECORD;


//base function


/*This API frees up resources such as allocated memory, flush caches etc, if possible. 
            This is invoked by Test and Diagnostic Manager, as a proactive measure, when it 
            detects low memory conditions.     
*/       
int CcspBaseIf_freeResources(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int priority
);

//Data model parameters "set" APIs,caller free invalidParameterName
int CcspBaseIf_setParameterValues(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName
);
int CcspBaseIf_setParameterValues_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName
);

//commit the change
int CcspBaseIf_setCommit(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    dbus_bool commit
);

int CcspBaseIf_setCommit_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    dbus_bool commit
);

//Data model parameters "get" APIs,caller need free val with free_parameterValStruct_t
int CcspBaseIf_getParameterValues(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val
);

int CcspBaseIf_getParameterValues_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val
);
void free_parameterValStruct_t (void* bus_handle, int size,parameterValStruct_t **val);

//sets the attributes on data model parameters
int CcspBaseIf_setParameterAttributes(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    parameterAttributeStruct_t *val,
    int size
);

int CcspBaseIf_setParameterAttributes_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    parameterAttributeStruct_t *val,
    int size
);

//gets the attributes on data model parameters, caller need free val
int CcspBaseIf_getParameterAttributes(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterAttributeStruct_t ***val

);

int CcspBaseIf_getParameterAttributes_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterAttributeStruct_t ***val
);

void free_parameterAttributeStruct_t(void* bus_handle, int size, parameterAttributeStruct_t **val);

/*
    This API adds a row to a table object. The object name is a partial path
    and must end with a "." (dot). The API returns the instance number of the
    row.
*/

int CcspBaseIf_AddTblRow(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName,
    int * instanceNumber
);

int CcspBaseIf_AddTblRow_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName,
    int * instanceNumber
);
/*
    This API deletes a row from the table object. The object name is a partial
    path and must end with a "." (dot) after the instance number.
*/

int CcspBaseIf_DeleteTblRow(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName
);

int CcspBaseIf_DeleteTblRow_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName
);
/*
    This API is used to return the supported parameter names under a data model object
    parameterName is either a complete Parameter name, or a partial path name of an object.
    nextLevel
        If false, the response MUST contain the Parameter or object whose name exactly
        matches the ParameterPath argument, plus all Parameters and objects that are
        descendents of the object given by the ParameterPath argument, if any (all levels
        below the specified object in the object hierarchy).

        If true, the response MUST contain all Parameters and objects that are next-level
        children of the object given by the ParameterPath argument, if any.
caller need free val
*/

int CcspBaseIf_getParameterNames(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterName,
    dbus_bool nextLevel,
    int *size ,
    parameterInfoStruct_t ***val
);

int CcspBaseIf_getParameterNames_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterName,
    dbus_bool nextLevel,
    int *size ,
    parameterInfoStruct_t ***val
);

void free_parameterInfoStruct_t (void* bus_handle, int size, parameterInfoStruct_t **val);

/*
    This API is used in diagnostic mode. This must be used asynchronously.
    The use case is that the Test and Diagnostic Manager (TDM) CCSP component can leverage this feature
    in the Component Registrar to validate parameter types. The TDM sends commands to other components to
    run diagnostics. The TDM invokes a buscheck() request to each component one at a time in diagnostic mode.
    When each component receives buscheck(), it invokes the namespace type check API in the Component
    Registrar for each of the data model parameters accessed by this component and owned by another component.
    The Component Registrar verifies that each data model parameter is registered by a component and that the
    data model type specified in the API is the same as the data model type registered by the ¡°owner¡± component.
    The component sends TDM a response to buscheck() with all checked parameter names and PASS/FAIL for each
    parameter. If during buscheck(), it is found that there are missing or unregistered parameters,
    appropriate errors are flagged.
*/

int CcspBaseIf_busCheck(
    void* bus_handle,
    const char* dst_component_id
);

int CcspBaseIf_initialize(
    void* bus_handle,
    const char* dst_component_id
);

int CcspBaseIf_finalize(
    void* bus_handle,
    const char* dst_component_id
);


/*
*  This helper functions calls CcspBaseIf_getParameterNames
*  to retrieve the sub-object instance numbers under the designated
*  object, parse the response and return the final instance numbers
*  caller need free pNumArray
*/
int
CcspBaseIf_GetNextLevelInstances
(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char* pObjectName,
    unsigned int*  pNums,
    unsigned int** pNumArray
);

int CcspBaseIf_GetNextLevelRecords
(
    void*   bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char* pParentPath,
    unsigned int * pulNumRec,
    PCCSP_BASE_RECORD*  ppRecArray
);

void free_CCSP_BASE_RECORD (void* bus_handle, CCSP_BASE_RECORD *pInstanceArray);

int CcspBaseIf_SenddiagCompleteSignal(void * bus_handle);
int CcspBaseIf_SendsystemReadySignal(void * bus_handle);
int CcspBaseIf_SendsystemRebootSignal(void * bus_handle);
int CcspBaseIf_SendsystemKeepaliveSignal(void * bus_handle);

//Helper function to register base data modal
int CcspBaseIf_registerBase(
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    int component_version,
    const char *dbus_path,
    const char *subsystem_prefix
);


//following helper functions are wrappers for CcspBaseIf_getParameterValues, have been removed from base interface
//This API returns the internal state of the component. The state reflects the Component¡¯s internal lifecycle state
int CcspBaseIf_queryStatus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *internalState
);

//This API returns the health of the component as ¡°Red/Bad¡±, ¡°Yellow/warning¡±, ¡°Green/good
int CcspBaseIf_healthCheck(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *health);

//This API returns the amount of direct memory allocated by the component.
int CcspBaseIf_getAllocatedMemory(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *directAllocatedMemory);

//This API returns the maximum memory requirements for the component. It is the component owner¡¯s best estimates
int CcspBaseIf_getMaxMemoryUsage(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *memoryUsage);

//This API returns the minimum memory requirements for the component
int CcspBaseIf_getMinMemoryUsage(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *memoryUsage);


//cr function
/*
This API allows other CCSP components to register with the Component Registrar, Data Model Objects/Parameter namespace they own and well known command namespaces they support.  The registration must be done using fully qualified namespace at the parameter level. Registration using partial paths (object level) is not accepted.
*/
int CcspBaseIf_registerCapabilities(
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    int component_version,
    const char *dbus_path,
    const char *subsystem_prefix,
    name_spaceType_t * name_space,
    int size
);

/*
This APIs allows CCSP components to unregister the Data Model namespace it owns, or commands they support, with the Component Registrar. This API can only be called if a component had been previously registered. Otherwise an error code (CCSP_CR_ERR_UNKNOWN_COMPONENT) is returned.
*/
int CcspBaseIf_unregisterNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    const char *name_space);

int CcspBaseIf_unregisterNamespace_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    const char *name_space);

/*
This API unregisters the component from the component internal registry, if it had been previously registered. All capabilities/namespaces for the component are removed from the internal registry.  This will also result in the generation of "deviceProfileChangeSignal" on the message bus. Any registered component interested in the signal will get notified by the message bus.
*/
int CcspBaseIf_unregisterComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name
);

int CcspBaseIf_unregisterComponent_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name
);
/*
This API returns the component that owns the given namespace. There can be only one owner of the namespace. If there is no owner for the requested namespace registered with the Component Registrar, an error code (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE) is returned.
   caller need free components
*/
int CcspBaseIf_discComponentSupportingNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t ***components,
    int *size);

int CcspBaseIf_discComponentSupportingNamespace_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t ***components,
    int *size);

void free_char_t (void* bus_handle, int size, char **val);

/*
This API returns the dbus path of the component that owns the dynamic table object. There can be only one owner of the dynamic table. If there is no owner for the requested namespace registered with the Component Registrar, an error code (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE) is returned.
*/
int CcspBaseIf_discComponentSupportingDynamicTbl (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t **component
);

void free_componentStruct_t2 (void* bus_handle, componentStruct_t *val);
/*
This API returns all the namespaces registered by the given component name.
*/
int CcspBaseIf_discNamespaceSupportedByComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    name_spaceType_t ***name_space,
    int *size
);

int CcspBaseIf_discNamespaceSupportedByComponent_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    name_spaceType_t ***name_space,
    int *size
);

void free_name_spaceType_t (void* bus_handle, int size, name_spaceType_t **val);

/*
This API returns all component names and their D-Bus paths registered with Component Regsitrar.
*/
int CcspBaseIf_getRegisteredComponents (
    void* bus_handle,
    const char* dst_component_id,
    registeredComponent_t ***components,
    int *size
);

int CcspBaseIf_getRegisteredComponents_rbus (
    void* bus_handle,
    const char* dst_component_id,
    registeredComponent_t ***components,
    int *size
);

void free_registeredComponent_t (void* bus_handle, int size, registeredComponent_t **val);

void free_componentStruct_t (void* bus_handle, int size, componentStruct_t **val);

/*
This API provides a run time diagnostic mechanism to validate parameter type checking for the data model parameters registered by other components. Functional Components use this API to validate data types of namespaces of other components that they need to interface with.
*/
int CcspBaseIf_checkNamespaceDataType (
    void* bus_handle,
    const char* dst_component_id,
    name_spaceType_t * name_space,
    const char *subsystem_prefix,
    dbus_bool *typeMatch);

/*
This API dumps the internal component registry of the Component Registrar. This is primary made available for diagnostic purposes.
*/
int CcspBaseIf_dumpComponentRegistry (
    void* bus_handle,
    const char* dst_component_id
);

/*
This API is used to indicate if the system is ready. If a component fails to register for the systemReadySignal, then it can use this API to find out if all the components from the device profile have been successfully registered. When all the components in the device profile have been successfully registered with the Component Registrar, the system is said to be ready.
*/
int CcspBaseIf_isSystemReady (
    void* bus_handle,
    const char* dst_component_id,
    dbus_bool *val
);

int CcspBaseIf_isSystemReady_rbus (
    void* bus_handle,
    const char* dst_component_id,
    dbus_bool *val
);

/*
This API is used by components to ensure session integrity of its transactions. It returns the session ID if there is no active session in progress.
*/
int CcspBaseIf_requestSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int priority,
    int *sessionID
);

int CcspBaseIf_requestSessionID_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int priority,
    int *sessionID
);

/*This API returns the current session ID and its associated priority. */
int CcspBaseIf_getCurrentSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int *sessionID,
    int *priority
);

int CcspBaseIf_getCurrentSessionID_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int *sessionID,
    int *priority
);

/*This API returns the telemetry information */
int CcspBaseIf_SendTelemetryDataSignal (
    void* bus_handle,
    char* telemetry_data
);

/*
This API informs the Component Registrar about the end of a particular session.  At this time the CR sets the session ID and priority back to 0 and generates a signal to inform all components about the session ID. The components now revert back to the normal operations on seeing session ID = 0.
*/
int CcspBaseIf_informEndOfSession (
    void* bus_handle,
    const char* dst_component_id,
    int sessionID
);

int CcspBaseIf_informEndOfSession_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int sessionID
);

int CcspBaseIf_SendparameterValueChangeSignal (
    void* bus_handle,
    parameterSigStruct_t *val,
    int size
);

int CcspBaseIf_SendtransferCompleteSignal (
    void* bus_handle
);

int CcspBaseIf_SendtransferFailedSignal (
    void* bus_handle
);

int CcspBaseIf_SenddeviceProfileChangeSignal (
    void* bus_handle,
    char *component_name,
    char *component_dbus_path,
    unsigned char isAvailable
);

int CcspBaseIf_SendcurrentSessionIDSignal (
    void* bus_handle,
    int priority,
    int sessionID
);

int CcspBaseIf_WebConfigSignal (
    void* bus_handle,
    char* webconfig
);


//server side function
typedef int  (*CCSPBASEIF_FREERESOURCES)(
    int priority,
    void            *user_data
);


typedef int  (*CCSPBASEIF_SETPARAMETERVALUES)(
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName,
    void            *user_data
);

typedef int  (*CCSPBASEIF_SETCOMMIT)(
    int sessionId,
    unsigned int writeID,
    dbus_bool commit,
    void            *user_data
);

typedef int  (*CCSPBASEIF_GETPARAMETERVALUES)(
    unsigned int writeID,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val,
    void            *user_data
);

typedef int  (*CCSPBASEIF_SETPARAMETERATTRIBUTES)(
    int sessionId,
    parameterAttributeStruct_t *val,
    int size,
    void            *user_data
);

typedef int  (*CCSPBASEIF_GETPARAMETERATTRIBUTES)(
    char * parameterNames[],
    int size,
    int *val_size,
    parameterAttributeStruct_t ***val,
    void            *user_data
);


typedef int  (*CCSPBASEIF_ADDTBLROW)(
    int sessionId,
    char * objectName,
    int * instanceNumber,
    void            *user_data
);

typedef int  (*CCSPBASEIF_DELETETBLROW)(
    int sessionId,
    char * objectName,
    void            *user_data
);

typedef int  (*CCSPBASEIF_GETPARAMETERNAMES)(
    char * parameterName,
    dbus_bool nextLevel,
    int *size ,
    parameterInfoStruct_t ***val,
    void            *user_data
);

typedef int (*CCSPBASEIF_BUSCHECK)(
    void            *user_data
);


typedef int (*CCSPBASEIF_INITIALIZE)(
    void            *user_data
);

typedef int (*CCSPBASEIF_FINALIZE)(
    void            *user_data
);


//signal handler alive_die: 1=start 0=die
typedef void  (*CCSPBASEIF_COMPONENTDIE)(
    char * Name,
    int start_die,
    void            *user_data
);

typedef  void (*CCSPBASEIF_PARAMETERVALUECHANGESIGNAL) (
    parameterSigStruct_t *val,
    int size,
    void            *user_data
);

typedef  void (*CCSPBASEIF_DEVICEPROFILECHANGESIGNAL )(
    char *component_name,
    char *component_dbus_path,
    dbus_bool isAvailable,
    void            *user_data
);

typedef void (*CCSPBASEIF_CURRENTSESSIONIDSIGNAL )(
    int priority,
    int sessionID,
    void            *user_data
);

typedef void (*CCSPBASEIF_TELEMETRYDATASIGNAL )(
    char* telemetry_data,
    void* user_data
);

typedef  void (*CCSPBASEIF_DIAGCOMPLETESIGNAL )(
    void            *user_data
);

typedef  void (*CCSPBASEIF_SYSTEMREADYSIGNAL )(
    void            *user_data
);

typedef  void (*CCSPBASEIF_SYSTEMREBOOTSIGNAL )(
    void            *user_data
);

typedef void (*CCSPBASEIF_WEBCONFIGSIGNAL )(
    char* webconfig_data,
    void* user1_data
);

typedef void (*CCSPBASEIF_REGISTERCAPS)(
    char* component_name,
    void* user_data
);

typedef int (*CCSPBASEIF_GETSYSTEMSTATUS)();

typedef void (*CCSPBASEIF_MULTICOMPBROADCASTSIGNAL )(
    char* multiCompBroadCastSignal_data,
    void* user1_data
);

typedef void (*CCSPBASEIF_MULTICOMPMASTERPROCESSSIGNAL )(
    char* multiCompMasterProcessSignal_data,
    void* user1_data
);

typedef void (*CCSPBASEIF_MULTICOMPSLAVEPROCESSSIGNAL )(
    char* multiCompSlaveProcessSignal_data,
    void* user1_data
);

typedef void (*CCSPBASEIF_TUNNELSTATUS )(
    char* TunnelStatus_data,
    void* user1_data
);

typedef void (*CCSPBASEIF_WIFIDBSTATUS )(
    char* WifiDbStatus_data,
    void* user1_data
);

typedef int (*CCSPBASEIF_GETHEEALTH)();

typedef int (*CCSPPAIF_RESTARTBOOTSTRAP)( 
    void *user_data
);

typedef void(*CCSPCDLIF_DLCOMPLETESIGNAL)(
    unsigned int  startTime,
    unsigned int  completeTime,
    void          *user_data
);

typedef struct _CCSP_Base_Func_CB
{
    CCSPBASEIF_SETPARAMETERVALUES                  setParameterValues ;
    void *                                         setParameterValues_data;   
    
    CCSPBASEIF_SETCOMMIT                           setCommit  ;
    void *                                         setCommit_data;   
    
    CCSPBASEIF_GETPARAMETERVALUES                  getParameterValues  ;
    void *                                         getParameterValues_data;   
    
    CCSPBASEIF_SETPARAMETERATTRIBUTES              setParameterAttributes  ;
    void *                                         setParameterAttributes_data;   
    
    CCSPBASEIF_GETPARAMETERATTRIBUTES              getParameterAttributes  ;
    void *                                         getParameterAttributes_data;   
    
    CCSPBASEIF_ADDTBLROW                           AddTblRow  ;
    void *                                         AddTblRow_data;   
    
    CCSPBASEIF_DELETETBLROW                        DeleteTblRow  ;
    void *                                         DeleteTblRow_data;   
    
    CCSPBASEIF_GETPARAMETERNAMES                   getParameterNames  ;
    void *                                         getParameterNames_data;   
    
    CCSPBASEIF_FREERESOURCES                       freeResources;
    void *                                         freeResources_data;   
    
    CCSPBASEIF_BUSCHECK                            busCheck;
    void *                                         busCheck_data;   
    
    CCSPBASEIF_INITIALIZE                          initialize;
    void *                                         initialize_data;   
    
    CCSPBASEIF_FINALIZE                            finalize;
    void *                                         finalize_data;   
    
    CCSPBASEIF_COMPONENTDIE                        componentDie;
    void *                                         componentDie_data;   

    CCSPBASEIF_GETHEEALTH                          getHealth;
    void *                                         getHealth_data;

    CCSPPAIF_RESTARTBOOTSTRAP                      restartBootstrap;
    void *                                         restartBootstrap_data;
 
    CCSPCDLIF_DLCOMPLETESIGNAL                     dlCompleteSignal;
    void *                                         dlCompleteSignal_data;

    CCSPBASEIF_PARAMETERVALUECHANGESIGNAL          parameterValueChangeSignal;
    void *                                         parameterValueChangeSignal_data;   
    
    CCSPBASEIF_DEVICEPROFILECHANGESIGNAL           deviceProfileChangeSignal;
    void *                                         deviceProfileChangeSignal_data;   
    
    CCSPBASEIF_CURRENTSESSIONIDSIGNAL              currentSessionIDSignal;
    void *                                         currentSessionIDSignal_data;   
   
    CCSPBASEIF_TELEMETRYDATASIGNAL                 telemetryDataSignal;
    void *                                         telemetryDataSignal_data;
 
    CCSPBASEIF_DIAGCOMPLETESIGNAL                  diagCompleteSignal;
    void *                                         diagCompleteSignal_data;   
    
    CCSPBASEIF_SYSTEMREADYSIGNAL                   systemReadySignal;
    void *                                         systemReadySignal_data;   

    CCSPBASEIF_SYSTEMREBOOTSIGNAL                  systemRebootSignal;
    void *                                         systemRebootSignal_data;  
    CCSPBASEIF_WEBCONFIGSIGNAL                     webconfigSignal;
    void *                                         webconfigSignal_data;

    CCSPBASEIF_REGISTERCAPS                        registerCaps;
    void *                                         registerCaps_data;

    CCSPBASEIF_GETSYSTEMSTATUS                     isSystemReady;
    void *                                         isSystemReady_data;
    
    CCSPBASEIF_MULTICOMPBROADCASTSIGNAL            multiCompBroadCastSignal;
    void *                                         multiCompBroadCastSignal_data;

    CCSPBASEIF_MULTICOMPMASTERPROCESSSIGNAL        multiCompMasterProcessSignal;
    void *                                         multiCompMasterProcessSignal_data;

    CCSPBASEIF_MULTICOMPSLAVEPROCESSSIGNAL         multiCompSlaveProcessSignal;
    void *                                         multiCompSlaveProcessSignal_data;

    CCSPBASEIF_TUNNELSTATUS                        TunnelStatus;
    void *                                         TunnelStatus_data;

    CCSPBASEIF_WIFIDBSTATUS                        WifiDbStatus;
    void *                                         WifiDbStatus_data;
} CCSP_Base_Func_CB;

/*
These callback will be called when the component receive the reqeust.
*/
void CcspBaseIf_SetCallback
(
    void* bus_handle,
    CCSP_Base_Func_CB*  func
);


void CcspBaseIf_SetCallback2
(
    void* bus_handle,
    char *name,
    void*  func,
    void * user_data
);

int CcspBaseIf_getObjType(char *parentName, char *name, int *inst_num, char *buf);



DBusHandlerResult
CcspBaseIf_base_path_message_func (DBusConnection  *conn,
                                   DBusMessage     *message,
                                   DBusMessage     *reply,
                                   const char *interface,
                                   const char *method,
                                   CCSP_MESSAGE_BUS_INFO *bus_info);

DBusHandlerResult
CcspBaseIf_evt_callback (DBusConnection  *conn,
              DBusMessage     *message,
              void            *user_data
);
                                   
/* The function is called to register event, if the interface name and data path is NULL. Default is register the base interface*/
int  CcspIf_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name,
    const char* dbus_path_event,
    const char* dbus_interface_event
);

int  CcspBaseIf_Register_Event_rbus
(
    void* bus_handle,
    const char* sender,
    const char* event_name
);

int  CcspIf_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name,
    const char* dbus_path_event,
    const char* dbus_interface_event
);

int  CcspBaseIf_UnRegister_Event_rbus
(
    void* bus_handle,
    const char* sender,
    const char* event_name
);


#define  NOTIFYCWMPEVENT_TIMEOUT                         30
#define  STBSERVICE_AP_INTERFACE_NAME                    "com.cisco.stbservice.autoprovisioning"
#define  STBSERVICE_AP_PATH_NAME                         "/com/cisco/stbservice/autoprovisioning"
#define  STBSERVICE_AP_DESTINATION_NAME                  STBSERVICE_AP_INTERFACE_NAME
#define  STBSERVICE_CDL_INTERFACE_NAME                   "com.cisco.stbservice.codedownload"
#define  STBSERVICE_CDL_PATH_NAME                        "/com/cisco/stbservice/codedownload"
#define  STBSERVICE_CDL_DESTINATION_NAME                 STBSERVICE_CDL_INTERFACE_NAME

/* PA shall notify AP after each CWMP Inform event completes successfully. */

typedef enum {
    cwmp_event_0bootstrap = 0,
    cwmp_event_bootstrap,
    cwmp_event_boot,
    cwmp_event_periodic,
    cwmp_event_valuechange,
    cwmp_event_mrlspvreceived,
    cwmp_event_transfercomplete,
    cwmp_event_activationcomplete,
    cwmp_event_errorcodereceived
} CwmpEventType;

int
CcspIf_notifyCwmpEventToAP
    (
        void*                               bus_handle,
        unsigned long                       ulEventCode,
        char*                               pEventDesp,
        char*                               pTimeStamp
    );

int  CcspBaseIf_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
);

int  CcspBaseIf_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
);

//call this if you need handle signal other than standard signals like component_die,parameterValueChangeSignal..
void  CcspBaseIf_Set_Default_Event_Callback
(
    void* bus_handle,
    DBusObjectPathMessageFunction   callback
);

int CcspBaseIf_SendSignal(
    void * bus_handle,
    char *event
);


int CcspBaseIf_SendSignal_WithData(
    void * bus_handle,
    char *event,
    char* data

);

int CcspBaseIf_SendSignal_rbus(
    void * bus_handle,
    char *event

);

int CcspBaseIf_GetRemoteParameterValue(
    void* bus_handle,
    const char* cr_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val
);

int CcspBaseIf_SetRemoteParameterValue
(
    void* bus_handle,
    const char* cr_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName
);

int getPartnerId ( char *partnerID ) ;

#define SHM_PARAM_NAME "dbus use SHM"

int CcspBaseIf_getParameterValues_Shm(
    CCSP_MESSAGE_BUS_INFO * bus_info,
    int shmSize,
    int *val_size,
    parameterValStruct_t ***parameterval
    );

int CcspBaseIf_base_path_message_write_shm (
    void* bus_handle,
    int size,
    parameterValStruct_t **val,
    int * shmSize
    );              

int PsmGroupGet(void *bus_handle, 
        const char *subsys, 
        const char *names[], 
        int nname, 
        parameterValStruct_t ***records, 
        int *nrec);

void PsmFreeRecords(void *bus_handle, 
        parameterValStruct_t **records, 
        int nrec);

int Rbus_to_CCSP_error_mapper(int error_code);
int Rbus2_to_CCSP_error_mapper(int error_code);
#endif /* CCSP_BASE_API_H */
