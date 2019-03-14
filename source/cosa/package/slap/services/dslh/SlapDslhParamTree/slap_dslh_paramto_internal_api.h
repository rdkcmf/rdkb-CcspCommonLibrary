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

    module:	slap_dslh_paramto_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Dslh ParamTree
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        10/20/05    initial revision.
		11/17/06    Bin Zhu added new apis to support GUI;
        03/22/10    Bin added api "GetParamInfoFromCLI" to support CLI;
		10/06/11	Ported to CCSP Cosa framework.

**********************************************************************/


#ifndef  _SLAP_DSLH_PARAMTO_INTERNAL_API_
#define  _SLAP_DSLH_PARAMTO_INTERNAL_API_


/***********************************************************
     FUNCTIONS IMPLEMENTED IN SLAP_DSLH_PARAMTO_STATES.C
***********************************************************/

ANSC_STATUS
SlapDslhParamtoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN SLAP_DSLH_PARAMTO_ACCESS.C
***********************************************************/
ULONG
SlapDslhParamtoGetChildObjNames
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName,
        SLAP_STRING_ARRAY**			pNameArray
    );

ULONG
SlapDslhParamtoGetParamNames
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName,
        ULONG                       uNextLevel,
        SLAP_STRING_ARRAY**			pNameArray
    );


SLAP_VARIABLE*
SlapDslhParamtoGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName
    );

ANSC_STATUS
SlapDslhParamtoSetParamValue
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

ANSC_STATUS
SlapDslhParamtoSetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        char*                       pParamValue
    );

ANSC_STATUS
SlapDslhParamtoSetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        int                         iParamValue
    );

ANSC_STATUS
SlapDslhParamtoSetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        ULONG                       ulParamValue
    );

ANSC_STATUS
SlapDslhParamtoSetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_BOOL                   bParamValue
    );

ULONG
SlapDslhParamtoGetParamInfo
	(
		ANSC_HANDLE					hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName,
		ULONG                       uNextLevel,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_INT_ARRAY**			pAccessArray,
		SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray,
		SLAP_STRING_ARRAY**			pAttrArray
	);

ULONG
SlapDslhParamtoGetParamInfoShort
	(
		ANSC_HANDLE					hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray
	);


ULONG
SlapDslhParamtoGetHiddenInfo
	(
		ANSC_HANDLE					hThisObject,
		char*				pSubsystemPrefix,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_STRING_ARRAY**			pValueArray
	);

ULONG
SlapDslhParamtoGetObjectInfo
	(
		ANSC_HANDLE					hThisObject,
		char*				pSubsystemPrefix,
		SLAP_STRING_ARRAY**			pAddableArray,
		SLAP_STRING_ARRAY**			pDeletableAttrArray
	);

void
SlapDslhParamtoGetParamTypeAndValue
	(
		ANSC_HANDLE					hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName,
		char**						ppType,
		char**						ppValue
	);

ANSC_STATUS
SlapDslhParamtoApplyChanges
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
        char*                       pObjName
    );

ULONG
SlapDslhParamtoAddObject
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
        char*                       pObjName
    );

ANSC_STATUS
SlapDslhParamtoDelObject
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID,
        char*                       pObjName
    );

INT
SlapDslhParamtoAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							priority
    );

ANSC_STATUS
SlapDslhParamtoRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
		int							sessionID
    );

BOOL
SlapDslhParamtoIsParamTreeReadOnly
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix
    );

BOOL
SlapDslhParamtoIsParameterReadOnly
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName
    );

SLAP_STRING
SlapDslhParamtoEscapeXmlString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_STRING                 pInputString
    );


#endif
