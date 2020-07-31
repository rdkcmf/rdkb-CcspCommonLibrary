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


#ifndef  _COSA_WEBCONFIG_FRAMEWORK_API_H
#define  _COSA_WEBCONFIG_FRAMEWORK_API_H


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h> 
#include<errno.h>
#include<time.h>
#include<pthread.h>
#include <mqueue.h>

#include "ccsp_base_api.h"
#include "ccsp_message_bus.h"   // for CCSP_Message_Bus_Init/Exit
#include "slap_definitions.h"
#include "ccsp_trace.h"
#include "webconfig_err.h"

extern ANSC_HANDLE bus_handle;


#ifdef WBCFG_MULTI_COMP_SUPPORT

#define MASTER_COMPONENT_NAME           	"CcspPandMSsp"
#define BROADCASTSIGNAL_NAME			"multiCompBroadCastSignal"
#define MASTER_COMP_SIGNAL_NAME 		"multiCompMasterProcessSignal"
#define SLAVE_COMP_SIGNAL_NAME 			"multiCompSlaveProcessSignal" 
#define HOTSPOT_VERSION_IGNORE          "/tmp/hotspot_version_ignore"

#define IAM_MASTER				0x000000AA    // 170
#define READY_TO_RECEIVE_DATA			0x000000BB  //187
#define BLOB_EXEC_DATA				0x000000BC
#define BLOB_EXEC_REQUEST_TIMEOUT 		0x000000CC	
#define TIMEOUT_VALUE				0x000000CD
#define CONFIRM_IF_EXEC_COMPLETED		0x000000DD
#define EXECUTION_STILL_IN_PROGRESS		0x000000DE
#define EXECUTION_SUCCESSFULLY_COMPLETED 	0x000000EE  //238
#define EXECUTION_FAILED			0x000000EF
#define ROLLBACK_LAST_REQUEST			0x000000FF

#define MAX_RESPONSE_TIME      			150  // max wait time for slave slave comp response in seconds
#define MAX_DATA_SIZE_TO_SLAVE  		20480

#define WEBCONFIG_QUEUE_NAME_MULTI_COMP 	"/webconfig_queue_mComp"

#define WEBCONFIG_QUEUE_NAME_MULTI_COMP_SLAVE 	"/webconfig_queue_mCompSlave"

#endif

#define WEBCONFIG_QUEUE_NAME 			"/webconfig_queue"

#define FRAMEWORK_DEBUG 			"/tmp/webconfig_dbg"
#define ACK 					1
#define NACK 					0

#define VERSION_ALREADY_EXIST    		100
#define EXECUTION_IN_QUEUE		 	101
#define VERSION_UPDATE_REQUIRED  		200

#define MAX_SIZE  				1024

#define MAX_EVENTS_IN_MQUEUE 			10
#define QUEUE_SIZE 				10

#define DEFAULT_TIMEOUT_PER_ENTRY 		3
#define DEFAULT_TIMEOUT 		   	10

// Blob MAX execution timeout will be  MAX_FUNC_EXEC_TIMEOUT * TIMEOUT 
#define MAX_FUNC_EXEC_TIMEOUT 			3

// Default log interval and iterations

#define DEFAULT_DEBUG_INTERVAL 			10
#define DEFAULT_DEBUG_ITER 			12


#define MAX_PARAM_NAME 				256
#define MAX_PARAM_VALUE 			256

#define SUBDOC_NAME_SZ              64
/************************************************************************************

 * blob_exec_state maintains the state of execution

*************************************************************************************/


#define COMPONENT_INIT_EVENT       		"COMP_INIT"
#define COMPONENT_CRASH_EVENT      		"COMP_CRASH"
typedef  enum
_blob_exec_state
{
	NOT_STARTED       = 0,
	PENDING,
    	IN_PROGRESS,
    	COMPLETED,
    	FAILED
   
}
blob_exec_state, *Pblob_exec_state;

