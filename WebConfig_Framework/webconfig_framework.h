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


#define FRAMEWORK_DEBUG 		 	"/tmp/webconfig_dbg"
#define ACK 					1
#define NACK 					0

#define VERSION_ALREADY_EXIST    		100
#define EXECUTION_IN_QUEUE		 	101
#define VERSION_UPDATE_REQUIRED  		200


#define WEBCONFIG_QUEUE_NAME 	 		"/webconfig_queue"

#define MAX_SIZE 				 1024

#define MAX_EVENTS_IN_MQUEUE 	   		   10  
#define QUEUE_SIZE 				   10


#define DEFAULT_TIMEOUT_PER_ENTRY 		    3
#define DEFAULT_TIMEOUT 		   	   10
// Blob MAX execution timeout will be  MAX_FUNC_EXEC_TIMEOUT * TIMEOUT 
#define MAX_FUNC_EXEC_TIMEOUT 			    3

// Default log interval and iterations

#define DEFAULT_DEBUG_INTERVAL                     10
#define DEFAULT_DEBUG_ITER                         12

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

typedef  struct
_blobRegInfo
{
	int version;
	char subdoc_name[64];
}
blobRegInfo,  *PblobRegInfo;

typedef struct 
_Error_   
{
           uint16_t ErrorCode;
           char ErrorMsg[64] ;
}Err,*pErr;


typedef struct 
_execData   
{
  	char subdoc_name[64];
	uint16_t txid;
	uint32_t version;
	size_t numOfEntries;
	void* user_data;
	size_t (*calcTimeout) (size_t);  
    	pErr (*executeBlobRequest) (void*); 
	int (*rollbackFunc) (); 
	void (*freeResources) (void *);

} execData; 

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

int checkNewVersionUpdateRequired(char *subdoc_name,uint32_t version,int *queueIndex);

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


size_t getPendingQueueTimeout(uint16_t txid);

#endif /* _COSA_WEBCONFIG_FRAMEWORK_API_H */
