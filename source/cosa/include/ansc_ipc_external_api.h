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

    module:	ansc_ipc_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Ipc Port Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/24/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_IPC_EXTERNAL_API_
#define  _ANSC_IPC_EXTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_IPC_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateIpcPort
    (
        ANSC_HANDLE                 hOwnerContext,
        char*                       name
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_IRO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscIroCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscIroRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscIroReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_IRO_MANAGEMENT.C
***********************************************************/

char*
AnscIroGetName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscIroSetName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ULONG
AnscIroGetId
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscIroSetId
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_IRO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscIroServ
    (
        ANSC_HANDLE                 hThisObject,
        char*                       srcPortName,
        ANSC_HANDLE                 hParams,
        ANSC_HANDLE                 hReturn
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_IMO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscImoCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscImoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscImoReset
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