/************************************************************************************

 * blobRegInfo maintains the subdoc_name supported by each component 
 * and its version

 * Component during it's initialization provide this data to webconfig framework
 * Framework later updates the version after successfull execution of blob request

 * version 			-> default version, component can fill it or set to 0

 * subdoc_name 		-> Subdoc name which needs to be bind to the webfig framework. 
					Example: Device.NAT.X_RDK_PortMapping.Data


*************************************************************************************/

typedef  struct
_blobRegInfo
{
	int version;
	char subdoc_name[SUBDOC_NAME_SZ];
}
blobRegInfo,  *PblobRegInfo;


/************************************************************************************

 * Err struct maintains ErrorCode and ErrorMsg

 * Blob execution handler to return error code and error message to framework after
 * executing blob request. 
 * Framework will return the code and message to webconfig client 

 * ErrorCode 		-> Retrurn value of blob execution
 * ErrorMsg 		-> Return message of execution 

*************************************************************************************/

typedef struct 
_Error_   
{
	uint16_t ErrorCode;
        char ErrorMsg[64] ;
}Err,*pErr;

#ifdef WBCFG_MULTI_COMP_SUPPORT

// newly modified changes
#if 0

/*************************************************************************************

 * parameterDetails struct maintains param_name and param_value

 * master component will fill the slave components parameters in this format 
 * and pass it to framework, aand framework will send this data to slave component

 * param_name 		-> name of the slave component parameter 
 * param_value 		-> value of the slave component parameter  

*************************************************************************************/


typedef struct 
parameterDetails   
{
	char param_name[MAX_PARAM_NAME];
	char param_value[MAX_PARAM_VALUE];
}parameterDetails,*pParameterDetails;

#endif

/*****************************************************************************************************

 * MultiComp_ExecInfo 

 * master component after receiving the blob needs to fill this data and pass it to
 * webconfig framework

 * isMaster 			-> For master component value needs to be 1 and for slave value will be 0
 * CompName 		    -> If component is slave , copy the slave component name 
 *						for master pass NULL in slavecomp name
 * comp_exec_data 		-> For master component assign the struct parsed from blob for master
 *						component execution in execData format and for slave components pass blob data
*********************************************************************************************************/


typedef struct 
MultiComp_ExecInfo   
{
    	int isMaster;
    	char CompName[64]; 	
    	void*  comp_exec_data ;  
    	/*int numOfParams ; //Applicable only for slave component
    	parameterDetails *parameterInfo; */
}MultiComp_ExecInfo,*pMultiComp_ExecInfo;


/**********************************************************************************************************************

 * policySequence struct 

 * policySequence maintains number of components involved , component execution data
 * and also it will provide the info whether execution should happen in serial or in parellel

 * isExecInSequenceNeeded 	->  Value should be set to 1 if multi component blob execution
 *							should happen in sequence , if any  other value is passed execution will happen in parellel

 * numOfComponents 			-> number of components involved in the multi component blob execution 

 * MultiComp_ExecInfo 		-> master component after receiving the blob needs to fill this data and pass it to
 * 							webconfig framework

*************************************************************************************************************************/

typedef struct 
policySequence   
{
     	int isExecInSequenceNeeded;
     	int numOfComponents;

     	MultiComp_ExecInfo *multiCompExecData;
        int (*postExecSettings) (); 
	
}policySequence,*pPolicySequence;

#endif


/*************************************************************************************************************************************************

 * execData 

 * Once message pack unpacking is completed, component needs fill *exec_data structure and call this API.
 * In case of multi component execution, master component data needs to filled in this struct


 * char* subdoc_name 						 –>  Subdoc name 

 * uint16_t txid 							 –> Transaction id to handle the error handling case. 
 *											 This should come from webconfig client in blob.

 * uint32_t version 						 –> version of the blob set request. This should come from webconfig client in blob.

 * void* user_data 							 –>  The structure which is parsed from the blob. Contains parameters based on feature.

 * uint32_t numOfEntries 					 –> Number of entries for *user_data

 * unsigned int (*calcTimeout) (uint32_t)  	 –> This function pointer expects a handler from component to calculate the timeout for blob request. 
 *											 If component doesn’t set it, then default handler in framework will be used.

 * pErr* (*executeBlobRequest) (void*)		 –> This function pointer expects a handler from component to handle blob request.
 *											 This should be feature specific.

 * int (*rollbackFunc) ()					 –> This function pointer expects a handler from component to handle rollback 
 *						 					 to previous values in case set request fails. This is an optional argument.

 * freeResources  (void *)					->  This function pointer expects a handler from component to free the resources


****************************************************************************************************************************************************/

