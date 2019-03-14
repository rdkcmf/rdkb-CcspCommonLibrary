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

    module: ssp_messagebus_interface.h

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    description:

        The header file for the CCSP Message Bus Interface
        Service.

    ---------------------------------------------------------------

    environment:

        Embedded Linux

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        06/23/2011  initial revision.

**********************************************************************/

#ifndef  _MESSAGEBUS_INTERFACE_HELPER_
#define  _MESSAGEBUS_INTERFACE_HELPER_



int 
CcspCcMbi_GetParameterValues
    (    
        unsigned int writeID,
        char * parameterNames[],
        int size,
        int *val_size,
        parameterValStruct_t ***val,
        void * user_data
    );

int 
CcspCcMbi_SetParameterValues
    (
        int sessionId,
        unsigned int writeID,
        parameterValStruct_t *val,
        int size,
        dbus_bool commit,
        char ** invalidParameterName,
        void * user_data
    );

int 
CcspCcMbi_SetCommit
    (
        int sessionId,
        unsigned int writeID,
        dbus_bool commit,
        void * user_data
    );

int 
CcspCcMbi_SetParameterAttributes
    (
        int sessionId,
        parameterAttributeStruct_t *val,
        int size,
        void * user_data
    );

int 
CcspCcMbi_GetParameterAttributes
    (
        char* parameterNames[],
        int size,
        int* val_size,
        parameterAttributeStruct_t ***val,
        void * user_data
    );

int  
CcspCcMbi_AddTblRow
    (
        int sessionId,
        char * objectName,
        int * instanceNumber,
        void * user_data
    );

int 
CcspCcMbi_DeleteTblRow
    (
        int sessionId,
        char * objectName,
        void * user_data
    );

int 
CcspCcMbi_GetParameterNames
    (
        char * parameterName,
        dbus_bool nextLevel,
        int *size ,
        parameterInfoStruct_t ***val,
        void * user_data
    );

void 
CcspCcMbi_CurrentSessionIdSignal
    (
        int priority,
        int sessionID,
        void * user_data
    );

char* 
AccessControlBitMaskToAccessList
    (
        ULONG AccessControlBitMask
    );

ULONG 
AccessListToAccessControlBitMask
    (
        char * pAccessList
    );

void*
MsgHelper_CreateCcdMbiIf
    (
        void*           dbusHandle,
        char*           pPrefix
    );

extern ULONG    g_lastWriteEntity;
extern ULONG    g_currentWriteEntity;

#define GET_LAST_WRITE_ENTITY()    g_lastWriteEntity
#define GET_CURRENT_WRITE_ENTITY() g_currentWriteEntity

#endif
