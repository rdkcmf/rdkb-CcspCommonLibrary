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

    module:	ansc_oco_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Object Container
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_OCO_INTERNAL_API_
#define  _ANSC_OCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_OCO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscOcoGetExternalOmo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetExternalOmo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOmo
    );

ANSC_HANDLE
AnscOcoGetInternalOmo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetInternalOmo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOmo
    );

ANSC_HANDLE
AnscOcoGetTso
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetTso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTso
    );

ANSC_HANDLE
AnscOcoGetToo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetToo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hToo
    );

ANSC_HANDLE
AnscOcoGetIpcPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetIpcPort
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIpcPort
    );

ANSC_HANDLE
AnscOcoGetCarrier
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetCarrier
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    );

ULONG
AnscOcoGetOpMask
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSetOpMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMask
    );

ANSC_STATUS
AnscOcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_OCO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscOcoEnrollAllObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoManufactureAllObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoDestroyAllObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_OCO_EXTENSION.C
***********************************************************/

ANSC_STATUS
AnscOcoEnrollExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoManufactureExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoDestroyExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_OCO_FEATURE.C
***********************************************************/

ANSC_STATUS
AnscOcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_OCO_OBJECTS.C
***********************************************************/

ANSC_STATUS
AnscOcoRegisterObject1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        void*                       pfnCreate,
        void*                       pfnRemove
    );

ANSC_STATUS
AnscOcoRegisterObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       objectName,
        ULONG                       oid,
        char*                       baseObjectName,
        ULONG                       baseOid,
        char*                       derivedType,
        ULONG                       brokerOid,
        void*                       pfnCreate,
        void*                       pfnRemove
    );

ANSC_HANDLE
AnscOcoCreateObjectByName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       objectName
    );

ANSC_HANDLE
AnscOcoCreateObjectByOid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       oid
    );

ANSC_HANDLE
AnscOcoCreateObjectByType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext,
        char*                       baseObjectName,
        char*                       derivedType
    );

ANSC_HANDLE
AnscOcoGetObjectByRid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       rid
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_OCO_TIMERS.C
***********************************************************/

ANSC_STATUS
AnscOcoRegisterTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    );

ANSC_STATUS
AnscOcoCancelTimer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimer
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_OCO_TASKS.C
***********************************************************/

ANSC_STATUS
AnscOcoRegisterTask
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTask
    );

ANSC_STATUS
AnscOcoCancelTask
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTask
    );


#endif