typedef struct 
_execData   
{
  	char subdoc_name[SUBDOC_NAME_SZ];
	uint16_t txid;
	uint32_t version;
	size_t numOfEntries;
	void* user_data;
	size_t (*calcTimeout) (size_t);  
    	pErr (*executeBlobRequest) (void*); 
	int (*rollbackFunc) (); 
	void (*freeResources) (void *);	
	int multiCompRequest;

} execData; 


/**********************************************************************************************************************

 * queueInfo struct 

 * queueInfo  struct is internal to webconfig framework, this maintains data of all the blob which are pending for 
 * execution, after completing the execution framework will remove the entry from queue

 * version_queue 		    ->  Maintains version number 

 * timeout_queue 			->  Maintains timeout value 

 * txid_queue 				->  Maintains transaction id

 * blob_state_queue 		->  Maintains state of execution, whether in in progress/pending/failed

 * execDataPointerAddr      -> Maintained to free the allocated memory after execution
*************************************************************************************************************************/


typedef  struct
_queueInfo
{
	uint32_t version_queue[QUEUE_SIZE+1];
	size_t timeout_queue[QUEUE_SIZE+1];
    	uint16_t txid_queue[QUEUE_SIZE+1]; 
	blob_exec_state blob_state_queue[QUEUE_SIZE+1];
	execData* execDataPointerAddr[QUEUE_SIZE+1];

	int front ;
	int rear ;
}
queueInfo,  *PqueueInfo;

#ifdef WBCFG_MULTI_COMP_SUPPORT

/**********************************************************************************************************************

 * multiCompExecStatus struct 

 * multiCompExecStatus  struct is internal to webconfig framework, this maintains exec state of components
 * in multi component execution. This data is later needed to conclude if all the components executed requests or not 
 * in case of failure rollback is triggered based on this struct data

 * componentName 		    ->  Name of the component in multi comp execution

 * executionStatus 			->  Set to 1 once execution is completed.

 * execResult 				->  Return value of execution

 * timeout 					->  Timeout of each component

 * execRetMsg      			->  Return message of the execution, passed to webconfig client in case of failue
*************************************************************************************************************************/

typedef  struct
_multiCompExecStatus
{

	char componentName[64];
	int executionStatus;
	int execResult;
	int timeout;
	char execRetMsg[64];
}
multiCompExecStatus,  *pMultiCompExecStatus;


/**********************************************************************************************************************************************

 * multiCompSubDocReg struct 

 * multiCompSubDocReg  is called by components which are slave to any subdoc execution during compoenent initialization

 * (*calcTimeout)   				    –> This function pointer expects a handler from component to calculate the timeout for blob request. 
 * executeBlobRequest 					->  This function pointer expects a handler from component to handle blob request.
 *										This should be feature specific.

 * rollbackFunc 						–> This function pointer expects a handler from component to handle rollback 
 *						 					 to previous values in case set request fails. This is an optional argument.

 * freeResources 						->  This function pointer expects a handler from component to free the resources

***********************************************************************************************************************************************/

typedef  struct
_multiCompSubDocReg
{

	char multi_comp_subdoc[SUBDOC_NAME_SZ];
	size_t (*calcTimeout) ();  
    	pErr (*executeBlobRequest) (void*); 
	int (*rollbackFunc) (); 
	void (*freeResources) ();

}
multiCompSubDocReg,  *pMultiCompSubDocReg;

#endif

void* display_subDocs();

void check_component_crash(char* init_file);


typedef uint32_t (*getVersion) (char*) ;

typedef int (*setVersion) (char* , uint32_t ) ;

void register_sub_docs(blobRegInfo *bInfo,int NumOfSubdocs, getVersion getv,setVersion setv);

void notifyVersion_to_Webconfig(char* subdoc_name, uint32_t version,int process_crashed);

size_t defFunc_calculateTimeout(size_t numOfEntries);

typedef size_t 
(*calcTimeout) 
	(
	size_t
	);

void send_ACK (char *subdoc_name, uint16_t txid, uint32_t version, unsigned long timeout,char *msg );

void send_NACK (char *subdoc_name, uint16_t txid, uint32_t version, uint16_t ErrCode,char *failureReason);

typedef pErr 
(*executeBlobRequest) 
	(
	void *
	); 

typedef int 
(*rollbackFunc) 
	(
	); 

typedef void 
(*freeResources) 
	(
		void *
	); 

void updateVersionAndState(uint32_t version, int blob_execution_retValue,PblobRegInfo blobUpdate);

int checkNewVersionUpdateRequired(execData *exec_data,int *queueIndex);

void PushBlobRequest (execData *exec_data ) ;

void removeEntryFromQueue() ;

int isQueueEmpty();

int addEntryToQueue(uint32_t version,uint16_t txid ,unsigned long timeout,void *execData);

int isQueueFull(); 

blobRegInfo* getAddress(char* subdoc_name);

void* execute_request(void *arg);

void* messageQueueProcessing();


void initMessageQueue();
int checkIfVersionExecInQueue(uint32_t version,int *queueIndex);
void sendWebConfigSignal(char* data);

size_t getPendingQueueTimeout(uint16_t txid);

#ifdef WBCFG_MULTI_COMP_SUPPORT


int checkIfVersionExecInMultiCompQueue(uint32_t version,int *queueIndex);

int addEntryToMultiCompQueue(uint32_t version,uint16_t txid, unsigned long timeout,void *exec_data);

void removeEntryFromMultiCompQueue();

void removeEntryfromRearEndMultiComp();

int isMultiCompQueueEmpty() ;

int isMultiCompQueueFull();

void initMultiCompMaster();
void initMultiCompSlave();
void eventRegisterSlave();
void eventRegisterMaster();

void sendBlobExecutionResult(char* subdoc_name, int exec_status, int execRet, char* execRetMsg);

size_t getMultiCompTimeOut(char* subdoc_name);

void rollbackLastExec(char* subdoc_name);

void sendTimeoutToMaster(char* subdoc_name,size_t timeout);

void multiCompBroadCastSignal_callbk(char* info, void *user_data);

void multiCompMasterProcessSignal_callbk(char* info, void *user_data);

void multiCompSlaveProcessSignal_callbk(char* info, void *user_data);

char *strtokstr_r(char *s, char *delim, char **save_ptr);

void PushMultiCompBlobRequest (execData *exec_data) ;

size_t getMultiCompPendingQueueTimeout(uint16_t txid);

void* messageQueueProcessingMultiComp();
void initMultiCompMaster();
void initMultiCompSlave();
#if 0
void* messageQueueProcessingMultiCompSlave();
#endif
void* ExecuteMultiCompRequest_thread(void* arg);

void register_MultiComp_subdoc_handler(multiCompSubDocReg *mCompReg,int NumOfMCompSubdocs);

void* execute_request_master(void *arg);

void* execute_request_slave(void *data);

void requestTimeOutFromSlave(char *compName,char *subdocName);

void sendSignaltoRollBack(char* comp_name, char* subdoc_name);

void sendBroadCastEvent(char* compName,char* subdoc_name );

void sendDataToSlaveComp(char* compName , char* subdoc_name, char*  blob);

size_t getMultiCompPendingQueueTimeout(uint16_t txid);
void parseMasterData(const char* info);
void parseBroadcastData(const char* info);
void parseSlaveData(const char* info);

#endif
#endif /* _COSA_WEBCONFIG_FRAMEWORK_API_H */
